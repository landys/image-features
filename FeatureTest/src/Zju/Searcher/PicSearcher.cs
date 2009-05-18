using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Util;
using Zju.Dao;

namespace Zju.Searcher
{
    public abstract class PicSearcher<T> : BaseSearcher where T : System.IComparable<T>
    {
        protected T limit;
        protected DelCalcDist<T> calcDist;

        protected static readonly DelCalcDist<float> DEFAULT_CALC_DIST_DELEGATE = new DelCalcDist<float>(ClothUtil.CalcManhattanDistance);
        protected static readonly DelCalcDist<double> DEFAULT_DOUBLE_CALC_DIST_DELEGATE = new DelCalcDist<double>(ClothUtil.CalcManhattanDistance);

        public PicSearcher(PicParam<T> picParam, T limit, DelCalcDist<T> calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, wrappedSearcher, maxResult)
        {
            this.limit = limit;
            this.calcDist = calcDist;
        }

        public PicSearcher(PicParam<T> picParam, T limit, DelCalcDist<T> calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, clothDao, maxResult)
        {
            this.limit = limit;
            this.calcDist = calcDist;
        }

        public override List<Cloth> Search()
        {
            PicParam<T> picParam = (PicParam<T>)param;

            List<Cloth> clothes = null;
            if (wrappedSearcher != null)
            {
                clothes = wrappedSearcher.Search();
            }
            else if (clothDao != null)
            {
                /*if (picParam.ColorNum > 0)
                {
                    clothes = clothDao.FindByColorNum(picParam.ColorNum);
                }
                else
                {
                    clothes = clothDao.FindAll();
                }*/
                if (picParam.Categories != null && picParam.Categories.Length > 0)
                {
                    foreach (int category in picParam.Categories)
                    {
                        List<Cloth> tcs = clothDao.FindByCategory(category);
                        if (clothes == null)
                        {
                            clothes = tcs;
                        }
                        else
                        {
                            clothes.AddRange(tcs);
                        }
                    }
                }
                else
                {
                    clothes = clothDao.FindAll();
                }
            }

            if (null == clothes)
            {
                throw new NullReferenceException("Both wrappedSearcher and clothDao are null, or some error happened.");
            }

            T[] featureVector = picParam.Feature;
            SortedDictionary<T, List<Cloth>> sortClothes = new SortedDictionary<T, List<Cloth>>();
            foreach (Cloth cloth in clothes)
            {
                T md = calcDist(featureVector, GetVector(cloth));
                if (md.CompareTo(limit) < 0)
                {
                    if (!sortClothes.ContainsKey(md))
                    {
                        sortClothes[md] = new List<Cloth>();
                    }
                    sortClothes[md].Add(cloth);
                }
            }

            List<Cloth> resultClothes = new List<Cloth>();
            foreach (List<Cloth> cs in sortClothes.Values)
            {
                resultClothes.AddRange(cs);
            }

            if (resultClothes.Count > MaxResult)
            {
                return resultClothes.GetRange(0, MaxResult);
            }

            return resultClothes;
        }

        public T Limit
        {
            get { return limit; }
            set { limit = value; }
        }

        /// <summary>
        /// Get the related vector of this search type.
        /// </summary>
        /// <param name="cloth"></param>
        /// <returns></returns>
        public abstract T[] GetVector(Cloth cloth);
    }
}

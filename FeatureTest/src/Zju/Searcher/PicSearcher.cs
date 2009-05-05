﻿using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Util;
using Zju.Dao;

namespace Zju.Searcher
{
    public abstract class PicSearcher : BaseSearcher
    {
        protected float limit;
        protected DelCalcDist calcDist;

        protected static readonly DelCalcDist DEFAULT_CALC_DIST_DELEGATE = new DelCalcDist(ClothUtil.CalcManhattanDistance);

        public PicSearcher(PicParam picParam, float limit, DelCalcDist calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, wrappedSearcher, maxResult)
        {
            this.limit = limit;
            this.calcDist = calcDist;
        }

        public PicSearcher(PicParam picParam, float limit, DelCalcDist calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, clothDao, maxResult)
        {
            this.limit = limit;
            this.calcDist = calcDist;
        }

        public override List<Cloth> Search()
        {
            PicParam picParam = (PicParam)param;

            List<Cloth> clothes = null;
            if (wrappedSearcher != null)
            {
                clothes = wrappedSearcher.Search();
            }
            else if (clothDao != null)
            {
                if (picParam.ClothNum > 0)
                {
                    clothes = clothDao.FindByClothNum(picParam.ClothNum);
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

            float[] featureVector = picParam.Feature;
            SortedDictionary<float, List<Cloth>> sortClothes = new SortedDictionary<float, List<Cloth>>();
            foreach (Cloth cloth in clothes)
            {
                float md = calcDist(featureVector, GetVector(cloth));
                if (md <= limit)
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

        public float Limit
        {
            get { return limit; }
            set { limit = value; }
        }

        /// <summary>
        /// Get the related vector of this search type.
        /// </summary>
        /// <param name="cloth"></param>
        /// <returns></returns>
        public abstract float[] GetVector(Cloth cloth);
    }
}

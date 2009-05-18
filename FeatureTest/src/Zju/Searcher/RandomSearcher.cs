using System;
using System.Collections.Generic;
using Zju.Dao;
using Zju.Domain;
using Zju.Util;

namespace Zju.Searcher
{
    public class RandomSearcher : BaseSearcher
    {
        private int[] categories;

        public RandomSearcher(IClothDao clothDao, int[] categories, int maxResult)
            : base(null, clothDao, maxResult)
        {
            this.categories = categories;
        }

        public override List<Cloth> Search()
        {
            List<Cloth> clothes = null;
            if (clothDao != null)
            {
                if (categories != null && categories.Length > 0)
                {
                    foreach (int category in categories)
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
                throw new NullReferenceException("ClothDao are null, or some error happened.");
            }

            if (clothes.Count <= MaxResult)
            {
                return clothes;
            }

            List<Cloth> resultClothes = new List<Cloth>();
            HashSet<int> hs = new HashSet<int>();
            Random r = new Random((int)(DateTime.UtcNow.Ticks));
            int n = clothes.Count;
            for (int i=0; i<MaxResult; ++i)
            {
                int t = r.Next() % n;
                while (hs.Contains(t))
                {
                    t = (++t) % n;
                }
                hs.Add(t);
                resultClothes.Add(clothes[t]);
            }

            return resultClothes;
        }
    }
}

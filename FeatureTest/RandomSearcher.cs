using System;
using System.Collections.Generic;
using Zju.Dao;
using Zju.Domain;
using Zju.Util;

namespace Zju.Searcher
{
    public class RandomSearcher : BaseSearcher
    {
        public RandomSearcher(IClothDao clothDao, int maxResult)
            : base(null, clothDao, maxResult)
        {

        }

        public override List<Cloth> Search()
        {
            List<Cloth> clothes = null;
            if (clothDao != null)
            {
                clothes = clothDao.FindAll();
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

using System;
using System.Collections.Generic;
using Zju.Dao;

namespace Zju.Searcher
{
    public abstract class ColorSearcher<T> : PicSearcher<T> where T : System.IComparable<T>
    {
        public ColorSearcher(PicParam<T> picParam, T limit, DelCalcDist<T> calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, limit, calcDist, clothDao, maxResult)
        {
           
        }

        public ColorSearcher(PicParam<T> picParam, T limit, DelCalcDist<T> calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, limit, calcDist, wrappedSearcher, maxResult)
        {

        }
    }
}

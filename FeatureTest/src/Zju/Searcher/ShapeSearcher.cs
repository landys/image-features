using System;
using System.Collections.Generic;
using Zju.Dao;
using Zju.Domain;

namespace Zju.Searcher
{
    public abstract class ShapeSearcher<T> : PicSearcher<T> where T : System.IComparable<T>
    {
        public ShapeSearcher(PicParam<T> picParam, T limit, DelCalcDist<T> calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, limit, calcDist, clothDao, maxResult)
        {

        }

        public ShapeSearcher(PicParam<T> picParam, T limit, DelCalcDist<T> calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, limit, calcDist, wrappedSearcher, maxResult)
        {

        }
    }
}

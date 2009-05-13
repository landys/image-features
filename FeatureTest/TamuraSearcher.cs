using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Util;
using Zju.Dao;

namespace Zju.Searcher
{
    public class TamuraSearcher : TextureSearcher
    {
        public TamuraSearcher(PicParam picParam, float limit, IBaseSearcher wrappedSearcher, int maxResult)
            : this(picParam, limit, ClothUtil.CalcManhattanDistance, wrappedSearcher, maxResult)
        {
           
        }

        public TamuraSearcher(PicParam picParam, float limit, DelCalcDist calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, limit, calcDist, wrappedSearcher, maxResult)
        {

        }

        public TamuraSearcher(PicParam picParam, float limit, IClothDao clothDao, int maxResult)
            : this(picParam, limit, ClothUtil.CalcManhattanDistance, clothDao, maxResult)
        {

        }

        public TamuraSearcher(PicParam picParam, float limit, DelCalcDist calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, limit, calcDist, clothDao, maxResult)
        {

        }


    
        public override float[] GetVector(Cloth cloth)
        {
            return cloth.TamuraVector;
        }

    }
}

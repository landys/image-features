using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Util;
using Zju.Dao;

namespace Zju.Searcher
{
    public class GaborSearcher : TextureSearcher<float>
    {
        public GaborSearcher(PicParam<float> picParam, float limit, IBaseSearcher wrappedSearcher, int maxResult)
            : this(picParam, limit, ClothUtil.CalcGaborDistance, wrappedSearcher, maxResult)
        {
           
        }

        public GaborSearcher(PicParam<float> picParam, float limit, DelCalcDist<float> calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, limit, calcDist, wrappedSearcher, maxResult)
        {

        }

        public GaborSearcher(PicParam<float> picParam, float limit, IClothDao clothDao, int maxResult)
            : this(picParam, limit, ClothUtil.CalcGaborDistance, clothDao, maxResult)
        {

        }

        public GaborSearcher(PicParam<float> picParam, float limit, DelCalcDist<float> calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, limit, calcDist, clothDao, maxResult)
        {

        }
    
        public override float[] GetVector(Cloth cloth)
        {
            return cloth.GaborVector;
        }
    }
}

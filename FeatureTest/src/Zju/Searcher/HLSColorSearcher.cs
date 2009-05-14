using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Dao;

namespace Zju.Searcher
{
    public class HLSColorSearcher : ColorSearcher<float>
    {
        public HLSColorSearcher(PicParam<float> picParam, float limit, DelCalcDist<float> calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, limit, calcDist, wrappedSearcher, maxResult)
        {

        }

        public HLSColorSearcher(PicParam<float> picParam, float limit, DelCalcDist<float> calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, limit, calcDist, clothDao, maxResult)
        {

        }

        public override float[] GetVector(Cloth cloth)
        {
            return cloth.HLSColorVector;
        }
    }
}

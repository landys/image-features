﻿using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Util;
using Zju.Dao;

namespace Zju.Searcher
{
    public class DaubechiesWaveletSearcher : TextureSearcher<float>
    {
        public DaubechiesWaveletSearcher(PicParam<float> picParam, float limit, IBaseSearcher wrappedSearcher, int maxResult)
            : this(picParam, limit, ClothUtil.CalcManhattanDistance, wrappedSearcher, maxResult)
        {
           
        }

        public DaubechiesWaveletSearcher(PicParam<float> picParam, float limit, DelCalcDist<float> calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, limit, calcDist, wrappedSearcher, maxResult)
        {

        }

        public DaubechiesWaveletSearcher(PicParam<float> picParam, float limit, IClothDao clothDao, int maxResult)
            : this(picParam, limit, ClothUtil.CalcManhattanDistance, clothDao, maxResult)
        {

        }

        public DaubechiesWaveletSearcher(PicParam<float> picParam, float limit, DelCalcDist<float> calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, limit, calcDist, clothDao, maxResult)
        {

        }


    
        public override float[] GetVector(Cloth cloth)
        {
            return cloth.DaubechiesWaveletVector;
        }
    }
}

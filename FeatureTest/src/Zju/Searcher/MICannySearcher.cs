using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Util;
using Zju.Dao;

namespace Zju.Searcher
{
    public class MICannySearcher : ShapeSearcher<double>
    {
        public MICannySearcher(PicParam<double> picParam, double limit, IBaseSearcher wrappedSearcher, int maxResult)
            : this(picParam, limit, ClothUtil.CalcManhattanDistance, wrappedSearcher, maxResult)
        {

        }

        public MICannySearcher(PicParam<double> picParam, double limit, DelCalcDist<double> calcDist, IBaseSearcher wrappedSearcher, int maxResult)
            : base(picParam, limit, calcDist, wrappedSearcher, maxResult)
        {

        }

        public MICannySearcher(PicParam<double> picParam, double limit, IClothDao clothDao, int maxResult)
            : this(picParam, limit, ClothUtil.CalcManhattanDistance, clothDao, maxResult)
        {

        }

        public MICannySearcher(PicParam<double> picParam, double limit, DelCalcDist<double> calcDist, IClothDao clothDao, int maxResult)
            : base(picParam, limit, calcDist, clothDao, maxResult)
        {

        }



        public override double[] GetVector(Cloth cloth)
        {
            return cloth.MICannyVector;
        }
    }
}

using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Dao;

namespace Zju.Searcher
{
    public delegate T DelCalcDist<T>(T[] v1, T[] v2) where T : System.IComparable<T>;

    public abstract class BaseSearcher : IBaseSearcher
    {
        protected IBaseSearcher wrappedSearcher;
        protected IClothDao clothDao;
        protected BaseParam param;

        protected const int DEFAULT_MAX_RESULT = 200;

        public int MaxResult
        {
            get;
            set;
        }

        #region IBaseSearcher Members

        public BaseSearcher(BaseParam param, IClothDao clothDao)
            : this(param, clothDao, DEFAULT_MAX_RESULT)
        {
            
        }

        public BaseSearcher(BaseParam param, IBaseSearcher wrappedSearcher)
            : this(param, wrappedSearcher, DEFAULT_MAX_RESULT)
        {
          
        }

        public BaseSearcher(BaseParam param, IBaseSearcher wrappedSearcher, int maxResult)
        {
            this.param = param;
            this.wrappedSearcher = wrappedSearcher;
            this.MaxResult = maxResult;
        }

        public BaseSearcher(BaseParam param, IClothDao clothDao, int maxResult)
        {
            this.param = param;
            this.clothDao = clothDao;
            this.MaxResult = maxResult;
        }

        public abstract List<Cloth> Search();

        #endregion
    }
}

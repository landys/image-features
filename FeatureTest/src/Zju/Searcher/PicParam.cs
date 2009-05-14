using System;
using System.Collections.Generic;
using System.Text;

namespace Zju.Searcher
{
    public class PicParam<T> : BaseParam where T : System.IComparable<T>
    {
        public PicParam()
        {

        }

        public PicParam(T[] feature, int clothNum)
        {
            this.Feature = feature;
            this.ClothNum = clothNum;
        }

        public T[] Feature
        {
            get;
            set;
        }


        public int ClothNum
        {
            get;
            set;
        }
    }
}

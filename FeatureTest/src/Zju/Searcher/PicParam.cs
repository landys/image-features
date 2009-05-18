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

        public PicParam(T[] feature, int colorNum)
        {
            this.Feature = feature;
            this.ColorNum = colorNum;
            this.Categories = null;
        }

        public PicParam(T[] feature, int colorNum, int[] categories)
        {
            this.Feature = feature;
            this.ColorNum = colorNum;
            this.Categories = categories;
        }

        public T[] Feature { get; set; }

        public int ColorNum { get; set; }

        public int[] Categories { get; set; }
    }
}

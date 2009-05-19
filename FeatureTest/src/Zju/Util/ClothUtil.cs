using System;
using System.Collections.Generic;
using Zju.Domain;
using System.IO;
using Zju.Image;

namespace Zju.Util
{
    public sealed class ClothUtil
    {
        private static ImageMatcher imageMatcher;

        private static StreamWriter log;

        static ClothUtil()
        {
            imageMatcher = new ImageMatcher();
            imageMatcher.LuvInit(SearchConstants.LuvFileName);
        }

        public static ImageMatcher ImageMatcherInst
        {
            get { return imageMatcher; }
        }

        public static StreamWriter Log
        {
            get
            {
                if (log == null)
                {
                    if (File.Exists(SearchConstants.LogFileName))
                    {
                        log = File.AppendText(SearchConstants.LogFileName);
                    }
                    else
                    {
                        log = File.CreateText(SearchConstants.LogFileName);
                    }
                    log.AutoFlush = true;
                }
                return log;
            }
        }

        /*public static float CalcManhattanDistance(int[] v1, int[] v2)
        {
            if (v1 == null || v2 == null || v1.Length != v2.Length)
            {
                return int.MaxValue;
            }

            float mds = 0.0f;
            int n = v1.Length;
            for (int i = 0; i < n; ++i)
            {
                mds += (v1[i] >= v2[i] ? v1[i] - v2[i] : v2[i] - v1[i]);
            }

            return mds / n;
        }
        */
        public static float CalcManhattanDistance(float[] v1, float[] v2)
        {
            if (v1 == null || v2 == null || v1.Length != v2.Length)
            {
                return float.MaxValue;
            }

            float mds = 0.0f;
            int n = v1.Length;
            for (int i = 0; i < n; ++i)
            {
                mds += (v1[i] >= v2[i] ? v1[i] - v2[i] : v2[i] - v1[i]);
            }

            return mds / n;
        }

        public static double CalcManhattanDistance(double[] v1, double[] v2)
        {
            if (v1 == null || v2 == null || v1.Length != v2.Length)
            {
                return double.MaxValue;
            }

            double mds = 0.0;
            int n = v1.Length;
            for (int i = 0; i < n; ++i)
            {
                mds += (v1[i] >= v2[i] ? v1[i] - v2[i] : v2[i] - v1[i]);
            }

            return mds / n;
        }

        public static float CalcEuclidDistance(float[] v1, float[] v2)
        {
            if (v1 == null || v2 == null || v1.Length != v2.Length)
            {
                return float.MaxValue;
            }

            float mds = 0.0f;
            int n = v1.Length;
            float t = 0.0f;
            for (int i = 0; i < n; ++i)
            {
                t = v1[i] - v2[i];
                mds += t * t;
            }

            return mds / n;
        }

        public static double CalcEuclidDistance(double[] v1, double[] v2)
        {
            if (v1 == null || v2 == null || v1.Length != v2.Length)
            {
                return double.MaxValue;
            }

            double mds = 0.0;
            int n = v1.Length;
            double t = 0.0f;
            for (int i = 0; i < n; ++i)
            {
                t = v1[i] - v2[i];
                mds += t * t;
            }

            return mds / n;
        }

        public static float CalcGaborDistance(float[] v1, float[] v2)
        {
            if (v1 == null || v2 == null || v1.Length != v2.Length)
            {
                return float.MaxValue;
            }

            float total = 0.0f;
            int n = v1.Length;
            for (int i=1; i<n; i+=2)
            {
                float t1 = v1[i - 1] - v2[i - 1];
                float t2 = (float)(Math.Sqrt(v1[i]) - Math.Sqrt(v2[i]));
                total += (float)Math.Sqrt(t1 * t1 + t2 * t2);
            }

            return total;
        }

        /// <summary>
        /// Each cloth list in <code>clothLists</code> should not contain duplicate element, or the algorithm will be acted correctly.
        /// </summary>
        /// <param name="clothLists"></param>
        /// <returns></returns>
        public static List<Cloth> IntersectClothLists(List<List<Cloth>> clothLists)
        {
            if (clothLists.Count == 0)
            {
                return new List<Cloth>();
            }
            if (clothLists.Count == 1)
            {
                return clothLists[0];
            }

            Dictionary<Cloth, int> tc = new Dictionary<Cloth, int>();
            foreach (List<Cloth> clothList in clothLists)
            {
                foreach (Cloth cloth in clothList)
                {
                    if (!tc.ContainsKey(cloth))
                    {
                        tc[cloth] = 1;
                    }
                    else
                    {
                        tc[cloth]++;
                    }
                }
            }

            int nLists = clothLists.Count;
            List<Cloth> result = new List<Cloth>();
            foreach (KeyValuePair<Cloth, int> kvp in tc)
            {
                if (kvp.Value == nLists)
                {
                    result.Add(kvp.Key);
                }
            }

            return result;
        }

        public static List<Cloth> UnionClothLists(List<List<Cloth>> clothLists)
        {
            if (clothLists.Count == 0)
            {
                return new List<Cloth>();
            }
            if (clothLists.Count == 1)
            {
                return clothLists[0];
            }

            HashSet<Cloth> hs = new HashSet<Cloth>();
            foreach (List<Cloth> clothList in clothLists)
            {
                foreach (Cloth cloth in clothList)
                {
                    hs.Add(cloth);
                }
            }

            List<Cloth> result = new List<Cloth>();
            foreach (Cloth cloth in hs)
            {
                result.Add(cloth);
            }

            return result;
        }

        /// <summary>
        /// Extract color and texture features for the cloth picture.
        /// And save the features back into the <code>cloth</code> objects.
        /// </summary>
        /// <param name="cloth"></param>
        /*public static void ExtractFeatures(Cloth cloth, bool isGabor)
        {
            if (String.IsNullOrEmpty(cloth.Path))
            {
                return;
            }

            // color features
            //cloth.RGBSeparateColorVector = ImageMatcherInst.ExtractRGBSeparateColorVector(cloth.Path, 8, SearchConstants.IgnoreColors);
            cloth.RGBColorVector = ImageMatcherInst.ExtractRGBColorVector(cloth.Path, 4, SearchConstants.IgnoreColors);
            cloth.HSVAynsColorVector = ImageMatcherInst.ExtractHSVAynsColorVector(cloth.Path, 0, SearchConstants.IgnoreColors);
            cloth.HSVColorVector = ImageMatcherInst.ExtractHSVColorVector(cloth.Path, 4, SearchConstants.IgnoreColors);
            //cloth.HLSColorVector = ImageMatcherInst.ExtractHLSColorVector(cloth.Path, 8, SearchConstants.IgnoreColors);

            // texture features
            cloth.DaubechiesWaveletVector = ImageMatcherInst.ExtractDaubechiesWaveletVector(cloth.Path);
            cloth.CooccurrenceVector = ImageMatcherInst.ExtractCooccurrenceVector(cloth.Path);
            //if (isGabor)
            //{
             //   cloth.GaborVector = ImageMatcherInst.ExtractGaborVector(cloth.Path);
            //}

            cloth.ColorNum = getColorNumber(cloth.HSVColorVector, 0.07f);
        }*/

        public static void ExtractFeaturesNecessary(Cloth cloth, bool isGabor)
        {
            if (String.IsNullOrEmpty(cloth.Path))
            {
                return;
            }

            // color features
           if (null == cloth.RGBSeparateColorVector)
           {
                cloth.RGBSeparateColorVector = ImageMatcherInst.ExtractRGBSeparateColorVector(cloth.Path, 8, SearchConstants.IgnoreColors);
            }
            if (null == cloth.RGBColorVector)
            {
                cloth.RGBColorVector = ImageMatcherInst.ExtractRGBColorVector(cloth.Path, 4, SearchConstants.IgnoreColors);
            }
            if (null == cloth.HSVAynsColorVector)
            {
                cloth.HSVAynsColorVector = ImageMatcherInst.ExtractHSVAynsColorVector(cloth.Path, 0, SearchConstants.IgnoreColors);
            }
            if (null == cloth.HSVColorVector)
            {
                cloth.HSVColorVector = ImageMatcherInst.ExtractHSVColorVector(cloth.Path, 4, SearchConstants.IgnoreColors);
                int colorNum = getColorNumber(cloth.HSVColorVector, 0.07f);
                if (colorNum > 8)
                {
                    colorNum = 8;
                }
                cloth.ColorNum = colorNum;
            }
            if (null == cloth.HLSColorVector)
            {
                cloth.HLSColorVector = ImageMatcherInst.ExtractHLSColorVector(cloth.Path, 4, SearchConstants.IgnoreColors);
            }
            
            // texture features
            if (null == cloth.DaubechiesWaveletVector)
            {
                cloth.DaubechiesWaveletVector = ImageMatcherInst.ExtractDaubechiesWaveletVector(cloth.Path);
            }
            if (null == cloth.CooccurrenceVector)
            {
                cloth.CooccurrenceVector = ImageMatcherInst.ExtractCooccurrenceVector(cloth.Path);
            }
            if (null == cloth.TamuraVector)
            {
                cloth.TamuraVector = ImageMatcherInst.ExtractTamuraVector(cloth.Path, 6);
            }
            if (null == cloth.MICannyVector)
            {
                cloth.MICannyVector = ImageMatcherInst.ExtractMICannyVector(cloth.Path);
            }

            if (null == cloth.MIHuVector)
            {
                cloth.MIHuVector = ImageMatcherInst.ExtractMIHuVector(cloth.Path);
            }

            if (null == cloth.FourierVector)
            {
                cloth.FourierVector = ImageMatcherInst.ExtractFourierVector(cloth.Path, 20);
            }

            //if (isGabor && null == cloth.GaborVector)
            //{
            //    cloth.GaborVector = ImageMatcherInst.ExtractGaborVector(cloth.Path);
            //}

            
        }

        /// <summary>
        /// Extract pattern string from the picture name. I.e.
        /// C;\man t-shirt\bcd.jpg -> "man t-shirt"
        /// </summary>
        /// <param name="picName"></param>
        /// <returns></returns>
        public static string ExtractPattern(string picName)
        {
            if (string.IsNullOrEmpty(picName))
            {
                return null;
            }

            int i = picName.LastIndexOfAny(new char[] { '/', '\\' });
            if (i <= 0)
            {
                return null;
            }
            int j = picName.LastIndexOfAny(new char[] { '/', '\\' }, i - 1);

            return i - j - 1 > 0 ? picName.Substring(j + 1, i - j - 1) : null;
        }

        /// <summary>
        /// Extract picture name string from the picture file name. I.e.
        /// C;\a\bcd.jpg -> bcd
        /// </summary>
        /// <param name="picName"></param>
        /// <returns></returns>
        public static string ExtractName(string picName)
        {
            if (string.IsNullOrEmpty(picName))
            {
                return null;
            }

            int i = picName.LastIndexOf('.');
            int j = picName.LastIndexOfAny(new char[] { '/', '\\' });

            j = j == -1 ? 0 : j;
            i = i == -1 ? picName.Length : i;

            return i - j - 1 > 0 ? picName.Substring(j + 1, i - j - 1) : null;
        }

        public static Cloth GenerateClothObject(string picName, bool isGabor)
        {
            Cloth cloth = new Cloth();

            cloth.Path = picName;
            cloth.Pattern = ClothUtil.ExtractPattern(cloth.Path);
            cloth.Category = ClothUtil.calcCategoryValue(cloth.Pattern);
            cloth.Name = ClothUtil.ExtractName(cloth.Path);

            //ClothUtil.Log.WriteLine("begin ExtractFeatures");
            ClothUtil.ExtractFeaturesNecessary(cloth, isGabor);

            

            return cloth;
        }

        public static int getColorNumber(float[] v, float limit)
        {
            if (v == null)
            {
                return 0;
            }

            int count = 0;
            foreach (float f in v)
            {
                if (f >= limit)
                {
                    ++count;
                }
            }
            return count;
        }

        public static int calcCategoryValue(string strCategory)
        {
            if (strCategory == null || strCategory.Trim().Length == 0)
            {
                return 0;
            }

            string[] ss = strCategory.Trim().Split(new char[] { SearchConstants.CATEGORY_GAP });
            Dictionary<String, int>[] dics = new Dictionary<string,int>[] { SearchConstants.INVERSE_CATEGORY_MAP_1ST,
                SearchConstants.INVERSE_CATEGORY_MAP_2ND, SearchConstants.INVERSE_CATEGORY_MAP_3RD };
            int[] defValues = new int[] { (int) SearchConstants.DEFAULT_CATEGORY_1ST, 
                (int) SearchConstants.DEFAULT_CATEGORY_2ND, (int) SearchConstants.DEFAULT_CATEGORY_3RD };
            int[] coefs = new int[] { SearchConstants.CATEGORY_COEF * SearchConstants.CATEGORY_COEF, SearchConstants.CATEGORY_COEF, 1 };

            int valCategory = 0;
            for (int i = 0; i < ss.Length && i < dics.Length; ++i)
            {
                int t = 0;
                if (dics[i].ContainsKey(ss[i]))
                {
                    t = (int)(dics[i][ss[i]]);
                }
                else
                {
                    t = (int)(defValues[i]);
                }
                valCategory += coefs[i] * t;
            }

            return valCategory;
        }

        public static string calcCategoryString(int valCategory)
        {
            if (valCategory <= 0)
            {
                return SearchConstants.DEFAULT_CATEGORY_NAME;
            }

            Dictionary<int, string>[] dics = new Dictionary<int, string>[] { SearchConstants.CATEGORY_MAP_1ST,
                SearchConstants.CATEGORY_MAP_2ND, SearchConstants.CATEGORY_MAP_3RD };
            int[] coefs = new int[] { SearchConstants.CATEGORY_COEF * SearchConstants.CATEGORY_COEF, SearchConstants.CATEGORY_COEF, 1 };

            string strCategory = "";
            for (int i = 0; i < dics.Length; ++i)
            {
                int t = valCategory / coefs[i];
                if (t == 0)
                {
                    break;
                }

                if (i > 0)
                {
                    strCategory += SearchConstants.CATEGORY_GAP;
                }

                if (dics[i].ContainsKey(t))
                {
                    strCategory += dics[i][t];
                }
                else
                {
                    strCategory += SearchConstants.DEFAULT_CATEGORY_NAME;
                }

                valCategory %= coefs[i];
            }


            if (strCategory.Length == 0)
            {
                strCategory = SearchConstants.DEFAULT_CATEGORY_NAME;
            }

            return strCategory;
        }
    }
}

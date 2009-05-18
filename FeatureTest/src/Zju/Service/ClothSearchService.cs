using System;
using System.Collections.Generic;
using Zju.Util;
using Zju.Domain;
using Zju.Dao;
using Zju.Searcher;

namespace Zju.Service
{
    public class ClothSearchService : IClothSearchService
    {

        private class ClothWeight
        {
            public byte[] flags;
            public float weight;

            public ClothWeight()
            {

            }

            public ClothWeight(byte[] flags, float weight)
            {
                this.flags = flags;
                this.weight = weight;
            }
        }

        private delegate List<Cloth> DelSearchByPic(Cloth keyCloth, int[] categories, int reSize);

        private IClothDao clothDao;

        private float colorMDLimit;

        private float textureMDLimit;

        private float gaborMDLimit;

        private float cooccurrenceMDLimit;

        private Dictionary<AlgorithmType, DelSearchByPic> algoMap = new Dictionary<AlgorithmType,DelSearchByPic>();

        public ClothSearchService()
        {
            colorMDLimit = SearchConstants.ColorMDLimits[0];
            textureMDLimit = SearchConstants.TextureMDLimits[0];
            clothDao = new ClothDao();

            algoMap.Add(AlgorithmType.DaubechiesWavelet, SearchByPicDaubechiesWavelet);
            algoMap.Add(AlgorithmType.Cooccurrence, SearchByPicCooccurrence);
            algoMap.Add(AlgorithmType.RGBSeparateColor, SearchByPicRGBSeparateColor);
            algoMap.Add(AlgorithmType.RGBColor, SearchByPicRGBColor);
            algoMap.Add(AlgorithmType.HSVAynsColor, SearchByPicHSVAynsColor);
            algoMap.Add(AlgorithmType.HSVColor, SearchByPicHSVColor);
            algoMap.Add(AlgorithmType.HLSColor, SearchByPicHLSColor);
            algoMap.Add(AlgorithmType.Tamura, SearchByPicTamura);
            algoMap.Add(AlgorithmType.MICanny, SearchByPicMICanny);
            algoMap.Add(AlgorithmType.MIHu, SearchByPicMIHu);
            algoMap.Add(AlgorithmType.Fourier, SearchByPicFourier);
        }

        #region IClothSearchService Members

        private float calcWeight(int rank, float weight)
        {
            return rank * weight;
        }

        public List<Cloth> SearchByPicCombine(Cloth keyCloth, AlgorithmType[] aTypes, float[] weights, int[] categories)
        {
            int nt = aTypes.Length;
            if (aTypes == null || weights == null || nt != weights.Length)
            {
                return new List<Cloth>();
            }
            
            if (nt == 1)
            {
                return algoMap[aTypes[0]](keyCloth, categories, SearchConstants.MAX_FINAL_RESULT_SIZE);
            }

            Dictionary<Cloth, ClothWeight> cd = new Dictionary<Cloth, ClothWeight>();
            for (int i=0; i<nt; ++i)
            {
                AlgorithmType aType = aTypes[i];
                float w = weights[i];
                if (algoMap.ContainsKey(aType))
                {
                    List<Cloth> clothes = algoMap[aType](keyCloth, categories, SearchConstants.MAX_INTER_RESULT_SIZE);
                    for (int j=0; j<clothes.Count; ++j)
                    {
                        Cloth c = clothes[j];

                        if (cd.ContainsKey(c))
                        {
                            ClothWeight v = cd[c];
                            v.flags[i] = 1;
                            v.weight += calcWeight(j, w);
                        }
                        else
                        {
                            byte[] flags = new byte[nt];
                            flags[i] = 1;
                            cd[c] = new ClothWeight(flags, calcWeight(j, w));
                        }
                    }
                }
            }

            SortedDictionary<float, List<Cloth>> scd = new SortedDictionary<float,List<Cloth>>();
            foreach (KeyValuePair<Cloth, ClothWeight> kv in cd)
            {
                float cw = kv.Value.weight;
                byte[] cb = kv.Value.flags;
                for (int i=0; i<nt; ++i)
                {
                    if (cb[i] == 0)
                    {
                        cw += calcWeight(SearchConstants.MAX_INTER_RESULT_SIZE, weights[i]);
                    }
                }
                if (scd.ContainsKey(cw))
                {
                    scd[cw].Add(kv.Key);
                }
                else
                {
                    List<Cloth> lc = new List<Cloth>();
                    lc.Add(kv.Key);
                    scd[cw] = lc;
                }
            }

            List<Cloth> resultClothes = new List<Cloth>();
            foreach (List<Cloth> cs in scd.Values)
            {
                resultClothes.AddRange(cs);
            }

            if (resultClothes.Count > SearchConstants.MAX_FINAL_RESULT_SIZE)
            {
                return resultClothes.GetRange(0, SearchConstants.MAX_FINAL_RESULT_SIZE);
            }

            return resultClothes;
        }

        public List<Cloth> SearchByRandom(int[] categories, int reSize)
        {
            return new RandomSearcher(clothDao, categories, reSize).Search();
        }

        /*public List<Cloth> SearchByText(string words, ColorEnum colors, ShapeEnum shapes)
        {
            return new TextSearcher(new TextParam(words, colors, shapes), clothDao).Search();
        }*/

        public List<Cloth> SearchByPicRGBSeparateColor(Cloth keyCloth, int[] categories, int reSize)
        {
            return new RGBSeparateColorSearcher(new PicParam<float>(keyCloth.RGBSeparateColorVector, keyCloth.ColorNum, categories), float.MaxValue/*colorMDLimit*/, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicRGBColor(Cloth keyCloth, int[] categories, int reSize)
        {
            return new RGBColorSearcher(new PicParam<float>(keyCloth.RGBColorVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicHSVColor(Cloth keyCloth, int[] categories, int reSize)
        {
            return new HSVColorSearcher(new PicParam<float>(keyCloth.HSVColorVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicHSVAynsColor(Cloth keyCloth, int[] categories, int reSize)
        {
            return new HSVAynsColorSearcher(new PicParam<float>(keyCloth.HSVAynsColorVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicHLSColor(Cloth keyCloth, int[] categories, int reSize)
        {
            return new HLSColorSearcher(new PicParam<float>(keyCloth.HLSColorVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicDaubechiesWavelet(Cloth keyCloth, int[] categories, int reSize)
        {
            return new DaubechiesWaveletSearcher(new PicParam<float>(keyCloth.DaubechiesWaveletVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcManhattanDistance,
                 clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicGabor(Cloth keyCloth, int[] categories, int reSize)
        {
            return new GaborSearcher(new PicParam<float>(keyCloth.GaborVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcGaborDistance, clothDao, reSize)
                .Search();
        }

        /*public List<Cloth> SearchByPicGaborAndHSVAsyColor(Cloth keyCloth, int reSize)
        {
            return new GaborSearcher(new PicParam<float>(keyCloth.GaborVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance,
                new HSVAynsColorSearcher(new PicParam<float>(keyCloth.HSVAynsColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize), reSize).Search();
        }*/

        public List<Cloth> SearchByPicCooccurrence(Cloth keyCloth, int[] categories, int reSize)
        {
            return new CooccurrenceSearcher(new PicParam<float>(keyCloth.CooccurrenceVector, keyCloth.ColorNum, categories), float.MaxValue/*cooccurrenceMDLimit*/, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicTamura(Cloth keyCloth, int[] categories, int reSize)
        {
            return new TamuraSearcher(new PicParam<float>(keyCloth.TamuraVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicMICanny(Cloth keyCloth, int[] categories, int reSize)
        {
            return new MICannySearcher(new PicParam<double>(keyCloth.MICannyVector, keyCloth.ColorNum, categories), double.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicMIHu(Cloth keyCloth, int[] categories, int reSize)
        {
            return new MIHuSearcher(new PicParam<double>(keyCloth.MIHuVector, keyCloth.ColorNum, categories), double.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, reSize)
                .Search();
        }

        public List<Cloth> SearchByPicFourier(Cloth keyCloth, int[] categories, int reSize)
        {
            return new FourierSearcher(new PicParam<float>(keyCloth.FourierVector, keyCloth.ColorNum, categories), float.MaxValue, ClothUtil.CalcEuclidDistance, clothDao, reSize)
                .Search();
        }
        /*
        public List<Cloth> SearchByTextAndPicColor(String words, ColorEnum colors, ShapeEnum shapes, Cloth keyCloth)
        {
            return new RGBColorSearcher(new PicParam<float>(keyCloth.RGBColorVector, keyCloth.ColorNum), colorMDLimit, ClothUtil.CalcManhattanDistance,
                new TextSearcher(new TextParam(words, colors, shapes), clothDao), 200).Search();
        }

        public List<Cloth> SearchByTextAndPicTexture(String words, ColorEnum colors, ShapeEnum shapes, Cloth keyCloth)
        {
            return new DaubechiesWaveletSearcher(new PicParam<float>(keyCloth.DaubechiesWaveletVector, keyCloth.ColorNum), textureMDLimit, ClothUtil.CalcManhattanDistance,
                new TextSearcher(new TextParam(words, colors, shapes), clothDao), 200).Search();
        }

        public List<Cloth> SearchTest(Cloth keyCloth)
        {
            ClothUtil.ExtractFeaturesNecessary(keyCloth, false);
            return new RGBColorSearcher(new PicParam<float>(keyCloth.RGBColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100).Search();
            //return new DaubechiesWaveletSearcher(new PicParam<float>(keyCloth.DaubechiesWaveletVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance,
              //  new RGBColorSearcher(new PicParam<float>(keyCloth.RGBColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100), 200).Search();
        }

        public List<Cloth> SearchTest2(Cloth keyCloth)
        {
            ClothUtil.ExtractFeaturesNecessary(keyCloth, false);
            return new HSVAynsColorSearcher(new PicParam<float>(keyCloth.HSVAynsColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100).Search();
            //return new DaubechiesWaveletSearcher(new PicParam<float>(keyCloth.DaubechiesWaveletVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance,
              //  new HSVAynsColorSearcher(new PicParam<float>(keyCloth.HSVAynsColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100), 200).Search();
        }

        public List<Cloth> SearchTest3(Cloth keyCloth)
        {
            ClothUtil.ExtractFeaturesNecessary(keyCloth, false);
            return new HSVColorSearcher(new PicParam<float>(keyCloth.HSVColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100).Search();
            //return new DaubechiesWaveletSearcher(new PicParam<float>(keyCloth.DaubechiesWaveletVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance,
              //  new HSVColorSearcher(new PicParam<float>(keyCloth.HSVColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100), 200).Search();
        }

        public List<Cloth> SearchTest4(Cloth keyCloth)
        {
            ClothUtil.ExtractFeaturesNecessary(keyCloth, false);
            return new HLSColorSearcher(new PicParam<float>(keyCloth.HLSColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100).Search();
            //return new DaubechiesWaveletSearcher(new PicParam<float>(keyCloth.DaubechiesWaveletVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance,
              //  new HLSColorSearcher(new PicParam<float>(keyCloth.HLSColorVector, keyCloth.ColorNum), float.MaxValue, ClothUtil.CalcManhattanDistance, clothDao, 100), 200).Search();
        }
        */
        public float GetColorMDLimit()
        {
            return colorMDLimit;
        }

        public void SetColorMDLimit(float colorMDLimit)
        {
            this.colorMDLimit = colorMDLimit;
        }

        public float GetTextureMDLimit()
        {
            return textureMDLimit;
        }

        public void SetTextureMDLimit(float textureMDLimit)
        {
            this.textureMDLimit = textureMDLimit;
        }

        public float GetGaborMDLimit()
        {
            return gaborMDLimit;
        }

        public void SetGaborMDLimit(float gaborMDLimit)
        {
            this.gaborMDLimit = gaborMDLimit;
        }

        public float GetCooccurrenceMDLimit()
        {
            return cooccurrenceMDLimit;
        }

        public void SetCooccurrenceMDLimit(float cooccurrenceMDLimit)
        {
            this.cooccurrenceMDLimit = cooccurrenceMDLimit;
        }

        #endregion

        public IClothDao ClothDao
        {
            set { this.clothDao = value; }
        }
    }
}

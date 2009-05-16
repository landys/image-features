using System;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Util;

namespace Zju.Service
{
    public interface IClothSearchService
    {
        List<Cloth> SearchByPicCombine(Cloth keyCloth, AlgorithmType[] aTypes, float[] weights);

        List<Cloth> SearchByRandom(int reSize);

        List<Cloth> SearchByText(String words, ColorEnum colors, ShapeEnum shapes);

        List<Cloth> SearchByPicRGBSeparateColor(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicRGBColor(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicHSVColor(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicHSVAynsColor(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicHLSColor(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicDaubechiesWavelet(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicGabor(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicCooccurrence(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicTamura(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicMICanny(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicMIHu(Cloth keyCloth, int reSize);

        List<Cloth> SearchByPicFourier(Cloth keyCloth, int reSize);

        List<Cloth> SearchByTextAndPicColor(String words, ColorEnum colors, ShapeEnum shapes, Cloth keyCloth);

        List<Cloth> SearchByTextAndPicTexture(String words, ColorEnum colors, ShapeEnum shapes, Cloth keyCloth);

        List<Cloth> SearchByPicGaborAndHSVAsyColor(Cloth keyCloth, int reSize);

        List<Cloth> SearchTest(Cloth keyCloth);

        List<Cloth> SearchTest2(Cloth keyCloth);

        List<Cloth> SearchTest3(Cloth keyCloth);

        List<Cloth> SearchTest4(Cloth keyCloth);

        float GetColorMDLimit();
        void SetColorMDLimit(float colorMDLimit);

        float GetTextureMDLimit();
        void SetTextureMDLimit(float textureMDLimit);

        float GetGaborMDLimit();
        void SetGaborMDLimit(float gaborMDLimit);

        float GetCooccurrenceMDLimit();
        void SetCooccurrenceMDLimit(float cooccurrenceMDLimit);
    }
}

namespace Zju.Util
{
    public enum AlgorithmType
    {
        DaubechiesWavelet, Cooccurrence, RGBSeparateColor, RGBColor, HSVColor, HSVAynsColor, HLSColor, Tamura, Gabor,
    };

    public enum RecallLevel
    {
        Default, Recall1, Recall2, Recall3,
    };

    public sealed class AlgorithmDesc
    {
        public AlgorithmType AType { get; set; }
        public RecallLevel RLevel { get; set; }

        public AlgorithmDesc()
        {
            AType = AlgorithmType.DaubechiesWavelet;
            RLevel = RecallLevel.Default;
        }
    }
}

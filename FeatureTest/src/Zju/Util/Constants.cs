using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Zju.Util
{
    public sealed class AppConstants
    {
        public const string ConfPropsFileName = @"data\cloth.properties";

        public const string DataPathKey = "dataPath";

        public const string DataBaseFileKey = "dbFileName";

        public const string LuvFileNameKey = "luvFileName";

        public const string LogFileNameKey = "logFileName";

        public static FileProperties ConfProps = new FileProperties(ConfPropsFileName);

        public static string DataPath = ConfProps[DataPathKey];
    }
    
    public sealed class DbConstants
    {
        // key in cloth.properties file.
        public static string DataBaseFilePath = AppConstants.DataPath + AppConstants.ConfProps[AppConstants.DataBaseFileKey];
        public const int PagePoolSize = 48 * 1024 * 1024;
        public const int ComitLimit = 100000;
    }

    public sealed class SearchConstants
    {
        /// <summary>
        /// Limit of Manhattan distance of color vectors between two cloth when searching.
        /// The cloth as searched should less or equal to the limit.
        /// 500, 2000, 6000
        /// </summary>
        public static float[] ColorMDLimits = { float.MaxValue - 1, 686.0f, 1064.0f, 1656.0f, float.MaxValue - 1 };

        public static float[] TextureMDLimits = { float.MaxValue - 1, 80.5f, 132.6f, 220.8f, float.MaxValue - 1 };

        public static float[] GaborMDLimits = { float.MaxValue - 1, 10700.0f, 42500.0f, 135000.0f, float.MaxValue - 1 };

        public static float[] CooccurrenceMDLimits = { float.MaxValue - 1, 0.6f, 0.94f, 1.59f, float.MaxValue - 1 };

        public static int[] IgnoreColors = { -1 };

        public static string LuvFileName = AppConstants.DataPath + AppConstants.ConfProps[AppConstants.LuvFileNameKey];

        public static string LogFileName = AppConstants.DataPath + AppConstants.ConfProps[AppConstants.LogFileNameKey];

        public const int MAX_IMPORT_THREADS = 100;

        public const int MAX_INTER_RESULT_SIZE = 2000;

        public const int MAX_FINAL_RESULT_SIZE = 200;

        // the gap must larger than the size of each category level
        public const int CATEGORY_COEF = 100;

        public const char CATEGORY_GAP = ' ';

        public static string[] CATEGORY_ALL = new string[] { "男装 T恤 无袖", "男装 T恤 短袖", "男装 T恤 长袖", "男装 中老年服装", 
            "男装 休闲 短袖衬衫", "男装 休闲 长袖衬衫", "男装 内衣", "男装 内裤", "男装 唐装", "男装 外套 休闲西装", "男装 外套 卫衣",
            "男装 外套 大衣", "男装 外套 夹克", "男装 外套 棉衣", "男装 外套 牛仔服", "男装 外套 皮衣", "男装 外套 羽绒服", 
            "男装 外套 风衣", "男装 外套 马甲背心", "男装 家居服睡衣", "男装 正装 短袖衬衫", "男装 正装 长袖衬衫", "男装 毛衣", 
            "男装 裤子", "男装 裤子 中短裤", "男装 裤子 休闲长裤", "男装 裤子 牛仔裤", "男装 西装 套装", "男装 西装 西裤", "男装 针织衫",
            "女装 T恤 中袖", "女装 T恤 无袖", "女装 T恤 短袖", "女装 T恤 长袖", "女装 中老年服装", "女装 内衣 保暖内衣", 
            "女装 内衣 内衣套装", "女装 内衣 内裤", "女装 内衣 塑身内衣", "女装 内衣 家居服", "女装 内衣 文胸", "女装 内衣 浴衣",
            "女装 内衣 睡衣", "女装 吊带", "女装 唐装", "女装 外套 卫衣", "女装 外套 夹克", "女装 外套 小西装", "女装 外套 棉衣",
            "女装 外套 皮草", "女装 外套 皮衣", "女装 外套 羽绒服", "女装 外套 风衣", "女装 外套 马甲", "女装 婚纱", "女装 旗袍", 
            "女装 无袖 针织衫", "女装 短袖 针织衫", "女装 礼服", "女装 职业套装", "女装 背心", "女装 蕾丝 中袖", "女装 蕾丝 无袖",
            "女装 蕾丝 短袖", "女装 蕾丝 长袖", "女装 衬衫 无袖", "女装 衬衫 短袖", "女装 衬衫 长袖", "女装 裙子 中袖连衣裙", 
            "女装 裙子 中裙", "女装 裙子 吊带裙", "女装 裙子 牛仔裙", "女装 裙子 短袖连衣裙", "女装 裙子 短裙", "女装 裙子 背心裙", 
            "女装 裙子 长袖连衣裙", "女装 裙子 长裙", "女装 裤子 中裤", "女装 裤子 休闲长裤", "女装 裤子 小脚裤", "女装 裤子 热裤",
            "女装 裤子 牛仔裤", "女装 裤子 短裤", "女装 裤子 背带裤", "女装 裤子 裙裤", "女装 裹胸", "女装 运动套装", "女装 长袖 针织衫", 
            "女装 雪纺 中袖", "女装 雪纺 无袖", "女装 雪纺 短袖", "女装 雪纺 长袖",
            "童装 女童 T恤", "童装 女童 内衣", "童装 女童 外套", "童装 女童 套装", "童装 女童 毛衫", "童装 女童 绒衫", "童装 女童 背心",
            "童装 女童 衬衫", "童装 女童 裙子", "童装 女童 裤子", "童装 女童 针织衫", "童装 女童 马甲", "童装 男童 T恤", "童装 男童 内衣",
            "童装 男童 外套", "童装 男童 套装", "童装 男童 毛衫", "童装 男童 绒衫", "童装 男童 背心", "童装 男童 衬衫", "童装 男童 裤子",
            "童装 男童 针织衫", "童装 男童 马甲", 
            "运动服 泳衣", "运动服 运动外套", "运动服 运动套装",
            "孕妇装 孕妇内衣", "孕妇装 普通孕妇装", "孕妇装 防辐射服",
            "其他" };

        public static Dictionary<int, String> CATEGORY_MAP_1ST = new Dictionary<int, string>();

        public static Dictionary<String, int> INVERSE_CATEGORY_MAP_1ST = new Dictionary<string, int>();

        public static Dictionary<int, String> CATEGORY_MAP_2ND = new Dictionary<int, string>();

        public static Dictionary<String, int> INVERSE_CATEGORY_MAP_2ND = new Dictionary<string, int>();

        public static Dictionary<int, String> CATEGORY_MAP_3RD = new Dictionary<int, string>();

        public static Dictionary<String, int> INVERSE_CATEGORY_MAP_3RD = new Dictionary<string, int>();

        public const Category1st DEFAULT_CATEGORY_1ST = Category1st.OTHERS;

        public const Category2nd DEFAULT_CATEGORY_2ND = Category2nd.OTHERS;

        public const Category3rd DEFAULT_CATEGORY_3RD = Category3rd.OTHERS;

        public const string DEFAULT_CATEGORY_NAME = "其他";
        
        static SearchConstants()
        {
            // first category
            CATEGORY_MAP_1ST[(int)Category1st.MAN] = "男装";
            CATEGORY_MAP_1ST[(int)Category1st.WOMAN] = "女装";
            CATEGORY_MAP_1ST[(int)Category1st.CHILD] = "童装";
            CATEGORY_MAP_1ST[(int)Category1st.SPORT] = "运动服";
            CATEGORY_MAP_1ST[(int)Category1st.MATERNITY] = "孕妇装";

            CATEGORY_MAP_1ST[(int)Category1st.OTHERS] = "其他";

            // second category
            CATEGORY_MAP_2ND[(int)Category2nd.TSHIRT] = "T恤";
            CATEGORY_MAP_2ND[(int)Category2nd.PAJAMAS] = "家居服睡衣";
            CATEGORY_MAP_2ND[(int)Category2nd.TROUSERS] = "裤子";
            CATEGORY_MAP_2ND[(int)Category2nd.SWEATER] = "毛衣";
            CATEGORY_MAP_2ND[(int)Category2nd.UNDERPANTS] = "内裤";
            CATEGORY_MAP_2ND[(int)Category2nd.UNDERWEAR] = "内衣";
            CATEGORY_MAP_2ND[(int)Category2nd.TANG_SUIT] = "唐装";
            CATEGORY_MAP_2ND[(int)Category2nd.COAT] = "外套";
            CATEGORY_MAP_2ND[(int)Category2nd.SUIT] = "西装";
            CATEGORY_MAP_2ND[(int)Category2nd.SPORTSWEAR] = "休闲";
            CATEGORY_MAP_2ND[(int)Category2nd.KNIT] = "针织衫";
            CATEGORY_MAP_2ND[(int)Category2nd.FORMAL_DRESS] = "正装";
            CATEGORY_MAP_2ND[(int)Category2nd.OLDER] = "中老年服装";

            CATEGORY_MAP_2ND[(int)Category2nd.SHIRT] = "衬衫";
            CATEGORY_MAP_2ND[(int)Category2nd.VEST] = "背心";
            CATEGORY_MAP_2ND[(int)Category2nd.CAMISOLE] = "吊带";
            CATEGORY_MAP_3RD[(int)Category2nd.SHORT_SLEEVE] = "短袖";
            CATEGORY_MAP_2ND[(int)Category2nd.PECTORAL] = "裹胸";
            CATEGORY_MAP_2ND[(int)Category2nd.BRIDAL_VEIL] = "婚纱";
            CATEGORY_MAP_2ND[(int)Category2nd.EVENING_DRESS] = "蕾丝";
            CATEGORY_MAP_2ND[(int)Category2nd.CHEONGSAM] = "旗袍";
            CATEGORY_MAP_2ND[(int)Category2nd.DRESS] = "裙子";
            CATEGORY_MAP_3RD[(int)Category2nd.SLEEVELESS] = "无袖";
            CATEGORY_MAP_2ND[(int)Category2nd.CHIFFON] = "雪纺";
            CATEGORY_MAP_2ND[(int)Category2nd.GYM_SUIT] = "运动套装";
            CATEGORY_MAP_2ND[(int)Category2nd.LONG_SLEEVE] = "长袖";
            CATEGORY_MAP_2ND[(int)Category2nd.BUSINESS_SUIT] = "职业套装";

            CATEGORY_MAP_2ND[(int)Category2nd.BOY] = "男童";
            CATEGORY_MAP_2ND[(int)Category2nd.GIRL] = "女童";

            CATEGORY_MAP_2ND[(int)Category2nd.SWIMSUIT] = "泳衣";
            CATEGORY_MAP_2ND[(int)Category2nd.TRACK_SUIT] = "运动外套";

            CATEGORY_MAP_2ND[(int)Category2nd.RADIATION_PROOF] = "防辐射服";
            CATEGORY_MAP_2ND[(int)Category2nd.COMMON] = "普通孕妇装";
            CATEGORY_MAP_2ND[(int)Category2nd.MATERNAL_UNDERWEAR] = "孕妇内衣";

            CATEGORY_MAP_2ND[(int)Category2nd.OTHERS] = "其他";

            // third category
            CATEGORY_MAP_3RD[(int)Category3rd.SHORT_SLEEVE] = "短袖";
            CATEGORY_MAP_3RD[(int)Category3rd.LONG_SLEEVE] = "长袖";
            CATEGORY_MAP_3RD[(int)Category3rd.SLEEVELESS] = "无袖";
            CATEGORY_MAP_3RD[(int)Category3rd.JEANS] = "牛仔裤";
            CATEGORY_MAP_3RD[(int)Category3rd.CASUAL_TROUSERS] = "休闲长裤";
            CATEGORY_MAP_3RD[(int)Category3rd.SHORT_TROUSERS] = "中短裤";
            CATEGORY_MAP_3RD[(int)Category3rd.OVERCOAT] = "大衣";
            CATEGORY_MAP_3RD[(int)Category3rd.DUSTCOAT] = "风衣";
            CATEGORY_MAP_3RD[(int)Category3rd.JACKET] = "夹克";
            CATEGORY_MAP_3RD[(int)Category3rd.WAISTCOAT] = "马甲背心";
            CATEGORY_MAP_3RD[(int)Category3rd.COTTON] = "棉衣";
            CATEGORY_MAP_3RD[(int)Category3rd.COWBOY_WEAR] = "牛仔服";
            CATEGORY_MAP_3RD[(int)Category3rd.FURRIERY] = "皮衣";
            CATEGORY_MAP_3RD[(int)Category3rd.INTERLOOP] = "卫衣";
            CATEGORY_MAP_3RD[(int)Category3rd.INFORMAL_SUIT] = "休闲西装";
            CATEGORY_MAP_3RD[(int)Category3rd.DOWN_COAT] = "羽绒服";
            CATEGORY_MAP_3RD[(int)Category3rd.SUIT] = "套装";
            CATEGORY_MAP_3RD[(int)Category3rd.WESTERN_TROUSERS] = "西裤";
            CATEGORY_MAP_3RD[(int)Category3rd.SHORT_SLEEVE_SHIRT] = "短袖衬衫";
            CATEGORY_MAP_3RD[(int)Category3rd.LONG_SLEEVE_SHIRT] = "长袖衬衫";

            CATEGORY_MAP_3RD[(int)Category3rd.HALF_SLEEVE] = "中袖";
            CATEGORY_MAP_3RD[(int)Category3rd.KNIT] = "针织衫";
            CATEGORY_MAP_3RD[(int)Category3rd.OVERALLS] = "背带裤";
            CATEGORY_MAP_3RD[(int)Category3rd.SHORTS] = "短裤";
            CATEGORY_MAP_3RD[(int)Category3rd.SPLIT_SKIRT] = "裙裤";
            CATEGORY_MAP_3RD[(int)Category3rd.HOT_PANTS] = "热裤";
            CATEGORY_MAP_3RD[(int)Category3rd.TOOTSIES] = "小脚裤";
            CATEGORY_MAP_3RD[(int)Category3rd.PINOCHETTO] = "中裤";
            CATEGORY_MAP_3RD[(int)Category3rd.THERMAL] = "保暖内衣";
            CATEGORY_MAP_3RD[(int)Category3rd.HOME_DRESS] = "家居服";
            CATEGORY_MAP_3RD[(int)Category3rd.UNDERPANTS] = "内裤";
            CATEGORY_MAP_3RD[(int)Category3rd.UNDERWEAR] = "内衣套装";
            CATEGORY_MAP_3RD[(int)Category3rd.PAJAMAS] = "睡衣";
            CATEGORY_MAP_3RD[(int)Category3rd.FIGURE_SHAPE] = "塑身内衣";
            CATEGORY_MAP_3RD[(int)Category3rd.BRA] = "文胸";
            CATEGORY_MAP_3RD[(int)Category3rd.BATHROBE] = "浴衣";
            CATEGORY_MAP_3RD[(int)Category3rd.SUNDRESS] = "背心裙";
            CATEGORY_MAP_3RD[(int)Category3rd.SUSPENDER_SKIRT] = "吊带裙";
            CATEGORY_MAP_3RD[(int)Category3rd.COCKTAIL] = "短裙";
            CATEGORY_MAP_3RD[(int)Category3rd.SHORT_SLEEVE_DRESS] = "短袖连衣裙";
            CATEGORY_MAP_3RD[(int)Category3rd.JEAN_SKIRT] = "牛仔裙";
            CATEGORY_MAP_3RD[(int)Category3rd.LONG_DRESS] = "长裙";
            CATEGORY_MAP_3RD[(int)Category3rd.LONG_SLEEVE_DRESS] = "长袖连衣裙";
            CATEGORY_MAP_3RD[(int)Category3rd.MEDIUM_DRESS] = "中裙";
            CATEGORY_MAP_3RD[(int)Category3rd.WESKIT] = "马甲";
            CATEGORY_MAP_3RD[(int)Category3rd.FURS] = "皮草";
            CATEGORY_MAP_3RD[(int)Category3rd.TENBORO] = "小西装";

            CATEGORY_MAP_3RD[(int)Category3rd.TSHIRT] = "T恤";
            CATEGORY_MAP_3RD[(int)Category3rd.VEST] = "背心";
            CATEGORY_MAP_3RD[(int)Category3rd.SHIRT] = "衬衫";
            CATEGORY_MAP_3RD[(int)Category3rd.TROUSERS] = "裤子";
            CATEGORY_MAP_3RD[(int)Category3rd.SWEATER] = "毛衫";
            CATEGORY_MAP_3RD[(int)Category3rd.WOOLEAN] = "绒衫";
            CATEGORY_MAP_3RD[(int)Category3rd.COAT] = "外套";
            CATEGORY_MAP_3RD[(int)Category3rd.DRESS] = "裙子";

            CATEGORY_MAP_3RD[(int)Category3rd.OTHERS] = "其他";

            // inverse category map
            // first category
            INVERSE_CATEGORY_MAP_1ST["男装"] = (int)Category1st.MAN;
            INVERSE_CATEGORY_MAP_1ST["女装"] = (int)Category1st.WOMAN;
            INVERSE_CATEGORY_MAP_1ST["童装"] = (int)Category1st.CHILD;
            INVERSE_CATEGORY_MAP_1ST["运动服"] = (int)Category1st.SPORT;
            INVERSE_CATEGORY_MAP_1ST["孕妇装"] = (int)Category1st.MATERNITY;

            INVERSE_CATEGORY_MAP_1ST["其他"] = (int)Category1st.OTHERS;

            // second category
            INVERSE_CATEGORY_MAP_2ND["T恤"] = (int)Category2nd.TSHIRT;
            INVERSE_CATEGORY_MAP_2ND["家居服睡衣"] = (int)Category2nd.PAJAMAS;
            INVERSE_CATEGORY_MAP_2ND["裤子"] = (int)Category2nd.TROUSERS;
            INVERSE_CATEGORY_MAP_2ND["毛衣"] = (int)Category2nd.SWEATER;
            INVERSE_CATEGORY_MAP_2ND["内裤"] = (int)Category2nd.UNDERPANTS;
            INVERSE_CATEGORY_MAP_2ND["内衣"] = (int)Category2nd.UNDERWEAR;
            INVERSE_CATEGORY_MAP_2ND["唐装"] = (int)Category2nd.TANG_SUIT;
            INVERSE_CATEGORY_MAP_2ND["外套"] = (int)Category2nd.COAT;
            INVERSE_CATEGORY_MAP_2ND["西装"] = (int)Category2nd.SUIT;
            INVERSE_CATEGORY_MAP_2ND["休闲"] = (int)Category2nd.SPORTSWEAR;
            INVERSE_CATEGORY_MAP_2ND["针织衫"] = (int)Category2nd.KNIT;
            INVERSE_CATEGORY_MAP_2ND["正装"] = (int)Category2nd.FORMAL_DRESS;
            INVERSE_CATEGORY_MAP_2ND["中老年服装"] = (int)Category2nd.OLDER;

            INVERSE_CATEGORY_MAP_2ND["衬衫"] = (int)Category2nd.SHIRT;
            INVERSE_CATEGORY_MAP_2ND["背心"] = (int)Category2nd.VEST;
            INVERSE_CATEGORY_MAP_2ND["吊带"] = (int)Category2nd.CAMISOLE;
            INVERSE_CATEGORY_MAP_3RD["短袖"] = (int)Category2nd.SHORT_SLEEVE;
            INVERSE_CATEGORY_MAP_2ND["裹胸"] = (int)Category2nd.PECTORAL;
            INVERSE_CATEGORY_MAP_2ND["婚纱"] = (int)Category2nd.BRIDAL_VEIL;
            INVERSE_CATEGORY_MAP_2ND["蕾丝"] = (int)Category2nd.EVENING_DRESS;
            INVERSE_CATEGORY_MAP_2ND["旗袍"] = (int)Category2nd.CHEONGSAM;
            INVERSE_CATEGORY_MAP_2ND["裙子"] = (int)Category2nd.DRESS;
            INVERSE_CATEGORY_MAP_3RD["无袖"] = (int)Category2nd.SLEEVELESS;
            INVERSE_CATEGORY_MAP_2ND["雪纺"] = (int)Category2nd.CHIFFON;
            INVERSE_CATEGORY_MAP_2ND["运动套装"] = (int)Category2nd.GYM_SUIT;
            INVERSE_CATEGORY_MAP_2ND["长袖"] = (int)Category2nd.LONG_SLEEVE;
            INVERSE_CATEGORY_MAP_2ND["职业套装"] = (int)Category2nd.BUSINESS_SUIT;

            INVERSE_CATEGORY_MAP_2ND["男童"] = (int)Category2nd.BOY;
            INVERSE_CATEGORY_MAP_2ND["女童"] = (int)Category2nd.GIRL;

            INVERSE_CATEGORY_MAP_2ND["泳衣"] = (int)Category2nd.SWIMSUIT;
            INVERSE_CATEGORY_MAP_2ND["运动外套"] = (int)Category2nd.TRACK_SUIT;

            INVERSE_CATEGORY_MAP_2ND["防辐射服"] = (int)Category2nd.RADIATION_PROOF;
            INVERSE_CATEGORY_MAP_2ND["普通孕妇装"] = (int)Category2nd.COMMON;
            INVERSE_CATEGORY_MAP_2ND["孕妇内衣"] = (int)Category2nd.MATERNAL_UNDERWEAR;

            INVERSE_CATEGORY_MAP_2ND["其他"] = (int)Category2nd.OTHERS;

            // third category
            INVERSE_CATEGORY_MAP_3RD["短袖"] = (int)Category3rd.SHORT_SLEEVE;
            INVERSE_CATEGORY_MAP_3RD["长袖"] = (int)Category3rd.LONG_SLEEVE;
            INVERSE_CATEGORY_MAP_3RD["无袖"] = (int)Category3rd.SLEEVELESS;
            INVERSE_CATEGORY_MAP_3RD["牛仔裤"] = (int)Category3rd.JEANS;
            INVERSE_CATEGORY_MAP_3RD["休闲长裤"] = (int)Category3rd.CASUAL_TROUSERS;
            INVERSE_CATEGORY_MAP_3RD["中短裤"] = (int)Category3rd.SHORT_TROUSERS;
            INVERSE_CATEGORY_MAP_3RD["大衣"] = (int)Category3rd.OVERCOAT;
            INVERSE_CATEGORY_MAP_3RD["风衣"] = (int)Category3rd.DUSTCOAT;
            INVERSE_CATEGORY_MAP_3RD["夹克"] = (int)Category3rd.JACKET;
            INVERSE_CATEGORY_MAP_3RD["马甲背心"] = (int)Category3rd.WAISTCOAT;
            INVERSE_CATEGORY_MAP_3RD["棉衣"] = (int)Category3rd.COTTON;
            INVERSE_CATEGORY_MAP_3RD["牛仔服"] = (int)Category3rd.COWBOY_WEAR;
            INVERSE_CATEGORY_MAP_3RD["皮衣"] = (int)Category3rd.FURRIERY;
            INVERSE_CATEGORY_MAP_3RD["卫衣"] = (int)Category3rd.INTERLOOP;
            INVERSE_CATEGORY_MAP_3RD["休闲西装"] = (int)Category3rd.INFORMAL_SUIT;
            INVERSE_CATEGORY_MAP_3RD["羽绒服"] = (int)Category3rd.DOWN_COAT;
            INVERSE_CATEGORY_MAP_3RD["套装"] = (int)Category3rd.SUIT;
            INVERSE_CATEGORY_MAP_3RD["西裤"] = (int)Category3rd.WESTERN_TROUSERS;
            INVERSE_CATEGORY_MAP_3RD["短袖衬衫"] = (int)Category3rd.SHORT_SLEEVE_SHIRT;
            INVERSE_CATEGORY_MAP_3RD["长袖衬衫"] = (int)Category3rd.LONG_SLEEVE_SHIRT;

            INVERSE_CATEGORY_MAP_3RD["中袖"] = (int)Category3rd.HALF_SLEEVE;
            INVERSE_CATEGORY_MAP_3RD["针织衫"] = (int)Category3rd.KNIT;
            INVERSE_CATEGORY_MAP_3RD["背带裤"] = (int)Category3rd.OVERALLS;
            INVERSE_CATEGORY_MAP_3RD["短裤"] = (int)Category3rd.SHORTS;
            INVERSE_CATEGORY_MAP_3RD["裙裤"] = (int)Category3rd.SPLIT_SKIRT;
            INVERSE_CATEGORY_MAP_3RD["热裤"] = (int)Category3rd.HOT_PANTS;
            INVERSE_CATEGORY_MAP_3RD["小脚裤"] = (int)Category3rd.TOOTSIES;
            INVERSE_CATEGORY_MAP_3RD["中裤"] = (int)Category3rd.PINOCHETTO;
            INVERSE_CATEGORY_MAP_3RD["保暖内衣"] = (int)Category3rd.THERMAL;
            INVERSE_CATEGORY_MAP_3RD["家居服"] = (int)Category3rd.HOME_DRESS;
            INVERSE_CATEGORY_MAP_3RD["内裤"] = (int)Category3rd.UNDERPANTS;
            INVERSE_CATEGORY_MAP_3RD["内衣套装"] = (int)Category3rd.UNDERWEAR;
            INVERSE_CATEGORY_MAP_3RD["睡衣"] = (int)Category3rd.PAJAMAS;
            INVERSE_CATEGORY_MAP_3RD["塑身内衣"] = (int)Category3rd.FIGURE_SHAPE;
            INVERSE_CATEGORY_MAP_3RD["文胸"] = (int)Category3rd.BRA;
            INVERSE_CATEGORY_MAP_3RD["浴衣"] = (int)Category3rd.BATHROBE;
            INVERSE_CATEGORY_MAP_3RD["背心裙"] = (int)Category3rd.SUNDRESS;
            INVERSE_CATEGORY_MAP_3RD["吊带裙"] = (int)Category3rd.SUSPENDER_SKIRT;
            INVERSE_CATEGORY_MAP_3RD["短裙"] = (int)Category3rd.COCKTAIL;
            INVERSE_CATEGORY_MAP_3RD["短袖连衣裙"] = (int)Category3rd.SHORT_SLEEVE_DRESS;
            INVERSE_CATEGORY_MAP_3RD["牛仔裙"] = (int)Category3rd.JEAN_SKIRT;
            INVERSE_CATEGORY_MAP_3RD["长裙"] = (int)Category3rd.LONG_DRESS;
            INVERSE_CATEGORY_MAP_3RD["长袖连衣裙"] = (int)Category3rd.LONG_SLEEVE_DRESS;
            INVERSE_CATEGORY_MAP_3RD["中裙"] = (int)Category3rd.MEDIUM_DRESS;
            INVERSE_CATEGORY_MAP_3RD["马甲"] = (int)Category3rd.WESKIT;
            INVERSE_CATEGORY_MAP_3RD["皮草"] = (int)Category3rd.FURS;
            INVERSE_CATEGORY_MAP_3RD["小西装"] = (int)Category3rd.TENBORO;

            INVERSE_CATEGORY_MAP_3RD["T恤"] = (int)Category3rd.TSHIRT;
            INVERSE_CATEGORY_MAP_3RD["背心"] = (int)Category3rd.VEST;
            INVERSE_CATEGORY_MAP_3RD["衬衫"] = (int)Category3rd.SHIRT;
            INVERSE_CATEGORY_MAP_3RD["裤子"] = (int)Category3rd.TROUSERS;
            INVERSE_CATEGORY_MAP_3RD["毛衫"] = (int)Category3rd.SWEATER;
            INVERSE_CATEGORY_MAP_3RD["绒衫"] = (int)Category3rd.WOOLEAN;
            INVERSE_CATEGORY_MAP_3RD["外套"] = (int)Category3rd.COAT;
            INVERSE_CATEGORY_MAP_3RD["裙子"] = (int)Category3rd.DRESS;

            INVERSE_CATEGORY_MAP_3RD["其他"] = (int)Category3rd.OTHERS;
        }
    }

    public enum Category1st { MAN = 1, WOMAN, CHILD, SPORT, MATERNITY, OTHERS };
    public enum Category2nd
    {
        TSHIRT = 1, PAJAMAS, TROUSERS, SWEATER, UNDERPANTS, UNDERWEAR, TANG_SUIT, COAT, SUIT, SPORTSWEAR,
        KNIT, FORMAL_DRESS, OLDER, VEST, SHIRT, CAMISOLE, SHORT_SLEEVE, PECTORAL, BRIDAL_VEIL, LACE,
        EVENING_DRESS, CHEONGSAM, DRESS, SLEEVELESS, CHIFFON, GYM_SUIT, LONG_SLEEVE, BUSINESS_SUIT, BOY, GIRL,
        SWIMSUIT, TRACK_SUIT, RADIATION_PROOF, COMMON, MATERNAL_UNDERWEAR, OTHERS
    };
    public enum Category3rd 
    { 
        SHORT_SLEEVE = 1, LONG_SLEEVE, SLEEVELESS, JEANS, CASUAL_TROUSERS, SHORT_TROUSERS, OVERCOAT,
        DUSTCOAT, JACKET, WAISTCOAT, COTTON, COWBOY_WEAR, FURRIERY, INTERLOOP, INFORMAL_SUIT, DOWN_COAT, SUIT,
        WESTERN_TROUSERS, SHORT_SLEEVE_SHIRT, LONG_SLEEVE_SHIRT, HALF_SLEEVE, KNIT, OVERALLS, SHORTS,
        SPLIT_SKIRT, HOT_PANTS, TOOTSIES, PINOCHETTO, THERMAL, HOME_DRESS, UNDERPANTS, UNDERWEAR, PAJAMAS,
        FIGURE_SHAPE, BRA, BATHROBE, SUNDRESS, SUSPENDER_SKIRT, COCKTAIL, SHORT_SLEEVE_DRESS, JEAN_SKIRT,
        LONG_DRESS, LONG_SLEEVE_DRESS, MEDIUM_DRESS, HALF_SLEEVE_DRESS, WESKIT, FURS, TENBORO, TSHIRT, VEST,
        SHIRT, TROUSERS, SWEATER, WOOLEAN, COAT, DRESS, OTHERS
    };

    [Flags]
    public enum ColorEnum
    {
        NONE = 0x0,
        BLACK = 0x1,
        WHITE = 0x2,
        BLUE = 0x4,
        RED = 0x8,
        PINK = 0x10,
        DARKRED = 0x20,
    };

    [Flags]
    public enum ShapeEnum
    {
        NONE = 0x0,
        STRIPE = 0x1,
        SQUARE = 0x2,
        CIRCLE = 0x4, 
        TRIANGLE = 0x8,
        SPECIAL = 0x10,
    };

}

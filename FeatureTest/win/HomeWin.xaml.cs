using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Threading;
using Microsoft.Win32;
using Zju.Domain;
using Zju.Image;
using Zju.Service;
using Zju.Util;
using Zju.View;

namespace FeatureTest
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class HomeWin : Window
    {
        /*private List<ColorItem> colorItems;

        private List<ShapeItem> shapeItems;

        private List<ColorItem> modifyColorItems;

        private List<ShapeItem> modifyShapeItems;*/

        private List<CategoryItem> categoryItems;


        private IClothLibService clothLibService;

        private IClothSearchService clothSearchService;

        private ImageMatcher imageMatcher;

        /// <summary>
        /// The Cloth object of opened key picture. Its property <code>Path</code> should be assigned.
        /// If it clears, <code>keyCloth</code> should be set <code>null</code>.
        /// </summary>
        private Cloth keyCloth;

        private AlgorithmType textureAType;

        private AlgorithmType colorAType;

        private AlgorithmType shapeAType;

        private AlgorithmType[] textureATypes;

        private AlgorithmType[] colorATypes;

        private AlgorithmType[] shapeATypes;

        private OpenFileDialog dlgOpenKeyPic;

        private OpenFileDialog dlgOpenPics;

        private ProgressWin progressWin;

        //private MatchAlgorithmWin matchAlgorithmWin;

        //private delegate void AsynOpenUI(int nTotal);

        private delegate void IntAsynUpdateUI(int nTotal);

        private delegate void NoArguAsynUpdateUI();

        //private delegate void AsynImportPics(List<String> picNames);

        private System.Windows.Forms.FolderBrowserDialog dlgOpenPicFolder;

        /// <summary>
        /// pages.
        /// </summary>
        private const int picsPerPage = 50;
        private List<Cloth> searchedClothes;
        private int curPage;
        /// <summary>
        /// totalPage = (seachedClothes.Count + picsPerPage - 1) / picsPerPage
        /// </summary>
        private int totalPage;

        /// <summary>
        /// the selected cloth in the result clothes.
        /// </summary>
        private Cloth selectedCloth;

        private const string imageNamePrefix = "img";
        private const string reImageNamePrefix = "r";

        /// <summary>
        ///  the finished picture when import
        /// </summary>
        private int nFinished;

        /// <summary>
        ///  Thread to import pictures.
        /// </summary>
        //private Thread importPicsThread;

        /// <summary>
        /// The picture file names to be imported, used in the thread <code>importPicsThread</code>.
        /// </summary>
        //private List<String> picNames;

        //public static int count = 0;

        //private int curKeyClothOperId = 0;

        //private static Object keyClothLock = new Object();

        //private static Object incKeyClothLock = new Object();

        /*private IndeterminateProgressWin inProgWin;

        private bool keyClothOpened = false;

        private Thread keyClothThread;

        private Thread searchThread;*/

        public HomeWin()
        {
            /*colorItems = ViewHelper.NewColorItems;
            shapeItems = ViewHelper.NewShapeItems;
            this.Resources.Add("colorItems", colorItems);
            this.Resources.Add("shapeItems", shapeItems);

            modifyColorItems = ViewHelper.NewColorItems;
            modifyShapeItems = ViewHelper.NewShapeItems;
            this.Resources.Add("modifyColorItems", modifyColorItems);
            this.Resources.Add("modifyShapeItems", modifyShapeItems);*/

            categoryItems = ViewHelper.NewCategoryItems;
            this.Resources.Add("categoryItems", categoryItems);

            textureATypes = new AlgorithmType[] { AlgorithmType.DaubechiesWavelet, AlgorithmType.Cooccurrence, AlgorithmType.Tamura, AlgorithmType.Gabor};

            colorATypes = new AlgorithmType[] { AlgorithmType.HSVAynsColor,
                AlgorithmType.RGBColor, AlgorithmType.HSVColor, AlgorithmType.RGBSeparateColor, AlgorithmType.HLSColor };

            shapeATypes = new AlgorithmType[] { AlgorithmType.Fourier, AlgorithmType.MIHu, AlgorithmType.MICanny, AlgorithmType.MIAll };

            InitializeComponent();

            //btnSearch.IsEnabled = false;

            dlgOpenKeyPic = newOpenFileDialog();
            dlgOpenKeyPic.Title = "请选择关键图";

            dlgOpenPics = newOpenFileDialog();
            dlgOpenPics.Title = "请选择多张图片进行导入";
            dlgOpenPics.Multiselect = true;

            dlgOpenPicFolder = new System.Windows.Forms.FolderBrowserDialog();
            dlgOpenPicFolder.Description = "请选择文件夹以导入其下的所有图片(JPG, PNG, BMP)";
            dlgOpenPicFolder.SelectedPath = Environment.GetFolderPath(Environment.SpecialFolder.MyPictures);
            //dlgOpenPicFolder.RootFolder = Environment.SpecialFolder.MyPictures;

            // It should be done by dependency injection here!!
            clothLibService = new ClothLibService();
            clothSearchService = new ClothSearchService();
            imageMatcher = ClothUtil.ImageMatcherInst;

            // temp
            //txtModifyName.IsEnabled = false;

            //picNames = new List<string>();
        }

        private void btnToolOpen_Click(object sender, RoutedEventArgs e)
        {
            AddPicWin addPicWin = new AddPicWin(keyCloth);
            addPicWin.Owner = this;
            addPicWin.ShowDialog();
        }

        private void btnOpenKeyPic_Click(object sender, RoutedEventArgs e)
        {
            if (dlgOpenKeyPic.ShowDialog() == true)
            {
                BitmapImage bi = ViewHelper.NewBitmapImage(dlgOpenKeyPic.FileName);
                if (bi == null)
                {
                    MessageBox.Show("您选择的文件无法识别, 可能不是图片文件.", "显示关键图...");
                    return;
                }

                imgKeyPic.Source = bi;
                keyCloth = new Cloth();
                keyCloth.Path = dlgOpenKeyPic.FileName;
                keyCloth.Pattern = ClothUtil.ExtractPattern(keyCloth.Path);
                keyCloth.Name = keyCloth.Pattern;

                updateSearchButtonByPic();
            }
        }

       /* private void fillKeyCloth(Object obj)
        {
            Cloth cloth = new Cloth();
            cloth.Path = (String)obj;
            cloth.Pattern = ClothUtil.ExtractPattern(cloth.Path);
            cloth.Name = cloth.Pattern;
            cloth.ColorVector = imageMatcher.ExtractRGBSeparateColorVector(cloth.Path, SearchConstants.IgnoreColors);
            cloth.TextureVector = imageMatcher.ExtractDaubechiesWaveletVector(cloth.Path);
            cloth.GaborVector = imageMatcher.ExtractGaborVector(cloth.Path);
            cloth.CooccurrenceVector = imageMatcher.ExtractCooccurrenceVector(cloth.Path);
            keyCloth = cloth;                
        }*/

        private OpenFileDialog newOpenFileDialog()
        {
            OpenFileDialog dlgOpenFile = new OpenFileDialog();
            //dlgOpenFile.Filter = "jpeg (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg;*.jpeg;*.jpe;*.jfif|All Image files|*.jpg;*.jpeg;*.jpe;*.jfif;*.gif;*.png;*.bmp;*.ico;*.tif;*.tiff|All files (*.*)|*.*";
            dlgOpenFile.Filter = "jpeg (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg;*.jpeg;*.jpe;*.jfif|所有支持的图片文件 (JPG;BMP;PNG;TIF;...)|*.jpg;*.jpeg;*.jpe;*.jfif;*.png;*.bmp;*.dib;*.tif;*.tiff";
            dlgOpenFile.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyPictures);
            dlgOpenFile.FilterIndex = 2; // begin from 1
            return dlgOpenFile;
        }

        private void btnToolImport_Click(object sender, RoutedEventArgs e)
        {
            if (dlgOpenPics.ShowDialog() == true)
            {
                String[] selectedFiles = dlgOpenPics.FileNames;
                int nFiles = selectedFiles.Length;
                if (nFiles == 0)
                {
                    MessageBox.Show("您未先选择任何图片, 请先选择.", "导入图片...");
                    return;
                }

                List<String> picNames = new List<string>(nFiles);
                picNames.AddRange(selectedFiles);

                // save to database and show progress bar asynchronously.
                asynImportClothPics(picNames);
            }
        }

        // import files under the folder recursively, subfolder is included
        private void btnToolImportFolder_Click(object sender, RoutedEventArgs e)
        {
            if (dlgOpenPicFolder.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                String selectedPath = dlgOpenPicFolder.SelectedPath;

                Queue<string> qp = new Queue<string>();
                qp.Enqueue(selectedPath);
                List<String> picNames = new List<string>();
                while (qp.Count > 0)
                {
                    string path = qp.Dequeue();

                    picNames.AddRange(Directory.GetFiles(path, "*.jpg"));
                    picNames.AddRange(Directory.GetFiles(path, "*.tif"));
                    picNames.AddRange(Directory.GetFiles(path, "*.png"));
                    picNames.AddRange(Directory.GetFiles(path, "*.bmp"));

                    string[] dirs = Directory.GetDirectories(path);
                    foreach (string dir in dirs)
                    {
                        qp.Enqueue(dir);
                    }
                }

                if (picNames.Count == 0)
                {
                    MessageBox.Show("您选择的文件夹中未包含任何图片, 请重新选择.", "导入图片...");
                    return;
                }

                // save to database and show progress bar asynchronously.
                asynImportClothPics(picNames);
            }
        }

        private void asynImportClothPics(List<String> picNames)
        {
            nFinished = 0;
            clothLibService.AsynImportClothPics(new ImportArgus(picNames, 2, 20,
                new ShouldStop(shouldStopCallback),
                new IntArgDelegate(asynShowProgressDialogCallback),
                new IntArgDelegate(asynUpdateProgressWinCallback),
                new NoArgDelegate(asynCloseProgressWinCallback)));
        }

        private void asynShowProgressDialogCallback(int picNum)
        {
            this.Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal,
                new IntAsynUpdateUI(showProgressDialog), picNum);
        }

        private void asynUpdateProgressWinCallback(int finish)
        {
            this.Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal,
                        new IntAsynUpdateUI(updateProgressWin), finish);
        }

        private void asynCloseProgressWinCallback()
        {
            this.Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal,
                new NoArguAsynUpdateUI(closeProgressWin));
        }

        private bool shouldStopCallback()
        {
            return progressWin != null && progressWin.StateFlag == 1;
        }      

        private void showProgressDialog(int nTotal)
        {
            progressWin = new ProgressWin(nTotal);
            progressWin.Owner = this;
            progressWin.ShowDialog();

            //importPicsThread.Abort();

            MessageBox.Show(String.Format("成功导入{0}张图片", nFinished), "祝贺您");
        }

        private void updateProgressWin(int finish)
        {
            // atomic add
            Interlocked.Add(ref nFinished, finish);
            progressWin.FinishedPics = nFinished;
        }

        private void closeProgressWin()
        {
            // set the closing event should not be cancelled.
            //nFinished = finish;
            progressWin.StateFlag = 2;
            progressWin.Close();
            progressWin = null;
        }

        private void btnToolAbout_Click(object sender, RoutedEventArgs e)
        {
            ClothSearchAboutBox aboutBox = new ClothSearchAboutBox();
            aboutBox.ShowDialog();
        }

		private void btnSearch_Click(object sender, RoutedEventArgs e)
        {
            List<int> categories = new List<int>();
            foreach (CategoryItem ci in categoryItems)
            {
                if (ci.Selected)
                {
                    categories.Add(ci.Value);
                }
            }

            if (!(cbTextureAlgorithm.IsChecked == true || cbColorAlgorithm.IsChecked == true || cbShapeAlgorithm.IsChecked == true))
            {
                lblSearchResultInfo.Content = "正在随机获得图像...";
                searchedClothes = clothSearchService.SearchByRandom(categories.ToArray(), 200);
            }
            else
            {
                if (null == keyCloth || string.IsNullOrEmpty(keyCloth.Path))
                {
                    MessageBox.Show("图片搜索必须先指定关键图.", "搜索图片...");
                    return;
                }

                lblSearchResultInfo.Content = "正在通过图片内容搜索请稍候...";
                searchedClothes = searchByPic(categories.ToArray());
            }
            
            updatePicResults();
        }

        private float convert2Float(string s)
        {
            float f = 0;
            try
            {
                f = float.Parse(s);
            }
            catch (Exception e)
            {

            }
            return f;
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <returns>Result list. Null if no search executed.</returns>
        private List<Cloth> searchByPic(int[] categories)
        {
            if (null == keyCloth || string.IsNullOrEmpty(keyCloth.Path))
            {
                return null;
            }

            List<AlgorithmType> algs = new List<AlgorithmType>();
            List<float> weights = new List<float>();
            if (cbTextureAlgorithm.IsChecked == true)
            {
                algs.Add(textureAType);
                weights.Add(convert2Float(txtTextureWeight.Text));
            }
            if (cbColorAlgorithm.IsChecked == true)
            {
                algs.Add(colorAType);
                weights.Add(convert2Float(txtColorWeight.Text));
            }
            if (cbShapeAlgorithm.IsChecked == true)
            {
                algs.Add(shapeAType);
                weights.Add(convert2Float(txtShapeWeight.Text));
            }

            foreach (AlgorithmType at in algs)
            {
                switch (at)
                {
                    case AlgorithmType.DaubechiesWavelet:
                        if (null == keyCloth.DaubechiesWaveletVector)
                        {
                            keyCloth.DaubechiesWaveletVector = imageMatcher.ExtractDaubechiesWaveletVector(keyCloth.Path);
                            if (null == keyCloth.DaubechiesWaveletVector)
                            {
                                MessageBox.Show("您选择的文件无法识别, 可能不是图片文件.", "提取DaubechiesWavelet...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.Cooccurrence:
                        if (null == keyCloth.CooccurrenceVector)
                        {
                            keyCloth.CooccurrenceVector = imageMatcher.ExtractCooccurrenceVector(keyCloth.Path);
                            if (keyCloth.CooccurrenceVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取CooccurrenceVector...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.RGBSeparateColor:
                        if (null == keyCloth.RGBSeparateColorVector)
                        {
                            keyCloth.RGBSeparateColorVector = imageMatcher.ExtractRGBSeparateColorVector(keyCloth.Path, 8, SearchConstants.IgnoreColors);
                            if (keyCloth.RGBSeparateColorVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取RGBSeparateColorVector...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.RGBColor:
                        if (null == keyCloth.RGBColorVector)
                        {
                            keyCloth.RGBColorVector = imageMatcher.ExtractRGBColorVector(keyCloth.Path, 4, SearchConstants.IgnoreColors);
                            if (keyCloth.RGBColorVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取RGBColor...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.HSVAynsColor:
                        if (null == keyCloth.HSVAynsColorVector)
                        {
                            keyCloth.HSVAynsColorVector = imageMatcher.ExtractHSVAynsColorVector(keyCloth.Path, 0, SearchConstants.IgnoreColors);
                            if (keyCloth.HSVAynsColorVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取HSVAynsColor...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.HSVColor:
                        if (null == keyCloth.HSVColorVector)
                        {
                            keyCloth.HSVColorVector = imageMatcher.ExtractHSVColorVector(keyCloth.Path, 4, SearchConstants.IgnoreColors);
                            /*int colorNum = ClothUtil.getColorNumber(keyCloth.HSVColorVector, 0.07f);
                            if (colorNum > 8)
                            {
                                colorNum = 8;
                            }
                            keyCloth.ColorNum = colorNum;*/
                            if (keyCloth.HSVColorVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取HSVColorVector...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.HLSColor:
                        if (null == keyCloth.HLSColorVector)
                        {
                            keyCloth.HLSColorVector = imageMatcher.ExtractHLSColorVector(keyCloth.Path, 4, SearchConstants.IgnoreColors);
                            if (keyCloth.HLSColorVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取HLSColor...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.Tamura:
                        if (!AppConstants.IsTamura)
                        {
                            MessageBox.Show("Tamura算法在配置文件中被禁止.", "提取TamuraVector...");
                            return null;
                        }
                        if (null == keyCloth.TamuraVector)
                        {
                            keyCloth.TamuraVector = imageMatcher.ExtractTamuraVector(keyCloth.Path, 6);
                            if (keyCloth.TamuraVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取TamuraVector...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.Gabor:
                        if (!AppConstants.IsGabor)
                        {
                            MessageBox.Show("Gabor算法在配置文件中被禁止.", "提取GaborVector...");
                            return null;
                        }
                        if (null == keyCloth.GaborVector)
                        {
                            keyCloth.GaborVector = imageMatcher.ExtractGaborVector(keyCloth.Path);
                            if (keyCloth.GaborVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取GaborVector...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.MICanny:
                        if (null == keyCloth.MICannyVector)
                        {
                            keyCloth.MICannyVector = imageMatcher.ExtractMICannyVector(keyCloth.Path);
                            if (keyCloth.MICannyVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取MICannyVector...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.MIHu:
                        if (null == keyCloth.MIHuVector)
                        {
                            keyCloth.MIHuVector = imageMatcher.ExtractMIHuVector(keyCloth.Path);
                            if (keyCloth.MIHuVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取MIHuVector...");
                                return null;
                            }
                        }
                        break;
                    case AlgorithmType.Fourier:
                        if (null == keyCloth.FourierVector)
                        {
                            keyCloth.FourierVector = imageMatcher.ExtractFourierVector(keyCloth.Path, 20);
                            if (keyCloth.FourierVector == null)
                            {
                                MessageBox.Show("无法识别指定图片文件, 请检查该文件是否正确.", "提取FourierVector...");
                                return null;
                            }
                        }
                        break;
                }
            }

            return clothSearchService.SearchByPicCombine(keyCloth, algs.ToArray(), weights.ToArray(), categories);
        }

        /// <summary>
        /// Update the WrapPanel of result pictures with <code>searchedClothes</code> of the class.
        /// </summary>
        private void updatePicResults()
        {
            curPage = 0;
            totalPage = searchedClothes == null ? 0 : (searchedClothes.Count + picsPerPage - 1) / picsPerPage;
            
            updatePageOfPicResults();
        }

        private void updatePageOfPicResults()
        {
            bool isLast = (curPage >= totalPage - 1);
            bool isFirst = (curPage <= 0);
            // update page button
            if (btnPrePage.IsEnabled == isFirst)
            {
                btnPrePage.IsEnabled = !isFirst;
            }
            if (btnNextPage.IsEnabled == isLast)
            {
                btnNextPage.IsEnabled = !isLast;
            }

            // update text info
            if (null == searchedClothes || searchedClothes.Count == 0)
            {
                lblSearchResultInfo.Content = "未搜索到任何结果.";
            }
            else
            {
                lblSearchResultInfo.Content = String.Format("找到的前{0}个结果, 共{1}页, 当前显示第{2}页:", searchedClothes.Count, totalPage, curPage + 1);
            }
            
            // update page pictures
            wpanResultPics.Children.Clear();
            if (curPage < totalPage)
            {
                int begin = curPage * picsPerPage;
                int num = isLast ? searchedClothes.Count - begin : picsPerPage;
                for (int i = 0; i < num; ++i)
                {
                    wpanResultPics.Children.Add(newBorder(searchedClothes[begin+i].Path, begin+i, 100, 100));
                }
            }
        }

        private Border newBorder(String picName, int index, int weight, int height)
        {
            Border border = new Border();
            border.BorderThickness = new Thickness(1);
            border.BorderBrush = Brushes.SteelBlue;
            border.Child = newImage(picName, index, weight, height);

            return border;
        }

        private Image newImage(String picName, int index, int weight, int height)
        {
            Image img = new Image();
            img.Stretch = Stretch.Uniform;
            img.Width = Width;
            img.Height = height;

            if (!String.IsNullOrEmpty(picName))
            {
                img.Source = ViewHelper.NewBitmapImage(picName);
                img.Name = imageNamePrefix + index.ToString();
                if (img.Source != null)
                {
                    img.MouseDown += new MouseButtonEventHandler(resultImg_MouseDown);
                    //img.MouseLeftButtonDown += new MouseButtonEventHandler(resultImg_MouseLeftButtonDown);
                }
            }

            return img;
        }

        private void resultImg_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Image image = (Image)sender;
            Cloth cloth = searchedClothes[int.Parse(image.Name.Substring(imageNamePrefix.Length))];
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                imgCurrentResult.Source = image.Source;
                //imgCurrentResult.Name = reImageNamePrefix + image.Name;

                selectedCloth = cloth;

                txtModifyName.Text = selectedCloth.Name;

                txtModifyCategory.Text = ClothUtil.calcCategoryString(selectedCloth.Category);

                txtModifyColorNum.Text = selectedCloth.ColorNum.ToString();

                //txtModifyName.IsEnabled = true;
                btnResultDelete.IsEnabled = true;
                //btnResultModify.IsEnabled = true;
            }
            else if (e.RightButton == MouseButtonState.Pressed)
            {
                imgKeyPic.Source = image.Source;
                keyCloth = cloth;
            }
        }


        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            imgKeyPic.Source = null;
            keyCloth = null;
            //keyClothOpened = false;
        }

        private void btnResultDelete_Click(object sender, RoutedEventArgs e)
        {
            //Cloth cloth = searchedClothes[int.Parse(imgCurrentResult.Name.Substring(reImageNamePrefix.Length + imageNamePrefix.Length))];
            bool isIn = searchedClothes.Remove(selectedCloth);
            clothLibService.Delete(selectedCloth.Oid);

            imgCurrentResult.Source = null;

            txtModifyName.Text = "";
            //txtModifyName.IsEnabled = false;

            txtModifyCategory.Text = "";
            //txtModifyPattern.IsEnabled = false;

            txtModifyColorNum.Text = "";

            btnResultDelete.IsEnabled = false;
            btnResultModify.IsEnabled = false;

            if (isIn)
            {   // the removed selected cloth is in the searched clothes
                updatePicResults();
            }
        }

        private void btnResultModify_Click(object sender, RoutedEventArgs e)
        {
            //Cloth cloth = searchedClothes[int.Parse(imgCurrentResult.Name.Substring(reImageNamePrefix.Length + imageNamePrefix.Length))];

            Cloth newCloth = new Cloth(selectedCloth);
            int colorNum = 0;
            try
            {
                if (!string.IsNullOrEmpty(txtModifyColorNum.Text.Trim()))
                {
                    colorNum = int.Parse(txtModifyColorNum.Text.Trim());
                }
            }
            catch (Exception)
            {
            	// do nothing
            }

            newCloth.ColorNum = colorNum;
            //newCloth.Pattern = string.IsNullOrEmpty(txtModifyPattern.Text) ? null : txtModifyPattern.Text;
            /*ColorEnum colors = ColorEnum.NONE;
            foreach (ColorItem ci in modifyColorItems)
            {
                if (ci.Selected)
                {
                    colors |= ci.Value;
                }
            }
            newCloth.Colors = colors;

            ShapeEnum shapes = ShapeEnum.NONE;
            foreach (ShapeItem si in modifyShapeItems)
            {
                if (si.Selected)
                {
                    shapes |= si.Value;
                }
            }
            newCloth.Shapes = shapes;*/

            clothLibService.Update(selectedCloth, newCloth);
        }

        private void btnFirstPage_Click(object sender, RoutedEventArgs e)
        {
            curPage = 0;
            updatePageOfPicResults();
        }

        private void btnPrePage_Click(object sender, RoutedEventArgs e)
        {
            curPage = curPage > 0 ? curPage - 1 : 0;
            updatePageOfPicResults();
        }

        private void btnNextPage_Click(object sender, RoutedEventArgs e)
        {
            curPage = curPage < totalPage - 1 ? curPage + 1 : (totalPage > 0 ? totalPage - 1 : 0);
            updatePageOfPicResults();
        }

        private void btnLastPage_Click(object sender, RoutedEventArgs e)
        {
            curPage = totalPage > 0 ? totalPage - 1 : 0;
            updatePageOfPicResults();
        }


		private bool canSearchByPic()
        {
            bool cando = false;

            if (null != keyCloth && !string.IsNullOrEmpty(keyCloth.Path))
            {
                cando = true;
            }

            return cando;
        }
 

        private void cmbTextureAlgorithm_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            textureAType = textureATypes[cmbTextureAlgorithm.SelectedIndex];
        }

        private void cmbColorAlgorithm_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            colorAType = colorATypes[cmbColorAlgorithm.SelectedIndex];
        }

        private void cmbShapeAlgorithm_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            shapeAType = shapeATypes[cmbShapeAlgorithm.SelectedIndex];
        }

        private void updateSearchButtonByPic()
        {
            /*bool cando = canSearchByPic();

            if (btnSearch.IsEnabled != cando)
            {
                btnSearch.IsEnabled = cando;
            }*/
        }

        private void chkCategoryInput_Click(object sender, RoutedEventArgs e)
        {
            String values = "";

            foreach (CategoryItem ci in categoryItems)
            {
                if (ci.Selected)
                {
                    values += String.IsNullOrEmpty(values) ? ci.Name : "," + ci.Name;
                }
            }

            cmbCategoryInput.Text = values;
        }

        private void btnToolModifyPath_Click(object sender, RoutedEventArgs e)
        {
            clothLibService.ModifyClothPicPath(@"F:\pic_skindetect\zz200", @"D:\kricel\My Documents\Work\Demo\zz200");

            MessageBox.Show("路径修改完毕.", "修改路径...");
        }
    }
}

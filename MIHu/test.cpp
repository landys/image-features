#include "cv.h" 
#include <stdio.h>
#include "cxcore.h" 
#include "highgui.h" 

#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "highgui.lib")

int main( int argc, char** argv ) 
{ 
	//声明IplImage指针 
	IplImage* pImg = NULL; 
	IplImage* pCannyImg = NULL; 

	double M20;

	//载入图像，强制转化为Gray 
	if( argc == 2 && 
		(pImg = cvLoadImage( argv[1], 0)) != 0 ) 
	{ 

		/*//为canny边缘图像申请空间 
		pCannyImg = cvCreateImage(cvGetSize(pImg), 
			IPL_DEPTH_8U, 
			1); 
		//canny边缘检测 
		cvCanny(pImg, pCannyImg, 50, 150, 3); 

		//创建窗口 
		cvNamedWindow("src", 1); 
		//cvNamedWindow("canny",1); 
		cvNamedWindow("canny",1);


		//显示图像 
		cvShowImage( "src", pImg ); 

		//去中间目标区域
		cvSetImageROI(pImg,cvRect(60,130,130,130));

		//中值滤波
		cvSmooth( pImg, pImg,CV_MEDIAN,3, 0, 0, 0 );

		//自动阈值分割 
		//cvAdaptiveThreshold(pImg, pImg, 255,
		//                        CV_ADAPTIVE_THRESH_GAUSSIAN_C,
		//                       CV_THRESH_BINARY, 31, 1 );

		//阈值分割
		cvThreshold( pImg, pImg, 76,
			255, CV_THRESH_BINARY );

		//一次膨胀运算
		cvDilate( pImg, pImg, 0,1 );

		cvShowImage( "canny", pImg ); 
		//cvShowImage( "canny", pCannyImg ); */
		CvMoments m; 
		CvMat mat; 
		cvMoments(pImg,&m,0); 
		M20=cvGetNormalizedCentralMoment(&m,2,0);
		printf("%lf\n",M20);

		CvHuMoments chu;
		cvGetHuMoments(&m, &chu);

		printf("%lg %lg %lg %lg %lg %lg %lg\n", chu.hu1, chu.hu2, chu.hu3, chu.hu4, chu.hu5, chu.hu6, chu.hu7);

		cvWaitKey(0); //等待按键 

		//销毁窗口 
		/*cvDestroyWindow( "src" ); 
		cvDestroyWindow( "canny" ); 
		//释放图像 
		cvReleaseImage( &pImg ); 
		cvReleaseImage( &pCannyImg ); */

		return 0; 
	} 

	return -1; 
} 
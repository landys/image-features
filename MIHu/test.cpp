#include "cv.h" 
#include <stdio.h>
#include "cxcore.h" 
#include "highgui.h" 

#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "highgui.lib")

int main( int argc, char** argv ) 
{ 
	//����IplImageָ�� 
	IplImage* pImg = NULL; 
	IplImage* pCannyImg = NULL; 

	double M20;

	//����ͼ��ǿ��ת��ΪGray 
	if( argc == 2 && 
		(pImg = cvLoadImage( argv[1], 0)) != 0 ) 
	{ 

		/*//Ϊcanny��Եͼ������ռ� 
		pCannyImg = cvCreateImage(cvGetSize(pImg), 
			IPL_DEPTH_8U, 
			1); 
		//canny��Ե��� 
		cvCanny(pImg, pCannyImg, 50, 150, 3); 

		//�������� 
		cvNamedWindow("src", 1); 
		//cvNamedWindow("canny",1); 
		cvNamedWindow("canny",1);


		//��ʾͼ�� 
		cvShowImage( "src", pImg ); 

		//ȥ�м�Ŀ������
		cvSetImageROI(pImg,cvRect(60,130,130,130));

		//��ֵ�˲�
		cvSmooth( pImg, pImg,CV_MEDIAN,3, 0, 0, 0 );

		//�Զ���ֵ�ָ� 
		//cvAdaptiveThreshold(pImg, pImg, 255,
		//                        CV_ADAPTIVE_THRESH_GAUSSIAN_C,
		//                       CV_THRESH_BINARY, 31, 1 );

		//��ֵ�ָ�
		cvThreshold( pImg, pImg, 76,
			255, CV_THRESH_BINARY );

		//һ����������
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

		cvWaitKey(0); //�ȴ����� 

		//���ٴ��� 
		/*cvDestroyWindow( "src" ); 
		cvDestroyWindow( "canny" ); 
		//�ͷ�ͼ�� 
		cvReleaseImage( &pImg ); 
		cvReleaseImage( &pCannyImg ); */

		return 0; 
	} 

	return -1; 
} 
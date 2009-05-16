#include "Stdafx.h"
#include "cv.h"
#include "cxcore.h" 
#include "highgui.h" 
#include "MI.h"

int extractMI(const char* imagenamefile, bool isCanny, double* feature)
{
	IplImage* pImg = cvLoadImage(imagenamefile, CV_LOAD_IMAGE_GRAYSCALE);

	if (pImg == 0)
	{
		return 0;
	}

	if (isCanny)
	{
		cvCanny(pImg, pImg, 50, 150, 3);
	} 

	CvMoments m;
	cvMoments(pImg, &m, 0);
	CvHuMoments chu;
	cvGetHuMoments(&m, &chu);

	feature[0] = chu.hu1;
	feature[1] = chu.hu2;
	feature[2] = chu.hu3;
	feature[3] = chu.hu4;
	feature[4] = chu.hu5;
	feature[5] = chu.hu6;
	feature[6] = chu.hu7;

	cvReleaseImage( &pImg );

	return 1;
}
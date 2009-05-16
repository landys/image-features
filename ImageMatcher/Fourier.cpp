#include "Stdafx.h"
#include "Fourier.h"
#include "cv.h"
#include "highgui.h"

// input: seq_pts, n_fourier; output: seq_fourier
void CalcFourierDescriptorCoeff(CvSeq* seq_pts, int n_fourier,CvSeq* seq_fourier);
// input: seq_fourier, n_pts; output: seq_pts. It just the opposite operation of CalcFourierDescriptorCoeff
void CalcBoundary(CvSeq* seq_fourier, int n_Pts,CvSeq* seq_pts);


int extractFourier(const char* imagenamefile, int n, double* feature)
{
	IplImage* gray = cvLoadImage(imagenamefile, CV_LOAD_IMAGE_GRAYSCALE);
	if (!gray)
	{
		return 0;
	}

	//cvThreshold(gray, gray, 100, 255, CV_THRESH_BINARY_INV);
	cvCanny(gray, gray, 50, 150, 3); 
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq* contours;
	CvSeq* seq_fourier = cvCreateSeq(CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint2D32f), storage);
	cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);

	CvSeq* mostContours = contours;
	for(; contours; contours = contours->h_next)
	{
		if (mostContours->total < contours->total)
		{
			mostContours = contours;
		}
	}

	int re = 0;
	if (mostContours)
	{
		CalcFourierDescriptorCoeff(mostContours, n, seq_fourier);

		for (int i=0; i<seq_fourier->total; ++i)
		{
			CvPoint2D32f* pt = (CvPoint2D32f*)cvGetSeqElem(seq_fourier, i);
			int t = i * 2;
			feature[t] = pt->x;
			feature[t+1] = pt->y;
		}
		re = 1;
	}

	cvReleaseMemStorage(&storage);
	cvReleaseImage(&gray);

	return re;
}

void CalcFourierDescriptorCoeff(CvSeq* seq_pts, int n_fourier,CvSeq* seq_fourier)
{
	int count = seq_pts->total;
	double *coeff_cos, *coeff_sin;
	coeff_cos = (double*)malloc(count*sizeof(double));
	coeff_sin = (double*)malloc(count*sizeof(double));
	int i;
	for(i = 0; i < count; i++)
	{
		coeff_sin[i] = sin(2*i*CV_PI/count);
		coeff_cos[i] = cos(2*i*CV_PI/count);
	}

	cvClearSeq(seq_fourier);
	for(int u = 0; u < n_fourier; u++)
	{
		CvPoint2D32f point_coeff = cvPoint2D32f(0, 0);
		for(i = 0; i < count; i+=4)
		{
			CvPoint* pt = (CvPoint*)cvGetSeqElem(seq_pts, i);
			point_coeff.x += (float)(pt->x*coeff_cos[(i*u)%count] + pt->y*coeff_sin[(i*u)%count]);
			point_coeff.y += (float)(pt->y*coeff_cos[(i*u)%count] - pt->x*coeff_sin[(i*u)%count]);
		}
		point_coeff.x/=count;
		point_coeff.y/=count;
		cvSeqPush(seq_fourier, &point_coeff);
	} 
	free(coeff_cos);
	free(coeff_sin);
}

void CalcBoundary(CvSeq* seq_fourier, int n_Pts, CvSeq* seq_pts)
{
	int count = seq_fourier->total;
	double *coeff_cos, *coeff_sin;
	coeff_cos = (double*)malloc(n_Pts*sizeof(double));
	coeff_sin = (double*)malloc(n_Pts*sizeof(double));
	int i;
	for(i = 0; i < n_Pts; i++)
	{
		coeff_sin[i] = sin(2*i*CV_PI/n_Pts);
		coeff_cos[i] = cos(2*i*CV_PI/n_Pts);
	}
	cvClearSeq(seq_pts);
	for(i = 0; i < n_Pts; i++)
	{
		CvPoint pt = cvPoint(0, 0);
		double sumx = 0, sumy = 0;
		for(int u = 0; u < count; u++)
		{
			CvPoint2D32f* point_coeff = (CvPoint2D32f*)cvGetSeqElem(seq_fourier, u);
			sumx += point_coeff->x*coeff_cos[(i*u)%n_Pts] - point_coeff->y*coeff_sin[(i*u)%n_Pts];
			sumy += point_coeff->y*coeff_cos[(i*u)%n_Pts] + point_coeff->x*coeff_sin[(i*u)%n_Pts];
		}
		pt.x = cvRound(sumx*n_Pts/count);
		pt.y = cvRound(sumy*n_Pts/count);
		cvSeqPush(seq_pts, &pt);
	}
	free(coeff_cos);
	free(coeff_sin);
}
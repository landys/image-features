#include "cv.h"
#include "highgui.h"
#include <stdio.h>

#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "highgui.lib")

// input: seq_pts, n_fourier; output: seq_fourier
void CalcFourierDescriptorCoeff(CvSeq* seq_pts, int n_fourier,CvSeq* seq_fourier);
// input: seq_fourier, n_pts; output: seq_pts. It just the opposite operation of CalcFourierDescriptorCoeff
void CalcBoundary(CvSeq* seq_fourier, int n_Pts,CvSeq* seq_pts);

int main(int argc, char* argv[])
{
	IplImage* color = cvLoadImage("E:\\pic_skindetect\\clothtest\\2.jpg", 1);
	IplImage* gray = cvCreateImage(cvGetSize(color), 8, 1);
	IplImage* show = cvCreateImage(cvGetSize(color), 8, 1);
	cvZero(show);
	int i = 0;

	cvCvtColor(color, gray, CV_RGB2GRAY);
	//cvThreshold(gray, gray, 100, 255, CV_THRESH_BINARY_INV);
	cvCanny(gray, gray, 50, 150, 3); 
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq* contours;
	CvSeq* seq_fourier = cvCreateSeq(CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint2D32f), storage);
	cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_TREE);

	CvSeq* mostContours = contours;
	/*for(; contours; contours = contours->h_next)
	{
		if (mostContours->total < contours->total)
		{
			mostContours = contours;
		}
	}*/

	int t = 0;
	for(; contours; contours = contours->h_next)
	{
	//contours = mostContours;
		++t;
		printf("%d\n", contours->total);
		cvDrawContours(color, contours, CV_RGB(255,0,0), CV_RGB(255,0,0), 1, 3);
		CalcFourierDescriptorCoeff(contours, 2000, seq_fourier);
		CalcBoundary(seq_fourier, contours->total, contours);

		for(int i = 0; i < contours->total; i++)
		{
			CvPoint* pt=(CvPoint*)cvGetSeqElem(contours, i);
			if(pt->x >= 0 && pt->x < show->width && pt->y >= 0 && pt->y < show->height)
			{
				((uchar*)(show->imageData+pt->y*show->widthStep))[pt->x] = 255;
			}
		}

		/*for(i = 0; i < contours->total; i++)
		{
			CvPoint* pt=(CvPoint*)cvGetSeqElem(contours, i);
			printf("%d, %d, %d\n", pt->x, pt->y, i);
		}*/
/*
		for(i = 0; i < seq_fourier->total; i++)
		{
			CvPoint2D32f* pt=(CvPoint2D32f*)cvGetSeqElem(seq_fourier, i);
			printf("%f, %f, %d\n", pt->x, pt->y, i);
		}*/
	}
	printf("t=%d\n", t);

	cvNamedWindow("color", 0);
	cvShowImage("color",color);
	//cvWaitKey(0);

	cvNamedWindow("gray", 0);
	cvShowImage("gray", gray);
	//cvWaitKey(0);

	cvNamedWindow("reconstructed", 0);
	cvShowImage("reconstructed", show);
	cvWaitKey(0);
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&color);
	cvReleaseImage(&gray);
	cvReleaseImage(&show);
	cvDestroyAllWindows();
	return 0;
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
		//point_coeff.x/=count;
		//point_coeff.y/=count;
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
		pt.x = cvRound(sumx/count);
		pt.y = cvRound(sumy/count);
		cvSeqPush(seq_pts, &pt);
	}
	free(coeff_cos);
	free(coeff_sin);
}

// Rearrange the quadrants of Fourier image so that the origin is at
// the image center
// src & dst arrays of equal size & type
/*void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr )
{
	CvMat * tmp;
	CvMat q1stub, q2stub;
	CvMat q3stub, q4stub;
	CvMat d1stub, d2stub;
	CvMat d3stub, d4stub;
	CvMat * q1, * q2, * q3, * q4;
	CvMat * d1, * d2, * d3, * d4;

	CvSize size = cvGetSize(src_arr);
	CvSize dst_size = cvGetSize(dst_arr);
	int cx, cy;

	if(dst_size.width != size.width || 
		dst_size.height != size.height){
			cvError( CV_StsUnmatchedSizes, "cvShiftDFT", "Source and Destination arrays must have equal sizes", __FILE__, __LINE__ );   
	}

	if(src_arr==dst_arr){
		tmp = cvCreateMat(size.height/2, size.width/2, cvGetElemType(src_arr));
	}

	cx = size.width/2;
	cy = size.height/2; // image center

	q1 = cvGetSubRect( src_arr, &q1stub, cvRect(0,0,cx, cy) );
	q2 = cvGetSubRect( src_arr, &q2stub, cvRect(cx,0,cx,cy) );
	q3 = cvGetSubRect( src_arr, &q3stub, cvRect(cx,cy,cx,cy) );
	q4 = cvGetSubRect( src_arr, &q4stub, cvRect(0,cy,cx,cy) );
	d1 = cvGetSubRect( src_arr, &d1stub, cvRect(0,0,cx,cy) );
	d2 = cvGetSubRect( src_arr, &d2stub, cvRect(cx,0,cx,cy) );
	d3 = cvGetSubRect( src_arr, &d3stub, cvRect(cx,cy,cx,cy) );
	d4 = cvGetSubRect( src_arr, &d4stub, cvRect(0,cy,cx,cy) );

	if(src_arr!=dst_arr){
		if( !CV_ARE_TYPES_EQ( q1, d1 )){
			cvError( CV_StsUnmatchedFormats, "cvShiftDFT", "Source and Destination arrays must have the same format", __FILE__, __LINE__ ); 
		}
		cvCopy(q3, d1, 0);
		cvCopy(q4, d2, 0);
		cvCopy(q1, d3, 0);
		cvCopy(q2, d4, 0);
	}
	else{
		cvCopy(q3, tmp, 0);
		cvCopy(q1, q3, 0);
		cvCopy(tmp, q1, 0);
		cvCopy(q4, tmp, 0);
		cvCopy(q2, q4, 0);
		cvCopy(tmp, q2, 0);
	}
}

int main(int argc, char ** argv)
{
	const char* filename = argc >=2 ? argv[1] : "d:\\pics\\1.jpg";
	IplImage * im;

	IplImage * realInput;
	IplImage * imaginaryInput;
	IplImage * complexInput;
	int dft_M, dft_N;
	CvMat* dft_A, tmp;
	IplImage * image_Re;
	IplImage * image_Im;
	double m, M;

	im = cvLoadImage( filename, CV_LOAD_IMAGE_GRAYSCALE );
	if( !im )
		return -1;

	realInput = cvCreateImage( cvGetSize(im), IPL_DEPTH_64F, 1);
	imaginaryInput = cvCreateImage( cvGetSize(im), IPL_DEPTH_64F, 1);
	complexInput = cvCreateImage( cvGetSize(im), IPL_DEPTH_64F, 2);

	cvScale(im, realInput, 1.0, 0.0);
	cvZero(imaginaryInput);
	cvMerge(realInput, imaginaryInput, NULL, NULL, complexInput);

	dft_M = cvGetOptimalDFTSize( im->height - 1 );
	dft_N = cvGetOptimalDFTSize( im->width - 1 );

	dft_A = cvCreateMat( dft_M, dft_N, CV_64FC2 );
	image_Re = cvCreateImage( cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);
	image_Im = cvCreateImage( cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect( dft_A, &tmp, cvRect(0,0, im->width, im->height));
	cvCopy( complexInput, &tmp, NULL );
	if( dft_A->cols > im->width )
	{
		cvGetSubRect( dft_A, &tmp, cvRect(im->width,0, dft_A->cols - im->width, im->height));
		cvZero( &tmp );
	}

	// no need to pad bottom part of dft_A with zeros because of
	// use nonzero_rows parameter in cvDFT() call below

	cvDFT( dft_A, dft_A, CV_DXT_FORWARD, complexInput->height );

	cvNamedWindow("win", 0);
	cvNamedWindow("magnitude", 0);
	cvShowImage("win", im);

	// Split Fourier in real and imaginary parts
	cvSplit( dft_A, image_Re, image_Im, 0, 0 );

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	cvPow( image_Re, image_Re, 2.0);
	cvPow( image_Im, image_Im, 2.0);
	cvAdd( image_Re, image_Im, image_Re, NULL);
	cvPow( image_Re, image_Re, 0.5 );

	// Compute log(1 + Mag)
	cvAddS( image_Re, cvScalarAll(1.0), image_Re, NULL ); // 1 + Mag
	cvLog( image_Re, image_Re ); // log(1 + Mag)


	// Rearrange the quadrants of Fourier image so that the origin is at
	// the image center
	cvShiftDFT( image_Re, image_Re );

	cvMinMaxLoc(image_Re, &m, &M, NULL, NULL, NULL);
	cvScale(image_Re, image_Re, 1.0/(M-m), 1.0*(-m)/(M-m));
	cvShowImage("magnitude", image_Re);

	cvWaitKey(-1);
	return 0;
}*/
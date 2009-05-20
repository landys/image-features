#include "Stdafx.h"
#include "Gabor.h"

CGabor::CGabor(void)
{
	m_bInitialised	= false;
	m_bKernel		= false;
	m_bConvImage	= false;
}

CGabor::~CGabor(void)
{
}

CGabor::CGabor(float fSigma, int nScale, float fAngle)
{
	m_fSigma = fSigma;
	m_nScale = nScale;
	m_fAngle = fAngle;
	Init();
}

void CGabor::Init()
{
	m_bInitialised = false;
	m_fRealScale = 6 * m_fSigma / m_nScale;  
	
	//¼ÆËãºË´°¿í¶È
	float k = (CV_PI/2)/pow(sqrt(2.0), m_nScale);   
	float w  = ((m_fSigma/k) * 6) + 1;  
	m_lWidth = (long)w;
	if (fmod(m_lWidth, 2.0) == 0.0) m_lWidth++;
	
	m_matReal = cvCreateMat( m_lWidth, m_lWidth, CV_32FC1);
	m_matImag = cvCreateMat( m_lWidth, m_lWidth, CV_32FC1);

	m_bInitialised = true;
	CreatKernel();
}

bool CGabor::IsInit()
{

	return m_bInitialised;
}


void CGabor::CreatKernel()
{
	long Width = m_lWidth;
	if (IsInit() == false) {perror("Error: The Object has not been initilised in CreatKernel()!\n");}
	else {
		CvMat *mReal, *mImag;
		mReal = cvCreateMat( Width, Width, CV_32FC1);
		mImag = cvCreateMat( Width, Width, CV_32FC1);

		/**************************** Gabor Function ****************************/ 
		int x, y;
		float dReal;
		float dImag;
		float dTemp1, dTemp2, dTemp3;

		for (int i = 0; i < Width; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				x = i-(Width-1)/2;
				y = j-(Width-1)/2;
				dTemp1 = (1/(2*CV_PI*pow(m_fSigma, 2))) *exp(-((pow((float)x,2)+pow((float)y,2))/(2*pow(m_fSigma,2))));
				dTemp2 = cos((2*CV_PI*(x*cos(m_fAngle)+y*sin(m_fAngle)))/m_fRealScale);
				dTemp3 = sin((2*CV_PI*(x*cos(m_fAngle)+y*sin(m_fAngle)))/m_fRealScale);
				dReal = dTemp1 * dTemp2;
				dImag = dTemp1 * dTemp3;
				/*dTemp1 = (pow(K,2)/pow(Sigma,2))*exp(-(pow((float)x,2)+pow((float)y,2))*pow(K,2)/(2*pow(Sigma,2)));
				dTemp2 = cos(K*cos(Phi)*x + K*sin(Phi)*y) - exp(-(pow(Sigma,2)/2));
				dTemp3 = sin(K*cos(Phi)*x + K*sin(Phi)*y);
				dReal = dTemp1*dTemp2;
				dImag = dTemp1*dTemp3; */
				cvmSet( (CvMat*)mReal, i, j, dReal );
				cvmSet( (CvMat*)mImag, i, j, dImag );

			} 
		}
		/**************************** Gabor Function ****************************/
		m_bKernel = true;
		cvCopy(mReal, m_matReal, NULL);
		cvCopy(mImag, m_matImag, NULL);
		//printf("A %d x %d Gabor kernel created.\n", m_lWidth, m_lWidth);
		cvReleaseMat( &mReal );
		cvReleaseMat( &mImag );
	}
}

bool CGabor::IsKernelCreate()
{

	return m_bKernel;
}

long CGabor::GetMaskWidth()
{
	return m_lWidth;
}

CvMat* CGabor::GetMatrix(int Type)
{
	if (!IsKernelCreate()) {perror("Error: the gabor kernel has not been created!\n"); return NULL;}
	switch (Type)
	{
	case REAL:
		return m_matReal;
		break;
	case IMAG:
		return m_matImag;
		break;
	default:
		break;
	}
}

void CGabor::ConvImage(IplImage *src, IplImage *dst, int Type, CvMat* realmat, CvMat* imagmat)
{
	double ve, re,im;

	CvMat *mat = cvCreateMat(src->width, src->height, CV_32FC1);
	for (int i = 0; i < src->width; i++)
	{
		for (int j = 0; j < src->height; j++)
		{
			ve = cvGetReal2D((IplImage*)src, j, i);
			cvSetReal2D( (CvMat*)mat, i, j, ve );
		}
	}

	CvMat *rmat = cvCreateMat(src->width, src->height, CV_32FC1);
	CvMat *imat = cvCreateMat(src->width, src->height, CV_32FC1);

	long Width = realmat->cols;
	CvMat *kernel = cvCreateMat( Width, Width, CV_32FC1 );

	switch (Type)
	{
	case REAL:
		cvCopy( (CvMat*)realmat, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)mat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		break;
	case IMAG:
		cvCopy( (CvMat*)imagmat, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)mat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		break;
	case MAG:
		/* Real Response */
		cvCopy( (CvMat*)realmat, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)rmat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		/* Imag Response */
		cvCopy( (CvMat*)imagmat, (CvMat*)kernel, NULL );
		cvFilter2D( (CvMat*)mat, (CvMat*)imat, (CvMat*)kernel, cvPoint( (Width-1)/2, (Width-1)/2));
		/* Magnitude response is the square root of the sum of the square of real response and imaginary response */
		for (int i = 0; i < mat->rows; i++)
		{
			for (int j = 0; j < mat->cols; j++)
			{
				re = cvGetReal2D((CvMat*)rmat, i, j);
				im = cvGetReal2D((CvMat*)imat, i, j);
				ve = sqrt(re*re + im*im);
				cvSetReal2D( (CvMat*)mat, i, j, ve );
			}
		}       
		break;
	default:
		break;
	}

	if (dst->depth == IPL_DEPTH_8U)
	{
		cvNormalize((CvMat*)mat, (CvMat*)mat, 0, 255, CV_MINMAX);
		for (int i = 0; i < mat->rows; i++)
		{
			for (int j = 0; j < mat->cols; j++)
			{
				ve = cvGetReal2D((CvMat*)mat, i, j);
				ve = cvRound(ve);
				cvSetReal2D( (IplImage*)dst, j, i, ve );
			}
		}
	}

	if (dst->depth == IPL_DEPTH_32F)
	{
		for (int i = 0; i < mat->rows; i++)
		{
			for (int j = 0; j < mat->cols; j++)
			{
				ve = cvGetReal2D((CvMat*)mat, i, j);
				cvSetReal2D( (IplImage*)dst, j, i, ve );
			}
		}
	}       

	cvReleaseMat(&kernel);
	cvReleaseMat(&imat);   
	cvReleaseMat(&rmat);
	cvReleaseMat(&mat);
}

bool CGabor::IsConvImage()
{
	return m_bConvImage;
}

void CGabor::ShowKernel(int Type)
{
	if(!IsInit()) {
		perror("Error: the gabor kernel has not been created!\n");
	}
	else {
		IplImage *pImage;
		pImage = GetKernelImage(Type);
		cvNamedWindow("Testing",1);
		cvShowImage("Testing",pImage);
		cvWaitKey(0);
		cvDestroyWindow("Testing");
		cvReleaseImage(&pImage);
	}

}

IplImage* CGabor::GetKernelImage(int Type)
{
	long Width = m_lWidth;
	if(IsKernelCreate() == false)
	{ 
		perror("Error: the Gabor kernel has not been created in get_image()!\n");
		return NULL;
	}
	else
	{  
		IplImage* pImage;
		IplImage *newimage;
		newimage = cvCreateImage(cvSize(Width,Width), IPL_DEPTH_8U, 1 );
		//printf("Width is %d.\n",(int)Width);
		//printf("Sigma is %f.\n", Sigma);
		//printf("F is %f.\n", F);
		//printf("Phi is %f.\n", Phi);

		//pImage = gan_image_alloc_gl_d(Width, Width);
		pImage = cvCreateImage( cvSize(Width,Width), IPL_DEPTH_32F, 1 );


		CvMat* kernel = cvCreateMat(Width, Width, CV_32FC1);
		double ve;
		CvScalar S;
		switch(Type)
		{
		case 1:  //Real

			cvCopy( (CvMat*)m_matReal, (CvMat*)kernel, NULL );
			//pImage = cvGetImage( (CvMat*)kernel, pImageGL );
			for (int i = 0; i < kernel->rows; i++)
			{
				for (int j = 0; j < kernel->cols; j++)
				{
					ve = cvGetReal2D((CvMat*)kernel, i, j);
					cvSetReal2D( (IplImage*)pImage, j, i, ve );
				}
			}
			break;
		case 2:  //Imag
			cvCopy( (CvMat*)m_matImag, (CvMat*)kernel, NULL );
			//pImage = cvGetImage( (CvMat*)kernel, pImageGL );
			for (int i = 0; i < kernel->rows; i++)
			{
				for (int j = 0; j < kernel->cols; j++)
				{
					ve = cvGetReal2D((CvMat*)kernel, i, j);
					cvSetReal2D( (IplImage*)pImage, j, i, ve );
				}
			}
			break; 
		default:
			break;
		}

		cvNormalize((IplImage*)pImage, (IplImage*)pImage, 0, 255, CV_MINMAX, NULL );


		cvConvertScaleAbs( (IplImage*)pImage, (IplImage*)newimage, 1, 0 );

		cvReleaseMat(&kernel);

		cvReleaseImage(&pImage);

		return newimage;
	}
}


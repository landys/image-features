#include "Stdafx.h"
#include "GaborFeature.h"
#include "Gabor.h"
#include <sstream>
#include <io.h>

GaborFeature::GaborFeature(void)
{
	m_fSigma	= CV_PI ;      
	m_lScale[0] = 2;		
	m_lScale[1] = 4;
	m_lScale[2] = 8;		
	m_nAngleNum = 8;        //相位个数

	m_bCreateFilter =false;
}

GaborFeature::~GaborFeature(void)
{
}

bool GaborFeature::CreateFilterArray()
{
	m_bCreateFilter = false;
	for (int i=0; i<3; i++)
		for (int j=0; j<m_nAngleNum; j++)
		{
			CGabor gabor(m_fSigma, m_lScale[i], j*CV_PI/8);
			m_FilterArray[i*8+j].realmat = gabor.GetMatrix(REAL);
			m_FilterArray[i*8+j].imagmat = gabor.GetMatrix(IMAG);

		}
	m_bCreateFilter = true;
	return true;
}

bool GaborFeature::CreateImageFeatureVec(IplImage* src)
{
	//cvNamedWindow("test", 1);
	if (!m_bCreateFilter)
	{
		CreateFilterArray();
	}

	char filename[100]; 
	IplImage* dst = cvCreateImage(cvSize(src->width, src->height), 8, 1);
	for (int i=0; i<3; i++)
		for (int j=0; j<m_nAngleNum; j++)
		{
			//CGabor gabor(m_fSigma, m_lScale[i], j*CV_PI/8);
			CGabor::ConvImage(src, dst, MAG, m_FilterArray[i*8+j].realmat, m_FilterArray[i*8+j].imagmat);

			//sprintf(filename, "E:\\pic_skindetect\\clothtest\\gabortest\\%d.bmp", i*8+j);
			//cvSaveImage(filename, dst);
			//cvShowImage("test", dst);
			//Sleep(100);
			m_dVector[(i*8+j)*2] = GetImgEx(dst);
			m_dVector[(i*8+j)*2+1] = GetImgVa(dst);
		}
	cvReleaseImage(&dst);
	//cvDestroyAllWindows();
	return true;
}

double GaborFeature::GetImgEx(IplImage* src)
{
	double ex = 0;
	for (int i = 0; i < src->width; i++)
	{
		for (int j = 0; j < src->height; j++)
		{
			ex += cvGetReal2D((IplImage*)src, j, i);
		}
	}
	return ex/(src->width * src->height);
}

double GaborFeature::GetImgVa(IplImage* src)
{
	double ex = GetImgEx(src);
	double va = 0;
	for (int i = 0; i < src->width; i++)
	{
		for (int j = 0; j < src->height; j++)
		{
			va += pow((cvGetReal2D((IplImage*)src, j, i) -ex), 2);
		}
	}
	return sqrt(va/(src->width * src->height));
}

double*	GaborFeature::getVector()
{
	return m_dVector;
}

/*void GaborFeature::CreateVecFile(string inFilepath, string vecfilename)
{
	FILE* fp = fopen(vecfilename.c_str(), "w+");
	struct _finddata_t c_file;
	long hFile = -1L;
	int fileNum = 0;
	IplImage* image;

	string inFilename = inFilepath + "*.bmp";
	string outFilename;
	stringstream strstream;
	string imagetype = ".bmp";

	if((hFile = _findfirst(inFilename.c_str(), &c_file )) == -1L ) {
		return ;
	}
	do 
	{
		string imagename(c_file.name);
		inFilename = inFilepath  + imagename;
		image = cvLoadImage(inFilename.c_str(), 1);
		IplImage* grayimage = cvCreateImage(cvSize(image->width, image->height), 8, 1);
		cvCvtColor(image, grayimage, CV_BGRA2GRAY);
		CreateImageFeatureVec(grayimage);
		fprintf(fp, inFilename.c_str());
		for (int i=0; i<48; i++)
		{
			fprintf(fp, " %lf", m_dVector[i]);
		}
		fprintf(fp, "\n");
		printf("%s 处理完毕！\n", inFilename.c_str());
		cvReleaseImage(&grayimage);

	} while(_findnext( hFile, &c_file ) == 0);


	fclose(fp);
}*/

void GaborFeature::initGaborFilter()
{
	if (!m_bCreateFilter)
	{
		CreateFilterArray();
	}
}

int GaborFeature::extractGaborFeature(const char* imgfile)
{
	IplImage* img = cvLoadImage(imgfile, CV_LOAD_IMAGE_GRAYSCALE);
	if (!img)
	{
		return 0;
	}

	CreateImageFeatureVec(img);

	cvReleaseImage(&img);

	return 1;
}
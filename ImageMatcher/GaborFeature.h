#pragma once
#include <string>
#include "highgui.h"
#include "cv.h"
#include "cxcore.h"
using namespace std;

typedef struct  
{
	CvMat* realmat;
	CvMat* imagmat;
}GABOR_FILTER;

class GaborFeature
{
public:
	GaborFeature(void);

public:
	~GaborFeature(void);
private:
	float			m_fSigma;      
	long			m_lScale[3];		//�߶�����
	int				m_nAngleNum;        //��λ����
	//int			m_nFilterNum;       //Gabor �˲�������
	double			m_dVector[48];      //����ֵ����
	GABOR_FILTER	m_FilterArray[24];   //�˲�����
	bool			m_bCreateFilter;
private:
	bool			CreateFilterArray(); //�����˲�����
	bool			CreateImageFeatureVec(IplImage* image);
	double			GetImgEx(IplImage* image);  //����ͼ���ֵ
	double			GetImgVa(IplImage* image);  //����ͼ�񷽲�
	
	//void			CreateVecFile(string imgfilepath, string vecfilename);
public:
	void initGaborFilter();
	int extractGaborFeature(const char* imgfile);
	double*	getVector();   //��ȡ����ֵ����
};

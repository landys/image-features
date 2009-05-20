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
	long			m_lScale[3];		//尺度数组
	int				m_nAngleNum;        //相位个数
	//int			m_nFilterNum;       //Gabor 滤波器个数
	double			m_dVector[48];      //特征值向量
	GABOR_FILTER	m_FilterArray[24];   //滤波器组
	bool			m_bCreateFilter;
private:
	bool			CreateFilterArray(); //生成滤波器组
	bool			CreateImageFeatureVec(IplImage* image);
	double			GetImgEx(IplImage* image);  //计算图像均值
	double			GetImgVa(IplImage* image);  //计算图像方差
	
	//void			CreateVecFile(string imgfilepath, string vecfilename);
public:
	void initGaborFilter();
	int extractGaborFeature(const char* imgfile);
	double*	getVector();   //获取特征值向量
};

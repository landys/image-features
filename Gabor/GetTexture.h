#pragma once
#include "Gabor.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <io.h>
using namespace std;

typedef struct  
{
	CvMat* realmat;
	CvMat* imagmat;
}GABOR_FILTER;

class CGetTexture
{
public:
	CGetTexture(void);

public:
	~CGetTexture(void);
private:
	float			m_fSigma;      
	long			m_lScale[3];		//�߶�����
	int				m_nAngleNum;        //��λ����
	//int			m_nFilterNum;       //Gabor �˲�������
	double			m_dVector[48];      //����ֵ����
	GABOR_FILTER	m_FilterArray[24];   //�˲�����
	bool			m_bCreateFilter;
public:
	bool			CreateFilterArray(); //�����˲�����
	bool			CreateImageFeatureVec(IplImage* image);
	double			GetImgEx(IplImage* image);  //����ͼ���ֵ
	double			GetImgVa(IplImage* image);  //����ͼ�񷽲�
	double*			GetVector();   //��ȡ����ֵ����
	void			CreateVecFile(string imgfilepath, string vecfilename);
};

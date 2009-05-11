#pragma once

#include "highgui.h"
#include "cv.h"
#include "cxcore.h"
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "highgui.lib")
#include <iostream>
using namespace std;

#define REAL 1   
#define IMAG 2
#define MAG  3


class CGabor
{
public:
	CGabor(void);
	CGabor(float fSigma, int nScale, float fAngle); 
public:
	~CGabor(void);
public:
	void			Init();					//��ʼ������������
	void			CreatKernel();          //��������
	bool			IsInit();
	long			GetMaskWidth();	
	bool			IsKernelCreate();
	bool			IsConvImage();
	CvMat*			GetMatrix(int Type);
	void			ShowKernel(int Type);
	IplImage*		GetKernelImage(int Type);
	static void		ConvImage(IplImage *src, IplImage *dst, int Type, CvMat* realmat, CvMat* imagmat);

private:
	float		m_fSigma;     
	int			m_nScale;				//�߶�	
	float		m_fRealScale;
	float		m_fAngle;				//��λ��
	long		m_lWidth;				//�˿��
	CvMat *		m_matImag;				//ʵ���˾���
	CvMat *		m_matReal;				//�鲿�˾���
	bool		m_bInitialised;
	bool		m_bKernel;
	bool		m_bConvImage;
};

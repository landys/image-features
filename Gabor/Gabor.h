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
	void			Init();					//初始化参数及矩阵
	void			CreatKernel();          //计算卷积核
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
	int			m_nScale;				//尺度	
	float		m_fRealScale;
	float		m_fAngle;				//相位角
	long		m_lWidth;				//核宽度
	CvMat *		m_matImag;				//实部核矩阵
	CvMat *		m_matReal;				//虚部核矩阵
	bool		m_bInitialised;
	bool		m_bKernel;
	bool		m_bConvImage;
};

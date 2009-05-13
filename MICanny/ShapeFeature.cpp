// ShapeFeature.cpp: implementation of the CShapeFeature class.
//
//////////////////////////////////////////////////////////////////////
#include<math.h>
#include "ShapeFeature.h"
#include <stack>
#include <utility>
using namespace std;
//#include<iostream.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShapeFeature::CShapeFeature(CImg * img):CFeatureBase(img)
{
	m_dimension = 7;

	m_feature = (double *)malloc(m_dimension*sizeof(double));
	memset(m_feature,0,7*sizeof(double));
	
}

CShapeFeature::~CShapeFeature()
{
	free(m_feature);
}




/********************************边缘检测和提取******************************/

/*************************************************************************
 *
 * \函数名称：
 *   MakeGauss()
 *
 * \输入参数:
 *   double sigma									        - 高斯函数的标准差
 *   double **pdKernel										- 指向高斯数据数组的指针
 *   int *pnWindowSize										- 数据的长度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数可以生成一个一维的高斯函数的数字数据，理论上高斯数据的长度应
 *   该是无限长的，但是为了计算的简单和速度，实际的高斯数据只能是有限长的
 *   pnWindowSize就是数据长度
 *   
 *************************************************************************
 */
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize)
{
	// 循环控制变量
	int i   ;
	
	// 数组的中心点
	int nCenter;

	// 数组的某一点到中心点的距离
	double  dDis  ; 

	double PI = 3.14159;
	// 中间变量
	double  dValue; 
	double  dSum  ;
	dSum = 0 ; 
	
	// 数组长度，根据概率论的知识，选取[-3*sigma, 3*sigma]以内的数据。
	// 这些数据会覆盖绝大部分的滤波系数
	*pnWindowSize = (int)1 + 2 * ceil(3 * sigma);
	
	// 中心
	nCenter = (*pnWindowSize) / 2;
	
	// 分配内存
	*pdKernel = new double[*pnWindowSize] ;
	
	for(i=0; i< (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma)) / (sqrt(2 * PI) * sigma );
		(*pdKernel)[i] = dValue ;
		dSum += dValue;
	}
	
	// 归一化
	for(i=0; i<(*pnWindowSize) ; i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}

/*************************************************************************
 *
 * \函数名称：
 *   GaussianSmooth()
 *
 * \输入参数:
 *   int * pUnchImg				- 指向图象数据的指针
 *   int nWidth											- 图象数据宽度
 *   int nHeight										- 图象数据高度
 *   double dSigma									- 高斯函数的标准差
 *   int * pUnchSmthdImg	- 指向经过平滑之后的图象数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   为了抑止噪声，采用高斯滤波对图象进行滤波，滤波先对x方向进行，然后对
 *   y方向进行。
 *   
 *************************************************************************
 */
void GaussianSmooth(int *pUnchImg, int nWidth, int nHeight, 
										double sigma, int * pUnchSmthdImg)
{
	// 循环控制变量
  int y;
	int x;
	
	int i;
	
	// 高斯滤波器的数组长度
	
	int nWindowSize;
	
	//  窗口长度的1/2
	int	nHalfLen;   
	
	// 一维高斯数据滤波器
	double *pdKernel ;
	
	// 高斯系数与图象数据的点乘
	double  dDotMul     ;
	
	// 高斯滤波系数的总和
	double  dWeightSum     ;          
  
	// 中间变量
	double * pdTmp ;
	
	// 分配内存
	pdTmp = new double[nWidth*nHeight];
	
	// 产生一维高斯数据滤波器
	// MakeGauss(sigma, &dKernel, &nWindowSize);
	MakeGauss(sigma, &pdKernel, &nWindowSize) ;
	
	// MakeGauss返回窗口的长度，利用此变量计算窗口的半长
	nHalfLen = nWindowSize / 2;
	
  // x方向进行滤波
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// 判断是否在图象内部
				if( (i+x) >= 0  && (i+x) < nWidth )
				{
					dDotMul += (double)pUnchImg[y*nWidth + (i+x)] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pdTmp[y*nWidth + x] = dDotMul/dWeightSum ;
		}
	}
	
	// y方向进行滤波
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// 判断是否在图象内部
				if( (i+y) >= 0  && (i+y) < nHeight )
				{
					dDotMul += (double)pdTmp[(y+i)*nWidth + x] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pUnchSmthdImg[y*nWidth + x] = (int)dDotMul/dWeightSum ;
		}
	}

	// 释放内存
	delete []pdKernel;
	pdKernel = NULL ;
	
	delete []pdTmp;
	pdTmp = NULL;
}


/*************************************************************************
 *
 * \函数名称：
 *   DirGrad()
 *
 * \输入参数:
 *   int *pUnchSmthdImg					- 经过高斯滤波后的图象
 *   int nWidht														- 图象宽度
 *   int nHeight      										- 图象高度
 *   int *pnGradX                         - x方向的方向导数
 *   int *pnGradY                         - y方向的方向导数
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数计算方向倒数，采用的微分算子是(-1 0 1) 和 (-1 0 1)'(转置)
 *   计算的时候对边界象素采用了特殊处理
 *   
 *   
 *************************************************************************
 */
void DirGrad(int *pUnchSmthdImg, int nWidth, int nHeight,
										 int *pnGradX , int *pnGradY)
{
	// 循环控制变量
	int y ;
	int x ;
	
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pnGradX[y*nWidth+x] = (int) ( pUnchSmthdImg[y*nWidth+min(nWidth-1,x+1)]  
													          - pUnchSmthdImg[y*nWidth+max(0,x-1)]     );
		}
	}

	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			pnGradY[y*nWidth+x] = (int) ( pUnchSmthdImg[min(nHeight-1,y+1)*nWidth + x]  
																		- pUnchSmthdImg[max(0,y-1)*nWidth+ x ]     );
		}
	}
}

/*************************************************************************
 *
 * \函数名称：
 *   GradMagnitude()
 *
 * \输入参数:
 *   int *pnGradX                         - x方向的方向导数
 *   int *pnGradY                         - y方向的方向导数
 *   int nWidht														- 图象宽度
 *   int nHeight      										- 图象高度
 *   int *pnMag                           - 梯度幅度   
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数利用方向倒数计算梯度幅度，方向倒数是DirGrad函数计算的结果
 *   
 *************************************************************************
 */
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag)
{
	
	// 循环控制变量
	int y ;
	int x ;

	// 中间变量
	double dSqtOne;
	double dSqtTwo;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dSqtOne = pnGradX[y*nWidth + x] * pnGradX[y*nWidth + x];
			dSqtTwo = pnGradY[y*nWidth + x] * pnGradY[y*nWidth + x];
			pnMag[y*nWidth + x] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
		}
	}
}
/*************************************************************************
 *
 * \函数名称：
 *   NonmaxSuppress()
 *
 * \输入参数:
 *   int *pnMag                - 梯度图
 *   int *pnGradX							 - x方向的方向导数	
 *   int *pnGradY              - y方向的方向导数
 *   int nWidth                - 图象数据宽度
 *   int nHeight               - 图象数据高度
 *   int *pUnchRst   - 经过NonmaxSuppress处理后的结果
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   抑止梯度图中非局部极值点的象素。
 *   
 *************************************************************************
 */
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
										int nHeight,	int *pUnchRst)
{
	// 循环控制变量
	int y ;
	int x ;
	int nPos;

	// x方向梯度分量
	int gx  ;
	int gy  ;

	// 临时变量
	int g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;
	
	// 设置图象边缘部分为不可能的边界点
	for(x=0; x<nWidth; x++)		
	{
		pUnchRst[x] = 0 ;
		pUnchRst[nHeight-1+x] = 0;
	}
	for(y=0; y<nHeight; y++)		
	{
		pUnchRst[y*nWidth] = 0 ;
		pUnchRst[y*nWidth + nWidth-1] = 0;
	}

	for(y=1; y<nHeight-1; y++)
	{
		for(x=1; x<nWidth-1; x++)
		{
			nPos = y*nWidth + x ;
			
			// 如果当前象素的梯度幅度为0，则不是边界点
			if(pnMag[nPos] == 0 )
			{
				pUnchRst[nPos] = 0 ;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pnMag[nPos] ;
				
				// x，y方向导数
				gx = pnGradX[nPos]  ;
				gy = pnGradY[nPos]  ;

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx)) 
				{
					// 计算插值的比例
					weight = fabs(double(gx))/fabs(double(gy)); 

					g2 = pnMag[nPos-nWidth] ; 
					g4 = pnMag[nPos+nWidth] ;
					
					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pnMag[nPos-nWidth-1] ;
						g3 = pnMag[nPos+nWidth+1] ;
					} 

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//		 C         
					//	g3 g4  
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					} 
				}
				
				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs(double(gy))/fabs(double(gx)); 
					
					g2 = pnMag[nPos+1] ; 
					g4 = pnMag[nPos-1] ;
					
					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pnMag[nPos+nWidth+1] ;
						g3 = pnMag[nPos-nWidth-1] ;
					} 
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;
					
					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pUnchRst[nPos] = 128 ;
					}
					else
					{
						// 不可能是边界点
						pUnchRst[nPos] = 0 ;
					}
				}
			} //else
		} // for

	}
} 

/*************************************************************************
 *
 * \函数名称：
 *   EstimateThreshold()
 *
 * \输入参数:
 *   int *pnMag               - 梯度幅度图
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *   int *pnThdHigh           - 高阈值
 *   int *pnThdLow            - 低阈值
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   int *pUnchEdge - 经过non-maximum处理后的数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   经过non-maximum处理后的数据pUnchEdge，统计pnMag的直方图，确定阈值。
 *   本函数中只是统计pUnchEdge中可能为边界点的那些象素。然后利用直方图，
 *   根据dRatioHigh设置高阈值，存储到pnThdHigh。利用dRationLow和高阈值，
 *   设置低阈值，存储到*pnThdLow。dRatioHigh是一种比例：表明梯度小于
 *   *pnThdHigh的象素数目占象素总数目的比例。dRationLow表明*pnThdHigh
 *   和*pnThdLow的比例，这个比例在canny算法的原文里，作者给出了一个区间。
 *
 *************************************************************************
 */
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 int * pUnchEdge, double dRatioHigh, double dRationLow) 
{ 
	// 循环控制变量
	int y;
	int x;
	int k;
	
	// 该数组的大小和梯度值的范围有关，如果采用本程序的算法，那么梯度的范围不会超过pow(2,10)
	int nHist[1024] ;

	// 可能的边界数目
	int nEdgeNb     ;

	// 最大梯度值
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 
	
	// 初始化
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// 统计直方图，然后利用直方图计算阈值
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if(pUnchEdge[y*nWidth+x]==128)
			{
				nHist[ pnMag[y*nWidth+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k;
		}
		
		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pnThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);
	
	k = 1;
	nEdgeNb = nHist[1];
	
	// 计算高阈值
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// 设置高阈值
	*pnThdHigh = k ;

	// 设置低阈值
	*pnThdLow  = (int)((*pnThdHigh) * dRationLow+ 0.5);
}


/*************************************************************************
 *
 * \函数名称：
 *   TraceEdge()
 *
 * \输入参数:
 *   int    x									- 跟踪起点的x坐标 
 *   int    y									- 跟踪起点的y坐标
 *   int nLowThd							- 判断一个点是否为边界点的低阈值
 *   int *pUnchEdge - 记录边界点的缓冲区
 *   int *pnMag               - 梯度幅度图
 *   int nWidth               - 图象数据宽度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   递归调用  
 *   从(x,y)坐标出发，进行边界点的跟踪，跟踪只考虑pUnchEdge中没有处理并且
 *   可能是边界点的那些象素(=128),象素值为0表明该点不可能是边界点，象素值
 *   为255表明该点已经被设置为边界点，不必再考虑
 *   
 * stack overflow error, so eliminate recursion with stack. -- add by rico.
 *************************************************************************
 */
void TraceEdge (int y, int x, int nLowThd, int *pUnchEdge, int *pnMag, int nWidth) 
{ 
	// 对8邻域象素进行查询
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;
	int t;
	
	int k =0 ;
	
	stack<Node> s;
	s.push(Node(x, y, 0));
	while (!s.empty()) 
	{
		Node nd = s.top();
		s.pop();
		for(k=nd.k; k<8; k++)
		{
			yy = nd.y + yNb[k] ;
			xx = nd.x + xNb[k] ;
			// 如果该象素为可能的边界点，又没有处理过
			// 并且梯度大于阈值
			t = yy*nWidth+xx;
			if(pUnchEdge[t] == 128  && pnMag[t]>=nLowThd)
			{
				// 把该点设置成为边界点
				pUnchEdge[t] = 255 ;

				// 以该点为中心进行跟踪
				if (k < 7)
				{
					s.push(Node(nd.x, nd.y, k+1));
				}
				s.push(Node(xx, yy, 0));
				//TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
			}
		}
	}
	
} 


/*************************************************************************
 *
 * \函数名称：
 *   Hysteresis()
 *
 * \输入参数:
 *   int *pnMag               - 梯度幅度图
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   int *pUnchEdge - 记录边界点的缓冲区
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   本函数实现类似“磁滞现象”的一个功能，也就是，先调用EstimateThreshold
 *   函数对经过non-maximum处理后的数据pUnchSpr估计一个高阈值，然后判断
 *   pUnchSpr中可能的边界象素(=128)的梯度是不是大于高阈值nThdHigh，如果比
 *   该阈值大，该点将作为一个边界的起点，调用TraceEdge函数，把对应该边界
 *   的所有象素找出来。最后，当整个搜索完毕时，如果还有象素没有被标志成
 *   边界点，那么就一定不是边界点。
 *   
 *************************************************************************
 */
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, int *pUnchEdge)
{
	// 循环控制变量
	int y;
	int x;

	int nThdHigh ;
	int nThdLow  ;

	int nPos;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh, 
		               &nThdLow, pUnchEdge,dRatioHigh, dRatioLow);

  // 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	// TraceEdge函数来跟踪该点对应的边界
   for(y=0; y<nHeight; y++)
	 {
      for(x=0; x<nWidth; x++)
			{
				nPos = y*nWidth + x ; 

				// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
				// 一个边界的起点
				if((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
				{
					// 设置该点为边界点
					pUnchEdge[nPos] = 255;
					TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
				}
      }
   }

	 // 那些还没有被设置为边界点的象素已经不可能成为边界点
   for(y=0; y<nHeight; y++)
	 {
		 for(x=0; x<nWidth; x++)
		 {
			 nPos = y*nWidth + x ; 
			 if(pUnchEdge[nPos] != 255)
			 {
				 // 设置为非边界点
				 pUnchEdge[nPos] = 0 ;
			 }
		 }
	 }
}







/*************************************************************************
 *
 * \函数名称：
 *   Canny()
 *
 * \输入参数:
 *   unsigned char *pUnchImage- 图象数据
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *   double sigma             - 高斯滤波的标准方差
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   int *pUnchEdge - canny算子计算后的分割图
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   canny分割算子，计算的结果保存在pUnchEdge中，逻辑1(255)表示该点为
 *   边界点，逻辑0(0)表示该点为非边界点。该函数的参数sigma，dRatioLow
 *   dRatioHigh，是需要指定的。这些参数会影响分割后边界点数目的多少
 *************************************************************************
 */
void Canny(int *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, int *pUnchEdge)
{
	// 经过高斯滤波后的图象数据
	int * pUnchSmooth ;
  
	// 指向x方向导数的指针
	int * pnGradX ; 

	// 指向y方向导数的指针
	int * pnGradY ;

	// 梯度的幅度
	int * pnGradMag ;

	pUnchSmooth  = new int[nWidth*nHeight] ;
	pnGradX      = new int [nWidth*nHeight];
	pnGradY      = new int [nWidth*nHeight];
	pnGradMag    = new int [nWidth*nHeight];

	// 对原图象进行滤波
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth) ;

	// 计算方向导数
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY) ;

	// 计算梯度的幅度
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag) ;

	// 应用non-maximum 抑制
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge) ;

	// 应用Hysteresis，找到所有的边界
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// 释放内存
	delete pnGradX      ;
	pnGradX      = NULL ;
	delete pnGradY      ;
	pnGradY      = NULL ;
	delete pnGradMag    ;
	pnGradMag    = NULL ;
	delete pUnchSmooth ;
	pUnchSmooth  = NULL ;
}

















/*****************************形态学操作******************************/

/*************************************************************************
 *
 * 函数名称：
 *   ErosiontionDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 
 
 * 返回值:
 *   BOOL               - 腐蚀成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行腐蚀运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

bool ErosionDIB(int* source, int lWidth, int lHeight)
{
	
	int size=lWidth*lHeight;
	int*dst=new int[size];
	memset(dst,0,size*sizeof(int));


	//循环变量
	int i;
	int j;
	int  n;
	int  pos;

	//像素值
	int pixel;




	//if(nMode == 0)
	{
		//使用水平方向的结构元素进行腐蚀
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{

				pos=lWidth * j + i;
				pixel=source[pos];
				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				//目标图像中的当前点先赋成黑色
			//	dst[pos] = 0;

				//如果源图像中当前点自身或者左右有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0;n < 3;n++ )
				{
					pixel =source[pos+n-1];
					if (pixel == 255 )
					{
						dst[pos]=255;
						break;
					}
				}
				
			}
		}

	}
//	else if(nMode == 1)
	{
		//使用垂直方向的结构元素进行腐蚀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针	
				pos=lWidth * j + i;

				pixel = source[pos];

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//目标图像中的当前点先赋成黑色
				dst[pos]=0;

				//如果源图像中当前点自身或者上下有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0;n < 3;n++ )
				{
					pixel = source[pos+(n-1)*lWidth];
					if (pixel == 255 )
					{
						dst[pos] = 255;
						break;
					}
				}

				
			}
		}

	}


	
	// 复制腐蚀后的图像
	memcpy(source, dst, size);



	// 返回
	return TRUE;
}




/*************************************************************************
 *
 * 函数名称：
 *   DilationDIB()
 *
 * 参数:
 *   int source    - 指向源图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 
 *
 * 返回值:
 *   BOOL               - 膨胀成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行膨胀运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/


bool DilationDIB(int*source, int lWidth, int lHeight)
{
	
	int size=lWidth*lHeight;
	int*dst=new int[size];
	memset(dst,255,size*sizeof(int));//目标图像中的当前点先赋成白色

	//循环变量
	long i;
	long j;
	int  n;
	int  pos;

	//像素值
	int pixel;




//	if(nMode == 0)
	{
		//使用水平方向的结构元素进行膨胀
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{

				pos=lWidth * j + i;
				pixel=source[pos];

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				
			

				//源图像中当前点自身或者左右只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0;n < 3;n++ )
				{
					pixel =source[pos+n-1];
					if (pixel == 0 )
					{
						dst[pos]=0;
						break;
					}
				}
				
			}
		}

	}
//	else if(nMode == 1)
	{
		//使用垂直方向的结构元素进行膨胀
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				pos=lWidth * j + i;
				pixel = source[pos];

				
				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//目标图像中的当前点先赋成白色
				dst[pos] = 255;

				//源图像中当前点自身或者上下只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0;n < 3;n++ )
				{
					pixel = source[pos+(n-1)*lWidth];
					if (pixel == 0 )
					{
						dst[pos]=0;
						break;
					}
				}
				
			}
		}

	}

	// 复制膨胀后的图像
	memcpy(source,dst,size);

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   CloseDIB()
 *
 * 参数:
 *   int* source    - 指向源图像指针
 *   int  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   int  lHeight      - 源图像高度（象素数）
 
 *
 * 返回值:
 *   BOOL               - 闭运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行开运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

bool CloseDIB(int* lpDIBBits, int lWidth, int lHeight)
{
		if (DilationDIB(lpDIBBits, lWidth, lHeight))
		{

			if (ErosionDIB(lpDIBBits, lWidth, lHeight))
			{
				// 返回
				return TRUE;

			}
		}
		return FALSE;
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   ThinDIB()
 *
 * 参数:
 *   int* source    - 指向源图像指针
 *   int  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   int  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 闭运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行细化运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

/*
void ThinnerRosenfeld(int* f, int lx, int ly)
{
    int  *g;
    int n[10];
    int a[5] = {0, -1, 1, 0, 0};
    int b[5] = {0, 0, 0, 1, -1};
    int nrnd, cond, n48, n26, n24, n46, n68, n82, n123, n345, n567, n781;
    short k, shori;
    int i, j;
    int ii, jj, kk, kk1, kk2, kk3, size;
    size = lx * ly;
//	int g[size];
    g = (int *)malloc(size);
    if(g==NULL)
    {
        return;
    }

    //f = (char *)image;
    for(kk=0l; kk<size; kk++)
    {
        g[kk] = f[kk];
    }

    do
    {
        shori = 0;
        for(k=1; k<=4; k++)
        {
            for(i=1; i<lx-1; i++)
            {
                ii = i + a[k];

                for(j=1; j<ly-1; j++)
                {
                    kk = i*ly + j;

                    if(!f[kk])
                        continue;

                    jj = j + b[k];
                    kk1 = ii*ly + jj;

                    if(f[kk1])
                        continue;

                    kk1 = kk - ly -1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[3] = f[kk1];
                    n[2] = f[kk2];
                    n[1] = f[kk3];
                    kk1 = kk - 1;
                    kk3 = kk + 1;
                    n[4] = f[kk1];
                    n[8] = f[kk3];
                    kk1 = kk + ly - 1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[5] = f[kk1];
                    n[6] = f[kk2];
                    n[7] = f[kk3];

                    nrnd = n[1] + n[2] + n[3] + n[4]
                        +n[5] + n[6] + n[7] + n[8];
                    if(nrnd<=1)
                        continue;

                    cond = 0;
                    n48 = n[4] + n[8];
                    n26 = n[2] + n[6];
                    n24 = n[2] + n[4];
                    n46 = n[4] + n[6];
                    n68 = n[6] + n[8];
                    n82 = n[8] + n[2];
                    n123 = n[1] + n[2] + n[3];
                    n345 = n[3] + n[4] + n[5];
                    n567 = n[5] + n[6] + n[7];
                    n781 = n[7] + n[8] + n[1];

                    if(n[2]==1 && n48==0 && n567>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[6]==1 && n48==0 && n123>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[8]==1 && n26==0 && n345>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[4]==1 && n26==0 && n781>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[5]==1 && n46==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[7]==1 && n68==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[1]==1 && n82==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[3]==1 && n24==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    cond = 1;
                    if(!cond)
                        continue;
                    g[kk] = 0;
                    shori = 1;
                }
            }

            for(i=0; i<lx; i++)
            {
                for(j=0; j<ly; j++)
                {
                    kk = i*ly + j;
                    f[kk] = g[kk];
                }
            }
        }
    }while(shori);

    free(g);
}              





bool Rosenfeld(int* source, int lWidth, int lHeight)
{
//	LPSTR	lpSrc;
    int i,j,pos;

    //建立存储区存放2值矩阵
	int *image=(int*)malloc(lWidth*lHeight*sizeof(int));//[lWidth*lHeight];
	//给2值矩阵赋值

	for(i=0;i<lHeight;i++)
	{
	    for(j=0;j<lWidth;j++)
		{
	      pos=i*lWidth+j;
	    
	      if(source[pos]==0)
	      image[i*lWidth+j]=1;

		  else
         image[i*lWidth+j]=0;
		}
	}
   ThinnerRosenfeld(image,lHeight,lWidth);

   //将结果赋值到原图像中

   for(i=0;i<lHeight;i++)
   {
     for(j=0;j<lWidth;j++)
	 {
	   pos=i*lWidth+j;

       if(image[pos]==1)
		source[pos]=0;

	   else
       source[pos]=255;
	 }
   
   }


 //清空内存
   free(image);

	return true;
}   */              

bool ThiningDIB(BYTE* source, int lWidth, int lHeight)
{
	
	int size=lWidth*lHeight;
	BYTE* dst=new BYTE[size];
	memset(dst,255,size*sizeof(BYTE));
	//脏标记
	bool bModified;

	//循环变量
	int i;
	int j;
	int  n;
	int  m,pos;

	//四个条件
	bool bCondition1;
	bool bCondition2;
	bool bCondition3;
	bool bCondition4;

	//计数器
	int nCount;
	
	//像素值
	BYTE pixel;

	//5×5相邻区域像素值
	int neighbour[5][5];

	bModified=TRUE;

	while(bModified)
	{

		bModified = FALSE;
		BYTE*dst=new BYTE[size];
		memset(dst,255,size*sizeof(BYTE));

		for(j = 2; j <lHeight-2; j++)
		{
			for(i = 2;i <lWidth-2; i++)
			{

				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;

				//由于使用5×5的结构元素，为防止越界，所以不处理外围的几行和几列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				pos=lWidth * j + i;;

				pixel = source[pos];

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && pixel != 0)
				//	return FALSE;
					continue;
				//如果源图像中当前点为白色，则跳过
				else if(pixel == 255)
					continue;

				//获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						neighbour[m][n] =(255 - source[pos + ((4 - m) - 2)*lWidth + n - 2 ]) / 255;
					}
				}
				//neighbour[][]
				//逐个判断条件。
				//判断2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] 
						+ neighbour[2][1] + neighbour[2][3] + 
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//判断Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;

				//判断P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}

				//判断P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					dst[pos]=255;
					bModified = TRUE;
				}
				else
				{
					dst[pos]=0;
				}
			}//end of for i to width-2
		}//end of for j to height-2
			// 复制腐蚀后的图像
		memcpy(source, dst, size*sizeof(BYTE));


	}//endof while
	// 复制腐蚀后的图像
	memcpy(source, dst, size*sizeof(BYTE));



	// 返回
	return TRUE;
}











/********************************种子填充***************************/
/*************************************************************************
 *
 * 函数名称：
 *   FillDIB()
 *
 * 参数:
 *   int* source    - 指向源图像指针
 *   int  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   int  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 种子填充成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行种子填充运算。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

bool FillDIB(int*source,int lWidth,int lHeight)
{
	
	// 指向源图像的指针
//	LPSTR	lpSrc;

	//像素值
	int pixel;
	int pos;

	//种子堆栈及指针
	Seed *Seeds;
	int StackPoint;

	//当前像素位置
	int iCurrentPixelx,iCurrentPixely;

	//初始化种子
	Seeds = new Seed[lWidth*lHeight];
	Seeds[1].Height = lHeight / 2;
	Seeds[1].Width = lWidth / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//取出种子
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;

	//	lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
		pos=lWidth * iCurrentPixely + iCurrentPixelx;;
		//取得当前指针处的像素值，注意要转换为unsigned char型
		pixel = source[pos];

		//目标图像中含有0和255外的其它灰度值
		if(pixel != 255 && pixel != 0)
			return FALSE;

		//将当前点涂黑
		source[pos]=0;

		//判断左面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixelx > 0)
		{
		//	lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx - 1;
			pos=lWidth * iCurrentPixely + iCurrentPixelx-1;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx - 1;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//判断上面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixely < lHeight - 1)
		{
			//lpSrc = (char *)lpDIBBits + lWidth * (iCurrentPixely + 1) + iCurrentPixelx;
			pos=lWidth*(iCurrentPixely+1) + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely + 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//判断右面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixelx < lWidth - 1)
		{
		//	lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx + 1;
			pos=lWidth * iCurrentPixely + iCurrentPixelx + 1;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx + 1;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//判断下面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixely > 0)
		{
		//	lpSrc = (char *)lpDIBBits + lWidth * (iCurrentPixely - 1) + iCurrentPixelx;
			pos=lWidth * (iCurrentPixely - 1) + iCurrentPixelx;
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely - 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}
	}

	//释放堆栈
	delete Seeds;
	// 返回
	return TRUE;
}










/*****************************调用上述函数进行特征提取******************************/
int CShapeFeature::get_feature()//提取形状特征
{
   //m_img->setMode(MODE_GRAYSCALE);
	int width=m_img->getWidth();
	int height=m_img->getHeight();
 	int size=width*height;
//	BYTE*source=new BYTE[size];
	int i,j,pos;
	bool closeOk=FALSE;
	bool thinOk=FALSE;
	bool fillOk=FALSE;
    int *pUnchImage=new int[size];//原始图像的灰度值
    for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			m_img->getPixel(i,j,pUnchImage[i*width+j]);//获取原始图像的灰度值

    int *pUnchEdge=new int[size];//canny算子分割后的图像
    memset(pUnchEdge,0,size*sizeof(int));
	Canny(pUnchImage,width,height,0.4,0.4,0.79, pUnchEdge);//canny算子提取图像边缘
	closeOk=CloseDIB(pUnchEdge, width, height);//闭操作，连接断点
	if(closeOk)
	{
		//cout<<"close ok"<<endl;
//		thinOk=Rosenfeld/*ThiningDIB*/(pUnchEdge,width,height);//轮廓细化

	}
/*	for(i=0;i<size;i++)
	{
		source[i]=(BYTE)pUnchEdge[i];
	}
	thinOk=ThiningDIB(source,width,height);//轮廓细化
	if(thinOk)
	{
		cout<<"thin ok"<<endl;
	//	fillOk=FillDIB(pUnchEdge,width,height);//种子填充
	}*/
//////////计算边界的七个不变矩

		int x=0,y=0,m00=0,m10=0,m01=0,xc=0,yc=0;
		for(pos=0;pos<size;pos++)
		{	
			y=pos/width;
			x=pos%width;
			if(pUnchEdge[pos]==255)
			{
				m00+=1;
				m10+=x;
				m01+=y;
			}
		}
		xc=m10/m00;
		yc=m01/m00;
		//计算u20等值
		int u20=0,u02=0,u11=0,u30=0,u12=0,u21=0,u03=0;
		for(pos=0;pos<size;pos++)
		{	
			y=pos/width;
			x=pos%width;
			if(pUnchEdge[pos]==255)
			{
				u20+=(x-xc)^2;
				u02+=(y-yc)^2;
				u11+=(x-xc)*(y-yc);
				u30+=(x-xc)^3;
				u03+=(y-yc)^3;
				u12+=(x-xc)*(y-yc)^2;
				u21+=(y-yc)*(x-xc)^2;
			}
		}

	//边界矩
		double tmp=pow(m00,4.0);
		double n20=u20/pow(m00,3.0);
		double n02=u02/pow(m00,3.0);
		double n12=u12/tmp;
		double n21=u21/tmp;
		double n30=u30/tmp;
		double n03=u03/tmp;
		double n11=u11/pow(m00,3.0);
		//计算边界的七个不变矩
		double ShapeFeature1[7];
		ShapeFeature1[0]=n20+n02;
		ShapeFeature1[1]=pow(n20-n02,2.0)+4*pow(n11,2.0);
		ShapeFeature1[2]=pow(n30-3*n12,2.0) +pow(3*n21 -n03,2.0);
		ShapeFeature1[3]=pow(n30+n12,2.0)+pow(n21+n03,2.0);
		ShapeFeature1[4]=(n30-3*n12)*(n30+n12)*(pow(n30+n12,2.0)-3*pow(n21+n03,2.0))
		    	+(3*n21-n03)*(n21+n03)*(3*pow(n30+n12,2.0)-pow(n21+n03,2.0));
		ShapeFeature1[5]=(n20-n02)*(pow(n30+n12,2.0)-pow(n21+n03,2.0))+4*n11*(n30+n12)*(n21+n03);
		ShapeFeature1[6]=(3*n21-n03)*(n30+n12)*(pow(n30+n12,2.0)-3*pow(n21+n03,2.0))+
				(3*n12-n30)*(n03+n21)*(3*pow(n30+n12,2.0)-pow(n21+n03,2.0));
		
/*		double sum=0;
		for(i=0;i<7;i++)//特征之归一化
		{	
			sum+=ShapeFeature1[i];
		}		*/
		for(i=0;i<7;i++)//赋值
		{	
			m_feature[i]=ShapeFeature1[i];//sum;
		}
//	}


/////////////////区域特征矩///////////////////////////////////////////

/*	fillOk=FillDIB(pUnchEdge,width,height);//种子填充

	if(fillOk)//计算区域的七个不变矩
    {
		cout<<"fill ok"<<endl;
		int x=0,y=0,m00=0,m10=0,m01=0,xc=0,yc=0;
		for(pos=0;pos<size;pos++)
		{	
			y=pos/width;
			x=pos%width;
			if(pUnchEdge[pos]==255)
			{
				m00+=1;
				m10+=x;
				m01+=y;
			}
		}
		xc=m10/m00;
		yc=m01/m00;
		//计算u20等值
		int u20=0,u02=0,u11=0,u30=0,u12=0,u21=0,u03=0;
		for(pos=0;pos<size;pos++)
		{	
			y=pos/width;
			x=pos%width;
			if(pUnchEdge[pos]==255)
			{
				u20+=(x-xc)^2;
				u02+=(y-yc)^2;
				u11+=(x-xc)*(y-yc);
				u30+=(x-xc)^3;
				u03+=(y-yc)^3;
				u12+=(x-xc)*(y-yc)^2;
				u21+=(y-yc)*(x-xc)^2;
			}
		}
		//计算n20等值
		double tmp=pow(m00,2.5);
		double n20=u20/pow(m00,2);
		double n02=u02/pow(m00,2);
		double n12=u12/tmp;
		double n21=u21/tmp;
		double n30=u30/tmp;
		double n03=u03/tmp;
		double n11=u11/pow(m00,2);

		//计算区域的七个不变矩
/*		double ShapeFeature2[10];
		ShapeFeature2[0]=n20+n02;
		ShapeFeature2[1]=pow(n20-n02,2)+4*pow(n11,2);
		ShapeFeature2[2]=pow(n30-3*n12,2) +pow(3*n21 -n03,2);
		ShapeFeature2[3]=pow(n30+n12,2)+pow(n21+n03,2);
		ShapeFeature2[4]=(n30-3*n12)*(n30+n12)*(pow(n30+n12,2)-3*pow(n21+n03,2))
		    	+(3*n21-n03)*(n21+n03)*(3*pow(n30+n12,2)-pow(n21+n03,2));
		ShapeFeature2[5]=(n20-n02)*(pow(n30+n12,2)-pow(n21+n03,2))+4*n11*(n30+n12)*(n21+n03);
		ShapeFeature2[6]=(3*n21-n03)*(n30+n12)*(pow(n30+n12,2)-3*pow(n21+n03,2))+
				(3*n12-n30)*(n03+n21)*(3*pow(n30+n12,2)-pow(n21+n03,2));
		for(i=0;i<7;i++)//赋值
		{	
			m_featrue[i]=ShapeFeature2[i];//sum;
		}*/
		//计算区域的三个仿射不变矩
	/*	double ShapeFeature3[3];
		ShapeFeature3[0]=(u20*u02-u11^2)/m00^4;
		ShapeFeature3[1]=(u30^2*u03^2-6*u30*u21*u12*u03+4*u30*u12^3+4*u21^3*u03-3*u21^2*u12^2)/m00^10;
		ShapeFeature3[2]=(u20*(u21*u03-u12^2)-u11*(u30*u03-u21*u12)+u02*(u30*u12-u21^2))/m00^7;

		for(i=0;i<3;i++)
			m_featrue[i]=ShapeFeature3[i];
	}//endof fill ok
*/


    return POK;
}

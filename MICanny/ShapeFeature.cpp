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




/********************************��Ե������ȡ******************************/

/*************************************************************************
 *
 * \�������ƣ�
 *   MakeGauss()
 *
 * \�������:
 *   double sigma									        - ��˹�����ı�׼��
 *   double **pdKernel										- ָ���˹���������ָ��
 *   int *pnWindowSize										- ���ݵĳ���
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���������������һ��һά�ĸ�˹�������������ݣ������ϸ�˹���ݵĳ���Ӧ
 *   �������޳��ģ�����Ϊ�˼���ļ򵥺��ٶȣ�ʵ�ʵĸ�˹����ֻ�������޳���
 *   pnWindowSize�������ݳ���
 *   
 *************************************************************************
 */
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize)
{
	// ѭ�����Ʊ���
	int i   ;
	
	// ��������ĵ�
	int nCenter;

	// �����ĳһ�㵽���ĵ�ľ���
	double  dDis  ; 

	double PI = 3.14159;
	// �м����
	double  dValue; 
	double  dSum  ;
	dSum = 0 ; 
	
	// ���鳤�ȣ����ݸ����۵�֪ʶ��ѡȡ[-3*sigma, 3*sigma]���ڵ����ݡ�
	// ��Щ���ݻḲ�Ǿ��󲿷ֵ��˲�ϵ��
	*pnWindowSize = (int)1 + 2 * ceil(3 * sigma);
	
	// ����
	nCenter = (*pnWindowSize) / 2;
	
	// �����ڴ�
	*pdKernel = new double[*pnWindowSize] ;
	
	for(i=0; i< (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma)) / (sqrt(2 * PI) * sigma );
		(*pdKernel)[i] = dValue ;
		dSum += dValue;
	}
	
	// ��һ��
	for(i=0; i<(*pnWindowSize) ; i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   GaussianSmooth()
 *
 * \�������:
 *   int * pUnchImg				- ָ��ͼ�����ݵ�ָ��
 *   int nWidth											- ͼ�����ݿ��
 *   int nHeight										- ͼ�����ݸ߶�
 *   double dSigma									- ��˹�����ı�׼��
 *   int * pUnchSmthdImg	- ָ�򾭹�ƽ��֮���ͼ������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Ϊ����ֹ���������ø�˹�˲���ͼ������˲����˲��ȶ�x������У�Ȼ���
 *   y������С�
 *   
 *************************************************************************
 */
void GaussianSmooth(int *pUnchImg, int nWidth, int nHeight, 
										double sigma, int * pUnchSmthdImg)
{
	// ѭ�����Ʊ���
  int y;
	int x;
	
	int i;
	
	// ��˹�˲��������鳤��
	
	int nWindowSize;
	
	//  ���ڳ��ȵ�1/2
	int	nHalfLen;   
	
	// һά��˹�����˲���
	double *pdKernel ;
	
	// ��˹ϵ����ͼ�����ݵĵ��
	double  dDotMul     ;
	
	// ��˹�˲�ϵ�����ܺ�
	double  dWeightSum     ;          
  
	// �м����
	double * pdTmp ;
	
	// �����ڴ�
	pdTmp = new double[nWidth*nHeight];
	
	// ����һά��˹�����˲���
	// MakeGauss(sigma, &dKernel, &nWindowSize);
	MakeGauss(sigma, &pdKernel, &nWindowSize) ;
	
	// MakeGauss���ش��ڵĳ��ȣ����ô˱������㴰�ڵİ볤
	nHalfLen = nWindowSize / 2;
	
  // x��������˲�
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+x) >= 0  && (i+x) < nWidth )
				{
					dDotMul += (double)pUnchImg[y*nWidth + (i+x)] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pdTmp[y*nWidth + x] = dDotMul/dWeightSum ;
		}
	}
	
	// y��������˲�
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+y) >= 0  && (i+y) < nHeight )
				{
					dDotMul += (double)pdTmp[(y+i)*nWidth + x] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pUnchSmthdImg[y*nWidth + x] = (int)dDotMul/dWeightSum ;
		}
	}

	// �ͷ��ڴ�
	delete []pdKernel;
	pdKernel = NULL ;
	
	delete []pdTmp;
	pdTmp = NULL;
}


/*************************************************************************
 *
 * \�������ƣ�
 *   DirGrad()
 *
 * \�������:
 *   int *pUnchSmthdImg					- ������˹�˲����ͼ��
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������㷽���������õ�΢��������(-1 0 1) �� (-1 0 1)'(ת��)
 *   �����ʱ��Ա߽����ز��������⴦��
 *   
 *   
 *************************************************************************
 */
void DirGrad(int *pUnchSmthdImg, int nWidth, int nHeight,
										 int *pnGradX , int *pnGradY)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pnGradX[y*nWidth+x] = (int) ( pUnchSmthdImg[y*nWidth+min(nWidth-1,x+1)]  
													          - pUnchSmthdImg[y*nWidth+max(0,x-1)]     );
		}
	}

	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
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
 * \�������ƣ�
 *   GradMagnitude()
 *
 * \�������:
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnMag                           - �ݶȷ���   
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������÷����������ݶȷ��ȣ���������DirGrad��������Ľ��
 *   
 *************************************************************************
 */
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag)
{
	
	// ѭ�����Ʊ���
	int y ;
	int x ;

	// �м����
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
 * \�������ƣ�
 *   NonmaxSuppress()
 *
 * \�������:
 *   int *pnMag                - �ݶ�ͼ
 *   int *pnGradX							 - x����ķ�����	
 *   int *pnGradY              - y����ķ�����
 *   int nWidth                - ͼ�����ݿ��
 *   int nHeight               - ͼ�����ݸ߶�
 *   int *pUnchRst   - ����NonmaxSuppress�����Ľ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ֹ�ݶ�ͼ�зǾֲ���ֵ������ء�
 *   
 *************************************************************************
 */
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
										int nHeight,	int *pUnchRst)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	int nPos;

	// x�����ݶȷ���
	int gx  ;
	int gy  ;

	// ��ʱ����
	int g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;
	
	// ����ͼ���Ե����Ϊ�����ܵı߽��
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
			
			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pnMag[nPos] == 0 )
			{
				pUnchRst[nPos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pnMag[nPos] ;
				
				// x��y������
				gx = pnGradX[nPos]  ;
				gy = pnGradY[nPos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs(double(gx))/fabs(double(gy)); 

					g2 = pnMag[nPos-nWidth] ; 
					g4 = pnMag[nPos+nWidth] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pnMag[nPos-nWidth-1] ;
						g3 = pnMag[nPos+nWidth+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		 C         
					//	g3 g4  
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					} 
				}
				
				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs(double(gy))/fabs(double(gx)); 
					
					g2 = pnMag[nPos+1] ; 
					g4 = pnMag[nPos-1] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pnMag[nPos+nWidth+1] ;
						g3 = pnMag[nPos-nWidth-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;
					
					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pUnchRst[nPos] = 128 ;
					}
					else
					{
						// �������Ǳ߽��
						pUnchRst[nPos] = 0 ;
					}
				}
			} //else
		} // for

	}
} 

/*************************************************************************
 *
 * \�������ƣ�
 *   EstimateThreshold()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   int *pnThdHigh           - ����ֵ
 *   int *pnThdLow            - ����ֵ
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   int *pUnchEdge - ����non-maximum����������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����non-maximum����������pUnchEdge��ͳ��pnMag��ֱ��ͼ��ȷ����ֵ��
 *   ��������ֻ��ͳ��pUnchEdge�п���Ϊ�߽�����Щ���ء�Ȼ������ֱ��ͼ��
 *   ����dRatioHigh���ø���ֵ���洢��pnThdHigh������dRationLow�͸���ֵ��
 *   ���õ���ֵ���洢��*pnThdLow��dRatioHigh��һ�ֱ����������ݶ�С��
 *   *pnThdHigh��������Ŀռ��������Ŀ�ı�����dRationLow����*pnThdHigh
 *   ��*pnThdLow�ı��������������canny�㷨��ԭ������߸�����һ�����䡣
 *
 *************************************************************************
 */
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 int * pUnchEdge, double dRatioHigh, double dRationLow) 
{ 
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;
	
	// ������Ĵ�С���ݶ�ֵ�ķ�Χ�йأ�������ñ�������㷨����ô�ݶȵķ�Χ���ᳬ��pow(2,10)
	int nHist[1024] ;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 
	
	// ��ʼ��
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pUnchEdge[y*nWidth+x]==128)
			{
				nHist[ pnMag[y*nWidth+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}
		
		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pnThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);
	
	k = 1;
	nEdgeNb = nHist[1];
	
	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pnThdHigh = k ;

	// ���õ���ֵ
	*pnThdLow  = (int)((*pnThdHigh) * dRationLow+ 0.5);
}


/*************************************************************************
 *
 * \�������ƣ�
 *   TraceEdge()
 *
 * \�������:
 *   int    x									- ��������x���� 
 *   int    y									- ��������y����
 *   int nLowThd							- �ж�һ�����Ƿ�Ϊ�߽��ĵ���ֵ
 *   int *pUnchEdge - ��¼�߽��Ļ�����
 *   int *pnMag               - �ݶȷ���ͼ
 *   int nWidth               - ͼ�����ݿ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ݹ����  
 *   ��(x,y)������������б߽��ĸ��٣�����ֻ����pUnchEdge��û�д�����
 *   �����Ǳ߽�����Щ����(=128),����ֵΪ0�����õ㲻�����Ǳ߽�㣬����ֵ
 *   Ϊ255�����õ��Ѿ�������Ϊ�߽�㣬�����ٿ���
 *   
 * stack overflow error, so eliminate recursion with stack. -- add by rico.
 *************************************************************************
 */
void TraceEdge (int y, int x, int nLowThd, int *pUnchEdge, int *pnMag, int nWidth) 
{ 
	// ��8�������ؽ��в�ѯ
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
			// ���������Ϊ���ܵı߽�㣬��û�д����
			// �����ݶȴ�����ֵ
			t = yy*nWidth+xx;
			if(pUnchEdge[t] == 128  && pnMag[t]>=nLowThd)
			{
				// �Ѹõ����ó�Ϊ�߽��
				pUnchEdge[t] = 255 ;

				// �Ըõ�Ϊ���Ľ��и���
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
 * \�������ƣ�
 *   Hysteresis()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   int *pUnchEdge - ��¼�߽��Ļ�����
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ������ʵ�����ơ��������󡱵�һ�����ܣ�Ҳ���ǣ��ȵ���EstimateThreshold
 *   �����Ծ���non-maximum����������pUnchSpr����һ������ֵ��Ȼ���ж�
 *   pUnchSpr�п��ܵı߽�����(=128)���ݶ��ǲ��Ǵ��ڸ���ֵnThdHigh�������
 *   ����ֵ�󣬸õ㽫��Ϊһ���߽����㣬����TraceEdge�������Ѷ�Ӧ�ñ߽�
 *   �����������ҳ�������󣬵������������ʱ�������������û�б���־��
 *   �߽�㣬��ô��һ�����Ǳ߽�㡣
 *   
 *************************************************************************
 */
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, int *pUnchEdge)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	int nThdHigh ;
	int nThdLow  ;

	int nPos;

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh, 
		               &nThdLow, pUnchEdge,dRatioHigh, dRatioLow);

  // ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
   for(y=0; y<nHeight; y++)
	 {
      for(x=0; x<nWidth; x++)
			{
				nPos = y*nWidth + x ; 

				// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
				// һ���߽�����
				if((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
				{
					// ���øõ�Ϊ�߽��
					pUnchEdge[nPos] = 255;
					TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
				}
      }
   }

	 // ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
   for(y=0; y<nHeight; y++)
	 {
		 for(x=0; x<nWidth; x++)
		 {
			 nPos = y*nWidth + x ; 
			 if(pUnchEdge[nPos] != 255)
			 {
				 // ����Ϊ�Ǳ߽��
				 pUnchEdge[nPos] = 0 ;
			 }
		 }
	 }
}







/*************************************************************************
 *
 * \�������ƣ�
 *   Canny()
 *
 * \�������:
 *   unsigned char *pUnchImage- ͼ������
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   double sigma             - ��˹�˲��ı�׼����
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   int *pUnchEdge - canny���Ӽ����ķָ�ͼ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   canny�ָ����ӣ�����Ľ��������pUnchEdge�У��߼�1(255)��ʾ�õ�Ϊ
 *   �߽�㣬�߼�0(0)��ʾ�õ�Ϊ�Ǳ߽�㡣�ú����Ĳ���sigma��dRatioLow
 *   dRatioHigh������Ҫָ���ġ���Щ������Ӱ��ָ��߽����Ŀ�Ķ���
 *************************************************************************
 */
void Canny(int *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, int *pUnchEdge)
{
	// ������˹�˲����ͼ������
	int * pUnchSmooth ;
  
	// ָ��x��������ָ��
	int * pnGradX ; 

	// ָ��y��������ָ��
	int * pnGradY ;

	// �ݶȵķ���
	int * pnGradMag ;

	pUnchSmooth  = new int[nWidth*nHeight] ;
	pnGradX      = new int [nWidth*nHeight];
	pnGradY      = new int [nWidth*nHeight];
	pnGradMag    = new int [nWidth*nHeight];

	// ��ԭͼ������˲�
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth) ;

	// ���㷽����
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY) ;

	// �����ݶȵķ���
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag) ;

	// Ӧ��non-maximum ����
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge) ;

	// Ӧ��Hysteresis���ҵ����еı߽�
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// �ͷ��ڴ�
	delete pnGradX      ;
	pnGradX      = NULL ;
	delete pnGradY      ;
	pnGradY      = NULL ;
	delete pnGradMag    ;
	pnGradMag    = NULL ;
	delete pUnchSmooth ;
	pUnchSmooth  = NULL ;
}

















/*****************************��̬ѧ����******************************/

/*************************************************************************
 *
 * �������ƣ�
 *   ErosiontionDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 
 
 * ����ֵ:
 *   BOOL               - ��ʴ�ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����и�ʴ���㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

bool ErosionDIB(int* source, int lWidth, int lHeight)
{
	
	int size=lWidth*lHeight;
	int*dst=new int[size];
	memset(dst,0,size*sizeof(int));


	//ѭ������
	int i;
	int j;
	int  n;
	int  pos;

	//����ֵ
	int pixel;




	//if(nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{

				pos=lWidth * j + i;
				pixel=source[pos];
				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
			//	dst[pos] = 0;

				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
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
		//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��	
				pos=lWidth * j + i;

				pixel = source[pos];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				dst[pos]=0;

				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
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


	
	// ���Ƹ�ʴ���ͼ��
	memcpy(source, dst, size);



	// ����
	return TRUE;
}




/*************************************************************************
 *
 * �������ƣ�
 *   DilationDIB()
 *
 * ����:
 *   int source    - ָ��Դͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 
 *
 * ����ֵ:
 *   BOOL               - ���ͳɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ������������㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/


bool DilationDIB(int*source, int lWidth, int lHeight)
{
	
	int size=lWidth*lHeight;
	int*dst=new int[size];
	memset(dst,255,size*sizeof(int));//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ

	//ѭ������
	long i;
	long j;
	int  n;
	int  pos;

	//����ֵ
	int pixel;




//	if(nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ�������
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{

				pos=lWidth * j + i;
				pixel=source[pos];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				
			

				//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
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
		//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				pos=lWidth * j + i;
				pixel = source[pos];

				
				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
				dst[pos] = 255;

				//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
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

	// �������ͺ��ͼ��
	memcpy(source,dst,size);

	// ����
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   CloseDIB()
 *
 * ����:
 *   int* source    - ָ��Դͼ��ָ��
 *   int  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   int  lHeight      - Դͼ��߶ȣ���������
 
 *
 * ����ֵ:
 *   BOOL               - ������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����п����㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

bool CloseDIB(int* lpDIBBits, int lWidth, int lHeight)
{
		if (DilationDIB(lpDIBBits, lWidth, lHeight))
		{

			if (ErosionDIB(lpDIBBits, lWidth, lHeight))
			{
				// ����
				return TRUE;

			}
		}
		return FALSE;
	
	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   ThinDIB()
 *
 * ����:
 *   int* source    - ָ��Դͼ��ָ��
 *   int  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   int  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - ������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ�����ϸ�����㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
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

    //�����洢�����2ֵ����
	int *image=(int*)malloc(lWidth*lHeight*sizeof(int));//[lWidth*lHeight];
	//��2ֵ����ֵ

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

   //�������ֵ��ԭͼ����

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


 //����ڴ�
   free(image);

	return true;
}   */              

bool ThiningDIB(BYTE* source, int lWidth, int lHeight)
{
	
	int size=lWidth*lHeight;
	BYTE* dst=new BYTE[size];
	memset(dst,255,size*sizeof(BYTE));
	//����
	bool bModified;

	//ѭ������
	int i;
	int j;
	int  n;
	int  m,pos;

	//�ĸ�����
	bool bCondition1;
	bool bCondition2;
	bool bCondition3;
	bool bCondition4;

	//������
	int nCount;
	
	//����ֵ
	BYTE pixel;

	//5��5������������ֵ
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

				//����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				pos=lWidth * j + i;;

				pixel = source[pos];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
				//	return FALSE;
					continue;
				//���Դͼ���е�ǰ��Ϊ��ɫ��������
				else if(pixel == 255)
					continue;

				//��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						neighbour[m][n] =(255 - source[pos + ((4 - m) - 2)*lWidth + n - 2 ]) / 255;
					}
				}
				//neighbour[][]
				//����ж�������
				//�ж�2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] 
						+ neighbour[2][1] + neighbour[2][3] + 
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//�ж�Z0(P1)=1
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

				//�ж�P2*P4*P8=0 or Z0(p2)!=1
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

				//�ж�P2*P4*P6=0 or Z0(p4)!=1
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
			// ���Ƹ�ʴ���ͼ��
		memcpy(source, dst, size*sizeof(BYTE));


	}//endof while
	// ���Ƹ�ʴ���ͼ��
	memcpy(source, dst, size*sizeof(BYTE));



	// ����
	return TRUE;
}











/********************************�������***************************/
/*************************************************************************
 *
 * �������ƣ�
 *   FillDIB()
 *
 * ����:
 *   int* source    - ָ��Դͼ��ָ��
 *   int  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   int  lHeight      - Դͼ��߶ȣ���������
 * ����ֵ:
 *   BOOL               - �������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ���������������㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

bool FillDIB(int*source,int lWidth,int lHeight)
{
	
	// ָ��Դͼ���ָ��
//	LPSTR	lpSrc;

	//����ֵ
	int pixel;
	int pos;

	//���Ӷ�ջ��ָ��
	Seed *Seeds;
	int StackPoint;

	//��ǰ����λ��
	int iCurrentPixelx,iCurrentPixely;

	//��ʼ������
	Seeds = new Seed[lWidth*lHeight];
	Seeds[1].Height = lHeight / 2;
	Seeds[1].Width = lWidth / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//ȡ������
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;

	//	lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
		pos=lWidth * iCurrentPixely + iCurrentPixelx;;
		//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
		pixel = source[pos];

		//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
		if(pixel != 255 && pixel != 0)
			return FALSE;

		//����ǰ��Ϳ��
		source[pos]=0;

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixelx > 0)
		{
		//	lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx - 1;
			pos=lWidth * iCurrentPixely + iCurrentPixelx-1;
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx - 1;
			}
			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixely < lHeight - 1)
		{
			//lpSrc = (char *)lpDIBBits + lWidth * (iCurrentPixely + 1) + iCurrentPixelx;
			pos=lWidth*(iCurrentPixely+1) + iCurrentPixelx;
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely + 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixelx < lWidth - 1)
		{
		//	lpSrc = (char *)lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx + 1;
			pos=lWidth * iCurrentPixely + iCurrentPixelx + 1;
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx + 1;
			}
			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixely > 0)
		{
		//	lpSrc = (char *)lpDIBBits + lWidth * (iCurrentPixely - 1) + iCurrentPixelx;
			pos=lWidth * (iCurrentPixely - 1) + iCurrentPixelx;
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = source[pos];
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely - 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && pixel != 0)
				return FALSE;
		}
	}

	//�ͷŶ�ջ
	delete Seeds;
	// ����
	return TRUE;
}










/*****************************����������������������ȡ******************************/
int CShapeFeature::get_feature()//��ȡ��״����
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
    int *pUnchImage=new int[size];//ԭʼͼ��ĻҶ�ֵ
    for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			m_img->getPixel(i,j,pUnchImage[i*width+j]);//��ȡԭʼͼ��ĻҶ�ֵ

    int *pUnchEdge=new int[size];//canny���ӷָ���ͼ��
    memset(pUnchEdge,0,size*sizeof(int));
	Canny(pUnchImage,width,height,0.4,0.4,0.79, pUnchEdge);//canny������ȡͼ���Ե
	closeOk=CloseDIB(pUnchEdge, width, height);//�ղ��������Ӷϵ�
	if(closeOk)
	{
		//cout<<"close ok"<<endl;
//		thinOk=Rosenfeld/*ThiningDIB*/(pUnchEdge,width,height);//����ϸ��

	}
/*	for(i=0;i<size;i++)
	{
		source[i]=(BYTE)pUnchEdge[i];
	}
	thinOk=ThiningDIB(source,width,height);//����ϸ��
	if(thinOk)
	{
		cout<<"thin ok"<<endl;
	//	fillOk=FillDIB(pUnchEdge,width,height);//�������
	}*/
//////////����߽���߸������

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
		//����u20��ֵ
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

	//�߽��
		double tmp=pow(m00,4.0);
		double n20=u20/pow(m00,3.0);
		double n02=u02/pow(m00,3.0);
		double n12=u12/tmp;
		double n21=u21/tmp;
		double n30=u30/tmp;
		double n03=u03/tmp;
		double n11=u11/pow(m00,3.0);
		//����߽���߸������
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
		for(i=0;i<7;i++)//����֮��һ��
		{	
			sum+=ShapeFeature1[i];
		}		*/
		for(i=0;i<7;i++)//��ֵ
		{	
			m_feature[i]=ShapeFeature1[i];//sum;
		}
//	}


/////////////////����������///////////////////////////////////////////

/*	fillOk=FillDIB(pUnchEdge,width,height);//�������

	if(fillOk)//����������߸������
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
		//����u20��ֵ
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
		//����n20��ֵ
		double tmp=pow(m00,2.5);
		double n20=u20/pow(m00,2);
		double n02=u02/pow(m00,2);
		double n12=u12/tmp;
		double n21=u21/tmp;
		double n30=u30/tmp;
		double n03=u03/tmp;
		double n11=u11/pow(m00,2);

		//����������߸������
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
		for(i=0;i<7;i++)//��ֵ
		{	
			m_featrue[i]=ShapeFeature2[i];//sum;
		}*/
		//����������������䲻���
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

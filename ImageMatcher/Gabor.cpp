#include "stdafx.h"
#include "Gabor.h"
#include "matlab.h"
#include "math.h"
#include "gaborkerneldll.h"
#include "gaborfilterdll.h"

Gabor::Gabor(int freq, int ori) : frequency(freq), oriention(ori)
{
	gaborkerneldllInitialize();
	gaborfilterdllInitialize();
	
	// ��ʼ��������˺���
	Init();
}

int Gabor::Init()
{
	int row = 0, col = 0;
	int i = 0;
	// ����һ��˺���
	for (int j = 0; j < frequency * 2; j+=2)
	{
		for (int k = 0; k < oriention; k+=1)
		{
			CalculateKernel(2, 4, j, k, i, &row, &col);	
			//printf("%d kernel generated\n", i);
			++i;			
		}
	}
/*
	CalculateKernel(2, 4, 16, 1.03, i, &row, &col);	
	printf("%d kernel generated\n", i);
	++i;
*/
	
	return 1;
}

Gabor::~Gabor()
{
	// free kernel function array
	for (int i = 0; i < KERNEL_NUM; ++i)
	{
		mxDestroyArray(kernel[i]);
	}
	gaborkerneldllTerminate();
	gaborfilterdllTerminate();
}

/************************************************************************/
/* ���ɺ˺��� 
/* Sx & Sy : Variances along x and y-axes respectively                    
/* f : The frequency of the sinusoidal function
/* theta : The orientation of Gabor filter
/************************************************************************/
int Gabor::CalculateKernel(double Sx, double Sy, double f, double theta, int index, int *row, int *col) 
{
	mxArray *ax, *bx, *cx, *dx, *g;
	
	mlfAssign(&ax,mlfScalar(Sx)); 
	mlfAssign(&bx,mlfScalar(Sy));
	mlfAssign(&cx,mlfScalar(f));
	mlfAssign(&dx,mlfScalar(theta));
	g=mlfGaborkernel(ax,bx,cx,dx);
	
	double *doubleG = (double *)mxGetData(g);
	*row = mxGetM(g);
	*col = mxGetN(g);

	mxArray *k = mxCreateDoubleMatrix(*row, *col, mxREAL);
	double *data = mxGetPr(k);
	int total = (*row) * (*col);
	for (int i = 0; i < total; ++i)
	{
		data[i] = doubleG[i];
		
	}

	kernel[index] = k;
	
	mxDestroyArray(ax);
	mxDestroyArray(bx);
	mxDestroyArray(cx);
	mxDestroyArray(dx);
	mxDestroyArray(g);

	return(1);
}

/************************************************************************/
/* ��ȡͼƬ����
/* [in]fname: ͼƬ�ļ���������·��
/* [out]pic_gaborWL: ��ȡ������������                                                                     */
/************************************************************************/
int Gabor::OnWenLi(const char *fname, Pic_GaborWL *pic_gaborWL)
{	
	int index = 0;
	for (int i = 0; i < KERNEL_NUM; ++i)
	{
		int row = mxGetM(kernel[i]);
		int col = mxGetN(kernel[i]);
		mxArray *k = mxCreateDoubleMatrix(row, col, mxREAL);
		double *data = mxGetPr(k);
		double * kernelData = (double *)mxGetData(kernel[i]);
		int total = (row) * (col);
		int j = 0;
		for (j = 0; j < total; ++j)
		{
			data[j] = kernelData[j];			
		}
		mxArray *pic = mxCreateString(fname);
		// ��ͼƬ����Gabor�˲�
		mxArray *gabout=mlfGaborfilter(pic, k);
		mxDestroyArray(k);
		mxDestroyArray(pic);

		double * result = (double *)mxGetData(gabout);

		row = mxGetM(gabout);
		col = mxGetN(gabout);
		total = row * col; // shit,�˴�����һ���ͼ���������

		// �����ֵ�ͷ���
		double sum = 0;
		for (j = 0; j < total; ++j)
		{
			sum += result[j];
		}
		double avg = sum / total;

		double deta = 0;
		for (j = 0; j < total; ++j)
		{
			deta += (result[j] - avg) * (result[j] - avg);
		}
		deta = deta / total;

		pic_gaborWL->wenli[index++] = avg;
		pic_gaborWL->wenli[index++] = deta;

		mxDestroyArray(gabout);
	}
	
	return 0;
}

/************************************************************************/
/* ����ͼƬ����
/* [in]model_WL: �ؼ�ͼ��������������
/* [in]Pic_WL: ����ͼƬ����������������
/* [in]fnum: ����ͼƬ��
/* [out]sort: �������ƶȵõ��Ĵ���ͼƬ�����ƶ�����                                                                     */
/************************************************************************/
int Gabor::OnRetrieveIn(Pic_GaborWL *model_WL, Pic_GaborWL *Pic_WL, int fnum, int *sort) 
{
    double *Dis_YS = (double *)malloc(fnum * sizeof(double));//�������������ľ���
	double *Dis_YS0 = (double *)malloc(fnum * sizeof(double));//��һ��������������ľ���
	
	int i = 0;
	for (i  = 0; i < fnum; ++i)
	{
		GetDistance(*model_WL, Pic_WL[i], &(Dis_YS[i]));//�������������ľ���	
		//printf("%d ���������\n", i);
	}
	GuiYiHua(Dis_YS, Dis_YS0, fnum);//�������Ĺ�һ��
	
	for(i=0;i<fnum;i++)   
		sort[i]=i;
    QSort(Dis_YS0,0,fnum-1,sort);//��õ����ƾ����������
	
	free(Dis_YS);
	free(Dis_YS0);
	return(1);
}

/************************************************************************/
/* ��������ͼƬ֮������ƾ���                                                                     */
/************************************************************************/
int Gabor::GetDistance(Pic_GaborWL model_WL, Pic_GaborWL Pic_WL, double *Dis_WL)
{  
	
	double Temp = 0;
	int r;
	
	double total=0;
	for(r=0;r<GABOR_TEXTURE_SIZE;r++)
		total+=(model_WL.wenli[r]-Pic_WL.wenli[r])*(model_WL.wenli[r]-Pic_WL.wenli[r]);
	
	Temp=(double)sqrt(total);

	*Dis_WL = Temp;
	return(1);
}

/************************************************************************/
/* ���һ����ֵ                                                                     */
/************************************************************************/
int Gabor::GuiYiHua(double *Dis_old,double *Dis_new, int fnum)
{
	double total=0;
	int i;
	double T_ave,T_bzc;//��ֵ�ͱ�׼��
	
	for(i=0;i<fnum;i++)   
		total+= Dis_old[i];
	T_ave=(double)total/fnum;//��ֵ
	
	total=0;
	for(i=0;i<fnum;i++)
		total+=(Dis_old[i]-T_ave)*(Dis_old[i]-T_ave);
    T_bzc=(double)sqrt(total/fnum);//��׼��
	
	for(i=0;i<fnum;i++)
	{
		Dis_new[i]=(double)(Dis_old[i]-T_ave)/(3*T_bzc);
		Dis_new[i]=(Dis_new[i]+1)/2;
        if(Dis_new[i]<0) Dis_new[i]=0;
		if(Dis_new[i]>1) Dis_new[i]=1;//��һ����(0,1)
	}//��˹��һ��
	
	return 1;
}


/************************************************************************/
/* ����������������                                                                     */
/************************************************************************/
int Gabor::QSort(double *List, int low, int high, int *sort)
{
	if(low<high)//���ȴ���1
	{
		int pivotloc=Partition(List,low,high,sort);//������һ��Ϊ��
		QSort(List,low,pivotloc-1,sort);//�Ե��ӱ�ݹ�����pivotloc������λ��
		QSort(List,pivotloc+1,high,sort);//�Ը��ӱ�ݹ�����
	}
	return(1);
}

/************************************************************************/
/* �����зֳ����Σ�ǰһ�ξ�С�������¼����һ�δ��������¼                                                                     */
/************************************************************************/
int Gabor::Partition(double *List,int low,int high,int *sort)
{
	double pivotkey=List[low];
	int z;
	z=sort[low];//���ӱ�ĵ�һ����¼�������¼
	
	while(low<high)
	{   
		while((low<high)&&(List[high]>=pivotkey)) 
		{ 
			--high;
		}
		if(low!=high)
		{ 
			List[low]=List[high];
			sort[low]=sort[high];
		}//���������¼С�ļ�¼�Ƶ��Ͷ�
		
		while((low<high)&&(List[low]<=pivotkey)) 
		{   
			++low;
		}
		if(low!=high)
		{
			List[high]=List[low];
			sort[high]=sort[low];
		}//���������¼��ļ�¼�Ƶ��߶�
	}
	List[low]=pivotkey;
	sort[low]=z;
	return low;//���������¼
	
}
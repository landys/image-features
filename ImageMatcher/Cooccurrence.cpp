#include "stdafx.h"
#include "Cooccurrence.h"
#include "string.h"
#include "math.h"
#include "malloc.h"
#include "stdio.h"
#include <highgui.h>

/************************************************************************/
/* ��BMP�ļ�����ȡ�Ҷ�����                                                                     */
/************************************************************************/
/*int Cooccurrence::ReadColorImage(char *FileName, int *Row, int *Col, unsigned char *BufR, unsigned char *BufG,
				   unsigned char *BufB,unsigned char *BufY, unsigned char *Fhead)
{ 
	unsigned long Index=0; 
	int i,j,k;
	FILE *ImageDataFile;  
  
	if((ImageDataFile=fopen(FileName,"rb"))==NULL) 
		return(0);
 
	for(i=0; i<54; i++) 
		Fhead[i]=fgetc(ImageDataFile);

	*Col=Fhead[19]*256+Fhead[18];//�õ�ͼ�񳤶�
	*Row=Fhead[23]*256+Fhead[22];//�õ�ͼ����

	k=4-(*Col)*3%4; 
	if(k==4) k=0;//�̶���ʽ

	for(i=0;i<(*Row);i++)
	{ 
		for(j=0;j<(*Col);j++,Index++)
		{ 
			BufB[Index]=fgetc(ImageDataFile);//�õ�ÿ�����ص�Bֵ
			BufG[Index]=fgetc(ImageDataFile);//�õ�ÿ�����ص�Gֵ
			BufR[Index]=fgetc(ImageDataFile);//�õ�ÿ�����ص�Rֵ
			BufY[Index]=(unsigned char)(0.299*BufR[Index]+0.587*BufG[Index]+0.114*BufB[Index]);//�õ��Ҷȷ���
		}
		for(j=1;j<=k;j++) fgetc(ImageDataFile);//�̶���ʽ��ȥ����������
	}  

	fclose(ImageDataFile);       
	return(1);
}*/

/************************************************************************/
/* ��ȡͼƬ����
/* [in]fname: �ļ����ƣ�����·��
/* [out]Pic_WenLi:��ȡ����������������                                                                     */
/************************************************************************/
int Cooccurrence::OnWenLi(const char *fname, Pic_WLType *Pic_WenLi) 
{
	IplImage* imgRgb = cvLoadImage(fname, CV_LOAD_IMAGE_COLOR);
	if (imgRgb == NULL)
	{
		// log error, cannot open file
		return 1;
	}
	CvSize imgSize;
	imgSize.width = imgRgb->width;
	imgSize.height = imgRgb->height;

	unsigned char *imageY = new unsigned char[imgSize.width * imgSize.height];
	memset(imageY, 0, sizeof(unsigned char) * imgSize.width * imgSize.height);
	
	unsigned char b, g, r;
	for( int h = 0; h < imgSize.height; ++h ) {
		for ( int w = 0; w < imgSize.width * 3; w += 3 ) {
			b  = ((unsigned char*)(imgRgb->imageData + imgRgb->widthStep * h))[w+0];
			g = ((unsigned char*)(imgRgb->imageData + imgRgb->widthStep * h))[w+1];
			r = ((unsigned char*)(imgRgb->imageData + imgRgb->widthStep * h))[w+2];
			imageY[imgSize.width * h + w / 3] = (unsigned char)(0.299*r + 0.587*g + 0.114*b); //�õ��Ҷȷ���
		}
	}
	cvReleaseImage(&imgRgb);

	GetDivision(imgSize.height, imgSize.width, imageY, Pic_WenLi); //�õ�25���ֿ�0,45,90,135�ȹ��������8������ֵ��������������
	delete[] imageY;
	
	return 0;
}

/************************************************************************/
/* ��������ȡ����                                                                     */
/************************************************************************/
int Cooccurrence::GetDivision(int Row,int Col,unsigned char *ImageY,struct Pic_WLType *Pic_WenLi)
{
	
	int a=0;
	int Index=0;
	int Col0=Col/5;
    int Row0=Row/5;
	
	int Co_Matrix_0[64][64],Co_Matrix_45[64][64],Co_Matrix_90[64][64],Co_Matrix_135[64][64];
	//0,45,90,135�ȷ���ĻҶȹ�������
    float CCON[4][Block_Total],AASM[4][Block_Total],EENT[4][Block_Total],CCOR[4][Block_Total];
	//25���ֿ���0,45,90,135�ȷ���ĻҶȹ��������4��������
    float  A_CON[Block_Total],A_ASM[Block_Total],A_ENT[Block_Total],A_COR[Block_Total],T_CON[Block_Total],T_ASM[Block_Total],T_ENT[Block_Total],T_COR[Block_Total];
    //25���ֿ��д�4����������ȡ�ľ�ֵ�ͷ���
	
	int i, j, p, q;
    for(p=0,i=0;i<Block_Num;p=p+Row0,i++)//��0Ϊ��㣬��Row0Ϊ��λ����ɨ��ȫͼ��Row0Ϊ1/5��ͼ�߶�
	{
		for(q=0,j=0;j<Block_Num;q=q+Col0,j++)//��0Ϊ��㣬��Col0Ϊ��λ����ɨ��ȫͼ��Col0Ϊ1/5��ͼ���
		{
			Index=(p/Row0)*(Row0*Col)+q;//ÿ���ֿ���������
			
			GetCoMatrix(Col,Row0,Col0,p,q,Index,ImageY,Co_Matrix_0,Co_Matrix_45,
				Co_Matrix_90,Co_Matrix_135);//��0,45,90,135�ȷ���Ĺ�������
			
			CCON[0][a]=GetCON(Co_Matrix_0);
			AASM[0][a]=GetASM(Co_Matrix_0);
            EENT[0][a]=GetENT(Co_Matrix_0);
			CCOR[0][a]=GetCOR(Co_Matrix_0);
			
			CCON[1][a]=GetCON(Co_Matrix_45);
			AASM[1][a]=GetASM(Co_Matrix_45);
            EENT[1][a]=GetENT(Co_Matrix_45);
			CCOR[1][a]=GetCOR(Co_Matrix_45);
			
			CCON[2][a]=GetCON(Co_Matrix_90);
			AASM[2][a]=GetASM(Co_Matrix_90);
            EENT[2][a]=GetENT(Co_Matrix_90);
			CCOR[2][a]=GetCOR(Co_Matrix_90);
			
			CCON[3][a]=GetCON(Co_Matrix_135);
			AASM[3][a]=GetASM(Co_Matrix_135);
            EENT[3][a]=GetENT(Co_Matrix_135);
			CCOR[3][a]=GetCOR(Co_Matrix_135);
			
			a++;//��0��24
		}
	}//�ֿ�����		    
	
	float total=0;
	
	for(i=0;i<Block_Total;i++)
	{
		for(j=0;j<4;j++) total=total+CCON[j][i];//�󷴲�ľ�ֵ�ͱ�׼��
		A_CON[i]=total/4;
		T_CON[i]=(float)(sqrt(((CCON[0][i]-A_CON[i])*(CCON[0][i]-A_CON[i])
			+(CCON[1][i]-A_CON[i])*(CCON[1][i]-A_CON[i])
			+(CCON[2][i]-A_CON[i])*(CCON[2][i]-A_CON[i])
			+(CCON[3][i]-A_CON[i])*(CCON[3][i]-A_CON[i]))/4));
		
		total=0;
        for(j=0;j<4;j++) total=total+AASM[j][i];//�������ľ�ֵ�ͱ�׼��
		A_ASM[i]=total/4;
		T_ASM[i]=(float)(sqrt(((AASM[0][i]-A_ASM[i])*(AASM[0][i]-A_ASM[i])
			+(AASM[1][i]-A_ASM[i])*(AASM[1][i]-A_ASM[i])
			+(AASM[2][i]-A_ASM[i])*(AASM[2][i]-A_ASM[i])
			+(AASM[3][i]-A_ASM[i])*(AASM[3][i]-A_ASM[i]))/4));
		
		total=0;
		for(j=0;j<4;j++) total=total+EENT[j][i];//���صľ�ֵ�ͱ�׼��
		A_ENT[i]=total/4;
		T_ENT[i]=(float)(sqrt(((EENT[0][i]-A_ENT[i])*(EENT[0][i]-A_ENT[i])
			+(EENT[1][i]-A_ENT[i])*(EENT[1][i]-A_ENT[i])
			+(EENT[2][i]-A_ENT[i])*(EENT[2][i]-A_ENT[i])
			+(EENT[3][i]-A_ENT[i])*(EENT[3][i]-A_ENT[i]))/4));
		
		total=0;
		for(j=0;j<4;j++) total=total+CCOR[j][i];//����صľ�ֵ�ͱ�׼��
		A_COR[i]=total/4;
		T_COR[i]=(float)(sqrt(((CCOR[0][i]-A_COR[i])*(CCOR[0][i]-A_COR[i])
			+(CCOR[1][i]-A_COR[i])*(CCOR[1][i]-A_COR[i])
			+(CCOR[2][i]-A_COR[i])*(CCOR[2][i]-A_COR[i])
			+(CCOR[3][i]-A_COR[i])*(CCOR[3][i]-A_COR[i]))/4));
		
	}//�ֱ��25���ֿ�������ֵ	
	
	for(i=0;i<Block_Total;i++) // Block_Total + 1 => Block_Total??, tony
	{
		Pic_WenLi->wenli[i][0]=A_CON[i]; Pic_WenLi->wenli[i][1]=T_CON[i];
		Pic_WenLi->wenli[i][2]=A_ASM[i]; Pic_WenLi->wenli[i][3]=T_ASM[i];
		Pic_WenLi->wenli[i][4]=A_ENT[i]; Pic_WenLi->wenli[i][5]=T_ENT[i];
		Pic_WenLi->wenli[i][6]=A_COR[i]; Pic_WenLi->wenli[i][7]=T_COR[i];
	}
	
	return 0;
}

/************************************************************************/
/* ���ɹ�������                                                                     */
/************************************************************************/
int Cooccurrence::GetCoMatrix(int Col,int RowX,int ColY,int p,int q,unsigned long Index,
				  unsigned char *BufY,int CMatrix_0[64][64],int CMatrix_45[64][64],
				  int CMatrix_90[64][64],int CMatrix_135[64][64])
{   
	int i,j,k,Newlevel,m,n;
	char** GrayMatrix = new char*[RowX];
	for (i=0; i<RowX; ++i)
	{
		GrayMatrix[i] = new char[ColY];
	}
	
	for(i=p;i<p+RowX;i++)
	{
		for(j=q;j<q+ColY;j++)
		{   
			k=BufY[Index]%4;
			Newlevel=(BufY[Index]-k)/4;
			GrayMatrix[i-p][j-q]=Newlevel;
			Index++;
		}
		Index=Index+(Col-ColY);
	}//���ɻҶ����ؾ���
	
	for(m=0;m<64;m++)
	{
		for(n=0;n<64;n++)
		{
			CMatrix_0[m][n]=0;
		}
	}//����������0

	for(i=p;i<RowX+p;i++)
	{
		for(j=q;j<q+ColY-1;j++)
		{   
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p][j-q+1];
			CMatrix_0[m][n]++;
		}
		for(j=q+ColY-1;j>q;j--)
		{
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p][j-q-1];
			CMatrix_0[m][n]++;
		}
	}//����0�ȹ�������
	
	for(m=0;m<64;m++)
	{
		for(n=0;n<64;n++)
		{
			CMatrix_45[m][n]=0;
		}
	}//����������0

	for(i=p;i<p+RowX-1;i++)
	{
		for(j=q+1;j<q+ColY;j++)
		{   
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p+1][j-q-1];
			CMatrix_45[m][n]++;
		}
	}
	for(i=p+RowX-1;i>p;i--)
	{
		for(j=q;j<q+ColY-1;j++)
		{
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p-1][j-q+1];
			CMatrix_45[m][n]++;
		}
	}//����45�ȹ�������
	

	for(m=0;m<64;m++)
	{
		for(n=0;n<64;n++)
		{
			CMatrix_90[m][n]=0;
		}
	}//����������0

	for(j=q;j<q+ColY;j++)
	{
		for(i=p;i<p+RowX-1;i++)
		{   
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p+1][j-q];
			CMatrix_90[m][n]++;
		}
		for(i=p+RowX-1;i>p;i--)
		{
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p-1][j-q];
			CMatrix_90[m][n]++;
		}
	}//����90�ȹ�������
	
	for(m=0;m<64;m++)
	{
		for(n=0;n<64;n++)
		{
			CMatrix_135[m][n]=0;
		}
	}//����������0

	for(i=p;i<p+RowX-1;i++)
	{
		for(j=q;j<q+ColY-1;j++)
		{   
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p+1][j-q+1];
			CMatrix_135[m][n]++;
		}
	}
	for(i=p+RowX-1;i>p;i--)
	{
		for(j=q+ColY-1;j>q;j--)
		{
			m=GrayMatrix[i-p][j-q];
			n=GrayMatrix[i-p-1][j-q-1];
			CMatrix_135[m][n]++;
		}
	}//����135�ȹ�������

	// free memory
	for (i=0; i<RowX; ++i)
	{
		delete[] GrayMatrix[i];
	}
	delete[] GrayMatrix;

	return 0;	
}

/************************************************************************/
/* ������                                                                     */
/************************************************************************/
float Cooccurrence::GetASM(int CMatrix[64][64])
{
	int i,j;
	float a=0,p=0;
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			p=CMatrix[i][j]+p;
		}
	}
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			a=(CMatrix[i][j]/p)*(CMatrix[i][j]/p)+a;
		}
	}
	return(a);
}

/************************************************************************/
/* ����                                                                     */
/************************************************************************/
float Cooccurrence::GetENT(int CMatrix[64][64])
{
	int i,j;
	float e=0,p=0;
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			p=CMatrix[i][j]+p;
		}
	}
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{   
			if(CMatrix[i][j]==0)
				CMatrix[i][j]=1;
			e=(float)((CMatrix[i][j]/p)*(log10(CMatrix[i][j]/p)/0.30103)+e);
		}
	}

	e=e*(-1);
	return(e);
}

/************************************************************************/
/* �����                                                                     */
/************************************************************************/
float Cooccurrence::GetCOR(int CMatrix[64][64])
{
	int i,j;
	float R,r=0,p=0,u1=0,u2=0,v1=0,v2=0,m=0,n=0;
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			p=CMatrix[i][j]+p;
		}
	}
	
	for(i=0;i<64;i++) 
	{
		m=(float)(i+1);
		for(j=0;j<64;j++)
		{
			n=(CMatrix[i][j]/p)+n;
		}
		u1=m*n+u1;
		n=0;
	}
	m=0;	
	
	for(j=0;j<64;j++) 
	{
		m=(float)(j+1);
		for(i=0;i<64;i++)
		{
			n=(CMatrix[i][j]/p)+n;
		}
		u2=m*n+u2;
		n=0;
	}
	m=0;	

	for(i=0;i<64;i++) 
	{
		m=i+1-u1;
		for(j=0;j<64;j++)
		{
			n=(CMatrix[i][j]/p)+n;
		}
		v1=m*m*n+v1;
		n=0;
	}
	m=0;
	
	for(j=0;j<64;j++) 
	{
		m=j+1-u2;
		for(i=0;i<64;i++)
		{
			n=(CMatrix[i][j]/p)+n;
		}
		v2=m*m*n+v2;
		n=0;
	}
	m=0;
	
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			r=(i+1)*(j+1)*(CMatrix[i][j]/p)+r;
		}
	}
	R=(r-u1*u2)/(v1*v2);
	return(R);
}

/************************************************************************/
/* �󷴲�                                                                     */
/************************************************************************/
float Cooccurrence::GetCON(int CMatrix[64][64])
{
	int i,j;
	float c=0,p=0;
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			p=CMatrix[i][j]+p;
		}
	}
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			c=(i-j)*(i-j)*(CMatrix[i][j]/p)+c;
		}
	}
	return(c);
}

/************************************************************************/
/* ����ͼƬ����
/* [in]model_WL: �ؼ�ͼ��������������
/* [in]Pic_WL: ����ͼƬ����������������
/* [in]fnum: ����ͼƬ��
/* [out]sort: �������ƶȵõ��Ĵ���ͼƬ�����ƶ�����                                                                          */
/************************************************************************/
int Cooccurrence::OnRetrieveIn(Pic_WLType *model_WL, Pic_WLType *Pic_WL, int fnum, int *sort) 
{
    float *Dis_YS = (float *)malloc(fnum * sizeof(float));//�������������ľ���
	float *Dis_YS0 = (float *)malloc(fnum * sizeof(float));//��һ��������������ľ���
	
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
int Cooccurrence::GetDistance(Pic_WLType model_WL, Pic_WLType Pic_WL, float *Dis_WL)
{  
	float TempBlock[Block_Total];
	float Temp;
	int j,r;
	
	for(Temp=0,j=0;j<Block_Total;j++)
	{
		float total=0;
		for(r=0;r<BLOCK_TEXTURE_SIZE;r++) // 8����������
			total+=(model_WL.wenli[j][r]-Pic_WL.wenli[j][r])*(model_WL.wenli[j][r]-Pic_WL.wenli[j][r]);
		
		TempBlock[j]=(float)sqrt(total);	
		Temp += TempBlock[j];//��������ÿ���ֿ��Ȩֵ�������ܵ����ƾ���
	}
	*Dis_WL = Temp;//�����зֿ�������Ȩ�;���
	return(1);
}

/************************************************************************/
/* ���һ����ֵ                                                                     */
/************************************************************************/
int Cooccurrence::GuiYiHua(float *Dis_old,float *Dis_new, int fnum)
{
	double total=0;
	int i;
	float T_ave,T_bzc;//��ֵ�ͱ�׼��
	
	for(i=0;i<fnum;i++)   
		total+= Dis_old[i];
	T_ave=(float)total/fnum;//��ֵ
	
	total=0;
	for(i=0;i<fnum;i++)
		total+=(Dis_old[i]-T_ave)*(Dis_old[i]-T_ave);
    T_bzc=(float)sqrt(total/fnum);//��׼��
	
	for(i=0;i<fnum;i++)
	{
		Dis_new[i]=(float)(Dis_old[i]-T_ave)/(3*T_bzc); // �������ֵ�����, ��ôT_bzc=0, ����??? --tony
		Dis_new[i]=(Dis_new[i]+1)/2;
        if(Dis_new[i]<0) Dis_new[i]=0;
		if(Dis_new[i]>1) Dis_new[i]=1;//��һ����(0,1)
	}//��˹��һ��
	
	return 1;
}


/************************************************************************/
/* ����������������                                                                     */
/************************************************************************/
int Cooccurrence::QSort(float *List, int low, int high, int *sort)
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
int Cooccurrence::Partition(float *List,int low,int high,int *sort)
{
	float pivotkey=List[low];
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

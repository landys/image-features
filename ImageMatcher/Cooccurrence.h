#pragma once

#include "string.h"

#define Block_Num 5 // �ֿ���
#define Block_Total (Block_Num * Block_Num) // ÿ��ͼƬ�ܹ��ֳɵĿ���
#define BLOCK_TEXTURE_SIZE 8

//#define IMG_ROW 600 // ͼƬ��С����
//#define IMG_COL 600

/************************************************************************/
/* ������������������ȡ�����                                                                     */
/************************************************************************/
class Cooccurrence
{
public:
	typedef struct Pic_WLType
	{
		float wenli[Block_Total][BLOCK_TEXTURE_SIZE];//25�ֿ���������
	} Pic_WLType;//ͼƬ���������ṹ��

public:
	
	int OnWenLi(const char *fname, Pic_WLType *Pic_WenLi);
	int OnRetrieveIn(Pic_WLType *model_WL, Pic_WLType *Pic_WL, int fnum, int *sort);
private:
	int GetDivision(int Row,int Col,unsigned char *ImageY,struct Pic_WLType * Pic_WenLi);
	int GetCoMatrix(int Col,int RowX,int ColY,int p,int q,unsigned long Index,
					unsigned char *BufY,int CMatrix_0[64][64],int CMatrix_45[64][64],
					int CMatrix_90[64][64],int CMatrix_135[64][64]);
	float GetASM(int CMatrix[64][64]);
	float GetENT(int CMatrix[64][64]);
	float GetCOR(int CMatrix[64][64]);
	float GetCON(int CMatrix[64][64]);

	int GetDistance(Pic_WLType model_WL, Pic_WLType Pic_WL, float *Dis_WL);
	int GuiYiHua(float *Dis_old,float *Dis_new, int fnum);
	int QSort(float *List, int low, int high, int *sort);
	int Partition(float *List,int low,int high,int *sort);

	//int ReadColorImage(char *FileName, int *Row, int *Col, unsigned char *BufR, unsigned char *BufG,
	//			   unsigned char *BufB,unsigned char *BufY, unsigned char *Fhead);
};
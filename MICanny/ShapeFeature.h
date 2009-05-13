// ShapeFeature.h: interface for the CShapeFeature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAPEFEATURE_H__DB708873_CDB0_4F57_B61C_C1FBA64E65FE__INCLUDED_)
#define AFX_SHAPEFEATURE_H__DB708873_CDB0_4F57_B61C_C1FBA64E65FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FeatureBase.h"

typedef struct{
	int Height;
	int Width;
}	Seed;

// used as stack node in TraceEdge. -- add by rico
class Node
{
public:
	int x;
	int y;
	int k;
	Node(int x, int y, int k) : x(x), y(y), k(k)
	{
	}
};

class CShapeFeature : public CFeatureBase  
{
public:
	CShapeFeature(CImg *img);
	virtual ~CShapeFeature();

	int get_feature();

};

#endif // !defined(AFX_SHAPEFEATURE_H__DB708873_CDB0_4F57_B61C_C1FBA64E65FE__INCLUDED_)

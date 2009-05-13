// FeatureBase.h: interface for the CFeatureBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEATRUEBASE_H__E72ADB3E_ADAA_4959_826D_75E47441579A__INCLUDED_)
#define AFX_FEATRUEBASE_H__E72ADB3E_ADAA_4959_826D_75E47441579A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CImg.h"
#include "RetureCode.h"

class CFeatureBase  
{
public:
	CFeatureBase(CImg * img);
	virtual ~CFeatureBase();
public:
	virtual int get_feature()=0;

public:
	double * m_feature;
	unsigned m_dimension; 
	int m_weight;

protected:
	CImg * m_img;

};

#endif // !defined(AFX_FEATRUEBASE_H__E72ADB3E_ADAA_4959_826D_75E47441579A__INCLUDED_)

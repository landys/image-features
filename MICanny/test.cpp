#include <iostream>
#include "CImg.h"
#include "ShapeFeature.h"
#include "extractMICanny.h"
using namespace std;

CFeatureBase* testExtractMICanny(const char* filename)
{
	CImg m_img(MODE_RGB);
	CFeatureBase* feature = 0;
	if (m_img.read_JPEG_file(filename))
	{
		feature = new CShapeFeature(&m_img);
		if (!feature->get_feature())
		{
			delete feature;
			feature = 0;
		}
	}
	
	// no need, deallocate is called in the destructor.
	//m_img.deallocate(m_img.m_mode);

	return feature;
}

void test1()
{
	try
	{
		CFeatureBase* feature = testExtractMICanny("D:\\pics\\0.jpg");

		if (feature) 
		{
			cout << "Success." << endl;
			for (int i=0; i<feature->m_dimension; ++i)
			{
				cout << feature->m_feature[i] << " ";
			}
			cout << endl;

			delete feature;
		}
	}
	catch(...)
	{
		cout << "error happened." << endl;
	}
}


void test2()
{
	double* feature = new double[7];
	if (extractMICanny("D:\\pics\\0.jpg", feature))
	{
		cout << "Success." << endl;
		for (int i=0; i<7; ++i)
		{
			cout << feature[i] << " ";
		}
		cout << endl;

		delete feature;
	}
}

int main(int argc, char** argv)
{
	test1();
	test2();
	

	return 0;
}

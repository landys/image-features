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
	if (extractMICanny("E:\\pic_skindetect\\clothtest\\selected\\fsre.bmp", feature))
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

void test3()
{
	/*int n = 0, i, j;
	FILE * imageset = fopen("E:\\pic_skindetect\\clothtest\\temp\\imgs.txt", "rt");
	FILE* logFile = fopen("E:\\pic_skindetect\\clothtest\\temp\\log.txt", "w");
	char imagename[255];
	int re = 0;

	long abt = clock();
	fprintf(logFile, "file_name calc_time total_time\n");
	while(fgets(imagename, 255, imageset) != NULL)
	{
		int i = 0;
		for (i=0; imagename[i] != '\n' && imagename[i] != '\r' && imagename[i] != '\0'; ++i); // loop stop here
		imagename[i] = '\0';
		cout << imagename << endl;
		doTamura(string(imagename), logFile);

	}
	fclose(imageset);
	fclose(logFile);*/
}


int main(int argc, char** argv)
{
	//test1();
	test3();
	

	return 0;
}

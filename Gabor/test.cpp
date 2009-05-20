#include "Gabor.h"
#include "GetTexture.h"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <algorithm>
using namespace std;

int main(int argc, char** argv)
{
	CGetTexture cgt;
	long bt = clock();
	//cgt.CreateVecFile("E:\\pic_skindetect\\clothtest\\gabortest\\", "E:\\pic_skindetect\\clothtest\\gabor2.dat");
	cgt.initGaborFilter();
	cgt.extractGaborFeature("E:\\pic_skindetect\\clothtest\\gabortest\\a000.bmp");
	double* feature = cgt.GetVector();
	ostream_iterator<double> out (cout, ",");
	copy(feature, feature+48, out);

	fprintf(stdout, "%ld ", clock() - bt);
	return 0;
}

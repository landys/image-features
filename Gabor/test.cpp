#include "Gabor.h"
#include "GetTexture.h"
#include <cstdio>
#include <ctime>

int main(int argc, char** argv)
{
	CGetTexture cgt;
	long bt = clock();
	cgt.CreateVecFile("E:\\pic_skindetect\\clothtest\\gabortest\\", "E:\\pic_skindetect\\clothtest\\gabor.dat");
	fprintf(stdout, "%ld ", clock() - bt);
	return 0;
}

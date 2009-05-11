#include <iostream>
#include "imagelib.h"
#include "tamurafeature.h"
#include <ctime>
#include <cstdio>
using namespace std;
using namespace Magick;

#pragma comment(lib, "CORE_RL_Magick++_.lib")

string getFileName(const string& fn);
void doTamura(const string& filename, FILE* flog);

string getFileName(const string& fn)
{
	int i = fn.rfind('.');
	int j = fn.rfind('/');
	if (j == -1)
	{
		j = fn.rfind('\\');
	}
	if (i == -1)
	{
		i = fn.length();
	}

	return fn.substr(j+1, i-j-1) + "_re.jpg";
}

void doTamura(const string& filename, FILE* flog)
{
	ImageFeature im;
	im.load(filename);
	long bt = clock();
	ImageFeature tamuraImage=calculate(im);
	long bt2 = clock();

	normalize(tamuraImage);

	HistogramFeature h=histogramize(tamuraImage);

	fprintf(flog, "%s %ld %ld\n", filename.c_str(), bt2-bt, clock()-bt);

	string re = "E:\\pic_skindetect\\clothtest\\temp\\" + getFileName(filename);
	tamuraImage.save(re);
	h.save(re+".his");
}

void test1()
{
	doTamura("E:\\pic_skindetect\\clothtest\\selected\\6003.jpg", stdout);
}

void test2()
{
	int n = 0, i, j;
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

		doTamura(string(imagename), logFile);
		cout << imagename << endl;
	}
	fclose(imageset);
	fclose(logFile);
}

int main(int argc, char** argv)
{
	test2();
	
	return 0;
}


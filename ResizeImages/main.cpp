#include <Magick++.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
using namespace Magick;
using namespace std;

#pragma comment(lib, "CORE_RL_Magick++_.lib")

/** the imgsfile should utf-8 encoded. Default of max width and height are both 512, 
 * which means the size of its larger dimension is 512 pixels.
 */
void resize(const char* imgsfile, const char* refileNmae, const char* inputPart, const char* outputPart, int width=512, int height=512);

// the imgsfile should utf-8 encoded. Default of max width and height are both 512, 
// which means the size of its larger dimension is 512 pixels.
void resize(const char* imgsfile, const char* refileNmae, const char* inputPart, const char* outputPart, int width, int height)
{
	
	int n = 0, i, j;
	FILE * imageset = fopen(imgsfile, "rt");
	FILE * refile = fopen(refileNmae, "wt");
	char imagename[255];
	//char reimg[255];
	long long re = 0;
	int nin = strlen(inputPart);

	long long count = 0;
	while(fgets(imagename, 255, imageset) != NULL)
	{
		if (++count % 1000 == 0)
		{
			printf("current processed: %lld\n", count);
		}

		int i = 0;
		for (i=0; imagename[i] != '\n' && imagename[i] != '\r' && imagename[i] != '\0'; ++i); // loop stop here
		imagename[i] = '\0';
		//fprintf(refile, "%s\n", imagename);

		string reimg(imagename);
		j = reimg.find(inputPart);
		if (j == -1)
		{
			fprintf(refile, "wrong generate result file name: %s\n", imagename);
			continue;
		}
		reimg.replace(j, nin, outputPart);

		Image img;
		try {
			img.read(imagename);
			if (img.columns() < width && img.rows() < height)
			{
				continue;
			}
			img.resize(Geometry(width, height));
			img.write(reimg.c_str());
			re++;
		} catch( ... ) {
			fprintf(refile, "wrong reading: %s\n", imagename);
		}
	}

	printf("%lld images processed.\n", count);
	printf("%lld images finished.\n", re);
	fclose(imageset);
	fclose(refile);
}

/*void dotest()
{
	Image img;
	img.read("J:\\pic_bak\\cgfa.sunsite.dk\\a\\t-alexander2.jpg");
	printf("%d, %d\n", img.columns(), img.rows());
}*/

// NOTICE: the image list file should be in UTF-8 encoding, if some chinese characters!
int main(int argc, char** argv)
{
	if (argc < 5)
	{
		printf("Usage %s input_image_list_file output_log_file input_path_part diff_of_output_path_part max_width max_height\n", argv[0]);
		return 1;
	}

	int width = 512;
	int height = 512;

	if (argc >= 7)
	{
		sscanf(argv[5], "%d", &width);
		sscanf(argv[6], "%d", &height);
	}

	resize(argv[1], argv[2], argv[3], argv[4], width, height);

	return 0;
}


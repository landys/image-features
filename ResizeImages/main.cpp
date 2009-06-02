#include <Magick++.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
using namespace Magick;
using namespace std;

#pragma comment(lib, "CORE_RL_Magick++_.lib")

// the imgsfile should utf-8 encoded.
void resize(const char* imgsfile, const char* refileNmae, const char* inputPart, const char* outputPart)
{
	
	int n = 0, i, j;
	FILE * imageset = fopen(imgsfile, "rt");
	FILE * refile = fopen(refileNmae, "wt");
	char imagename[255];
	//char reimg[255];
	int re = 0;
	int nin = strlen(inputPart);

	while(fgets(imagename, 255, imageset) != NULL)
	{
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
			img.resize(Geometry(200, 200));
			img.write(reimg.c_str());
			re++;
		} catch( ... ) {
			fprintf(refile, "wrong reading: %s\n", imagename);
		}

		
	}

	printf("%d images finished.\n", re);
	fclose(imageset);
	fclose(refile);
}


int main(int argc, char** argv)
{
	if (argc < 5)
	{
		printf("Usage %s input_image_list_file output_log_file input_path_part diff_of_output_path_part\n", argv[0]);
		return 1;
	}

	resize(argv[1], argv[2], argv[3], argv[4]);

	return 0;
}


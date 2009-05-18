#include <Magick++.h>
#include <iostream>
using namespace Magick;
using namespace std;

#pragma comment(lib, "CORE_RL_Magick++_.lib")

void resize(const char* imgsfile)
{
	
	int n = 0, i, j;
	FILE * imageset = fopen(imgsfile, "rt");
	FILE * refile = fopen("E:\\pic_skindetect\\clothtest\\zz\\zz3.txt", "wt");
	char imagename[255];
	char reimg[255];
	int re = 0;

	while(fgets(imagename, 255, imageset) != NULL)
	{
		int i = 0;
		for (i=0; imagename[i] != '\n' && imagename[i] != '\r' && imagename[i] != '\0'; ++i); // loop stop here
		imagename[i] = '\0';
		//fprintf(refile, "%s\n", imagename);
		Image img;
		try {
			img.read(imagename);
			img.resize(Geometry(200, 200));
			strncpy(reimg, imagename, 28);
			strcpy(reimg+28, "zz200");
			strcpy(reimg+33, imagename+30);
			img.write(reimg);
			re++;
		} catch( ... ) {
			fprintf(refile, "wrong reading %s\n", imagename);
		}

		
	}

	printf("%d images finished.\n", re);
	fclose(imageset);
	fclose(refile);
}


int main(int argc, char** argv)
{
	resize("E:\\pic_skindetect\\clothtest\\zz\\zz.txt");
}

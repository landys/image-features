/*
This file is part of the FIRE -- Flexible Image Retrieval System

FIRE is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

FIRE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FIRE; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <cmath>
#include <math.h>
#include <limits>
#include "imagelib.h"
#include "diag.h"
#include "imagefeature.h"
#include <map>

using namespace std;


void multiply(ImageFeature &img1, const ImageFeature& img2) {
  if ((img1.xsize() != img2.xsize()) ||
      (img1.ysize() != img2.ysize()) ||
      (img1.zsize() != img2.zsize())) {
    ////DBG(10) << "image features have different sizes, cannot be multiplied.";
  }
  for (uint z = 0; z < img1.zsize(); z++)
    for (uint x = 0; x < img1.xsize(); x++)
      for (uint y = 0; y < img1.ysize(); y++) {
	img1(x, y, z) = img1(x, y, z) * img2(x, y, z);
      }
}

void multiply(ImageFeature &img, const double s) {
  for(uint z=0;z<img.zsize();++z) {
    for(uint x=0;x<img.xsize();++x) {
      for(uint y=0;y<img.ysize();++y)  {
        img(x,y,z)*=s;
      }
    }
  }
}

void convolve(ImageFeature &img, const ImageFeature &filter) {
	ImageFeature copy=img;

	int height2=filter.ysize()/2;
	int width2=filter.xsize()/2;

	double tmp;
	for(uint c=0;c<img.zsize();++c) {
		for(uint x=0;x<img.xsize();++x) {
			for(uint y=0;y<img.ysize();++y) {
				tmp=0.0;
				for(int i=-width2;i<=width2;++i) {
					int xx=x+i;
					if(xx<int(img.xsize()) && int(xx) >= 0) {
						for(int j=-height2;j<=height2;++j) {
							int yy=y+j; 
							if(int(yy)>=0 && yy < int(img.ysize())) {
								tmp+=filter(i+width2,j+height2,0)*copy(xx,yy,c);
							}
						}
					}
				}
				img(x,y,c)=tmp;
			}
		}
	}
}



void square(ImageFeature &img1) {
  multiply(img1, img1);
}

void power(ImageFeature &img1, int p) {
  for (uint z = 0; z < img1.zsize(); z++)
    for (uint x = 0; x < img1.xsize(); x++)
      for (uint y = 0; y < img1.ysize(); y++) {
	double tmp = 1.0;
	double val = img1(x, y, z);
	for (int t = 0; t < p; t++) {
	  tmp *= val;
	}
	img1(x, y, z) = tmp;
      }
}

void signedPower(ImageFeature &img1, int p) {
  for (uint z = 0; z < img1.zsize(); z++)
    for (uint x = 0; x < img1.xsize(); x++)
      for (uint y = 0; y < img1.ysize(); y++) {
	double tmp = 1.0;
	for (int t = 0; t < p; t++) {
	  tmp *= img1(x, y, z);
	}
	if ((img1(x, y, z) < 0.0) && ((p % 2) == 0)) {
	  tmp *= -1.0;
	}
	img1(x, y, z) = tmp;
      }
}

void absolute(ImageFeature &img1) {
  for (uint z = 0; z < img1.zsize(); z++)
    for (uint x = 0; x < img1.xsize(); x++)
      for (uint y = 0; y < img1.ysize(); y++) {
	if (img1(x, y, z) < 0.0) {
	  img1(x, y, z) = -img1(x, y, z);
	} 
      }
}

void normalize(ImageFeature &img) {
  double min, max;
  
  for(uint c=0;c<img.zsize();++c) {
    min=minimum(img,c);
    max=maximum(img,c);
    if (max - min != 0.0) {
      double d=(1.0/(max-min));
      for(uint x=0;x<img.xsize();++x) {
        for(uint y=0;y<img.ysize();++y) {
          img(x,y,c)=(img(x,y,c)-min)*d;
        }
      }
    }
  }
}


double minimum(const ImageFeature &img, uint c) {
  double min=numeric_limits<double>::max();
  for(uint x=0;x<img.xsize();++x) {
    for(uint y=0;y<img.ysize();++y) {
      min=::std::min(min,img(x,y,c));
    }
  }
  return min;
}

double maximum(const ImageFeature &img, uint c) {
  double max=-1.0;
  for(uint x=0;x<img.xsize();++x) {
    for(uint y=0;y<img.ysize();++y) {
      max=::std::max(max,img(x,y,c));
    }
  }
  return max;
}


::std::pair<uint,uint> argmin(const ImageFeature &img,uint c) {
  pair<uint,uint> res;
  double min=numeric_limits<double>::max();
  for(uint x=0;x<img.xsize();++x) {
    for(uint y=0;y<img.ysize();++y) {
      if(min>img(x,y,c)) {
        min=img(x,y,c); res.first=x; res.second=y;
      }
    }
  }
  return res;
}

::std::pair<uint,uint> argmax(const ImageFeature &img,uint c) {
  pair<uint,uint> res;
  double max=0.0;
  for(uint x=0;x<img.xsize();++x) {
    for(uint y=0;y<img.ysize();++y) {
      if(max<img(x,y,c)) {
        max=img(x,y,c); res.first=x; res.second=y;
      }
    }
  }
  return res;
}



void add(ImageFeature &a, const ImageFeature &b, const uint z) {
  for(uint x=0;x<a.xsize();++x) {
    for(uint y=0;y<a.ysize();++y) {
      a(x,y,z)+=b(x,y,0);
    }
  }
}


void divide(ImageFeature &a, const double &d) {
  for(uint x=0;x<a.xsize();++x) {
    for(uint y=0;y<a.ysize();++y) {
      for(uint z=0;z<a.zsize();++z) {
        a(x,y,z)/=d;
      }
    }
  }
}


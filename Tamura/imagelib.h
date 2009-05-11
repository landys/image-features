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
#ifndef __imagelib_hpp__
#define __imagelib_hpp__


#include "imagefeature.h"
#include <string>

void normalize(ImageFeature &img);

/// multiply to images, pixelwise
void multiply(ImageFeature &img1, const ImageFeature& img2);

/// multiply an image with a scalar, pixelwise
void multiply(ImageFeature &img, const double s);

void convolve(ImageFeature &img, const ImageFeature &filter);

void square(ImageFeature &img1);
void power(ImageFeature &img, int p);
void signedPower(ImageFeature &img, int p);
void absolute(ImageFeature &img);

// add plane 0 from b to plane z from a
void add(ImageFeature &a, const ImageFeature &b, const uint z);

//divide each value from a by d
void divide(ImageFeature &a, const double &d);


// get the min/max of the layer c of the image
double minimum(const ImageFeature &img, uint c=0);
double maximum(const ImageFeature &img, uint c=0);

#endif

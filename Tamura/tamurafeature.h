#include <string>
#include "diag.h"
#include "imagefeature.h"
#include "histogramfeature.h"
using namespace std;


ImageFeature calculate(const ImageFeature &img);

ImageFeature contrast(const ImageFeature &image, uint z);
ImageFeature directionality(const ImageFeature &image, uint z);
ImageFeature coarseness(const ImageFeature &image, uint z);


double getLocalContrast(const ImageFeature &image, int xpos, int ypos, uint z);
double efficientLocalMean(const uint y,const uint x,const uint k,const ImageFeature &laufendeSumme);

HistogramFeature histogramize(const ImageFeature &image, int n);

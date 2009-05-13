#include "extracttamura.h"
#include "imagelib.h"
#include "tamurafeature.h"
#include <algorithm>

extern "C" DLL_EXPORT int extractTamura(const char* imagenamefile, int n, double* feature)
{
	ImageFeature im;
	im.load(imagenamefile);

	ImageFeature tamuraImage = calculate(im);
	normalize(tamuraImage);

	HistogramFeature h = histogramize(tamuraImage, n);

	copy(h.ndata().begin(), h.ndata().end(), feature);
	
	return 0;
}

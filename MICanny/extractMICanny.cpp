#include "extractMICanny.h"
#include "CImg.h"
#include "ShapeFeature.h"
#include "string.h"

extern "C" DLL_EXPORT int extractMICanny(const char* imagenamefile, double* feature)
{
	CImg m_img(MODE_RGB);
	int re = 0;

	if (m_img.read_JPEG_file(imagenamefile))
	{
		CFeatureBase* ft = new CShapeFeature(&m_img);
		if (ft->get_feature())
		{
			for (int i=0; i<ft->m_dimension; ++i)
			{
				feature[i] = ft->m_feature[i];
			}
			re = 1;
		}
		delete ft;
	}

	// no need, deallocate is called in the destructor.
	//m_img.deallocate(m_img.m_mode);

	return re;
}
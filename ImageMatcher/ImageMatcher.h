// ImageMatcher.h

#pragma once

//#include <string>
//#include "Gabor.h"
#include "Cooccurrence.h"
#include "GaborFeature.h"

using namespace System;

namespace Zju 
{
	namespace Image
	{
		public ref class ImageMatcher
		{
		public:
			// Extract 24-v color vector for a image.
			// Format of ingoreColors is as: 0xffffff.
			// Return null if failed.
			array<float>^ ExtractRGBSeparateColorVector(String^ imageFileName, int n, array<int>^ ignoreColors);

			// Extract n*n*n-v color vector for a image.
			array<float>^ ExtractRGBColorVector(String^ imageFileName, int n, array<int>^ ignoreColors);

			// Extract n*n*n-v HSV color vector for a image.
			array<float>^ ExtractHSVColorVector(String^ imageFileName, int n, array<int>^ ignoreColors);

			// Extract 11*4*4-v HSV color vector for a image.
			array<float>^ ExtractHSVAynsColorVector(String^ imageFileName, int n, array<int>^ ignoreColors);

			// Extract n*n*n-v HLS color vector for a image.
			array<float>^ ExtractHLSColorVector(String^ imageFileName, int n, array<int>^ ignoreColors);

			// Extract 64-v texture vector for a image. Return null if failed.
			array<float>^ ExtractDaubechiesWaveletVector(String^ imageFileName);

			// It should be called before get_waveletfeature.
			// It can be called just once before several "get_waveletfeature"
			// Return null if failed.
			bool LuvInit(String^ luvFileName);

			void GaborInit();

			//bool GaborKernelInit(String^ gaborKernelFileName);

			// the size of dimensions is 48
			array<float>^ ExtractGaborVector(String^ imageFileName);

			// the size of dimensions is 200
			array<float>^ ExtractCooccurrenceVector(String^ imageFileName);

			// tarmura texture, n*n*n is the size of dimensions, n=6 is prefer
			array<float>^ ExtractTamuraVector(String^ imageFileName, int n);

			// Moment Invariants using Canny descriptor, the size of dimensions is 7
			array<double>^ ExtractMICannyVector(String^ imageFileName);

			// Hu Moment Invariants, the size of dimensions is 7
			array<double>^ ExtractMIHuVector(String^ imageFileName);

			// tarmura texture, n*2 is the size of dimensions
			array<float>^ ExtractFourierVector(String^ imageFileName, int n);

			ImageMatcher();

			~ImageMatcher();
		private:
			// mark if lvuInit method called.
			bool isLuvInited;

			//Gabor* pGabor;

			GaborFeature* pGabor;

			Cooccurrence* pCoocc;
		private:
			//bool ImageMatcher::to_CharStar(String^ source, char*& target);
			//bool ImageMatcher::to_string(String^ source, std::string &target);
			array<float>^ to_array(float* pf, int n);

			array<float>^ to_array(double* pd, int n);

			array<double>^ to_double_array(double* pd, int n);

//			array<float>^ to_array(Gabor::Pic_GaborWL* picwl);

			array<float>^ to_array(Cooccurrence::Pic_WLType* picwl);

			int calcHSVIndex(float h, float s, float v);

			// use utf8 to encode the string and store as char*. for windows, #include <wtypes.h>.
			// not support linux, ps. iconv can do the same thing in linux.
			// src -- a string to be converted, may contains characters like Chinese.
			// return -- a point to a string which allocated in heap, it should be delete[] outside the function.
			char* acp2utf8(const char* src);
		};
	}
}

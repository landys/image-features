// CImg.h
// Decompressed Image Data Object - contains extracted RGB values
// Written by Stephen Wong

/** 
 * This CImg class is a wrapper that contains decompressed RGB values of an
 * image.  This class can be used to store decompressed values of a JPEG image,
 * and its data can also be used to create a new JPEG image.  Furthermore,
 * 24-bit uncompressed bitmap reading and writing are now supported.
 *
 * LIBJAVA Open Source Copyright (C) 1991-1998, Thomas G. Lane.
 * A member of the Independent JPEG Group.
 */

#ifndef _CIMG_H_
#define _CIMG_H_

#include "windows.h"		// Windows BMP format declaration
#include <stdio.h>			// ANSI C input/output
#include <math.h>
// libjpeg is written in C, so ensure compilation is accurate
extern "C"
{
	#include "jpeglib.h"		// include JPEG library declarations
	#include "jerror.h"		// JPEG error handler
}

// colour modes
#define MODE_GRAYSCALE	1
#define MODE_RGB		3

// read_BMP_file() error codes
#define READ_BMP_SUCCESS			0
#define READ_BMP_FILENOTFOUND_ERR	1	// input file not found
#define READ_BMP_INCOMPAT_ERR		2	// not 24-bit uncompressed bitmap
#define READ_BMP_FSEEK_ERR			3	// file seek error (incomplete file)
#define READ_BMP_EOF_ERR			4	// end-of-file unexpected (incomplete
										// file)

// write_BMP_file() error codes
#define WRITE_BMP_SUCCESS			0
#define WRITE_BMP_FILENOTFOUND_ERR	1	// output file cannot be created
#define WRITE_BMP_WRITE_ERR			2	// write error (permissions, file
										// space)

// pixel structure - used for setPixel() and getPixel()
struct RGBpixel
{
	int red;
	int blue;
	int green;
};

typedef struct HSVpixel
{
	double hue;
	double sat;
	double value;
}HSVpixel;

struct LABpixel
{
	double L;
	double A;
	double B;
};

class CImg {
	// Paramters --------------------------------------------------------------
private:	

public:
	int m_mode;				// image mode, either grayscale or RGB
	int m_width, m_height;	// width and height of image
	RGBpixel ** image;  	// RGB pixel array
	int ** gImage;			// grayscale pixel array
	HSVpixel ** hsvImage;  //HSV pixel array
	LABpixel ** labImage;	//Lab pixel array
	

	// Function Prototypes ----------------------------------------------------
	void allocate(int mode);
	void deallocate(int mode);
	// Memory allocation/deallocation functions (of type mode)



public:
	//-------------------------------------------------------------------------
	inline CImg(int mode = MODE_GRAYSCALE) : m_width(0), m_height(0), m_mode(mode) {}
	// Default constructor - creates an empty (0 by 0 pixel) image

	//-------------------------------------------------------------------------
	inline CImg(int height, int width, int mode = MODE_RGB)
		: m_width(width), m_height(height), m_mode(mode)
	{
		allocate(m_mode);
	}
	// Constructor - creates a blank image of specified size

	//-------------------------------------------------------------------------
	CImg(const CImg & image);
	// Copy constructor - duplicates an image matrix

	//-------------------------------------------------------------------------
	inline int getHeight() const { return m_height;}
	// getHeight() returns the height of the image, in pixels.

	//-------------------------------------------------------------------------
	inline int getWidth() const { return m_width;}
	// getWidth() returns the width of the image, in pixels.

	//-------------------------------------------------------------------------
	inline int getMode() const { return m_mode;}
	// getMode() returns the image mode of the image, either MODE_RGB or
	// MODE_GRAY

	//-------------------------------------------------------------------------
	void setPixel(int row, int col, const struct RGBpixel pixel);
	void setPixel(int row, int col, const int pixel);
	// setPixel() takes in an input RGBpixel struct and stores the RGB data
	// into the specified image location.  The alternate function is to set the
	// pixel with a grayscale value.  Does nothing if row/column is out of
	// range.

	//-------------------------------------------------------------------------
	void getPixel(int row, int col, struct RGBpixel & pixel) const;
	void getPixel(int row, int col, int & pixel) const;
	// getPixel() takes in an input RGBpixel struct and fills the structure
	// with the RGB data at the specified image location.  The alternate
	// function is to get the pixel's grayscale value, if the image is a
	// grayscale image. Does not modify the structure if the row/column is out
	// of range.

	//-------------------------------------------------------------------------
	void setMode(int mode);
	// setMode() changes the mode of the CImg object from MODE_RGB to
	// MODE_GRAYSCALE, and vice versa.  Going from MODE_RGB to MODE_GRAYSCALE
	// will automatically desaturate an image - all colour information will
	// be lost!

	//-------------------------------------------------------------------------
	CImg & operator=(const CImg & src);
	// operator overloaded equals: creates a duplicate of the image matrix

	//-------------------------------------------------------------------------
	bool read_JPEG_file(const char * filename);
	// JPEG file-reader.  Specify the filename of the JPEG image.  The
	// decompressed data will appear in the CImg object.  The function will
	// return true if the decompression was successful and false otherwise.
	//
	// The colour mode of the image will depend on the CImg.  If the CImg was
	// created using MODE_RGB, then the extraction of the image will be done
	// in RGB; the same goes with MODE_GRAYSCALE.

	//-------------------------------------------------------------------------
	bool write_JPEG_file(const char * filename, int quality);
	// JPEG file-writer.  Specify the filename of the JPEG image that should be
	// created and a pointer to the CImg object that contains RGB values.  A
	// quality factor is required - a number in the range of 0-100.  The
	// function will return true if the decompression was successful and false
	// otherwise.
	
	//-------------------------------------------------------------------------
	int read_BMP_file(const char * infile);
	// Reads a bitmap (.bmp) file, and places data into CImg object.  The
	// colour mode of the image will depend on the CImg.  If the CImg was
	// created using MODE_RGB, then the extraction of the image will be done
	// in RGB; the same goes with MODE_GRAYSCALE.
	//
	// The bitmap file must be an uncompressed 24-bit colour image; if not, the
	// function will return an error code READ_BMP_INCOMPAT_ERR.  If an error
	// occurs, the CImg object will contain invalid data.

	//-----------------------------------------------------------------------------
	int write_BMP_file(const char * outfile);
	// Writes a bitmap (.bmp) file, given the CImg object.  The bitmap will be
	// in 24-bit colour, which is the standard uncompressed format.

	~CImg();
	// Class destructor

	void constructHSV();

	double calculateL(double num);
	double calculateA(double num1,double num2);
	double calculateB(double num1,double num2);
	void constructLAB();
	void scale(int wratio,int hratio);


};

#endif
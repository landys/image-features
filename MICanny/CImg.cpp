// CImg.cpp
// Decompressed Image Data Object - contains extracted RGB values
// Written by Stephen Wong

/** 
 * This CImg class is a wrapper that contains decompressed RGB values of an
 * image.  With cjpeg.h and djpeg.h, this class can be used to store
 * decompressed values of a JPEG image, and its data can also be used to
 * create a new JPEG image.
 *
 * LIBJAVA Open Source Copyright (C) 1991-1998, Thomas G. Lane.
 * A member of the Independent JPEG Group.
 */

#include "CImg.h"


#define UNDEFINEDCOLOR -0.1

// TEMPORARY //////////////////////////////////////////////////////////////////
//#include <ctype.h>		// to declare isprint()
//#include <stdio.h>		// ANSI C input/output
//#include "jpeglib.h"
// END TEMPORARY //////////////////////////////////////////////////////////////

// Helper Functions =======================================================
inline int max3(int a, int b, int c) { return (a > b) ? ((a > c) ? a : c) : b; }
inline int min3(int a, int b, int c) { return (a < b) ? ((a < c) ? a : c) : b; }
// Integer maximum, minumum functions

//=========================================================================
CImg::CImg(const CImg & src)
: m_width(src.getWidth()), m_height(src.getHeight()), m_mode(src.getMode())
// Copy constructor - duplicates an image matrix
{
	// Allocate memory	
	allocate(m_mode);

	// Copy data over from image matrix, depending on which colourspace it is
	int i, j;
	switch (m_mode)
	{
		case MODE_RGB:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width; i++)
				{
					image[j][i] = src.image[j][i];
					hsvImage[j][i] = src.hsvImage[j][i];
					labImage[j][i] = src.labImage[j][i];
				}
			}
			break;
		case MODE_GRAYSCALE:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width; i++)
				{
					gImage[j][i] = src.gImage[j][i];
				}
			}
	}

}

//=========================================================================
void CImg::setPixel(int row, int col, const struct RGBpixel pixel)
// setPixel() takes in an input RGBpixel struct and stores the RGB data
// into the specified image location.  Does nothing if row/column is out of
// range.
{
	if ((row >= 0) && (col >= 0) && (row < m_height) && (col < m_width))
	{
		int max_lum, min_lum;	// Luminosity calculation values
		switch (m_mode)
		{
			case MODE_RGB:
				image[row][col] = pixel;
				break;
			case MODE_GRAYSCALE:
				// Calculate the grayscale equivalent of the RGBpixel
				max_lum = max3(pixel.red, pixel.green, pixel.blue);
				min_lum = min3(pixel.red, pixel.green, pixel.blue);
				gImage[row][col] = (int)((max_lum + min_lum) / 2);
				break;
		}
	}
}

//=========================================================================
void CImg::setPixel(int row, int col, const int pixel)
// setPixel() can take in an input greyscale pixel and stores the value
// into the specified image location.  Does nothing if row/column is out of
// range.
{
	if ((row >= 0) && (col >= 0) && (row < m_height) && (col < m_width))
	{
		switch (m_mode)
		{
			case MODE_RGB:
				image[row][col].red = pixel;
				image[row][col].green = pixel;
				image[row][col].blue = pixel;
				break;
			case MODE_GRAYSCALE:		
				gImage[row][col] = pixel;
				break;
		}
	}
}

//=========================================================================
void CImg::getPixel(int row, int col, struct RGBpixel & pixel) const
// getPixel() takes in an input RGBpixel struct and fills the structure
// with the RGB data at the specified image location.  Does not modify the
// structure if the row/column is out of range.
{
	if ((row >= 0) && (col >= 0) && (row < m_height) && (col < m_width))
	{
		switch (m_mode)
		{
			case MODE_RGB:
        		pixel = image[row][col];
				break;
			case MODE_GRAYSCALE:
				pixel.red = gImage[row][col];
				pixel.green = gImage[row][col];
				pixel.blue = gImage[row][col];
				break;
		}		
	}
}

//=========================================================================
void CImg::getPixel(int row, int col, int & pixel) const
// getPixel() can take retrieve greyscale pixel data.  Does not modify the
// structure if the row/column is out of range.
{
	if ((row >= 0) && (col >= 0) && (row < m_height) && (col < m_width))
	{
		int max_lum, min_lum;	// Luminosity calculation values
		switch (m_mode)
		{
			case MODE_RGB:
				// calculate the grayscale equivalent of the RGBpixel
				max_lum = max3(image[row][col].red, image[row][col].green, image[row][col].blue);
				min_lum = min3(image[row][col].red, image[row][col].green, image[row][col].blue);
				pixel = (int)((max_lum + min_lum) / 2);
				break;
			case MODE_GRAYSCALE:
				pixel = gImage[row][col];
				break;
		}
	}
}

//=========================================================================
void CImg::setMode(int mode)
// setMode() changes the mode of the CImg object from MODE_RGB t
// MODE_GRAYSCALE, and vice versa.
{
	int i, j;
	int max_lum, min_lum;	// Luminosity calculation values
	switch (m_mode)
	{
		case MODE_RGB:
			// From RGB to grayscale
			if (mode == MODE_GRAYSCALE)
			{
				if (!((m_height == 0) || (m_width == 0)))
				{
					// Allocate grayscale memory
					allocate(MODE_GRAYSCALE);
					for (j = 0; j < m_height; j++)
					{
						for (i = 0; i < m_width; i++)
						{
							// calculate the grayscale equivalent of the RGBpixel
							max_lum = max3(image[j][i].red, image[j][i].green, image[j][i].blue);
							min_lum = min3(image[j][i].red, image[j][i].green, image[j][i].blue);
							// set the grayscale pixel
							gImage[j][i] = (int)((max_lum + min_lum) / 2);
						}
					}
					// Deallocate RGB memory
					deallocate(MODE_RGB);
				}
				// Finally, change the mode of the image
				m_mode = MODE_GRAYSCALE;
			}
			//else
				//allocate(MODE_RGB);
			break;
		case MODE_GRAYSCALE:
			// From grayscale to RGB
			if (mode == MODE_RGB)
			{
				if (!((m_height == 0) || (m_width == 0)))
				{
					// Allocate RGB memory
					allocate(MODE_RGB);
					for (j = 0; j < m_height; j++)
					{
						for (i = 0; i < m_width; i++)
						{
							image[j][i].red = gImage[j][i];
							image[j][i].green = gImage[j][i];
							image[j][i].blue = gImage[j][i];
						}
					}
					// Deallocate grayscale memory
					deallocate(MODE_GRAYSCALE);
				}
				// Finally, change the mode of the image
				m_mode = MODE_RGB;
			}
			break;
	}
}

//=========================================================================
CImg & CImg::operator=(const CImg & src)
// operator overloaded equals: creates a duplicate of the image matrix
{
	// don't do anything if self-assignment
	if (this == &src) return *this;

	// if image sizes are not identical then resize as necessary, or image
	// mode is different
	if (src.getHeight() != m_height || src.getWidth() != m_width || m_mode != src.getMode())
	{
		// delete old memory if present (usually is)
		if (m_width != 0 && m_height != 0) deallocate(m_mode);
		// obtain new height, source, mode
		m_height = src.getHeight();
		m_width = src.getWidth();
		m_mode = src.getMode();
		// allocate new memory
		allocate(m_mode);
	}
		
	// Copy data over from image matrix, depending on which colourspace it is
	int i, j;
	switch (m_mode)
	{
		case MODE_RGB:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width; i++)
				{
					image[j][i] = src.image[j][i];
					hsvImage[j][i] = src.hsvImage[j][i];
					labImage[j][i] = src.labImage[j][i];
				}
			}
			break;
		case MODE_GRAYSCALE:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width; i++)
				{
					gImage[j][i] = src.gImage[j][i];
				}
			}
	}

	// Return this pointer when complete assignment
	return *this;
}

//=========================================================================
CImg::~CImg()
// Class destructor - deletes allocated array space 
{
	deallocate(m_mode);
}

//=========================================================================
void CImg::allocate(int mode)
// private function that allocates memory for image matrices
{
	// If height or width is zero, no memory allocation is made (and image
	// is nothing!
	if ((m_height == 0) || (m_width == 0)) return;

	// allocate space for Img struct
	int i;
	switch (mode)
	{
	case MODE_RGB:
		image = new RGBpixel* [m_height];
		image[0] = new RGBpixel[m_width * m_height];
		hsvImage = new HSVpixel*[m_height];
		hsvImage[0] = new HSVpixel[m_width * m_height];
		labImage = new LABpixel*[m_height];
		labImage[0] = new LABpixel[m_width * m_height];

		for (i = 0; i < m_height; i++)
		{
			image[i] = image[0] + m_width * i;
			hsvImage[i] = hsvImage[0] + m_width * i;
			labImage[i] = labImage[0] + m_width*i;
		}

		break;
	case MODE_GRAYSCALE:
		gImage = new int* [m_height];
		gImage[0] = new int[m_width * m_height];
		for (i = 0; i < m_height; i++) gImage[i] = gImage[0] + m_width * i;
		break;
	}
}

//=========================================================================
void CImg::deallocate(int mode)
// private function that deallocates memory for image matrices
{
	// If height or width is zero, no memory allocation is made (and image
	// is nothing!
	if ((m_height == 0) || (m_width == 0)) return;

	// Delete allocated memory upon request
	switch (mode)
	{
		case MODE_RGB:
            delete[] image[0], image;
			delete[] hsvImage[0], hsvImage;
			delete[] labImage[0],labImage;
			break;
		case MODE_GRAYSCALE:
			delete[] gImage[0], gImage;
			break;
	}
	/*m_height = 0;
	m_width = 0;*/
}

//=============================================================================
bool CImg::read_JPEG_file(const char * filename)
// JPEG decompression routine.
{
	struct jpeg_decompress_struct cinfo;	// JPEG image parameters
	struct jpeg_error_mgr jerr;			// default JPEG error handler
	FILE * infile;						// source file handler
	unsigned char ** buffer;			// points to large array of RGB data
	int row_stride;						// physical row width in output buffer

	// open input file
	infile = fopen(filename, "rb");
	if (infile == NULL) return false;

	// Set up the normal JPEG error routines
	cinfo.err = jpeg_std_error(&jerr);

	// Initialize the JPEG decompression object with a data source (file).
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);

	// Read file parameters with jpeg_read_header()
	jpeg_read_header(&cinfo, TRUE);

	// Set parameters for decompression, if necessary (not necessary
	// here - the defaults set by jpeg_read_header() is sufficient.
	switch (m_mode)
	{
		case MODE_RGB:
            cinfo.out_color_space = JCS_RGB;
			break;
		case MODE_GRAYSCALE:
			cinfo.out_color_space = JCS_GRAYSCALE;
			break;
	}

	// Start decompressor
	jpeg_start_decompress(&cinfo);

	// Decompression complete, so now begin extraction of data
	// row_stride = output pixels per row * number of colours per row
	row_stride = cinfo.output_width * cinfo.output_components;

	// Create data buffer
	buffer = new unsigned char * [cinfo.output_height];
	buffer[0] = new unsigned char[cinfo.output_height * row_stride];
	for (int k = 0; k < (int)(cinfo.output_height); k++)
	{
		buffer[k] = buffer[0] + row_stride*k;
	}

	// The JPEG decompress parameters (cinfo) contains a counter, the output
	// row counter.  Keep extracting data until the output row is at the end.
	int counter = 0;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		counter += jpeg_read_scanlines(&cinfo, &buffer[counter], cinfo.output_height);
	}

	// initialize this CImg object
	deallocate(m_mode);
	m_width = cinfo.output_width;
	m_height = cinfo.output_height;
	allocate(m_mode);

	// Convert buffer data to Img struct
	int i, j;
	switch (m_mode)
	{
		case MODE_RGB:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width*3; i += 3)
				{
					image[j][i/3].red = buffer[j][i];
					image[j][i/3].green = buffer[j][i+1];
					image[j][i/3].blue = buffer[j][i+2];
				}
			}
			break;
		case MODE_GRAYSCALE:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width; i++)
				{
					gImage[j][i] = buffer[j][i];
				}
			}
			break;
	}

	// Destroy buffer
	delete [] buffer[0];
	delete [] buffer;

	// Finish decompression and release JPEG compression object
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	// Close file in use
	fclose(infile);
	return true;
}
//=============================================================================
bool CImg::write_JPEG_file(const char * filename, int quality)
// JPEG compression routine.
{
	struct jpeg_compress_struct cinfo;	// JPEG image parameters
	struct jpeg_error_mgr jerr;			// default JPEG error handler
	FILE * outfile;						// target file handler
	unsigned char ** buffer;			// points to large array of RGB data
	int row_stride;						// physical row width in image buffer
	
	// create/open output file
	outfile = fopen(filename, "wb");
	if (outfile == NULL) return false;

	// Set up the normal JPEG error routines
	cinfo.err = jpeg_std_error(&jerr);
	
	// Initialize the JPEG compression object
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	// Set parameters for compression
	cinfo.image_width = m_width; 		// set image width
	cinfo.image_height = m_height;		// set image height
	cinfo.input_components = m_mode;	// number of compents per pixel
	// set colour space
	switch (m_mode)
	{
		case MODE_RGB:
			cinfo.in_color_space = JCS_RGB;
			break;
		case MODE_GRAYSCALE:
			cinfo.in_color_space = JCS_GRAYSCALE;
			break;
	}
	jpeg_set_defaults(&cinfo);					// set rest of values default
	jpeg_set_quality(&cinfo, quality, TRUE);	// set image quality

	// Start compressor
	jpeg_start_compress(&cinfo, TRUE);

	// Now, write to scan lines as the JPEG is being created
	// JSAMPLEs per row in image_buffer
	row_stride = m_width * cinfo.input_components;
	
	// Create data buffer
	buffer = new unsigned char * [cinfo.image_height];
	buffer[0] = new unsigned char[cinfo.image_height * row_stride];
	for (int k = 0; k < (int)(cinfo.image_height); k++) {
		buffer[k] = buffer[0] + row_stride*k;
	}

	// Load input buffer with data
	int i, j;
	switch (m_mode)
	{
		case MODE_RGB:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width*3; i += 3)
				{
					buffer[j][i] = image[j][i/3].red;
					buffer[j][i+1] = image[j][i/3].green;
					buffer[j][i+2] = image[j][i/3].blue;
				}
			}
			break;
		case MODE_GRAYSCALE:
			for (j = 0; j < m_height; j++)
			{
				for (i = 0; i < m_width; i++)
				{
					buffer[j][i] = gImage[j][i];
				}
			}
			break;
	}

	while (cinfo.next_scanline < cinfo.image_height)
    // jpeg_write_scanlines expects an array of pointers to scanlines.
    // Here the array is only one element long, but you could pass
    // more than one scanline at a time if that's more convenient.
	{
	    (void) jpeg_write_scanlines(&cinfo, &buffer[cinfo.next_scanline], 1);
	}

	// Complete compression and close output file
	jpeg_finish_compress(&cinfo);
	fclose(outfile);

	// Delete image buffer
	delete [] buffer[0];
	delete [] buffer;

	// Destroy compression object
	jpeg_destroy_compress(&cinfo);
	return true;
}

//=============================================================================
int CImg::read_BMP_file(const char * infile)
// Reads a bitmap (.bmp) file, and places data into CImg object.  The colour
// mode of the image will depend on the CImg.  If the CImg was created using
// MODE_RGB, then the extraction of the image will be done in RGB; the same
// goes with MODE_GRAYSCALE.
//
// The bitmap file must be an uncompressed 24-bit colour image; if not, the
// function will return an error code READ_BMP_INCOMPAT_ERR.  If an error
// occurs, the CImg object will contain invalid data.
{
	FILE * fp;						// File pointer (will use fopen() below)
	BITMAPFILEHEADER bmFileHdr;		// Bitmap file header
	BITMAPINFO bmInfo;				// Bitmap file info
	BYTE buffer;					// Buffer byte when scanning
	RGBpixel pbuf;					// Buffer pixel when scanning
	int EndByte;					// End byte marker (for junk bytes)
	bool bEof_Unexp = false;		// Unexpected EOF marker found

	// Open file to read (rb = read in binary mode)
	fp = fopen(infile, "rb");
	if (fp == NULL) return READ_BMP_FILENOTFOUND_ERR;

	// Read BITMAPFILEHEADER, the bitmap's file header
	// Note: fread(void * buffer, size_t buf_size, size_t count, FILE * fs)
	if (!feof(fp))
		fread((char *)&bmFileHdr, sizeof(BITMAPFILEHEADER), 1, fp);
	else
	{
		fclose(fp);
		return READ_BMP_EOF_ERR;
	}

	// Read BITMAPINFOHEADER and RGBQUAD separately or together by reading
	// them into the BITMAPINFO structure. When RGBQUAD is not empty (bitmap
	// is 16 colors or 256 colors), you must read these two structures
	// separately.
	if (!feof(fp))
		fread((char *)&bmInfo, sizeof(BITMAPINFO), 1, fp);
	else
	{
		fclose(fp);
		return READ_BMP_EOF_ERR;
	}

	// Check the bitmap info.  If it is not 24-bit, and uncompressed, and the
	// colors used and are important is not 0, return READ_BMP_INCOMPAT_ERR.
	// (last two terms refer to compressed bitmaps)
	if (bmInfo.bmiHeader.biBitCount != 24 || bmInfo.bmiHeader.biCompression != 0 ||
		(bmInfo.bmiHeader.biClrUsed != 0 &&	bmInfo.bmiHeader.biClrImportant != 0))
	{
		fclose(fp);
		return READ_BMP_INCOMPAT_ERR;
	}

	// initialize this CImg object
	deallocate(m_mode);
	m_width = bmInfo.bmiHeader.biWidth;
	m_height = bmInfo.bmiHeader.biHeight;
	allocate(m_mode);

	// Set new file pointer position at 54 from the beginning of the file
	// (Recall that 0 to 53 are used to store BITMAPFILEINFO and
	// BITMAPINFOHEADER)
	if (fseek(fp, bmFileHdr.bfOffBits, SEEK_SET) != 0)
	{
		fclose(fp);
		return READ_BMP_FSEEK_ERR;
	}

	// Do mathematical calculation for 'junk' bytes
	LONG junkBytes = bmInfo.bmiHeader.biSizeImage - m_width * m_height * 3;
	EndByte = junkBytes / m_height; 
	
	// Read the pixels, as long as we don't reach eof (end of file)
	for (int j = m_height - 1; j >= 0; j--)
	{
		// Read scanline
		for (int i = 0; i < m_width; i++)
		{
			// Process blue byte (blue byte is first because image is stored
			// in reverse order)
			if (!feof(fp))
			{
				fread((BYTE *)&buffer, sizeof(BYTE), 1, fp);
				pbuf.blue = (int)buffer;
			}
			else
			{
				// unexpected termination
				fclose(fp);
				return READ_BMP_EOF_ERR;
			}
			
			// Read green byte
			if (!feof(fp))
			{
				fread((BYTE *)&buffer, sizeof(BYTE), 1, fp);
				pbuf.green = (int)buffer;
			}
			else
			{
				// unexpected termination
				fclose(fp);
				return READ_BMP_EOF_ERR;
			}

			// Read red byte
			if (!feof(fp))
			{
				fread((BYTE *)&buffer, sizeof(BYTE), 1, fp);
				pbuf.red = (int)buffer;
			}
			else
			{
				// unexpected termination
				fclose(fp);
				return READ_BMP_EOF_ERR;
			}

			// Set the image pixel
			this->setPixel(j, i, pbuf);
		}

		// When the for loop is done, one scan line is completed.  This is
		// where you skip the junk bytes.
		if (fseek(fp, EndByte, SEEK_CUR) != 0)
		{
			fclose(fp);
			return READ_BMP_FSEEK_ERR;
		}
	}

	// Close file when complete write operation
	fclose(fp);
	// Return READ_BMP_SUCCESS on success
	return READ_BMP_SUCCESS;
}

//=============================================================================
int CImg::write_BMP_file(const char * outfile)
// Writes a bitmap (.bmp) file, given the CImg object.  The bitmap will be
// in 24-bit colour, which is the standard uncompressed format.
{
	FILE * fp;						// File pointer (will use fopen() below)
	size_t result;					// File i/o result checker
	int rowEndBytes = (4-(m_width*3)%4)%4;	// number of junk bytes to add to end of
									// each row

	// Initialize Bitmap file header
	BITMAPFILEHEADER bmFileHdr;			// Create a bitmap file header
	bmFileHdr.bfType		= 0x4d42;	// File type "BM" = bitmap
	bmFileHdr.bfSize		= 58 + (m_width*3 + rowEndBytes) * m_height;
	bmFileHdr.bfReserved1	= 0;
	bmFileHdr.bfReserved2	= 0;
	bmFileHdr.bfOffBits		= 58;

	// Initialize Bitmap info header
	// Note: Usually a BITMAPINFO struct (containing -both- BITMAPINFOHEADER
	// and RGBQUAD) is used; however, with no need for RGBQUAD, this struct
	// will do.
	BITMAPINFOHEADER bmInfo;				// Create a bitmap file header
	bmInfo.biSize			= 40;
	bmInfo.biWidth			= m_width;
	bmInfo.biHeight			= m_height;
	bmInfo.biPlanes			= 1;
	bmInfo.biBitCount		= 24;
	bmInfo.biCompression	= BI_RGB;	// No compression
	bmInfo.biSizeImage		= (m_width*3 + rowEndBytes) * m_height;
	bmInfo.biXPelsPerMeter	= 0;
	bmInfo.biYPelsPerMeter	= 0;
	bmInfo.biClrUsed		= 0;
	bmInfo.biClrImportant	= 0;

	// Open file to write (wb = read in binary mode).  If a file exists, the
	// file will be overwritten
	fp = fopen(outfile, "wb");
	if (fp == NULL) return WRITE_BMP_FILENOTFOUND_ERR;

	// Write BITMAPFILEHEADER, the bitmap's file header
	// Note: size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream)
	result = fwrite((char *)&bmFileHdr, sizeof(BITMAPFILEHEADER), 1, fp);
	if (result == 0)
	{
		fclose(fp);
		return WRITE_BMP_WRITE_ERR;
	}

	// Write BITMAPINFO, the bitmap's info header
	result = fwrite((char *)&bmInfo, sizeof(BITMAPINFO), 1, fp);
	if (result == 0)
	{
		fclose(fp);
		return WRITE_BMP_WRITE_ERR;
	}

	// Write image after header contents complete
	RGBpixel pixel;
	BYTE pJunk = 0;
	for (int j = m_height - 1; j >= 0; j--)
	{
		for (int i = 0; i < m_width; i++)
		{
			// Get pixel data
			this->getPixel(j, i, pixel);
			// Write blue, green, red bytes
			result = fwrite((BYTE *)&pixel.blue, sizeof(BYTE), 1, fp);
			if (result == 0)
			{
				// Write error
				fclose(fp);
				return WRITE_BMP_WRITE_ERR;
			}
			result = fwrite((BYTE *)&pixel.green, sizeof(BYTE), 1, fp);
			if (result == 0)
			{
				// Write error
				fclose(fp);
				return WRITE_BMP_WRITE_ERR;
			}
			result = fwrite((BYTE *)&pixel.red, sizeof(BYTE), 1, fp);
			if (result == 0)
			{
				// Write error
				fclose(fp);
				return WRITE_BMP_WRITE_ERR;
			}
		}
		// Append extra 0s to make row length a multiple of 4 (.bmp file
		// requirement)
		if (rowEndBytes != 0) 
			result = fwrite((BYTE *)&pJunk, sizeof(BYTE), rowEndBytes, fp);
			if (result == 0)
			{
				// Write error
				fclose(fp);
				return WRITE_BMP_WRITE_ERR;
			}
	}

	// Close file when complete write operation
	fclose(fp);
	// Return true on success
	return WRITE_BMP_SUCCESS;
}

void CImg::constructHSV()
{
	// r,g,b values are from 0 to 1
	// h = [0,1], s = [0,1], v = [0,1]
	// if s == 0, then h = -1 (undefined)
	//setMode(MODE_RGB);
	double min, max, delta,tmp;
	double R,G,B,V,S,H;
	for(int i = 0; i < getHeight(); i++)
		for(int j = 0; j < getWidth(); j++)
		{
			R = (double)image[i][j].red/(double)255;
			G = (double)image[i][j].green/(double)255;
			B = (double)image[i][j].blue/(double)255;
			tmp = min(R, G);
			min = min( tmp, B );
			tmp = max( R, G);
			max = max(tmp, B );
			hsvImage[i][j].value = V = max; // v
			delta = max - min;

			if( max != 0.0 )
				hsvImage[i][j].sat = S = delta / max; // s
			else
			{
				// r = g = b = 0 // s = 0, v is undefined
				hsvImage[i][j].sat = S = 0;
				hsvImage[i][j].hue = H = UNDEFINEDCOLOR;
				continue;
			}

			if(S == 0.0)H = UNDEFINEDCOLOR;
			else {
				if( R == max )
					H = ( G - B ) / delta; // between yellow & magenta
				else if( G == max )
					H = 2 + ( B - R ) / delta; // between cyan & yellow
				else
					H = 4 + ( R - G ) / delta; // between magenta & cyan

				H *= 60; // degrees
				if( H < 0 )
					H += 360;
			}
			hsvImage[i][j].hue = H/360;


		}
}

double CImg::calculateL(double num)
{
	double temp = 0.33333333333,result=0;
	if(num > 0.008856)result = 116*pow((num),temp)-16;
	else result = 903.3*num;
	return result;
}

double CImg::calculateA(double num1,double num2)
{
	double temp = 0.33333333333,result=0;
	if(num1 > 0.008856)num1 = pow((num1),temp);
	else num1 = 7.787*num1+16/116;
	if(num2 > 0.008856)num2 = pow((num2),temp);
	else num2 = 7.787*num2+16/116;	
		result = 500*(num1-num2);
	return result;
}
double CImg::calculateB(double num1,double num2)
{
	double temp = 0.33333333333,result=0;
	if(num1 > 0.008856)num1 = pow((num1),temp);
	else num1 = 7.787*num1+16/116;
	if(num2 > 0.008856)num2 = pow((num2),temp);
	else num2 = 7.787*num2+16/116;	
		result = 200*(num1-num2);
		return result;
}

void CImg::constructLAB()
{
	setMode(MODE_RGB);
	double r,g,b,x,y,z,wx,wy,wz;
	wx = 0.4124*255+0.3575*255+0.1804*255;
	wy = 0.2128*255+0.7152*255+0.0722*255;
	wz = 0.0193*255+0.1192*255+0.9502*255;
	for(int i = 0; i < getHeight(); i++)
		for(int j = 0; j < getWidth(); j++)
		{
			r = image[i][j].red;
			g = image[i][j].green;
			b = image[i][j].blue;
			x = 0.4124*r+0.3575*g+0.1804*b;
			y = 0.2128*r+0.7152*g+0.0722*b;
			z = 0.0193*r+0.1192*g+0.9502*b;		
			x = x/wx;
			y = y/wy;
			z = z/wz;
			labImage[i][j].L = calculateL(y);
			labImage[i][j].A = calculateA(x,y);
			labImage[i][j].B = calculateB(y,z);
		}

}

void CImg::scale(int wratio,int hratio)
{
	CImg simage(*this);
	RGBpixel pixel,rpixel,dpixel,upixel,lpixel,tempixel;
	int width,height,swidth,sheight;
	width = getWidth();
	height = getHeight();
	swidth = width*wratio;
	sheight = height*hratio;
    this->deallocate(m_mode);
	m_height = sheight;
	m_width = swidth;
	m_mode = simage.m_mode;
	this->setMode(m_mode);
	//this->allocate(m_mode);
	for(int h=1;h<height-1;h++)
		for(int w=1;w<width-1;w++)
		{
			simage.getPixel(h,w,pixel);
			simage.getPixel(h,w+1,rpixel);
			simage.getPixel(h,w-1,lpixel);
			simage.getPixel(h+1,w,dpixel);
			simage.getPixel(h-1,w,upixel);

			setPixel(2*h,2*w,pixel);
			tempixel.red = (pixel.red+rpixel.red+upixel.red)/3;
			tempixel.green = (pixel.green + rpixel.green+upixel.green)/3;
			tempixel.blue = (pixel.blue + rpixel.blue+upixel.blue)/3;
			setPixel(2*h,2*w+1,tempixel);
			tempixel.red = (pixel.red+dpixel.red+lpixel.red)/3;
			tempixel.green = (pixel.green + dpixel.green+lpixel.green)/3;
			tempixel.blue = (pixel.blue + dpixel.blue+lpixel.blue)/3;
			setPixel(2*h+1,2*w,tempixel);
			tempixel.red = (pixel.red+dpixel.red+rpixel.red)/3;
			tempixel.green = (pixel.green + dpixel.green + rpixel.green)/3;
			tempixel.blue = (pixel.blue + dpixel.blue + rpixel.blue)/3;
			setPixel(2*h+1,2*w+1,tempixel);
		}

		for(int h=0;h<height;h++)
		{
			simage.getPixel(h,width-1,pixel);
			setPixel(h*2,(width-1)*2,pixel);
			setPixel(h*2+1,(width-1)*2,pixel);
			setPixel(h*2,width*2-1,pixel);
			setPixel(h*2+1,width*2-1,pixel);
			simage.getPixel(h,0,pixel);
			setPixel(h*2,0,pixel);
			setPixel(h*2,1,pixel);
			setPixel(h*2+1,0,pixel);
			setPixel(h*2+1,1,pixel);
		}
		for(int w=0;w<width;w++)
		{
			simage.getPixel(height-1,w,pixel);
			setPixel((height-1)*2,w*2,pixel);
			setPixel(height*2-1,w*2,pixel);
			setPixel((height-1)*2,w*2+1,pixel);
			setPixel(height*2-1,w*2+1,pixel);
			simage.getPixel(0,w,pixel);
			setPixel(0,w*2,pixel);
			setPixel(0,w*2+1,pixel);
			setPixel(1,w*2,pixel);
			setPixel(1,w*2+1,pixel);
		}

	simage.deallocate(simage.m_mode);

}



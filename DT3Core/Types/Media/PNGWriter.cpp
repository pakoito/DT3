//==============================================================================
///	
///	File: PNGWriter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Media/PNGWriter.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"

#include "png.h"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTerr PNGWriter::write (const FilePath &pathname, const std::shared_ptr<TextureResource2D> &tex)
{
    ASSERT(tex->format() == DT3GL_FORMAT_RGBA);

	//
	// From the libpng source code
	//

	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;

	// open the file
	fp = ::fopen(pathname.full_path().c_str(), "wb");
	if (fp == NULL)
		return false;

	// Create and initialize the png_struct with the desired error handler
	// functions.  If you want to use the default stderr and longjump method,
	// you can supply NULL for the last three parameters.  We also check that
	// the library version is compatible with the one used at compile time,
	// in case we are using dynamically linked libraries.  REQUIRED.
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		::fclose(fp);
		return false;
	}

	// allocate/initialize the image information data. 
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		::fclose(fp);
		::png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}

	// set error handling.  REQUIRED if you aren't supplying your own
	// error handling functions in the png_create_write_struct() call.
	if (setjmp ((*png_set_longjmp_fn((png_ptr), (png_longjmp_ptr)longjmp, sizeof(jmp_buf))))) {
		// If we get here, we had a problem reading the file
		::fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}

	// set up the output control if you are using standard C streams
	::png_init_io(png_ptr, fp);

	// set the image information here.  width and height are up to 2^31,
	// bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	// the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	// PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	// or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	// PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	// currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE.
	png_set_IHDR(png_ptr, info_ptr, tex->width(), tex->height(), 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// We are dealing with a color image then
	png_color_8 sig_bit;
	sig_bit.red = 8;
	sig_bit.green = 8;
	sig_bit.blue = 8;
	sig_bit.alpha = 8;
	png_set_sBIT(png_ptr, info_ptr, &sig_bit);


	// Optional gamma chunk is strongly suggested if you have any guess
	// as to the correct gamma of the image.
	//
	png_set_gAMA(png_ptr, info_ptr, 1.8F);

	// write the file header information.
	png_write_info(png_ptr, info_ptr);

	// Shift the pixels up to a legal bit depth and fill in
	// as appropriate to correctly scale the image.
	png_set_shift(png_ptr, &sig_bit);

	// pack pixels into bytes
	png_set_packing(png_ptr);

	// swap location of alpha bytes from ARGB to RGBA
	//png_set_swap_alpha(png_ptr);

	// get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
	// RGB (4 channels -> 3 channels). The second parameter is not used.
	//png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

	// flip BGR pixels to RGB 
	//png_set_bgr(png_ptr);

	// swap bytes of 16-bit files to most significant byte first
	//png_set_swap(png_ptr);

	// swap bits of 1, 2, 4 bit packed pixel formats
	//png_set_packswap(png_ptr);

	// The easiest way to write the image (you may have a different memory
	// layout, however, so choose what fits your needs best).  You need to
	// use the first method if you aren't handling interlacing yourself.
	
	std::vector<png_bytep> row_pointers;
	row_pointers.resize( tex->height() );

    Color4b *buffer = (Color4b*) tex->buffer();
    
	for (DTint k = 0; k < tex->height(); k++)
		row_pointers[k] = (png_bytep) &(buffer[tex->width() * (tex->height() - k - 1)]);

	//write out the entire image data in one call
	png_write_image(png_ptr, &row_pointers[0]);

	// You can write optional chunks like tEXt, zTXt, and tIME at the end
	// as well.  Shouldn't be necessary in 1.1.0 and up as all the public
	// chunks are supported and you can use png_set_unknown_chunks() to
	// register unknown chunks into the info structure to be written out.

	// It is REQUIRED to call this to finish writing the rest of the file
	png_write_end(png_ptr, info_ptr);

	// Similarly, if you png_malloced any data that you passed in with
	// png_set_something(), such as a hist or trans array, free it here,
	// when you can be sure that libpng is through with it.
	png_free(png_ptr, NULL);

	// clean up after the write, and free any memory allocated
	png_destroy_write_struct(&png_ptr, &info_ptr);

	// close the file
	fclose(fp);

	return true;

}

//==============================================================================
//==============================================================================

} // DT3

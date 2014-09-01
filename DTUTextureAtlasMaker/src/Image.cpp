//==============================================================================
///	
///	File: Image.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include	"Image.hpp"
#include	<fstream>

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Image::Image (void)
	:	_data				(NULL)
{  

}
		
Image::Image (const Image &rhs)
    :	_data				(rhs._data)
{   

}

Image & Image::operator = (const Image &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		_data = rhs._data;
	}
    return (*this);
}
			
Image::~Image (void)
{
	delete _data;
}

//==============================================================================
//==============================================================================

void png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	std::ifstream *read_ptr = reinterpret_cast<std::ifstream*>(png_get_io_ptr(png_ptr));
	read_ptr->read( (char*) data, length);
}

void png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	std::ofstream *write_ptr = reinterpret_cast<std::ofstream*>(png_get_io_ptr(png_ptr));
	write_ptr->write( (char*) data, length);
}

void png_flush_data(png_structp png_ptr)
{
	std::ofstream *write_ptr = reinterpret_cast<std::ofstream*>(png_get_io_ptr(png_ptr));
	write_ptr->flush();
}

//==============================================================================
//==============================================================================

bool Image::load (const std::string &s)
{	
	//
	// The following is based on the libpng manual. http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-3.1
	//

	//
	// Check the file header
	//

	// Open the file
	std::ifstream infile;
	infile.open(s.c_str(), std::ifstream::in | std::ifstream::binary);

	// Read the header
	const unsigned int NUM_HEADER_BYTES_TO_READ = 8;
	char header[NUM_HEADER_BYTES_TO_READ];
	
	infile.read(header, NUM_HEADER_BYTES_TO_READ);
    if (png_sig_cmp((png_bytep) header, 0, NUM_HEADER_BYTES_TO_READ)) {
        return false;
	}
	
	//
	// Read the image data
	//
	
	png_bytep *row_pointers = NULL;

	// Create and initialize the png_struct with the desired error handler
	// functions.  If you want to use the default stderr and longjump method,
	// you can supply NULL for the last three parameters.  We also supply the
	// the compiler header file version, so that we know if the application
	// was compiled with a compatible version of the library.  REQUIRED

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return false;
	}
	
	// Allocate/initialize the memory for image information
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return false;
    }

	// Set error handling if you are using the setjmp/longjmp method (this is
	// the normal method of doing things with libpng).  REQUIRED unless you
	// set up your own error handlers in the png_create_read_struct() earlier.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		delete row_pointers;	// Gets allocated after here, but don't forget the jump!
        return false;
	}

	// Set custom read routine
	png_set_read_fn(png_ptr, &infile, &png_read_data);
   
	// If we have already read some of the signature
	png_set_sig_bytes(png_ptr, NUM_HEADER_BYTES_TO_READ);
   
	//
	// Read the entire image
	//
	
	// Read PNG header info
	png_read_info(png_ptr, info_ptr);

	int bit_depth, color_type, interlace_type;
	
	// Header info
	png_uint_32 width,height;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	
	// Strip 16 bit down to 8 bit
	png_set_strip_16(png_ptr);
		
	// Extract components to even bytes
	png_set_packing(png_ptr);

	// Expand grayscale images to 8 bit
	png_set_expand(png_ptr);
	
	// Add a filler byte when missing
	png_set_filler(png_ptr,0xFFFFFFFF,PNG_FILLER_AFTER);

	// Expand grayscale images to rgb
	png_set_gray_to_rgb(png_ptr);
	
	// Update the transformation info within libpng
	png_read_update_info(png_ptr, info_ptr);
		
	// Header info again
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	
	// Allocate target image
	allocate(width, height);

	// Handle Gamma
	png_set_gamma(png_ptr, 0.0, 0.0);
	
	// Setup row pointers
	row_pointers = new png_bytep[_data_height];
	if (!row_pointers) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        return false;
	}
	
	for (unsigned int y = 0; y < _data_height; ++y) {
		row_pointers[y] = reinterpret_cast<png_byte*>( &getPixel(0,y) );
	}	
	
	// Read the entire image
	png_read_image(png_ptr, row_pointers);
	
	//
	// Clean up
	//
	
	delete row_pointers;
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	
	return true;
}

bool Image::save (const std::string &s)
{
	// Open the file
	std::ofstream outfile;
	outfile.open(s.c_str(), std::ifstream::out | std::ifstream::binary);

	png_structp png_ptr;
	png_infop info_ptr;

	// Create and initialize the png_struct with the desired error handler
	// functions.  If you want to use the default stderr and longjump method,
	// you can supply NULL for the last three parameters.  We also check that
	// the library version is compatible with the one used at compile time,
	// in case we are using dynamically linked libraries.  REQUIRED.
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		return false;
	}

	// Allocate/initialize the image information data. 
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		::png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return false;
	}

	// Set error handling.  REQUIRED if you aren't supplying your own
	// error handling functions in the png_create_write_struct() call.
	if (setjmp(png_jmpbuf(png_ptr))) {
		// If we get here, we had a problem reading the file
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}

	// Set custom write routine
	png_set_write_fn(png_ptr, &outfile, &png_write_data, &png_flush_data);

	// Set the image information here.  Width and height are up to 2^31,
    // bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
    // the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
    // PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
    // or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
    // PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
    // currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	png_set_IHDR(png_ptr, info_ptr, _window_width, _window_height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

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
	png_set_gAMA(png_ptr, info_ptr, 0.0);

	// Write the file header information.
	png_write_info(png_ptr, info_ptr);

	// Shift the pixels up to a legal bit depth and fill in
	// as appropriate to correctly scale the image.
	png_set_shift(png_ptr, &sig_bit);

	// pack pixels into bytes
	png_set_packing(png_ptr);

	// swap location of alpha bytes from ARGB to RGBA
	//png_set_swap_alpha(png_ptr);

	// Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
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

	// Setup row pointers
	png_bytep *row_pointers = new png_bytep[_window_height];

	for (png_uint_32 k = 0; k < _window_height; k++)
		row_pointers[k] = (png_bytep) &getPixel(0,k);

	png_write_image(png_ptr, row_pointers);
	
	delete row_pointers;

	// It is REQUIRED to call this to finish writing the rest of the file
	png_write_end(png_ptr, info_ptr);

	// Similarly, if you png_malloced any data that you passed in with
	// png_set_something(), such as a hist or trans array, free it here,
	// when you can be sure that libpng is through with it.
	png_free(png_ptr, NULL);

	// clean up after the write, and free any memory allocated
	png_destroy_write_struct(&png_ptr, &info_ptr);

	return false;
}

void Image::allocate (int width, int height)
{
	delete _data;
	_data = new Pixel[width * height];
	_data_width = width;
	_data_height = height;

	// Reset the window
	_window_width = _data_width;
	_window_height = _data_height;
	_window_x = 0;
	_window_y = 0;
	
	_orig_width = _data_width;
	_orig_height = _data_height;
	_orig_x = 0;
	_orig_y = 0;
}

//==============================================================================
//==============================================================================

void Image::blackOutTransparency		(void)
{
	for (int y = 0; y < _window_height; ++y) {
		for (int x = 0; x < _window_width; ++x) {
			Pixel p = getPixel(x,y);
			
			if (p.a == 0) {
				p.r = 0;
				p.g = 0;
				p.b = 0;
			}
		
			setPixel(x, y, p);
		}
	}
}

//==============================================================================
//==============================================================================

void Image::trim		(void)
{
	int c;

	// Left
	while (_window_width > 0) {
		for (c = 0; c < _window_height; ++c) {
			if (getPixel(0,c).a != 0)
				break;
		}

		// Do trim
		if (c == _window_height) {
			_window_x += 1;
			_window_width -= 1;
		} else {
			break;
		}
	}

	// Right
	while (_window_width > 0) {
		for (c = 0; c < _window_height; ++c) {
			if (getPixel(_window_width-1,c).a != 0)
				break;
		}

		// Do trim
		if (c == _window_height) {
			_window_width -= 1;
		} else {
			break;
		}
	}

	// Top
	while (_window_height > 0) {
		for (c = 0; c < _window_width; ++c) {
			if (getPixel(c,0).a != 0)
				break;
		}

		// Do trim
		if (c == _window_width) {
			_window_y += 1;
			_window_height -= 1;
		} else {
			break;
		}
	}

	// Bottom
	while (_window_height > 0) {
		for (c = 0; c < _window_width; ++c) {
			if (getPixel(c,_window_height-1).a != 0)
				break;
		}

		// Do trim
		if (c == _window_width) {
			_window_height -= 1;
		} else {
			break;
		}
	}

}

void Image::pad (int p)
{
	Image new_image;
	new_image.allocate(_window_width + p*2, _window_height + p*2);

	for (int y = 0; y < _window_height; ++y) {
		for (int x = 0; x < _window_width; ++x) {
			new_image.setPixel(x+p,y+p, getPixel(x,y));
		}
	}

	_orig_x -= _window_x - p;
	_orig_y -= _window_y - p;

	// Swap images
	std::swap(_data_width, new_image._data_width);
	std::swap(_data_height, new_image._data_height);
	std::swap(_window_x, new_image._window_x);
	std::swap(_window_y, new_image._window_y);
	std::swap(_window_width, new_image._window_width);
	std::swap(_window_height, new_image._window_height);
	std::swap(_data, new_image._data);
    
    // Stretch Top
    for (int y = p - 1; y >= 0; --y) {
        for (int x = 0; x < _data_width; ++x) {
            setPixel(x,y, getPixel(x,y+1));            
        }
    }

    // Stretch Bottom
    for (int y = _data_height - p; y < _data_height; ++y) {
        for (int x = 0; x < _data_width; ++x) {
            setPixel(x,y, getPixel(x,y-1));
        }
    }
    
    // Stretch Left
    for (int x = p - 1; x >= 0; --x) {
        for (int y = 0; y < _data_height; ++y) {
            setPixel(x,y, getPixel(x+1,y));
        }
    }

    // Stretch Right
    for (int x = _data_width - p; x < _data_width; ++x) {
        for (int y = 0; y < _data_height; ++y) {
            setPixel(x,y, getPixel(x-1,y));
        }
    }

    
}

//==============================================================================
//==============================================================================

Image::Pixel& Image::getPixel	(int x, int y) const
{
	return _data[(_data_width * _window_y + _window_x) + (y * _data_width) + x];
}

void Image::setPixel	(int x, int y, Image::Pixel &p)
{
    int index = (_data_width * _window_y + _window_x) + (y * _data_width) + x;
    
    if (index < 0 || index >= _data_width * _data_height)
        return;
        
	_data[index] = p;
}

//==============================================================================
//==============================================================================

void Image::copy (Image *img, int xoff, int yoff)
{	
	for (int y = 0; y < img->_window_height; ++y) {
		for (int x = 0; x < img->_window_width; ++x) {
			setPixel(x+xoff, y+yoff, img->getPixel(x,y));
		}
	}
}

//==============================================================================
//==============================================================================

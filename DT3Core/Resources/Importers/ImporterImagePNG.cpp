//==============================================================================
///	
///	File: ImporterImagePNG.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterImagePNG.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource3D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResourceCube.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"
#include <setjmp.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterImagePNG,png)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterImagePNG::ImporterImagePNG (void)
{    

}

ImporterImagePNG::~ImporterImagePNG (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr ImporterImagePNG::import (TextureResource2D *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat           format;

    import(pathname, args, width, height, data, format);
    
    DTboolean mipmapped = (MoreStrings::lowercase(args).find("mipmapped") != std::string::npos);

    target->set_textels(width, height, data, format, mipmapped);

	return DT3_ERR_NONE;
}

DTerr ImporterImagePNG::import (TextureResource3D *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat           format;

    import(pathname, args, width, height, data, format);
    
    // TODO: Set textels

	return DT3_ERR_NONE;
}

DTerr ImporterImagePNG::import (TextureResourceCube *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat           format;

    import(pathname, args, width, height, data, format);

    // TODO: Set textels

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

void ImporterImagePNG::png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	BinaryFileStream *read_ptr = reinterpret_cast<BinaryFileStream*>(png_get_io_ptr(png_ptr));
	read_ptr->read_raw(data,length);
}
	
//==============================================================================
//==============================================================================

DTerr ImporterImagePNG::import(const FilePath &pathname, const std::string &args, DTuint &width, DTuint &height, std::shared_ptr<DTubyte> &data, DT3GLTextelFormat &format)
{
	//
	// The following is based on the libpng manual. http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-3.1
	//

	//
	// Check the file header
	//

	// open the file
	BinaryFileStream file;
	DTerr err;
	if ((err = FileManager::open(file, pathname, true)) != DT3_ERR_NONE)
		return err;
	
	// read the header
	const DTuint NUM_HEADER_BYTES_TO_READ = 8;
	DTubyte header[NUM_HEADER_BYTES_TO_READ];
	
	file.read_raw(header, NUM_HEADER_BYTES_TO_READ);
    if (png_sig_cmp((png_bytep) header, 0, NUM_HEADER_BYTES_TO_READ)) {
        return DT3_ERR_FILE_WRONG_TYPE;
	}
	
	//
	// read the image data
	//
	
	png_bytep *row_pointers = NULL;

	// Create and initialize the png_struct with the desired error handler
	// functions.  If you want to use the default stderr and longjump method,
	// you can supply NULL for the last three parameters.  We also supply the
	// the compiler header file version, so that we know if the application
	// was compiled with a compatible version of the library.  REQUIRED

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return DT3_ERR_FILE_WRONG_TYPE;
	}
	
	// allocate/initialize the memory for image information
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return DT3_ERR_FILE_WRONG_TYPE;
    }

	// set error handling if you are using the setjmp/longjmp method (this is
	// the normal method of doing things with libpng).  REQUIRED unless you
	// set up your own error handlers in the png_create_read_struct() earlier.
	if (setjmp ((*png_set_longjmp_fn((png_ptr), (png_longjmp_ptr)longjmp, sizeof(jmp_buf))))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		delete[] row_pointers;	// Gets allocated after here, but don't forget the jump!
        return DT3_ERR_FILE_WRONG_TYPE;
	}
   
	// set custom read routine
	png_set_read_fn(png_ptr, &file, &png_read_data);
   
	// If we have already read some of the signature
	png_set_sig_bytes(png_ptr, NUM_HEADER_BYTES_TO_READ);
   
	//
	// read the entire image
	//
	
	// read PNG header info
	png_read_info(png_ptr, info_ptr);
	
	int bit_depth, color_type, interlace_type;
	
	// Header info
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	
	// Strip 16 bit down to 8 bit
	png_set_strip_16(png_ptr);
		
	// Extract components to even bytes
	png_set_packing(png_ptr);

	// Expand grayscale images to 8 bit
	png_set_expand(png_ptr);
		// add a filler byte when missing
	png_set_filler(png_ptr,0xFFFFFFFF,PNG_FILLER_AFTER);
	
	// Expand grayscale images to rgb
	png_set_gray_to_rgb(png_ptr);
	
	// update the transformation info within libpng
	png_read_update_info(png_ptr, info_ptr);
	
	// Header info again
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	
	// allocate target image
    std::shared_ptr<DTuint> buffer = std::shared_ptr<DTuint>(new DTuint[width * height]);
		
	// Setup row pointers
	row_pointers = new ("ImporterImagePNG") png_bytep[height];
	if (!row_pointers) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        return DT3_ERR_FILE_WRONG_TYPE;
	}
	
	for (DTuint y = 0; y < height; ++y) {
		row_pointers[height - y - 1] = reinterpret_cast<png_byte*>( &(buffer.get())[y * width] );
	}	
	
	// read the entire image
	png_read_image(png_ptr, row_pointers);
	
	//
	// Clean up
	//
	
	delete[] row_pointers;
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    
	// Check for down conversion
	if (MoreStrings::lowercase(args).find("r5g5b5a1") != std::string::npos) {
		// Change data format to 16 bit
        std::shared_ptr<DTubyte> b = std::shared_ptr<DTubyte>(new DTubyte[width * height * 2]);
        
		DTuint *src = buffer.get();
		DTushort *dst = (DTushort*) b.get();
		
		for (DTuint y = 0; y < height; ++y) {
			for (DTuint x = 0; x < width; ++x) {
				*dst =	(DTushort) (((((*src) >>  0) & 0xFF) >> 3) << 11 |
									((((*src) >>  8) & 0xFF) >> 3) << 6 |
									((((*src) >>  16) & 0xFF) >> 3) << 1 |
									((*src) >> 31));
													
				++src;
				++dst;
			}
		}
        
        data = b;
        format = DT3GL_FORMAT_RGBA_5551;

	} else 	if (MoreStrings::lowercase(args).find("r4g4b4a4") != std::string::npos) {
		// Change data format to 16 bit
        std::shared_ptr<DTubyte> b = std::shared_ptr<DTubyte>(new DTubyte[width * height * 2]);
        
		DTuint *src = buffer.get();
		DTushort *dst = (DTushort*) b.get();
		
		for (DTuint y = 0; y < height; ++y) {
			for (DTuint x = 0; x < width; ++x) {
				*dst =	(DTushort) (((((*src) >>  0) & 0xFF) >> 4) << 12 |
									((((*src) >>  8) & 0xFF) >> 4) << 8 |
									((((*src) >>  16) & 0xFF) >> 4) << 4 |
									((((*src) >>  24) & 0xFF) >> 4) << 0);
													
				++src;
				++dst;
			}
		}

        data = b;
        format = DT3GL_FORMAT_RGBA_4444;

	} else 	if (MoreStrings::lowercase(args).find("r5g6b5") != std::string::npos) {
		// Change data format to 16 bit
        std::shared_ptr<DTubyte> b = std::shared_ptr<DTubyte>(new DTubyte[width * height * 2]);
        
		DTuint *src = buffer.get();
		DTushort *dst = (DTushort*) b.get();
		
		for (DTuint y = 0; y < height; ++y) {
			for (DTuint x = 0; x < width; ++x) {
				*dst =	(DTushort) (((((*src) >>  0) & 0xFF) >> 3) << 11 |
									((((*src) >>  8) & 0xFF) >> 2) << 5 |
									((((*src) >>  16) & 0xFF) >> 3) << 0);
													
				++src;
				++dst;
			}
		}

        data = b;
        format = DT3GL_FORMAT_RGB_565;

	} else 	if (MoreStrings::lowercase(args).find("lum8") != std::string::npos) {
		// Change data format to 8 bit
        std::shared_ptr<DTubyte> b = std::shared_ptr<DTubyte>(new DTubyte[width * height * 2]);
        
		DTuint *src = buffer.get();
		DTushort *dst = (DTushort*) b.get();
		
		for (DTuint y = 0; y < height; ++y) {
			for (DTuint x = 0; x < width; ++x) {
				*dst =	(DTubyte)	(((*src) >>  0) & 0xFF);
													
				++src;
				++dst;
			}
		}
        
        data = b;
        format = DT3GL_FORMAT_LUM_8;

	} else {
		// Change data format to 8 bit
        std::shared_ptr<DTubyte> b = std::shared_ptr<DTubyte>(new DTubyte[width * height * 4]);
        
		DTuint *src = buffer.get();
		DTushort *dst = (DTushort*) b.get();
        
		memcpy(dst, src, width * height * 4);

        data = b;
        format = DT3GL_FORMAT_RGBA;
	}
	   
	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3


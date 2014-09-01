//==============================================================================
///	
///	File: ImporterImageJPG.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterImageJPG.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource3D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResourceCube.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterImageJPG,jpg)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterImageJPG::ImporterImageJPG (void)
{    

}

ImporterImageJPG::~ImporterImageJPG (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr ImporterImageJPG::import (TextureResource2D *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat            format;

    import(pathname, args, width, height, data, format);
    
    
    DTboolean mipmapped = (MoreStrings::lowercase(args).find("mipmapped") != std::string::npos);

    target->set_textels(width, height, data, format, mipmapped);

	return DT3_ERR_NONE;
}

DTerr ImporterImageJPG::import (TextureResource3D *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat            format;

    import(pathname, args, width, height, data, format);
    
    // TODO: Set textels

	return DT3_ERR_NONE;
}

DTerr ImporterImageJPG::import (TextureResourceCube *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat            format;

    import(pathname, args, width, height, data, format);

    // TODO: Set textels

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

void ImporterImageJPG::error_handler (j_common_ptr cinfo)
{
    // cinfo->err really points to a ImportErr struct, so coerce pointer
    ImportErr *err = (ImportErr *) cinfo->err;

    // Return control to the setjmp point
    longjmp(err->setjmp_buffer, 1);
}

//==============================================================================
//==============================================================================

void ImporterImageJPG::init_source (j_decompress_ptr cinfo)
{
    // Do nothing
}

void ImporterImageJPG::term_source (j_decompress_ptr cinfo)
{

}


boolean ImporterImageJPG::fill_input_buffer (j_decompress_ptr cinfo)
{
    ImportSource *src = (ImportSource *) cinfo->src;

    // Read the buffer
    DTsize nbytes = src->infile->read_raw(src->buffer, sizeof(src->buffer));
    if (nbytes <= 0) {
        // Insert a fake EOI marker
        src->buffer[0] = (JOCTET) 0xFF;
        src->buffer[1] = (JOCTET) JPEG_EOI;
        nbytes = 2;
    }

    src->pub.next_input_byte = src->buffer;
    src->pub.bytes_in_buffer = (size_t) nbytes;

    return TRUE;
}

void ImporterImageJPG::skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
    ImportSource *src = (ImportSource *) cinfo->src;

    if (num_bytes > 0) {
        while (num_bytes > (long) src->pub.bytes_in_buffer) {
            num_bytes -= (long) src->pub.bytes_in_buffer;
            fill_input_buffer(cinfo);
        }
        
        src->pub.next_input_byte += (size_t) num_bytes;
        src->pub.bytes_in_buffer -= (size_t) num_bytes;
    }
}

void ImporterImageJPG::jpeg_stream_src (j_decompress_ptr cinfo, BinaryFileStream &file)
{
    ImportSource *src;

    src = (ImportSource*) cinfo->src;
    src->pub.init_source = init_source;
    src->pub.fill_input_buffer = fill_input_buffer;
    src->pub.skip_input_data = skip_input_data;
    src->pub.resync_to_restart = jpeg_resync_to_restart; // use default method
    src->pub.term_source = term_source;
    
    src->infile = &file;
    
    src->pub.bytes_in_buffer = 0;       // forces fillInputBuffer on first read
    src->pub.next_input_byte = NULL;    // until buffer loaded
}

//==============================================================================
//==============================================================================

DTerr ImporterImageJPG::import(const FilePath &pathname, const std::string &args, DTuint &width, DTuint &height, std::shared_ptr<DTubyte> &data, DT3GLTextelFormat &format)
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

    // This struct contains the JPEG decompression parameters and pointers to
    // working space (which is allocated as needed by the JPEG library).
    jpeg_decompress_struct cinfo;
    ImportSource           csrc;

    // We use our private extension JPEG error handler.
    // Note that this struct must live as long as the main JPEG parameter
    // struct, to avoid dangling-pointer problems.
    ImportErr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = ImporterImageJPG::error_handler;
    
    // Establish the setjmp return context for my_error_exit to use.
    if (setjmp(jerr.setjmp_buffer)) {
        // If we get here, the JPEG code has signaled an error.
        // We need to clean up the JPEG object, close the input file, and return.
        jpeg_destroy_decompress(&cinfo);
        file.close();
        return DT3_ERR_FILE_WRONG_TYPE;
    }

    //  Now we can initialize the JPEG decompression object.
    jpeg_create_decompress(&cinfo);
    cinfo.src = (jpeg_source_mgr*) &csrc;

    //  Step 2: specify data source (eg, a file)
    jpeg_stream_src(&cinfo, file);

    //  Step 3: read file parameters with jpeg_read_header()
    jpeg_read_header(&cinfo, TRUE);

    //  Step 4: set parameters for decompression

    //  In this example, we don't need to change any of the defaults set by
    //  jpeg_read_header(), so we do nothing here.

    //  Step 5: Start decompressor
    jpeg_start_decompress(&cinfo);
    
    ASSERT(cinfo.out_color_components == 3 && cinfo.data_precision == 8);
    
    // Build a buffer for reading in the image
    width = cinfo.output_width;
    height = cinfo.output_height;
    format = DT3GL_FORMAT_RGB;
    
    // Change data format to 16 bit
    data = std::shared_ptr<DTubyte>(new DTubyte[width * height * 3]);
	DTubyte *buffer = data.get();
    
    DTint row_stride = cinfo.output_width * cinfo.output_components;
    
    // Make a one-row-high sample array that will go away when done with image
    JSAMPARRAY scanline_buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    
    // Step 6: while (scan lines remain to be read)
    // Here we use the library's state variable cinfo.output_scanline as the
    // loop counter, so that we don't have to keep track ourselves.

    while (cinfo.output_scanline < cinfo.output_height) {
        //jpeg_read_scanlines expects an array of pointers to scanlines.
        // Here the array is only one element long, but you could ask for
        // more than one scanline at a time if that's more convenient.

        jpeg_read_scanlines(&cinfo, scanline_buffer, 1);
        
        // Copy Row into buffer
        DTubyte *src_data = (DTubyte*) scanline_buffer[0];
        DTubyte *dst_data = (DTubyte*) &buffer[width * (height - cinfo.output_scanline) * 3];
        
        // Copy row
        ::memcpy(dst_data, src_data, row_stride);
    }

    // Step 7: Finish decompression
    jpeg_finish_decompress(&cinfo);

    // Step 8: Release JPEG decompression object
    // This is an important step since it will release a good deal of memory.
    jpeg_destroy_decompress(&cinfo);
    
//#endif

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3


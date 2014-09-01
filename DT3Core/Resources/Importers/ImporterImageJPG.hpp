#ifndef DT3_IMPORTERIMAGEJPG
#define DT3_IMPORTERIMAGEJPG
//==============================================================================
///	
///	File: ImporterImageJPG.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterImage.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include <vector>
#include <string>
#include <setjmp.h>

extern "C" {
    #include "jpeglib.h"
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FilePath;
class BinaryFileStream;

//==============================================================================
/// Class
//==============================================================================

class ImporterImageJPG: public ImporterImage {
    public:
        DEFINE_TYPE(ImporterImageJPG,ImporterImage)
		DEFINE_CREATE
         
									ImporterImageJPG	(void);	
    
	private:
									ImporterImageJPG	(const ImporterImageJPG &rhs);
        ImporterImageJPG &			operator =			(const ImporterImageJPG &rhs);
    
    public:
        virtual						~ImporterImageJPG	(void);
            
    public:        	
		/// Imports an image into a TextureResource2D
		/// \param target object to import texture into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(TextureResource2D *target, std::string args);

		/// Imports an image into a TextureResource3D
		/// \param target object to import texture into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(TextureResource3D *target, std::string args);

		/// Imports an image into a TextureResourceCube
		/// \param target object to import texture into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(TextureResourceCube *target, std::string args);
    
    private:
    
        DTerr                       import              (   const FilePath &pathname,
                                                            const std::string &args,
                                                            DTuint &width,
                                                            DTuint &height,
                                                            std::shared_ptr<DTubyte> &data,
                                                            DT3GLTextelFormat &format);

        struct ImportErr {
            jpeg_error_mgr pub;         //  "public" fields
            jmp_buf setjmp_buffer;      //  for return to caller
        };
    
        struct ImportSource {
            jpeg_source_mgr pub;        // public fields

            BinaryFileStream *infile;   // source stream
            JOCTET buffer[4096];        // start of buffer
        };


        static void                 error_handler       (j_common_ptr cinfo) __attribute__ ((noreturn));

        static void                 init_source         (j_decompress_ptr cinfo);
        static void                 term_source         (j_decompress_ptr cinfo);

        static boolean              fill_input_buffer   (j_decompress_ptr cinfo);
        static void                 skip_input_data     (j_decompress_ptr cinfo, long num_bytes);
        
        static void                 jpeg_stream_src     (j_decompress_ptr cinfo, BinaryFileStream &file);

};

//==============================================================================
//==============================================================================

} // DT3

#endif

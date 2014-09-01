#ifndef DT3_IMPORTERIMAGEPNG
#define DT3_IMPORTERIMAGEPNG
//==============================================================================
///	
///	File: ImporterImagePNG.hpp
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

extern "C" {
    #include "png.h"
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

class ImporterImagePNG: public ImporterImage {
    public:
        DEFINE_TYPE(ImporterImagePNG,ImporterImage)
		DEFINE_CREATE
         
									ImporterImagePNG	(void);	
    
	private:
									ImporterImagePNG	(const ImporterImagePNG &rhs);
        ImporterImagePNG &			operator =			(const ImporterImagePNG &rhs);
    
    public:
        virtual						~ImporterImagePNG	(void);
            
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

        static void                 png_read_data       (png_structp png_ptr, png_bytep data, png_size_t length);
};

//==============================================================================
//==============================================================================

} // DT3

#endif

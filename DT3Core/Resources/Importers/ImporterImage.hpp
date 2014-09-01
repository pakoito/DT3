#ifndef DT3_IMPORTERIMAGE
#define DT3_IMPORTERIMAGE
//==============================================================================
///	
///	File: ImporterImage.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TextureResource2D;
class TextureResource3D;
class TextureResourceCube;

//==============================================================================
/// Class
//==============================================================================

class ImporterImage: public BaseClass {
    public:
        DEFINE_TYPE(ImporterImage,BaseClass)
         
									ImporterImage		(void);	
    
	private:
									ImporterImage		(const ImporterImage &rhs);
        ImporterImage &				operator =			(const ImporterImage &rhs);
    
    public:
        virtual						~ImporterImage		(void);
    
    public:        	
		/// Imports an image into a TextureResource2D
		/// \param target object to import texture into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(TextureResource2D *target, std::string args) = 0;

		/// Imports an image into a TextureResource3D
		/// \param target object to import texture into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(TextureResource3D *target, std::string args) = 0;

		/// Imports an image into a TextureResourceCube
		/// \param target object to import texture into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(TextureResourceCube *target, std::string args) = 0;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

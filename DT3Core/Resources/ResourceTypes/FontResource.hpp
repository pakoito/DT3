#ifndef DT3_FONTRESOURCE 
#define DT3_FONTRESOURCE
//==============================================================================
///	
///	File: FontResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include <string>
#include <map>
#include <mutex>

extern "C" {
    #include "ft2build.h"
    #include "freetype/freetype.h"
    #include "freetype/ftmodapi.h"
    #include "freetype/ftglyph.h"
    #include "freetype/ftoutln.h"
    #include "freetype/ftstroke.h"
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================



//==============================================================================
/// Font resource type
//==============================================================================

class FontResource: public Resource {
    public:
        DEFINE_TYPE(FontResource,Resource)
		DEFINE_CREATE

                                FontResource            (void);	
	private:
                                FontResource            (const FontResource &rhs);
        FontResource &          operator =				(const FontResource &rhs);	
	public:
        virtual                 ~FontResource           (void);
                
    public:        	
		/// Called to initialize the class
		static void             initialize_static       (void);

		/// Called to uninitialize the class
		static void             uninitialize_static     (void);

		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		DTerr                   import					(const FilePath &pathname, std::string args);
		    
        /// Gets the freetype typeface
		/// \return freetype typeface
        FT_Face&                typeface                (void);

        /// Gets the freetype library
		/// \return freetype library
        static FT_Library       library                 (void);
        
        
		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<FontResource> import_resource (const FilePath &pathname, std::string args = "");

	private:
        static std::mutex                                               _font_map_lock;
		static std::map<std::string, std::shared_ptr<FontResource>>     _font_map;

        static void             reload_if_changed       (void);

        static FT_Library       _library;
        static FT_MemoryRec_    _memory;
        FT_Face                 _typeface;
		
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<FontResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<FontResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif

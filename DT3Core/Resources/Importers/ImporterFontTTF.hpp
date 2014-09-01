#ifndef DT3_IMPORTERFONTTTF
#define DT3_IMPORTERFONTTTF
//==============================================================================
///	
///	File: ImporterFontTTF.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterFont.hpp"

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
/// Class
//==============================================================================

class ImporterFontTTF: public ImporterFont {
    public:
        DEFINE_TYPE(ImporterFontTTF,ImporterFont)
		DEFINE_CREATE
         
										ImporterFontTTF			(void);	
    
	private:
										ImporterFontTTF			(const ImporterFontTTF &rhs);
        ImporterFontTTF &				operator =				(const ImporterFontTTF &rhs);
    
    public:
        virtual							~ImporterFontTTF		(void);
    
    public: 
		/// Imports a sound group into an FontResource
		/// \param target object to import sound into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import					(FontResource *target, std::string args);
		
        
        static unsigned long            ft_io_func   (  FT_Stream       stream,
                                                        unsigned long   offset,
                                                        unsigned char*  buffer,
                                                        unsigned long   count);

        static void                     ft_close_func(   FT_Stream    stream);
};

//==============================================================================
//==============================================================================

} // DT3

#endif

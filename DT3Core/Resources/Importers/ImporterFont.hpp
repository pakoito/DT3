#ifndef DT3_IMPORTERFONT
#define DT3_IMPORTERFONT
//==============================================================================
///	
///	File: ImporterFont.hpp
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

class FontResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterFont: public BaseClass {
    public:
        DEFINE_TYPE(ImporterFont,BaseClass)

         
									ImporterFont	(void);	
    
	private:
									ImporterFont	(const ImporterFont &rhs);
        ImporterFont &        		operator =		(const ImporterFont &rhs);
    
    public:
        virtual						~ImporterFont	(void);
    
	public:
		/// Imports a font into a FontResource
		/// \param target object to import sound into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(FontResource *target, std::string args) = 0;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif

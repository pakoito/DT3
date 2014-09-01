#ifndef DT3_IMPORTERSOUND
#define DT3_IMPORTERSOUND
//==============================================================================
///	
///	File: ImporterSound.hpp
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

class SoundResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterSound: public BaseClass {
    public:
        DEFINE_TYPE(ImporterSound,BaseClass)

         
									ImporterSound		(void);	
    
	private:
									ImporterSound		(const ImporterSound &rhs);
        ImporterSound &				operator =			(const ImporterSound &rhs);
    
    public:
        virtual						~ImporterSound		(void);
    
	public:
		/// Imports a sound into an SoundResource
		/// \param target object to import sound into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import				(SoundResource *target, std::string args) = 0;

		/// Loads sound data from the file
		/// \param data pointer to buffer
		/// \param start start of sound data (doesn't include header)
		/// \param length length of sound data in bytes
		virtual DTsize				stream				(DTubyte *data, DTsize start, DTsize length) = 0;
		
		/// Returns length of sound data
		/// \return length of sound data in bytes
		virtual DTsize				length              (void) = 0;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

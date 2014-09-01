#ifndef DT3_IMPORTERSHADER
#define DT3_IMPORTERSHADER
//==============================================================================
///	
///	File: ImporterShader.hpp
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

class ShaderResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterShader: public BaseClass {
    public:
        DEFINE_TYPE(ImporterShader,BaseClass)

         
									ImporterShader			(void);	
    
	private:
									ImporterShader			(const ImporterShader &rhs);
        ImporterShader &			operator =				(const ImporterShader &rhs);
    
    public:
        virtual						~ImporterShader			(void);
    
	public:
		/// Imports a shader into a ShaderResource
		/// \param target object to import sound into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import					(ShaderResource *target, std::string args) = 0;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif

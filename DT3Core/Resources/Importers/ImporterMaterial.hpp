#ifndef DT3_IMPORTERMATERIAL
#define DT3_IMPORTERMATERIAL
//==============================================================================
///	
///	File: ImporterMaterial.hpp
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

class MaterialResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterMaterial: public BaseClass {
    public:
        DEFINE_TYPE(ImporterMaterial,BaseClass)

         
									ImporterMaterial		(void);	
    
	private:
									ImporterMaterial		(const ImporterMaterial &rhs);
        ImporterMaterial &			operator =				(const ImporterMaterial &rhs);
    
    public:
        virtual						~ImporterMaterial		(void);
             
	public:
		/// Imports a material into an MaterialResource
		/// \param target object to import material into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import					(MaterialResource *target, std::string args) = 0;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif

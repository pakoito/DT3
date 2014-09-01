#ifndef DT3_IMPORTERGEOMETRY
#define DT3_IMPORTERGEOMETRY
//==============================================================================
///	
///	File: ImporterGeometry.hpp
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

class GeometryResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterGeometry: public BaseClass {
    public:
        DEFINE_TYPE(ImporterGeometry,BaseClass)
         
								ImporterGeometry	(void);	
    
	private:
								ImporterGeometry	(const ImporterGeometry &rhs);
        ImporterGeometry &		operator =			(const ImporterGeometry &rhs);
    
    public:
        virtual					~ImporterGeometry	(void);
                
    public:        	
		/// Imports an geometry into an GeometryResource
		/// \param target object to import geometry into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr			import				(GeometryResource *target, std::string args) = 0;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

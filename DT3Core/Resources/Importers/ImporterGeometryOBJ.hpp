#ifndef DT3_IMPORTERGEOMETRYOBJ
#define DT3_IMPORTERGEOMETRYOBJ
//==============================================================================
///	
///	File: ImporterGeometryOBJ.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterGeometry.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TextFileStream;

//==============================================================================
/// Class
//==============================================================================

class ImporterGeometryOBJ: public ImporterGeometry {
    public:
        DEFINE_TYPE(ImporterGeometryOBJ,ImporterGeometry)
		DEFINE_CREATE
         
										ImporterGeometryOBJ	(void);	
    
	private:
										ImporterGeometryOBJ	(const ImporterGeometryOBJ &rhs);
        ImporterGeometryOBJ &			operator =			(const ImporterGeometryOBJ &rhs);
    
    public:
        virtual							~ImporterGeometryOBJ(void);
                
    public:        	
		/// Imports an geometry into an GeometryResource
		/// \param target object to import geometry into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import				(GeometryResource *target, std::string args);

	private:
		
		std::vector<Vector3>	_vertices;
		std::vector<Vector3>	_normals;
		std::vector<Vector2>	_texcoords;
		
		struct Face {
			DTint				_v[3];
			DTint				_vt[3];
			DTint				_vn[3];
		};
		std::vector<Face>		_faces;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

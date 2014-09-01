//==============================================================================
///	
///	File: ImporterGeometryOBJ.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterGeometryOBJ.hpp"
#include "DT3Core/Resources/ResourceTypes/GeometryResource.hpp"
#include "DT3Core/Types/FileBuffer/TextFileStream.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/Factory.hpp"
#include <sstream>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterGeometryOBJ,obj)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterGeometryOBJ::ImporterGeometryOBJ (void)		
{    

}
			
ImporterGeometryOBJ::~ImporterGeometryOBJ (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr	ImporterGeometryOBJ::import(GeometryResource *target, std::string args)
{
	FilePath pathname(target->path());
	
    // open the file
    TextFileStream	file;
	FileManager::open(file, pathname, true);
     
    while (1) {
		DTcharacter buffer[1024];
		
		file.line (buffer, sizeof(buffer));
		if (file.is_eof())
			break;
		
		std::stringstream ss(buffer);
	
		// read the command
		std::string command;
		ss >> command;
		
		if (command == "v") {
			DTfloat x,y,z;
			ss >> x >> y >> z;
			_vertices.push_back(Vector3(x,y,z));
		} else if (command == "vt") {
			DTfloat u,v,w;
			ss >> u >> v >> w;
			_texcoords.push_back(Vector2(u,v));
		} else if (command == "vn") {
			DTfloat x,y,z;
			ss >> x >> y >> z;
			_normals.push_back(Vector3(x,y,z));
		} else if (command == "f") {
			
			Face face;
			
			for (DTint i = 0; i < 3; ++i) {
				std::string f;
				ss >> f;
				
				// get rid of slashes
				for (std::size_t j = 0; j < f.size(); ++j)
					if (f[j] == '/')	f[j] = ' ';
				
				std::stringstream ssv(f);
				
				ssv >> face._v[i];
				ssv >> face._vt[i];
				ssv >> face._vn[i];
                                
                if (face._v[i] < 0) face._v[i] = static_cast<DTint>(_vertices.size()) + face._v[i];
                else                face._v[i] -= 1;
                
                if (face._vt[i] < 0)face._vt[i] = static_cast<DTint>(_texcoords.size()) + face._vt[i];
                else                face._vt[i] -= 1;

                if (face._vn[i] < 0)face._vn[i] = static_cast<DTint>(_normals.size()) + face._vn[i];
                else                face._vn[i] -= 1;
                
                ASSERT(face._v[i] >= 0 && face._v[i] < static_cast<DTint>(_vertices.size()));
                ASSERT(face._vt[i] >= 0 && face._vt[i] < static_cast<DTint>(_texcoords.size()));
                ASSERT(face._vn[i] >= 0 && face._vn[i] < static_cast<DTint>(_normals.size()));
            }
			
			_faces.push_back(face);
			
		}
	}
    
    LOG_MESSAGE << "Read " << static_cast<DTint>(_vertices.size()) << " vertices";
    LOG_MESSAGE << "Read " << static_cast<DTint>(_texcoords.size()) << " texcoords";
    LOG_MESSAGE << "Read " << static_cast<DTint>(_normals.size()) << " normals";
    LOG_MESSAGE << "Read " << static_cast<DTint>(_faces.size()) << " faces";
	
	//
	// compile geometry
	//
    
    std::vector<Vector3>	vertices_stream;
    std::vector<Vector3>	normals_stream;
    std::vector<Vector2>	uvs_stream;
    std::vector<Triangle>	indices_stream;

	vertices_stream.resize(_faces.size() * 3);
	normals_stream.resize(_faces.size() * 3);
	uvs_stream.resize(_faces.size() * 3);
	indices_stream.resize(_faces.size());
	
	DTint index = 0;
	for (std::size_t f = 0; f < _faces.size(); ++f) {
		indices_stream[f].v[0] = index+0;
		indices_stream[f].v[1] = index+1;
		indices_stream[f].v[2] = index+2;

		for (DTint v = 0; v < 3; ++v) {
			vertices_stream[index] = _vertices[ _faces[f]._v[v]];
			normals_stream[index] = _normals[ _faces[f]._vn[v] ];
			uvs_stream[index] = _texcoords[ _faces[f]._vt[v] ];
			++index;
		}
	}

    std::shared_ptr<Mesh>  mesh = Mesh::create();
    mesh->set_name("Mesh");

	mesh->set_vertex_stream(vertices_stream);
	mesh->set_normals_stream(normals_stream);
	mesh->set_uv_stream0(uvs_stream);
	mesh->set_index_stream(indices_stream);
	    
    target->add_mesh(mesh);

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3


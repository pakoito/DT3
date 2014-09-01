#ifndef DT3_IMPORTERGEOMETRY3DS
#define DT3_IMPORTERGEOMETRY3DS
//==============================================================================
///	
///	File: ImporterGeometry3DS.hpp
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
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Math/WeightsIndex.hpp"
#include "DT3Core/Types/Math/Triangle.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class BinaryFileStream;

//==============================================================================
/// Class
//==============================================================================

class ImporterGeometry3DS: public ImporterGeometry {
    public:
        DEFINE_TYPE(ImporterGeometry3DS,ImporterGeometry)
		DEFINE_CREATE
         
										ImporterGeometry3DS	(void);	
    
	private:
										ImporterGeometry3DS	(const ImporterGeometry3DS &rhs);
        ImporterGeometry3DS &			operator =			(const ImporterGeometry3DS &rhs);
    
    public:
        virtual							~ImporterGeometry3DS(void);
                
    public:        	
		/// Imports an geometry into an GeometryResource
		/// \param target object to import geometry into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import				(GeometryResource *target, std::string args);

    private:	

        enum	{	
            CHUNK_MAIN3DS 			= 0x4D4D,
            CHUNK_EDIT3DS 			= 0x3D3D,
            CHUNK_EDITOBJECT		= 0x4000,
            CHUNK_OBJ_TRIMESH		= 0x4100,
            CHUNK_TRI_VERTEX		= 0x4110,
            CHUNK_TRI_MAPPINGCOORDS	= 0x4140,
            CHUNK_TRI_FACE			= 0x4120,
            CHUNK_TRI_FACE_MAP		= 0x4130,
            CHUNK_TRI_LOCAL			= 0x4160,
			
			CHUNK_EDIT_MATERIAL		= 0xAFFF,
			CHUNK_MAT_NAME01		= 0xA000,
			//CHUNK_MAT_TEXTURE		= 0xA200,
			//CHUNK_MAT_BUMPMAP		= 0xA230,
			//CHUNK_MAT_MAPFILE		= 0xA300,
            
            CHUNK_KEYF3DS			= 0xB000,
            CHUNK_OBJDES			= 0xB002,
            CHUNK_OBJHIERARCH		= 0xB010,
            CHUNK_OBJPIVOT			= 0xB013
        };
        
        DTuint				read					(BinaryFileStream &in, DTubyte &v);
        DTuint				read					(BinaryFileStream &in, DTushort &v);
        DTuint				read					(BinaryFileStream &in, DTshort &v);
        DTuint				read					(BinaryFileStream &in, DTuint &v);
        DTuint				read					(BinaryFileStream &in, DTfloat &v);
        DTuint				read					(BinaryFileStream &in, std::string &v);

        DTuint				parse_main_3DS			(BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_edit_3DS			(BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_edit_object       (BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_obj_trimesh       (BinaryFileStream &in, DTuint owner_chunk_size);
        
        DTuint				parse_tri_vertex        (BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_tri_mapping_coords(BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_tri_face			(BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_tri_face_map      (BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_tri_local			(BinaryFileStream &in, DTuint owner_chunk_size);

        DTuint				parse_key_f3DS			(BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_obj_des           (BinaryFileStream &in, DTuint owner_chunk_size);
        DTuint				parse_obj_hier			(BinaryFileStream &in, DTuint owner_chunk_size);

        DTuint				parse_edit_material		(BinaryFileStream &in, DTuint owner_chunk_size);
		DTuint				parse_mat_name_01       (BinaryFileStream &in, DTuint owner_chunk_size);
    
        DTuint				parse_next_chunk        (BinaryFileStream &in, DTuint owner_chunk_size);


    private:
        void                left_to_right_handed    (const Vector3 &a, Vector3 &r);

		std::vector<Vector3>        _vertices_stream;
		std::vector<Vector2>        _uvs_stream_0;
		std::vector<Vector2>        _uvs_stream_1;
		std::vector<WeightsIndex>   _weights_index_stream;
		std::vector<Vector4>        _weights_strength_stream;
		std::vector<Triangle>       _indices_stream;

		DTint                       _current_obj_id;
		DTint                       _current_hier_id;
		
		DTint                       _base_face;
		DTint                       _base_vertex;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

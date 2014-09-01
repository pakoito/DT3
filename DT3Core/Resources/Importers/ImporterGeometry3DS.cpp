//==============================================================================
///	
///	File: ImporterGeometry3DS.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterGeometry3DS.hpp"
#include "DT3Core/Resources/ResourceTypes/GeometryResource.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Graphics/Mesh.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterGeometry3DS,3ds)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterGeometry3DS::ImporterGeometry3DS (void)
{    

}
			
ImporterGeometry3DS::~ImporterGeometry3DS (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr	ImporterGeometry3DS::import(GeometryResource *target, std::string args)
{
	FilePath pathname(target->path());
	    
    // open the file
    BinaryFileStream	infile;
	FileManager::open(infile, pathname, true);
    
    // Verify file type
    DTushort	chunk_id;
    DTuint	chunk_size;
    read(infile, chunk_id);
    read(infile, chunk_size);
    
    // Check first word
    if (chunk_id != CHUNK_MAIN3DS) {
        return DT3_ERR_FILE_WRONG_TYPE;
    }
    
    // Go back to beginning
    infile.seek_g(0, BinaryStream::FROM_BEGINNING);
    
	// start parsing
	_base_face = 0;
	_base_vertex = 0;
		
    _current_obj_id = -1;
    _current_hier_id = -1;
		
    // read the file
    parse_next_chunk(infile, chunk_size);
    	

	// Post processing
    std::shared_ptr<Mesh>  mesh = Mesh::create();
    mesh->set_name("Mesh");

	mesh->set_vertex_stream(_vertices_stream);
	mesh->set_uv_stream0(_uvs_stream_0);
	mesh->set_uv_stream1(_uvs_stream_1);
	mesh->set_weights_index_stream(_weights_index_stream);
	mesh->set_weights_strength_stream(_weights_strength_stream);
	mesh->set_index_stream(_indices_stream);

	mesh->generate_normals();
    
    target->add_mesh(mesh);
			
	ASSERT(mesh->vertex_stream().size());
	
	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::read(BinaryFileStream &in, DTubyte &v)
{
    in.read_raw((DTubyte*)&v, sizeof(DTbyte));
    return sizeof(DTubyte);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::read(BinaryFileStream &in, DTushort &v)
{

#if DT3_BYTEORDER == DT3_LIL_ENDIAN
    in.read_raw((DTubyte*)&v, sizeof(DTushort));
#else
    DTubyte	b1,b2;
    
    read(in, b1);
    read(in, b2);
    v = (DTushort) ((static_cast<DTuint>(b2) << 8) | (static_cast<DTuint>(b1) << 0));
#endif
    return sizeof(DTushort);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::read(BinaryFileStream &in, DTshort &v)
{
    
#if DT3_BYTEORDER == DT3_LIL_ENDIAN
    in.read_raw((DTubyte*)&v, sizeof(DTshort));
#else
    DTubyte	b1,b2;
    
    read(in, b1);
    read(in, b2);
    v = (DTshort) ((static_cast<DTuint>(b2) << 8) | (static_cast<DTuint>(b1) << 0));
#endif

    return sizeof(DTshort);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::read(BinaryFileStream &in, DTuint &v)
{    
#if DT3_BYTEORDER == DT3_LIL_ENDIAN
    in.read_raw((DTubyte*)&v, sizeof(DTuint));
#else
    DTubyte	b1,b2,b3,b4;
    
    read(in, b1);
    read(in, b2);
    read(in, b3);
    read(in, b4);
	
	v = (DTuint)    (static_cast<DTuint>(b4) << 24) |
                    (static_cast<DTuint>(b3) << 16) |
                    (static_cast<DTuint>(b2) << 8) |
                    (static_cast<DTuint>(b1) << 0);
#endif

    return sizeof(DTuint);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::read(BinaryFileStream &in, DTfloat &v)
{

#if DT3_BYTEORDER == DT3_LIL_ENDIAN
    in.read_raw((DTubyte*)&v, sizeof(DTfloat));
#else    
    DTuint loaded_float;
    read(in, loaded_float);
    v = *((DTfloat*) &loaded_float);
#endif

    return sizeof(DTfloat);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::read(BinaryFileStream &in, std::string &v)
{
    DTubyte character;

    // clear the string
    v = "";
    
    read(in, character);
    while (character != '\0') {
        v += (DTcharacter) character;
        read(in, character);
    };
    
    return static_cast<DTuint>(v.size()) + 1;
}


//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_main_3DS(BinaryFileStream &in, DTuint owner_chunk_size)
{
    return parse_next_chunk(in, owner_chunk_size);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_edit_3DS(BinaryFileStream &in, DTuint owner_chunk_size)
{
    return parse_next_chunk(in, owner_chunk_size);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_edit_object(BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTuint size_read = 0;

    // add another object
    ++_current_obj_id;
	//_hierarchy.resize(_hierarchy.size()+1);
            
    // read object name
    std::string object_name;
    size_read += read(in, object_name);
    //_hierarchy[_current_obj_id]._name = object_name;
	
	// Remember base index
	_base_face = (DTint) _indices_stream.size();
	_base_vertex = (DTint) _vertices_stream.size();
            
    // Keep on Parsing
    size_read += parse_next_chunk(in, owner_chunk_size - size_read);
    
    return size_read;
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_obj_trimesh(BinaryFileStream &in, DTuint owner_chunk_size)
{	
    return parse_next_chunk(in, owner_chunk_size);
}

//==============================================================================
//==============================================================================

void ImporterGeometry3DS::left_to_right_handed (const Vector3 &a, Vector3 &r)
{
    DTfloat x = -a.x;
    DTfloat y = a.z;
    DTfloat z = a.y;

    // Rotate Coords
    r.x = x;
    r.y = y;
    r.z = z;
}

DTuint	ImporterGeometry3DS::parse_tri_vertex(BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTint size_read = 0;
    DTushort	num_verts;
    
    size_read += read(in,num_verts);
	
	_vertices_stream.resize(_base_vertex + num_verts);
	_weights_index_stream.resize(_base_vertex + num_verts);
	_weights_strength_stream.resize(_base_vertex + num_verts);
	_uvs_stream_0.resize(_base_vertex + num_verts);
	_uvs_stream_1.resize(_base_vertex + num_verts);
    
    for (DTuint i = 0; i < num_verts; ++i) {
        DTfloat x,y,z;
                        
        size_read += read(in,x);
        size_read += read(in,y);
        size_read += read(in,z);
		
		_vertices_stream[_base_vertex + i] = Vector3(x,y,z);

		left_to_right_handed( _vertices_stream[_base_vertex + i], _vertices_stream[_base_vertex + i] );
		
		_weights_index_stream[_base_vertex + i] = WeightsIndex(_current_obj_id,0,0,0);
		_weights_strength_stream[_base_vertex + i] = Vector4(1.0F,0.0F,0.0F,0.0F);
    }
    return size_read;
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_tri_mapping_coords(BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTuint size_read = 0;
    DTushort num_coords;
    
    size_read += read(in,num_coords);

    for (DTuint i = 0; i < num_coords; ++i) {
		DTfloat u,v;
    
        size_read += read(in,u);
        size_read += read(in,v);
        
		_uvs_stream_0[_base_vertex + i].u = u;
		_uvs_stream_0[_base_vertex + i].v = v;

		_uvs_stream_1[_base_vertex + i].u = u;
		_uvs_stream_1[_base_vertex + i].v = v;
    }
    return size_read;	
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_tri_face(BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTuint size_read = 0;
    DTushort	num_faces, info;
    
    size_read += read(in,num_faces);

	_indices_stream.resize(_base_face + num_faces);
	
	//_hierarchy[_current_obj_id]._num_faces = num_faces;
		
    for (DTuint i = 0; i < num_faces; ++i) {
		DTushort v1,v2,v3;
	
        size_read += read(in,v1);
        size_read += read(in,v2);
        size_read += read(in,v3);
                
        size_read += read(in,info);
		
		
        
        // Test for inverting normal
        if (!(info & 0x0007)) {
			_indices_stream[_base_face + i].v[0] = (DTushort) (v1 + _base_vertex);
			_indices_stream[_base_face + i].v[1] = (DTushort) (v3 + _base_vertex);
			_indices_stream[_base_face + i].v[2] = (DTushort) (v2 + _base_vertex);
		} else {
			_indices_stream[_base_face + i].v[0] = (DTushort) (v1 + _base_vertex);
			_indices_stream[_base_face + i].v[1] = (DTushort) (v2 + _base_vertex);
			_indices_stream[_base_face + i].v[2] = (DTushort) (v3 + _base_vertex);
		}
	
    }
    return size_read;	
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_tri_face_map(BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTuint size_read = 0;
    DTushort num_faces;
	std::string name;
 
    size_read += read(in,name);
    size_read += read(in,num_faces);
		
    for (DTuint i = 0; i < num_faces; ++i) {
		DTushort face;
	
        size_read += read(in,face);
    }
    return size_read;	
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_tri_local(BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTuint size_read = 0;
    DTfloat xx,xy,xz, yx,yy,yz, zx,zy,zz, ox,oy,oz;
    
    size_read += read(in,xx);
    size_read += read(in,xy);
    size_read += read(in,xz);

    size_read += read(in,yx);
    size_read += read(in,yy);
    size_read += read(in,yz);

    size_read += read(in,zx);
    size_read += read(in,zy);
    size_read += read(in,zz);

    size_read += read(in,ox);
    size_read += read(in,oy);
    size_read += read(in,oz);
    
    Vector3 translation(ox,oy,oz);
    left_to_right_handed(translation,translation);
	
	Matrix4 transform(Matrix3::identity(), translation);
	
	//_hierarchy[_current_obj_id]._transform = transform;
	    
    return size_read;	
}
//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_key_f3DS (BinaryFileStream &in, DTuint owner_chunk_size)
{
    return parse_next_chunk(in, owner_chunk_size);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_obj_des (BinaryFileStream &in, DTuint owner_chunk_size)
{
    return parse_next_chunk(in, owner_chunk_size);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_obj_hier (BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTuint size_read = 0;
    
    DTshort       flags1,flags2,parent;
    std::string     name;
    
    size_read += read(in, name);
    size_read += read(in, flags1);
    size_read += read(in, flags2);
    size_read += read(in, parent);
    
    // set object info
    ++_current_hier_id;
    //_hierarchy[_current_hier_id]._parent = parent;
    
    //size_read += parseNextChunk(in, owner_chunk_size - size_read);
    return size_read;
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_edit_material (BinaryFileStream &in, DTuint owner_chunk_size)
{
    return parse_next_chunk(in, owner_chunk_size);
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_mat_name_01 (BinaryFileStream &in, DTuint owner_chunk_size)
{
    DTint size_read = 0;
	std::string materialname;
  
    size_read += read(in,materialname);

    return size_read;
}

//==============================================================================
//==============================================================================

DTuint	ImporterGeometry3DS::parse_next_chunk(BinaryFileStream &in, DTuint owner_chunk_size)
{
    const DTuint    chunk_header_size = 6;	// 6 bytes for data read (chunk_id & chunk_size)
    DTushort        chunk_id;
    DTuint          chunk_size;
    DTuint          offset_in_chunk = 0;    // start after chunk id

    do {
        offset_in_chunk += read(in, chunk_id);
        offset_in_chunk += read(in, chunk_size);
        
        chunk_size -= chunk_header_size;	// Compensate for header

        switch (chunk_id) {
            case CHUNK_MAIN3DS:                 offset_in_chunk += parse_main_3DS(in, chunk_size);			break;
            case CHUNK_EDIT3DS:                 offset_in_chunk += parse_edit_3DS(in, chunk_size);			break;
            case CHUNK_EDITOBJECT:              offset_in_chunk += parse_edit_object(in, chunk_size);		break;

            case CHUNK_OBJ_TRIMESH:             offset_in_chunk += parse_obj_trimesh(in, chunk_size);		break;
            case CHUNK_TRI_VERTEX:              offset_in_chunk += parse_tri_vertex(in, chunk_size);		break;
            case CHUNK_TRI_MAPPINGCOORDS:       offset_in_chunk += parse_tri_mapping_coords(in, chunk_size);break;
            case CHUNK_TRI_FACE:                offset_in_chunk += parse_tri_face(in, chunk_size);			break;
            case CHUNK_TRI_FACE_MAP:            offset_in_chunk += parse_tri_face_map(in, chunk_size);		break;
            case CHUNK_TRI_LOCAL:               offset_in_chunk += parse_tri_local(in, chunk_size);			break;

			case CHUNK_KEYF3DS:					offset_in_chunk += parse_key_f3DS(in, chunk_size);			break;
			case CHUNK_OBJDES:					offset_in_chunk += parse_obj_des(in, chunk_size);			break;
			case CHUNK_OBJHIERARCH:				offset_in_chunk += parse_obj_hier(in, chunk_size);			break;

			case CHUNK_EDIT_MATERIAL:			offset_in_chunk += parse_edit_material(in, chunk_size);		break;
			case CHUNK_MAT_NAME01:				offset_in_chunk += parse_mat_name_01(in, chunk_size);		break;

            default:                            in.seek_g((DTint) chunk_size, BinaryStream::FROM_CURRENT);
                                                offset_in_chunk += chunk_size;		
                                                break;	// Skip chunk
        };
        
        // Check for incorrectly read file
        if (offset_in_chunk > owner_chunk_size) {				
            return offset_in_chunk;
        }

            
    } while (offset_in_chunk < owner_chunk_size);

    return offset_in_chunk;
}

//==============================================================================
//==============================================================================

} // DT3


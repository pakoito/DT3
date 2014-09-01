//==============================================================================
///	
///	File: Mesh.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Graphics/Mesh.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(Mesh)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Mesh::Mesh (void)
{

}

Mesh::Mesh (const Mesh &rhs)
    :   _vertex_stream(rhs._vertex_stream),
        _normals_stream(rhs._normals_stream),
        _tangents_stream(rhs._tangents_stream),
        _colors_stream(rhs._colors_stream),
        _uvs_stream_0(rhs._uvs_stream_0),
        _uvs_stream_1(rhs._uvs_stream_1),
        _weights_index_stream(rhs._weights_index_stream),
        _weights_strength_stream(rhs._weights_strength_stream),
        _indices_stream(rhs._indices_stream)
{

}

Mesh::Mesh (Mesh &&rhs)
    :   _vertex_stream(std::move(rhs._vertex_stream)),
        _normals_stream(std::move(rhs._normals_stream)),
        _tangents_stream(std::move(rhs._tangents_stream)),
        _colors_stream(std::move(rhs._colors_stream)),
        _uvs_stream_0(std::move(rhs._uvs_stream_0)),
        _uvs_stream_1(std::move(rhs._uvs_stream_1)),
        _weights_index_stream(std::move(rhs._weights_index_stream)),
        _weights_strength_stream(std::move(rhs._weights_strength_stream)),
        _indices_stream(std::move(rhs._indices_stream))
{

}
		
Mesh& Mesh::operator = (const Mesh &rhs)
{
    _vertex_stream = rhs._vertex_stream;
    _normals_stream = rhs._normals_stream;
    _tangents_stream = rhs._tangents_stream;
    _colors_stream = rhs._colors_stream;
    _uvs_stream_0 = rhs._uvs_stream_0;
    _uvs_stream_1 = rhs._uvs_stream_1;
    _weights_index_stream = rhs._weights_index_stream;
    _weights_strength_stream = rhs._weights_strength_stream;
    _indices_stream = rhs._indices_stream;

    return (*this);
}	

Mesh& Mesh::operator = (Mesh &&rhs)
{
    _vertex_stream = std::move(rhs._vertex_stream);
    _normals_stream = std::move(rhs._normals_stream);
    _tangents_stream = std::move(rhs._tangents_stream);
    _colors_stream = std::move(rhs._colors_stream);
    _uvs_stream_0 = std::move(rhs._uvs_stream_0);
    _uvs_stream_1 = std::move(rhs._uvs_stream_1);
    _weights_index_stream = std::move(rhs._weights_index_stream);
    _weights_strength_stream = std::move(rhs._weights_strength_stream);
    _indices_stream = std::move(rhs._indices_stream);

    return (*this);
}	

Mesh::~Mesh (void)
{

}

//==============================================================================
//==============================================================================		

void Mesh::generate_normals (void)
{
    ASSERT(_vertex_stream.size() > 0);

    _normals_stream.clear();
    _normals_stream.resize(_vertex_stream.size(), Vector3(0.0F,0.0F,0.0F));

	for (DTuint i = 0; i < _indices_stream.size(); ++i) {
		DTuint ai,bi,ci;
		
		ai = _indices_stream[i].v[0];
		bi = _indices_stream[i].v[1];
		ci = _indices_stream[i].v[2];
		
		ASSERT(ai < _vertex_stream.size());
		ASSERT(bi < _vertex_stream.size());
		ASSERT(ci < _vertex_stream.size());
		
		Vector3 n = Vector3::cross(_vertex_stream[bi] - _vertex_stream[ai], _vertex_stream[ci] - _vertex_stream[ai]);
		n.normalize();
		
		_normals_stream[ai] += n;
		_normals_stream[bi] += n;
		_normals_stream[ci] += n;
	}
	
	for (DTuint i = 0; i < _vertex_stream.size(); ++i)
		if (_normals_stream[i] != Vector3(0.0F,0.0F,0.0F))
			_normals_stream[i].normalize();
		
}
		
//==============================================================================
//==============================================================================		

void Mesh::generate_tangents	(void)
{    
    std::vector<Vector3> tan1;
    std::vector<Vector3> tan2;
    
    tan1.resize(_vertex_stream.size(), Vector3(0.0F,0.0F,0.0F));
    tan2.resize(_vertex_stream.size(), Vector3(0.0F,0.0F,0.0F));

    for (DTuint face_index = 0; face_index < _indices_stream.size(); ++face_index) {
		DTuint i1 = _indices_stream[face_index].v[0];
		DTuint i2 = _indices_stream[face_index].v[1];
		DTuint i3 = _indices_stream[face_index].v[2];
		
		ASSERT(i1 < _vertex_stream.size());
		ASSERT(i2 < _vertex_stream.size());
		ASSERT(i3 < _vertex_stream.size());
		
		Vector3	&v1 = _vertex_stream[i1];
		Vector3	&v2 = _vertex_stream[i2];
		Vector3	&v3 = _vertex_stream[i3];

		Vector2	&w1 = _uvs_stream_0[i1];
		Vector2	&w2 = _uvs_stream_0[i2];
		Vector2	&w3 = _uvs_stream_0[i3];

		DTfloat x1 = v2.x - v1.x;
		DTfloat x2 = v3.x - v1.x;
		DTfloat y1 = v2.y - v1.y;
		DTfloat y2 = v3.y - v1.y;
		DTfloat z1 = v2.z - v1.z;
		DTfloat z2 = v3.z - v1.z; 

		DTfloat s1 = w2.u - w1.u;
		DTfloat s2 = w3.u - w1.u;
		DTfloat t1 = w2.v - w1.v;
		DTfloat t2 = w3.v - w1.v; 

		Vector3 sdir, tdir;

		DTfloat r = s1 * t2 - s2 * t1;
		if (r != 0.0F) {
			r = 1.0F / r;
			sdir = Vector3 ((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			tdir = Vector3 ((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r); 
		} else {
			sdir = Vector3 (0.0F,0.0F,0.0F);
			tdir = Vector3 (0.0F,0.0F,0.0F);		
		}
		

		tan1[i1] = tan1[i1] + sdir;
		tan1[i2] = tan1[i2] + sdir;
		tan1[i3] = tan1[i3] + sdir; 

		tan2[i1] = tan2[i1] + tdir;
		tan2[i2] = tan2[i2] + tdir;
		tan2[i3] = tan2[i3] + tdir; 
    } 

    _tangents_stream.resize(_vertex_stream.size());

    for (DTuint vert_index = 0; vert_index < _vertex_stream.size(); ++vert_index) {
		const Vector3 n = _normals_stream[vert_index];
		const Vector3 t = tan1[vert_index];
		
		Vector3 temp_v;
		DTfloat temp_s;
		
		temp_s = Vector3::dot(n,t);
		temp_v = temp_s * n;
		temp_v = t - temp_v;
		
		if (temp_v.x != 0.0F || temp_v.y != 0.0F || temp_v.z != 0.0F) 
			temp_v.normalize();
				
		_tangents_stream[vert_index] = temp_v;
		
		temp_v = Vector3::cross(n,t);
		temp_s = Vector3::dot(temp_v,tan2[vert_index]);
		if (temp_s < 0.0F)
			_tangents_stream[vert_index] = -_tangents_stream[vert_index];
    } 
}

//==============================================================================
//==============================================================================	

#define TOBITS(x)  *((DTuint*)(&x))

DTuint Mesh::hash_vertex (DTuint i)
{
    DTuint h = 0;
    
    if (_vertex_stream.size() > 0) {
        h ^= TOBITS(_vertex_stream[i].x);
        h ^= TOBITS(_vertex_stream[i].y);
        h ^= TOBITS(_vertex_stream[i].z);
    }

    if (_uvs_stream_0.size() > 0) {
        h ^= TOBITS(_uvs_stream_0[i].u);
        h ^= TOBITS(_uvs_stream_0[i].v);
    }

    if (_uvs_stream_1.size() > 0) {
        h ^= TOBITS(_uvs_stream_1[i].u);
        h ^= TOBITS(_uvs_stream_1[i].v);
    }

    if (_normals_stream.size() > 0) {
        h ^= TOBITS(_normals_stream[i].x);
        h ^= TOBITS(_normals_stream[i].y);
        h ^= TOBITS(_normals_stream[i].z);
    }

    return h;
}

DTboolean Mesh::equal_vertex(DTuint v1, DTuint v2)
{
    if (v1 == v2)
        return true;

    // Test to see if the vertices are the same
    if (_vertex_stream.size() > 0 && (_vertex_stream[v1] != _vertex_stream[v2]))
        return false; 
                            
    if (_normals_stream.size() > 0 && (_normals_stream[v1] != _normals_stream[v2]))
        return false; 

    if (_tangents_stream.size() > 0 && (_tangents_stream[v1] != _tangents_stream[v2]))
        return false; 

    if (_colors_stream.size() > 0 && (_colors_stream[v1] != _colors_stream[v2]))
        return false; 

    if (_uvs_stream_0.size() > 0 && (_uvs_stream_0[v1] != _uvs_stream_0[v2]))
        return false; 
            
    if (_uvs_stream_1.size() > 0 && (_uvs_stream_1[v1] != _uvs_stream_1[v2]))
        return false; 

    if (_weights_index_stream.size() > 0 && (_weights_index_stream[v1] != _weights_index_stream[v2]))
        return false; 

    if (_weights_strength_stream.size() > 0 && (_weights_strength_stream[v1] != _weights_strength_stream[v2]))
        return false; 

    return true;
}

void Mesh::collapse_verts (void)
{
    const DTuint HASH_TABLE_SIZE = 1021;
    DTsize num_old_verts = _vertex_stream.size();
    DTsize num_new_verts = 0;
        
    //
    // Rebuild the mesh
    //
    
    std::vector<Vector3>		vertex_stream;
    std::vector<Vector3>		normals_stream;
    std::vector<Vector3>		tangents_stream;
    std::vector<Color4b>		colors_stream;
    std::vector<Vector2>        uvs_stream_0;
    std::vector<Vector2>        uvs_stream_1;
    std::vector<WeightsIndex>   weights_index_stream;
    std::vector<Vector4>        weights_strength_stream;

    if (_vertex_stream.size() > 0)              vertex_stream.resize(num_old_verts);
    if (_normals_stream.size() > 0)             normals_stream.resize(num_old_verts);
    if (_tangents_stream.size() > 0)            tangents_stream.resize(num_old_verts);
    if (_colors_stream.size() > 0)              colors_stream.resize(num_old_verts);
    if (_uvs_stream_0.size())                   uvs_stream_0.resize(num_old_verts);
    if (_uvs_stream_1.size() > 0)               uvs_stream_1.resize(num_old_verts);
    if (_weights_index_stream.size() > 0)       weights_index_stream.resize(num_old_verts);
    if (_weights_strength_stream.size() > 0)    weights_strength_stream.resize(num_old_verts);
    
    // Hash all vertices
    std::list<std::pair<DTuint, DTuint>> vert_hashes[HASH_TABLE_SIZE];   // new index, old index
    for (DTuint i = 0; i < num_old_verts; ++i) {
    
        DTuint h = hash_vertex(i);
        
        // Add vertex only if unique
        std::list<std::pair<DTuint, DTuint>> &list = vert_hashes[h % HASH_TABLE_SIZE];
        std::list<std::pair<DTuint, DTuint>>::iterator j;
        
        for (j = list.begin(); j != list.end(); ++j) {
            if (j->second == i)
                continue;
        
            if (equal_vertex(j->second, i))
                break;
        }

        // It is unique so add the vertex to the new geometry
        if (j == list.end()) {
            if (vertex_stream.size() > 0)           vertex_stream[num_new_verts] = _vertex_stream[i];
            if (normals_stream.size() > 0)          normals_stream[num_new_verts] = _normals_stream[i];
            if (tangents_stream.size() > 0)			tangents_stream[num_new_verts] = _tangents_stream[i];
            if (colors_stream.size() > 0)           colors_stream[num_new_verts] = _colors_stream[i];
            if (uvs_stream_0.size() > 0)            uvs_stream_0[num_new_verts] = _uvs_stream_0[i];
            if (uvs_stream_1.size() > 0)            uvs_stream_1[num_new_verts] = _uvs_stream_1[i];
            if (weights_index_stream.size() > 0)    weights_index_stream[num_new_verts] = _weights_index_stream[i];
            if (weights_strength_stream.size() > 0) weights_strength_stream[num_new_verts] = _weights_strength_stream[i];
                    
            vert_hashes[h % HASH_TABLE_SIZE].push_back( std::make_pair(num_new_verts, i) );
            
            ++num_new_verts;
        }
    }
    
    // Time to re-wire the face indices
    for (DTuint f = 0; f < _indices_stream.size(); ++f) {
        DTuint &v0 = _indices_stream[f].v[0];
        DTuint &v1 = _indices_stream[f].v[1];
        DTuint &v2 = _indices_stream[f].v[2];
        
        ASSERT(v0 < num_old_verts);
        ASSERT(v1 < num_old_verts);
        ASSERT(v2 < num_old_verts);
    
        DTuint h0 = hash_vertex(v0);
        DTuint h1 = hash_vertex(v1);
        DTuint h2 = hash_vertex(v2);
        
        // Add vertex only if unique
        std::list<std::pair<DTuint, DTuint>> &list0 = vert_hashes[h0 % HASH_TABLE_SIZE];
        std::list<std::pair<DTuint, DTuint>> &list1 = vert_hashes[h1 % HASH_TABLE_SIZE];
        std::list<std::pair<DTuint, DTuint>> &list2 = vert_hashes[h2 % HASH_TABLE_SIZE];

        std::list<std::pair<DTuint, DTuint>>::iterator j;
        
        for (j = list0.begin(); j != list0.end(); ++j) {
            if (equal_vertex(j->second, v0)) {
                v0 = j->first;
                break;
            }
        }
                
        ASSERT(j != list0.end());
        ASSERT(v0 < num_new_verts);

        for (j = list1.begin(); j != list1.end(); ++j) {
            if (equal_vertex(j->second, v1)) {
                v1 = j->first;
                break;
            }
        }

        ASSERT(j != list1.end());
        ASSERT(v1 < num_new_verts);

        for (j = list2.begin(); j != list2.end(); ++j) {
            if (equal_vertex(j->second, v2)) {
                v2 = j->first;
                break;
            }
        }

        ASSERT(j != list2.end());
        ASSERT(v2 < num_new_verts);

    } 
    
                
	if (vertex_stream.size() > 0)           vertex_stream.resize(num_new_verts);
	if (normals_stream.size() > 0)          normals_stream.resize(num_new_verts);
	if (tangents_stream.size() > 0)			tangents_stream.resize(num_new_verts);
	if (colors_stream.size() > 0)           colors_stream.resize(num_new_verts);
	if (uvs_stream_0.size() > 0)            uvs_stream_0.resize(num_new_verts);
	if (uvs_stream_1.size() > 0)            uvs_stream_1.resize(num_new_verts);
	if (weights_index_stream.size() > 0)    weights_index_stream.resize(num_new_verts);
	if (weights_strength_stream.size() > 0) weights_strength_stream.resize(num_new_verts);
    
    _vertex_stream = vertex_stream;
    _normals_stream = normals_stream;
    _tangents_stream = tangents_stream;
    _colors_stream = colors_stream;
    _uvs_stream_0 = uvs_stream_0;
    _uvs_stream_1 = uvs_stream_1;
    _weights_index_stream = weights_index_stream;
    _weights_strength_stream = weights_strength_stream;
    
    LOG_MESSAGE << "Old size: " << num_old_verts;
    LOG_MESSAGE << "New size: " << num_new_verts;

}

//==============================================================================
//==============================================================================		

void Mesh::generate_weights (void)
{
	_weights_index_stream.resize(_vertex_stream.size(), WeightsIndex(0, 0, 0, 0));
	_weights_strength_stream.resize(_vertex_stream.size(), Vector4(1.0F, 0.0F, 0.0F, 0.0F));
}			
	
//==============================================================================
//==============================================================================

} // DT3

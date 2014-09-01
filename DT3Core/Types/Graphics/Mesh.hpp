#ifndef DT3_MESH
#define DT3_MESH
//==============================================================================
///	
///	File: Mesh.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Types/Math/WeightsIndex.hpp"
#include "DT3Core/Types/Math/Triangle.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Mesh: public BaseClass {
    public:		
        DEFINE_TYPE(Mesh,BaseClass)
		DEFINE_CREATE_AND_CLONE
         
                                        Mesh                (void);
                                        Mesh                (const Mesh &rhs);
                                        Mesh                (Mesh &&rhs);
        Mesh&                           operator =          (const Mesh &rhs);
        Mesh&                           operator =          (Mesh &&rhs);
                                        ~Mesh               (void);
		
    public:
    
		DEFINE_ACCESSORS(vertex_stream, set_vertex_stream, std::vector<Vector3>, _vertex_stream);
		DEFINE_ACCESSORS(normals_stream, set_normals_stream, std::vector<Vector3>, _normals_stream);
		DEFINE_ACCESSORS(tangents_stream, set_tangents_stream, std::vector<Vector3>, _tangents_stream);
		DEFINE_ACCESSORS(color_stream, set_color_stream, std::vector<Color4b>, _colors_stream);
		DEFINE_ACCESSORS(uv_stream0, set_uv_stream0, std::vector<Vector2>, _uvs_stream_0);
		DEFINE_ACCESSORS(uv_stream1, set_uv_stream1, std::vector<Vector2>, _uvs_stream_1);
		DEFINE_ACCESSORS(weights_index_stream, set_weights_index_stream, std::vector<WeightsIndex>, _weights_index_stream);
		DEFINE_ACCESSORS(weights_strength_stream, set_weights_strength_stream, std::vector<Vector4>, _weights_strength_stream);

		DEFINE_ACCESSORS(index_stream, set_index_stream, std::vector<Triangle>, _indices_stream);


		/// Generate some normals for the geometry
		void                            generate_normals    (void);

		/// Generate some tangents for the geometry
		void                            generate_tangents   (void);

		/// Generate some weights for the geometry
		void                            generate_weights    (void);
    
		/// Save some space by removing redundant vertices
		void                            collapse_verts      (void);
    

        // Accessors for mesh name
        DEFINE_ACCESSORS(name, set_name, std::string, _name)
    
	private:
        DTuint                          hash_vertex         (DTuint i);
        DTboolean                       equal_vertex        (DTuint v1, DTuint v2);

        std::string                     _name;
    
		std::vector<Vector3>            _vertex_stream;
		std::vector<Vector3>            _normals_stream;
		std::vector<Vector3>            _tangents_stream;
		std::vector<Color4b>            _colors_stream;
		std::vector<Vector2>            _uvs_stream_0;
		std::vector<Vector2>            _uvs_stream_1;
		std::vector<WeightsIndex>       _weights_index_stream;
		std::vector<Vector4>            _weights_strength_stream;
		std::vector<Triangle>           _indices_stream;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

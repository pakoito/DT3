#ifndef DT3_IMPORTERGEOMETRYTWM
#define DT3_IMPORTERGEOMETRYTWM
//==============================================================================
///	
///	File: ImporterGeometryTWM.hpp
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
#include "DT3Core/Types/Math/Weights.hpp"
#include "DT3Core/Types/Math/Triangle.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Animation/SkeletonJoint.hpp"
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

class ImporterGeometryTWM: public ImporterGeometry {
    public:
        DEFINE_TYPE(ImporterGeometryTWM,ImporterGeometry)
		DEFINE_CREATE
         
										ImporterGeometryTWM	(void);	
    
	private:
										ImporterGeometryTWM	(const ImporterGeometryTWM &rhs);
        ImporterGeometryTWM &			operator =			(const ImporterGeometryTWM &rhs);
    
    public:
        virtual							~ImporterGeometryTWM(void);
                
    public:        	
		/// Imports an geometry into an GeometryResource
		/// \param target object to import geometry into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import				(GeometryResource *target, std::string args);

    private:	
	
		enum {
			MAGIC = 0x5E11E70D,
			
			FILE = 0,
				MESHES = 1,
					MESHES_MESH = 2,
						MESHES_MESH_NAME = 3,
						MESHES_MESH_POSITIONS = 4,
						MESHES_MESH_NORMALS = 5,
						MESHES_MESH_UV_SETS = 6,
							MESHES_MESH_UVS = 7,
						MESHES_MESH_SKINNING = 8,
							MESHES_MESH_SKINNING_JOINTS = 9,
							MESHES_MESH_SKINNING_INFLUENCES = 10,
						MESHES_MESH_INDICES = 12,
				SKELETON = 13
		};
		
		//
		// Meshes
		//
		
		struct UVset {
			std::vector<Vector2>            _uvs;
		};
				
		struct MeshData {
			std::string                     _name;
			
			std::vector<Vector3>			_vertices;
			std::vector<Vector3>			_normals;
			std::vector<UVset>              _uv_sets;
			
			std::vector<std::string>        _joint_names;
			std::vector<Weights>			_weights;
			
			std::vector<Triangle>           _indices;
		};
		
		std::vector<MeshData>               _meshes;
		
		//
		// Skeleton
		//
		
		struct Joint {
			std::vector<Joint>              _children;
			std::string                     _name;
			Matrix4                         _local_transform;
			Matrix4                         _world_transform;
		};
		
		struct SkeletonData {
			std::vector<Joint>              _joints;
		};
		
		SkeletonData                        _skeleton;
		
		
		// Some book keeping
		DTuint                              _normals_count;
		DTuint                              _uv_set_0_count;
		DTuint                              _uv_set_1_count;
		DTuint                              _weights_count;
		
		
		void	read_mesh_name		(BinaryFileStream &file, DTuint remaining_size, std::string &name);
		void	read_mesh_positions	(BinaryFileStream &file, DTuint remaining_size, std::vector<Vector3> &positions);
		void	read_mesh_normals   (BinaryFileStream &file, DTuint remaining_size, std::vector<Vector3> &normals);
		void	read_mesh_uvs       (BinaryFileStream &file, DTuint remaining_size, std::vector<Vector2> &uvs);
		void	read_mesh_uv_sets   (BinaryFileStream &file, DTuint remaining_size, std::vector<UVset> &uvs_sets);
		void	read_mesh_joints    (BinaryFileStream &file, DTuint remaining_size, std::vector<std::string> &joints);
		void	read_mesh_influences(BinaryFileStream &file, DTuint remaining_size, std::vector<Weights> &weights);
		void	read_mesh_skinning	(BinaryFileStream &file, DTuint remaining_size, std::vector<std::string> &joints, std::vector<Weights> &weights);
		void	read_mesh_indices   (BinaryFileStream &file, DTuint remaining_size, std::vector<Triangle> &indices);
		void	read_mesh			(BinaryFileStream &file, DTuint remaining_size, MeshData &mesh);
		void	read_meshes			(BinaryFileStream &file, DTuint remaining_size, std::vector<MeshData> &meshes);
		
		void	read_skeleton_joints(BinaryFileStream &file, DTuint remaining_size, std::vector<Joint> &joints);
		void	read_skeleton		(BinaryFileStream &file, DTuint remaining_size, SkeletonData &skeleton);
		
		void	read_file			(BinaryFileStream &file, DTuint remaining_size);


		// Recursive function to build the skeleton
		void	build_skeleton		(std::vector<std::string> &joint_names, std::vector<Joint> &src_joints, std::vector<SkeletonJoint> &dst_joints);

};

//==============================================================================
//==============================================================================

} // DT3

#endif

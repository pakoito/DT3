//==============================================================================
///	
///	File: ImporterGeometryTWM.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterGeometryTWM.hpp"
#include "DT3Core/Resources/ResourceTypes/GeometryResource.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterGeometryTWM,twm)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterGeometryTWM::ImporterGeometryTWM (void)		
{    

}
			
ImporterGeometryTWM::~ImporterGeometryTWM (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr	ImporterGeometryTWM::import(GeometryResource *target, std::string args)
{
	FilePath pathname(target->path());
	
    // open the file
    BinaryFileStream	file;
	FileManager::open(file, pathname, true);
     
    // 
	// Read_ in header
	//
	
	DTuint magic;
	file >> magic;
    if (magic != MAGIC) {
        return DT3_ERR_FILE_WRONG_TYPE;
    }
	
	DTuint version;
	file >> version;
	
	//
	// Read_ in data
	//
	
	_normals_count = 0;
	_uv_set_0_count = 0;
	_uv_set_1_count = 0;
	_weights_count = 0;
	
	DTint section,size;
	file >> section >> size;
	switch(section) {
		case FILE:		read_file(file, size);			break;
		default:		WARNINGMSG("Invalid section");	break;
	};
	
	//
	// Build Streams
	//
    
	// Calculate sizes needed
	DTuint joints_size = 0;
	
	for (DTuint i = 0; i < _meshes.size(); ++i) {
		joints_size += _meshes[i]._joint_names.size();
	}

	// allocate joint names, force first one to always be identity
	std::vector<std::string>	joint_names;
	joint_names.resize(joints_size);	
	DTuint joint_offset = 0;	// Account for identity above

	for (DTuint i = 0; i < _meshes.size(); ++i) {
		MeshData &meshdata = _meshes[i];

        if (meshdata._vertices.size() == 0)
            continue;
    
        std::shared_ptr<Mesh> mesh = Mesh::create();
		
		// copy joint names
		for (DTuint j = 0; j < meshdata._joint_names.size(); ++j) {
			joint_names[joint_offset + j] = meshdata._joint_names[j];
		}
        
        std::vector<Vector3> vertex_stream;
        std::vector<Vector3> normals_stream;
        std::vector<Vector2> uvs_stream_0;
        std::vector<Vector2> uvs_stream_1;
        std::vector<Vector4> weights_strength;
        std::vector<WeightsIndex> weights_index;
        std::vector<Triangle> indices;
        
        vertex_stream = meshdata._vertices;
        normals_stream = meshdata._normals;
        uvs_stream_0 = meshdata._uv_sets[0]._uvs;
        uvs_stream_1 = meshdata._uv_sets[1]._uvs;
        indices = meshdata._indices;
		
        if (meshdata._weights.size()) {
            
            weights_index.resize(meshdata._weights.size());
            weights_strength.resize(meshdata._weights.size());
        
            for (DTuint j = 0; j < meshdata._weights.size(); ++j) {
                weights_strength[j] = Vector4(      meshdata._weights[j].weight_1(),
                                                    meshdata._weights[j].weight_2(),
                                                    meshdata._weights[j].weight_3(),
                                                    meshdata._weights[j].weight_4()    );

                weights_index[j] = WeightsIndex(    meshdata._weights[j].bone_1(),
                                                    meshdata._weights[j].bone_2(),
                                                    meshdata._weights[j].bone_3(),
                                                    meshdata._weights[j].bone_4()    );
            
            }
        }

			

        // Some stuff we can generate
        if (normals_stream.size() == 0)     mesh->generate_normals();
        if (uvs_stream_0.size() == 0)       mesh->generate_tangents();
        
        // Add the mesh
        target->add_mesh(mesh);

		joint_offset += (DTushort) meshdata._joint_names.size();
	}
		 	
	
	// Build skeleton
	std::vector<SkeletonJoint> s;
	build_skeleton(joint_names, _skeleton._joints, s);
    
    Skeleton skeleton;
    skeleton.set_skeleton(s);
    
	target->set_skeleton(skeleton);

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::build_skeleton(   std::vector<std::string> &joint_names,
                                            std::vector<Joint> &src_joints,
                                            std::vector<SkeletonJoint> &dst_joints)
{	
	if (src_joints.size() <= 0)
		return;
		
	// allocate space for destination joints
	dst_joints.resize(src_joints.size());

	for (DTuint i = 0; i < src_joints.size(); ++i) {
		Joint &joint = src_joints[i];
			
		// find index of joint in names
		DTsize joint_index = 0;
        std::size_t j;
		for (j = 0; j < joint_names.size(); ++j) {
			if (joint_names[j] == joint._name) {
				joint_index = j;
				break;
			}
		}
		
        if (j == joint_names.size()) {
            LOG_MESSAGE << "Warning: Unknown joint found in skeleton: " << joint._name;
            joint_names.push_back(joint._name);
            joint_index = joint_names.size()-1;
        }

		// Buid copy of joint
		SkeletonJoint &dest_joint = dst_joints[i];
		dest_joint.set_name(joint._name);
		dest_joint.set_joint_index( static_cast<DTuint>(joint_index) );
		dest_joint.set_local_transform(joint._local_transform);
		dest_joint.set_world_transform(joint._world_transform);
		
		// Build child joints
		std::vector<SkeletonJoint> dest_joints;
		build_skeleton(joint_names, joint._children, dest_joints);
		dest_joint.set_children(dest_joints);
	}
}


//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_mesh_name(BinaryFileStream &file, DTuint remaining_size, std::string &name)
{	
	// Read_ string
	file >> name;
	LOG_MESSAGE << "Reading name: " << name; 
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_mesh_positions(BinaryFileStream &file, DTuint remaining_size, std::vector<Vector3> &positions)
{
	// Read_ positions
	DTuint size;
	file >> size;
	positions.resize(size);
	
	LOG_MESSAGE << "Reading positions: " << (DTsize) positions.size();

	for (DTuint i = 0; i < positions.size(); ++i) {
		file >> positions[i].x;
		file >> positions[i].y;
		file >> positions[i].z;
	}
}

void ImporterGeometryTWM::read_mesh_normals(BinaryFileStream &file, DTuint remaining_size, std::vector<Vector3> &normals)
{
	// Read_ normals
	DTuint size;
	file >> size;
	normals.resize(size);
	
	LOG_MESSAGE << "Reading normals: " << (DTsize) normals.size();

	for (DTuint i = 0; i < normals.size(); ++i) {
		file >> normals[i].x;
		file >> normals[i].y;
		file >> normals[i].z;
	}
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_mesh_uvs(BinaryFileStream &file, DTuint remaining_size, std::vector<Vector2> &uvs)
{
	// Read_ uvs
	DTuint size;
	file >> size;
	uvs.resize(size);
	
	LOG_MESSAGE << "Reading uvs: " << (DTsize) uvs.size();

	for (DTuint i = 0; i < uvs.size(); ++i) {
		file >> uvs[i].u;
		file >> uvs[i].v;
	}
}

void ImporterGeometryTWM::read_mesh_uv_sets(BinaryFileStream &file, DTuint remaining_size, std::vector<UVset> &uvs_sets)
{	
	//DTuint start_pos = file.getG();

	// Read_ uvs
	DTuint size;
	file >> size;
	uvs_sets.resize(size);

	LOG_MESSAGE << "Reading uv sets: " << (DTsize) uvs_sets.size();
	
	for (DTuint i = 0; i < uvs_sets.size(); ++i) {
	
		//while (file.getG() - start_pos < remaining_size) {
			DTint section, section_size;
			file >> section >> section_size;
		
			switch(section) {
				case MESHES_MESH_UVS:		read_mesh_uvs(file, section_size, uvs_sets[i]._uvs);    break;
				default:					WARNINGMSG("Invalid section");							break;
			};
		//}

	}
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_mesh_joints(BinaryFileStream &file, DTuint remaining_size, std::vector<std::string> &joints)
{
	// Read_ joints
	DTuint size;
	file >> size;
	joints.resize(size);

	LOG_MESSAGE << "Reading joints: " << (DTsize) joints.size();
	
	for (DTuint i = 0; i < joints.size(); ++i) {
		file >> joints[i];
	}
}

void ImporterGeometryTWM::read_mesh_influences(BinaryFileStream &file, DTuint remaining_size, std::vector<Weights> &weights)
{	
	// Read_ influences
	DTuint size;
	file >> size;
	weights.resize(size);

	LOG_MESSAGE << "Reading weights: " << (DTsize) weights.size();
	
	for (DTuint i = 0; i < weights.size(); ++i) {
		DTuint	b[4];
		DTfloat	w[4];
			
		file >> b[0];
		file >> w[0];
		file >> b[1];
		file >> w[1];
		file >> b[2];
		file >> w[2];
		file >> b[3];
		file >> w[3];
		
		weights[i].set( (DTushort) b[0],w[0],
                        (DTushort) b[1],w[1],
                        (DTushort) b[2],w[2],
                        (DTushort) b[3],w[3]);
	}
}

void ImporterGeometryTWM::read_mesh_skinning(BinaryFileStream &file, DTuint remaining_size, std::vector<std::string> &joints, std::vector<Weights> &weights)
{
	DTsize start_pos = file.g();

	//
	// Read_ out skinning info
	//
	
	while (file.g() - start_pos < remaining_size) {
		DTint section, size;
		file >> section >> size;
	
		switch(section) {
			case MESHES_MESH_SKINNING_JOINTS:		read_mesh_joints(file, size, joints);       break;
			case MESHES_MESH_SKINNING_INFLUENCES:	read_mesh_influences(file, size, weights);	break;
			default:								WARNINGMSG("Invalid section");				break;
		};
	}
	
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_mesh_indices(BinaryFileStream &file, DTuint remaining_size, std::vector<Triangle> &indices)
{
	// Read_ triangle indices
	DTuint size;
	file >> size;
	indices.resize(size);

	LOG_MESSAGE << "Reading indices: " << (DTsize) indices.size();
	
	for (DTuint i = 0; i < indices.size(); ++i) {
		DTushort i1,i2,i3;
	
		file >> i1;
		file >> i2;
		file >> i3;
		indices[i].v[0] = i1;
		indices[i].v[1] = i2;
		indices[i].v[2] = i3;
	}
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_mesh(BinaryFileStream &file, DTuint remaining_size, MeshData &mesh)
{	
	DTsize start_pos = file.g();

	//
	// Read_ out mesh parts
	//
	
	// Make sure there are 2 uv sets allocated just in case	
	while (file.g() - start_pos < remaining_size) {
		DTint section, size;
		file >> section >> size;
	
		switch(section) {
			case MESHES_MESH_NAME:		read_mesh_name(file, size, mesh._name);                             break;
			case MESHES_MESH_POSITIONS:	read_mesh_positions(file, size, mesh._vertices);					break;
			case MESHES_MESH_NORMALS:	read_mesh_normals(file, size, mesh._normals);						break;
			case MESHES_MESH_UV_SETS:	read_mesh_uv_sets(file, size, mesh._uv_sets);						break;
			case MESHES_MESH_SKINNING:	read_mesh_skinning(file, size, mesh._joint_names, mesh._weights);   break;
			case MESHES_MESH_INDICES:	read_mesh_indices(file, size, mesh._indices);						break;
			default:					WARNINGMSG("Invalid section");                                      break;
		};
	}

	if (mesh._uv_sets.size() > 0)	++_uv_set_0_count;
	if (mesh._uv_sets.size() > 1)	++_uv_set_1_count;
	if (mesh._weights.size() > 0)	++_weights_count;
	if (mesh._normals.size() > 0)	++_normals_count;
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_meshes(BinaryFileStream &file, DTuint remaining_size, std::vector<MeshData> &meshes)
{	
	//DTuint start_pos = file.getG();

	//
	// Meshes header
	//
	DTuint size;
	file >> size;		
	meshes.resize(size);

	//
	// Individual meshes
	//

	for (DTuint i = 0; i < meshes.size(); ++i) {
	
		//while (file.getG() - start_pos < remaining_size) {
			DTint section, section_size;
			file >> section >> section_size;
		
			switch(section) {
				case MESHES_MESH:	read_mesh(file, section_size, meshes[i]);	break;
				default:			WARNINGMSG("Invalid section");				break;
			};
		//}

	}
}

//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_skeleton_joints (BinaryFileStream &file, DTuint remaining_size, std::vector<Joint> &joints)
{
    if (remaining_size == 0)
        return;

	//
	// Joints list
	//
	
	DTuint size;
	file >> size;
	joints.resize(size);
	
	for (DTuint i = 0; i < joints.size(); ++i) {
		file >> joints[i]._name;
		
		file >> joints[i]._local_transform._m11 >> joints[i]._local_transform._m21 >> joints[i]._local_transform._m31 >> joints[i]._local_transform._m41;
		file >> joints[i]._local_transform._m12 >> joints[i]._local_transform._m22 >> joints[i]._local_transform._m32 >> joints[i]._local_transform._m42;
		file >> joints[i]._local_transform._m13 >> joints[i]._local_transform._m23 >> joints[i]._local_transform._m33 >> joints[i]._local_transform._m43;
		file >> joints[i]._local_transform._m14 >> joints[i]._local_transform._m24 >> joints[i]._local_transform._m34 >> joints[i]._local_transform._m44;

		file >> joints[i]._world_transform._m11 >> joints[i]._world_transform._m21 >> joints[i]._world_transform._m31 >> joints[i]._world_transform._m41;
		file >> joints[i]._world_transform._m12 >> joints[i]._world_transform._m22 >> joints[i]._world_transform._m32 >> joints[i]._world_transform._m42;
		file >> joints[i]._world_transform._m13 >> joints[i]._world_transform._m23 >> joints[i]._world_transform._m33 >> joints[i]._world_transform._m43;
		file >> joints[i]._world_transform._m14 >> joints[i]._world_transform._m24 >> joints[i]._world_transform._m34 >> joints[i]._world_transform._m44;

		
		read_skeleton_joints (file, remaining_size, joints[i]._children);
	}

}

void ImporterGeometryTWM::read_skeleton (BinaryFileStream &file, DTuint remaining_size, SkeletonData &skeleton)
{
	read_skeleton_joints(file, remaining_size, skeleton._joints);
}


//==============================================================================
//==============================================================================

void ImporterGeometryTWM::read_file (BinaryFileStream &file, DTuint remaining_size)
{
	DTsize start_pos = file.g();

	//
	// Scene header
	//
	
	while (file.g() - start_pos < remaining_size) {
		DTint section, size;
		file >> section >> size;
	
		switch(section) {
			case MESHES:		read_meshes(file, size, _meshes);		break;
			case SKELETON:		read_skeleton(file, size, _skeleton);	break;
			default:			WARNINGMSG("Invalid section");			break;
		};
	}
	
}

//==============================================================================
//==============================================================================

} // DT3


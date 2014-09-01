//==============================================================================
///	
///	File: GeometryResource_cmd.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Command.hpp"

#ifdef DT3_COMMANDS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Graphics/Mesh.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Resources/ResourceTypes/GeometryResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class GeometryResource_cmd: public Command {
    public:
        DEFINE_TYPE(GeometryResource_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("MeshToTWM", &GeometryResource_cmd::do_mesh_to_TWM);
        }

        static CommandResult do_mesh_to_TWM (CommandContext &ctx, const CommandParams &p);
        
    private:
        static void    write_mesh_name      (BinaryFileStream &file, std::shared_ptr<Mesh> mesh);
        static void    write_mesh_positions (BinaryFileStream &file, std::shared_ptr<Mesh> mesh);
        static void    write_mesh_normals   (BinaryFileStream &file, std::shared_ptr<Mesh> mesh);
        static void    write_mesh_uvs       (BinaryFileStream &file, std::shared_ptr<Mesh> mesh);
        static void    write_mesh_uv_sets   (BinaryFileStream &file, std::shared_ptr<Mesh> mesh);
        static void    write_mesh_indices   (BinaryFileStream &file, std::shared_ptr<Mesh> mesh);
        static void    write_mesh           (BinaryFileStream &file, std::shared_ptr<Mesh> mesh);
        static void    write_meshes         (BinaryFileStream &file, const std::shared_ptr<GeometryResource> &geo);




};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(GeometryResource_cmd)

//==============================================================================
//==============================================================================

namespace {
    const DTint MAGIC = 0x5E11E70D;								// Smellytoad
    const DTint VERSION = (1 << 24) | (0 << 16) | (0 << 16);		// 1.0.0

    const DTint FILE = 0;
        const DTint MESHES = 1;
            const DTint MESHES_MESH = 2;
                const DTint MESHES_MESH_NAME = 3;
                const DTint MESHES_MESH_POSITIONS = 4;
                const DTint MESHES_MESH_NORMALS = 5;
                const DTint MESHES_MESH_UV_SETS = 6;
                    const DTint MESHES_MESH_UVS = 7;
//                const DTint MESHES_MESH_SKINNING = 8;
//                    const DTint MESHES_MESH_SKINNING_JOINTS = 9;
//                    const DTint MESHES_MESH_SKINNING_INFLUENCES = 10;
                const DTint MESHES_MESH_INDICES = 12;
                
//        const DTint SKELETON = 13;
};

//==============================================================================
// Useful macros for writing out sections
//==============================================================================

#define BEGIN_EXPORT_SECTION(S)											\
	file << (DTint) S;													\
	DTsize size_location = file.p();                                 \
	file << (DTint) 0;						
	
#define END_EXPORT_SECTION												\
	DTsize save_location = file.p();                                 \
	file.seek_p(size_location, BinaryFileStream::FROM_BEGINNING);		\
	file << (DTint) (save_location - size_location - sizeof(DTint));	\
	file.seek_p(save_location, BinaryFileStream::FROM_BEGINNING);

//==============================================================================
//==============================================================================

void GeometryResource_cmd::write_mesh_name(BinaryFileStream &file, std::shared_ptr<Mesh> mesh)
{
	BEGIN_EXPORT_SECTION(MESHES_MESH_NAME);
	
	// Write string
	LOG_MESSAGE << "Writing name: " << mesh->name();
	file << mesh->name();
	
	END_EXPORT_SECTION;
}

//==============================================================================
//==============================================================================

void GeometryResource_cmd::write_mesh_positions(BinaryFileStream &file, std::shared_ptr<Mesh> mesh)
{
	BEGIN_EXPORT_SECTION(MESHES_MESH_POSITIONS);
    
    std::vector<Vector3> vertices = mesh->vertex_stream();
	
	// Write vertices
	file << static_cast<DTuint>(vertices.size());
	LOG_MESSAGE << "Writing positions: " << (DTsize) vertices.size();
	
	for (DTuint i = 0; i < vertices.size(); ++i) {
		file << static_cast<DTfloat>(vertices[i].x);
		file << static_cast<DTfloat>(vertices[i].y);
		file << static_cast<DTfloat>(vertices[i].z);
	}

	END_EXPORT_SECTION;
}

void GeometryResource_cmd::write_mesh_normals(BinaryFileStream &file, std::shared_ptr<Mesh> mesh)
{
	BEGIN_EXPORT_SECTION(MESHES_MESH_NORMALS);

    std::vector<Vector3> normals = mesh->normals_stream();

	// Write normals
	file << static_cast<DTuint>(normals.size());
	LOG_MESSAGE << "Writing normals: " << (DTsize) normals.size();

	for (DTuint i = 0; i < normals.size(); ++i) {
		file << static_cast<DTfloat>(normals[i].x);
		file << static_cast<DTfloat>(normals[i].y);
		file << static_cast<DTfloat>(normals[i].z);
    }
	
	END_EXPORT_SECTION;
}

//==============================================================================
//==============================================================================

void GeometryResource_cmd::write_mesh_uvs(BinaryFileStream &file, std::shared_ptr<Mesh> mesh)
{
	BEGIN_EXPORT_SECTION(MESHES_MESH_UVS);
    
    std::vector<Vector2> uvs = mesh->uv_stream0();
	
	// Write uvs
	file << static_cast<DTuint>(uvs.size());
	LOG_MESSAGE << "Writing uvs: " << (DTsize) uvs.size();

	for (DTuint i = 0; i < uvs.size(); ++i) {
		file << static_cast<DTfloat>(uvs[i].u);
		file << static_cast<DTfloat>(uvs[i].v);
	}
	
	END_EXPORT_SECTION;
}

void GeometryResource_cmd::write_mesh_uv_sets(BinaryFileStream &file, std::shared_ptr<Mesh> mesh)
{
	BEGIN_EXPORT_SECTION(MESHES_MESH_UV_SETS);
	
	// Write uvs
	file << static_cast<DTuint>(1);
	LOG_MESSAGE << "Writing uv sets: " << 1;
	
    write_mesh_uvs(file, mesh);
	
	END_EXPORT_SECTION;
}

//==============================================================================
//==============================================================================

void GeometryResource_cmd::write_mesh_indices(BinaryFileStream &file, std::shared_ptr<Mesh> mesh)
{
	BEGIN_EXPORT_SECTION(MESHES_MESH_INDICES);
    
    std::vector<Triangle> indices = mesh->index_stream();

	// Write triangle indices
	file << static_cast<DTuint>(indices.size());
	LOG_MESSAGE << "Writing indices: " << (DTsize) indices.size();
	
	for (DTuint i = 0; i < indices.size(); ++i) {
		file << indices[i].v[0];
		file << indices[i].v[1];
		file << indices[i].v[2];
    }
	
	END_EXPORT_SECTION;
}

//==============================================================================
//==============================================================================

void GeometryResource_cmd::write_mesh(BinaryFileStream &file, std::shared_ptr<Mesh> mesh)
{
	BEGIN_EXPORT_SECTION(MESHES_MESH);
	
	//
	// Write out mesh parts
	//
	
	write_mesh_name(file, mesh);
	write_mesh_positions(file, mesh);
	write_mesh_normals(file, mesh);
	write_mesh_uv_sets(file, mesh);
	write_mesh_indices(file, mesh);
	
	END_EXPORT_SECTION;
}

//==============================================================================
//==============================================================================

void GeometryResource_cmd::write_meshes(BinaryFileStream &file, const std::shared_ptr<GeometryResource> &geo)
{
	BEGIN_EXPORT_SECTION(MESHES);
	
	//
	// Meshes header
	//
    
    const std::vector<std::shared_ptr<Mesh>>& meshes = geo->meshes ();
    
    file << static_cast<DTuint>(meshes.size());

    for (DTuint i = 0; i < meshes.size(); ++i) {
        write_mesh(file, meshes[i]);
    }
	
	END_EXPORT_SECTION;
}

//==============================================================================
//==============================================================================

CommandResult GeometryResource_cmd::do_mesh_to_TWM (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: MeshToTWM srcpath dstpath", CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<GeometryResource> geo = GeometryResource::create();
    DTerr err = geo->import(FilePath(p[1]), "");
    if (err != DT3_ERR_NONE) {
        CommandResult cr;
        cr.set_message("Unable to convert " + p[1]);
        cr.set_result(false);
        cr.set_update_hint(CommandResult::UPDATE_NONE);
        return cr;
    }
        
    //
	// Open a binary file
	//
	
	BinaryFileStream file;
    FileManager::open(file, FilePath(p[2]), false);
	
	//
	// Write header
	//
		
	file << MAGIC;
	file << VERSION;
	
	BEGIN_EXPORT_SECTION(FILE);

	write_meshes(file, geo);
	
	END_EXPORT_SECTION;

	file.close();
    
    
    CommandResult cr;
    cr.set_message("Converted " + p[1] + " to " + p[2]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_NONE);
    return cr;
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS


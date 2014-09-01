//==============================================================================
///	
///	File: Factory.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveBinaryReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveTextReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::shared_ptr<Callback<std::shared_ptr<BaseClass>>> Factory::_factory_created_callback;

//==============================================================================
/// This is to make sure that the map gets initialized because all of the
/// objects are registered in random orders and rely on this map. The first
/// time you use a static variable, it is initialized.
//==============================================================================

std::map<std::string, std::shared_ptr<CreatorBase>>&   factory_map	(void)
{
	static std::map<std::string, std::shared_ptr<CreatorBase>> factory_map;
	return factory_map;
}

std::map<std::string, std::shared_ptr<CreatorBase>>&   unit_test_map	(void)
{
	static std::map<std::string, std::shared_ptr<CreatorBase>> factory_map;
	return factory_map;
}

std::map<std::string, std::shared_ptr<CreatorBase>>&   command_map	(void)
{
	static std::map<std::string, std::shared_ptr<CreatorBase>> factory_map;
	return factory_map;
}

std::map<std::string, std::shared_ptr<CreatorBase>>&   factory_importer_map	(void)
{
	static std::map<std::string, std::shared_ptr<CreatorBase>> factory_map;
	return factory_map;
}

std::map<std::string,std::set<std::string>>&           placeable_map	(void)
{
	static std::map<std::string,std::set<std::string>> placeable_map;
	return placeable_map;
}

std::map<std::string,std::set<std::string>>&           component_map	(void)
{
	static std::map<std::string,std::set<std::string>> component_map;
	return component_map;
}

std::map<std::string,std::shared_ptr<CreatorBase>>&    world_map	(void)
{
	static std::map<std::string,std::shared_ptr<CreatorBase>> world_map;
	return world_map;
}

std::map<std::string,std::set<std::string>>&           script_map	(void)
{
	static std::map<std::string,std::set<std::string>> script_map;
	return script_map;
}

std::map<std::string, std::shared_ptr<CreatorBase>>&   tool_map	(void)
{
	static std::map<std::string, std::shared_ptr<CreatorBase>> factory_map;
	return factory_map;
}

//==============================================================================
//==============================================================================

std::map<std::string, std::string>&                     adapter_map	(void)
{
	static std::map<std::string, std::string> adapter_map;
	return adapter_map;
}

//==============================================================================
/// C interface to factory
//==============================================================================

std::shared_ptr<BaseClass> Factory::create_world (const std::string cid)
{
    std::string real_cid = Globals::substitute_global(cid);

    auto iter = world_map().find(real_cid);
    if (iter == world_map().end())
		return NULL;
		
	std::shared_ptr<BaseClass> obj = iter->second->create_object();
    
    if (_factory_created_callback)   
        (*_factory_created_callback)(obj); 
	
	return obj;
}

//==============================================================================
//==============================================================================

std::shared_ptr<BaseClass> Factory::create_object (const std::string cid, DTboolean net_create)
{
    std::string real_cid = Globals::substitute_global(cid);

    auto iter = factory_map().find(real_cid);
    if (iter == factory_map().end())
		return NULL;
		
	std::shared_ptr<BaseClass> obj = iter->second->create_object();

    if (_factory_created_callback)   
        (*_factory_created_callback)(obj); 
	
	return obj;
}

//==============================================================================
//==============================================================================

std::shared_ptr<BaseClass> Factory::create_object_from_stream ( const FilePath &pathname,
                                                                std::shared_ptr<Progress> progress,
                                                                std::shared_ptr<Callback<std::shared_ptr<BaseClass>>> obj_loaded_cb)
{
	// First try reading binary
	std::shared_ptr<ArchiveBinaryReader> archive_binary = ArchiveBinaryReader::create();
	if (archive_binary->open(pathname,progress) == DT3_ERR_NONE) {
		std::shared_ptr<BaseClass> obj = ArchiveObjectQueue::queue_in_tree(archive_binary, obj_loaded_cb);
		return obj;
	}

	// Try reading as text
	std::shared_ptr<ArchiveTextReader> archive = ArchiveTextReader::create();
	if (archive->open(pathname,progress) == DT3_ERR_NONE) {
		std::shared_ptr<BaseClass> obj = ArchiveObjectQueue::queue_in_tree(archive, obj_loaded_cb);
		return obj;
	}
	
	return NULL;
}

//==============================================================================
//==============================================================================

std::shared_ptr<BaseClass> Factory::create_importer (const std::string ext)
{
    std::string real_ext = Globals::substitute_global(ext);

    auto iter = factory_importer_map().find(real_ext);
    if (iter == factory_importer_map().end())
		return NULL;
        
    std::shared_ptr<BaseClass> obj = iter->second->create_object();

    if (_factory_created_callback)   
        (*_factory_created_callback)(obj); 

	return obj;
}

//==============================================================================
//==============================================================================

std::shared_ptr<BaseClass> Factory::create_unit_test (const std::string cid)
{
    std::string real_cid = Globals::substitute_global(cid);

    auto iter = unit_test_map().find(real_cid);
    if (iter == unit_test_map().end())
		return NULL;

    std::shared_ptr<BaseClass> obj = iter->second->create_object();

    if (_factory_created_callback)   
        (*_factory_created_callback)(obj); 

	return obj;
}

//==============================================================================
//==============================================================================

std::shared_ptr<BaseClass> Factory::create_command (const std::string cid)
{
    std::string real_cid = Globals::substitute_global(cid);

    auto iter = command_map().find(real_cid);
    if (iter == command_map().end())
		return NULL;

    std::shared_ptr<BaseClass> obj = iter->second->create_object();

    if (_factory_created_callback)   
        (*_factory_created_callback)(obj); 

	return obj;
}

//==============================================================================
//==============================================================================

std::shared_ptr<BaseClass> Factory::create_tool (const std::string cid)
{
    std::string real_cid = Globals::substitute_global(cid);
    
    std::map<std::string, std::shared_ptr<CreatorBase>> &t = tool_map();
    std::map<std::string, std::string>& a = adapter_map();

    auto iter = t.find(a[cid]);
    if (iter == t.end())
		return NULL;

    std::shared_ptr<BaseClass> obj = iter->second->create_object();

    if (_factory_created_callback)   
        (*_factory_created_callback)(obj); 

	return obj;
}

//==============================================================================
//==============================================================================

} // DT3


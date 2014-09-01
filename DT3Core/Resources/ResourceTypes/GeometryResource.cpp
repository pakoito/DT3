//==============================================================================
///	
///	File: GeometryResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/GeometryResource.hpp"
#include "DT3Core/Resources/Importers/ImporterGeometry.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Factory.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(GeometryResource)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,GeometryResource::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,GeometryResource::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                                  GeometryResource::_geometry_map_lock;
std::map<std::string, std::shared_ptr<GeometryResource>>    GeometryResource::_geometry_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GeometryResource::GeometryResource (void)
{    

}
				
GeometryResource::~GeometryResource (void)
{ 
    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<GeometryResource> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<GeometryResource> &r)
{
	std::string path;
	s >> path;
	
	r = GeometryResource::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void GeometryResource::reload_if_changed (void)
{
    FOR_EACH (i,_geometry_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<GeometryResource> GeometryResource::import_resource (const FilePath &pathname, std::string args)
{	
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_geometry_map_lock);
    
    auto i = _geometry_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _geometry_map.end()) {
                        
        std::shared_ptr<GeometryResource> res = GeometryResource::create();
        DTerr err;
        if ((err = res->import(pathname,args)) != DT3_ERR_NONE) {
            return NULL;
        }
                    
        _geometry_map[pathname.full_path()] = res;
        return res;
    }
    
    return i->second;
}

//==============================================================================
//==============================================================================

void GeometryResource::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void GeometryResource::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

void GeometryResource::initialize (void)
{
	Resource::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

void GeometryResource::screen_opened (DTuint width, DTuint height)
{

}

void GeometryResource::screen_closed (void)
{

}

//==============================================================================
//==============================================================================

DTerr GeometryResource::import (const FilePath &pathname, std::string args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
			
	FilePath original_path(path());
	std::string extension = original_path.file_ext();
	
	// Build a generic importer for the file type
	std::shared_ptr<BaseClass> generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isa(ImporterGeometry::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of an image importer
	std::shared_ptr<ImporterGeometry> geometry_importer = checked_static_cast<ImporterGeometry>(generic_importer);
    err = geometry_importer->import(this, args);
	
	return err;
}

//==============================================================================
//==============================================================================		

void GeometryResource::add_mesh (const std::shared_ptr<Mesh> &m)
{
    if (std::find(_meshes.begin(), _meshes.end(), m) == _meshes.end()) {
        _meshes.push_back(m);
    }
}

void GeometryResource::remove_mesh (const std::shared_ptr<Mesh> &m)
{
    auto i = std::find(_meshes.begin(), _meshes.end(), m);
    if (i != _meshes.end()) {
        _meshes.erase(i);
    }
}

void GeometryResource::remove_all_meshes (void)
{
    _meshes.clear();
}

//==============================================================================		
//==============================================================================		

void GeometryResource::draw (const std::shared_ptr<CameraObject> &camera)
{

}

//==============================================================================		
//==============================================================================		

} // DT3


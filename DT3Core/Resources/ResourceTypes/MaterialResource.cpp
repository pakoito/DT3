//==============================================================================
///	
///	File: MaterialResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource3D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResourceCube.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Resources/Importers/ImporterMaterial.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(MaterialResource)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,MaterialResource::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,MaterialResource::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                                  MaterialResource::_material_map_lock;
std::map<std::string, std::shared_ptr<MaterialResource>>   MaterialResource::_material_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

MaterialResource::MaterialResource (void)
{
	reset();
}
			
MaterialResource::~MaterialResource (void)
{         
    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<MaterialResource> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<MaterialResource> &r)
{
	std::string path;
	s >> path;
	
	r = MaterialResource::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void MaterialResource::reload_if_changed (void)
{
    FOR_EACH (i,_material_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<MaterialResource> MaterialResource::import_resource (const FilePath &pathname, std::string args)
{	
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_material_map_lock);
    
    auto i = _material_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _material_map.end()) {
                        
        std::shared_ptr<MaterialResource> res = MaterialResource::create();
        DTerr err;
        if ((err = res->import(pathname,args)) != DT3_ERR_NONE) {
            return NULL;
        }
                    
        _material_map[pathname.full_path()] = res;
        return res;
    }
    
    return i->second;
}

//==============================================================================
//==============================================================================

void MaterialResource::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void MaterialResource::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

void MaterialResource::initialize (void)
{
	Resource::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this, &type::screen_closed));
}
    
//==============================================================================
//==============================================================================

void MaterialResource::screen_opened (DTuint width, DTuint height)
{
    LOG_MESSAGE << "MaterialResource::screen_opened";
}

void MaterialResource::screen_closed (void)
{
    LOG_MESSAGE << "MaterialResource::screen_closed";

    _depth_stencil_resource.reset();
    _blend_state_resource.reset();
    _rasterizer_resource.reset();
    
    _depth_stencil_state_dirty = true;
    _blend_state_dirty = true;
    _rasterizer_state_dirty = true;

    for (DTint i = 0; i < ARRAY_SIZE(_textures); ++i) {
        TextureSlot &ts = _textures[i];
        ts._sampler_state_resource.reset();
        ts._transform_uniform.reset();
        
        ts._sampler_state_dirty = true;
    }
}

//==============================================================================
//==============================================================================

DTerr MaterialResource::import (const FilePath &pathname, std::string args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
			
	FilePath original_path(path());
	std::string extension = original_path.file_ext();
	
	// Build a generic importer for the file type
	std::shared_ptr<BaseClass> generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isa(ImporterMaterial::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of an image importer
	std::shared_ptr<ImporterMaterial> material_importer = checked_static_cast<ImporterMaterial>(generic_importer);
    err = material_importer->import(this, args);
	   
	return err;	
}

DTboolean MaterialResource::is_changed (void) const
{
    if (Resource::is_changed())
        return true;

    for (DTuint i = 0; i < ARRAY_SIZE(_textures); ++i) {            
        if (_textures[i]._texture_2D && _textures[i]._texture_2D->is_changed())
            return true;
        if (_textures[i]._texture_3D && _textures[i]._texture_3D->is_changed())
            return true;
        if (_textures[i]._texture_cube && _textures[i]._texture_cube->is_changed())
            return true;
    }
    
    return false;
}

//==============================================================================
//==============================================================================

void MaterialResource::reset (void)
{
    DeviceGraphics::reset (_depth_stencil_state);
    _depth_stencil_state_dirty = true;
    _depth_stencil_resource.reset();
    
    DeviceGraphics::reset (_blend_state);
    _blend_state_dirty = true;
    _blend_state_resource.reset();

    DeviceGraphics::reset (_rasterizer_state);
    _rasterizer_state_dirty = true;
    _rasterizer_resource.reset();
    
    for (DTint i = 0; i < ARRAY_SIZE(_textures); ++i) {
        DeviceGraphics::reset (_textures[i]._sampler_state);
        
        _textures[i]._sampler_state_dirty = true;
        _textures[i]._sampler_state_resource.reset();
    }
}

//==============================================================================
//==============================================================================

void MaterialResource::set_rectangle (const Rectangle &rectangle)
{
    _textures[_unit]._transform.set_translation(Vector3(rectangle.minus_x(), rectangle.minus_y(), 0.0F));
    _textures[_unit]._transform.set_scale(Vector3(  rectangle.plus_x() - rectangle.minus_x(),
                                                    rectangle.plus_y() - rectangle.minus_y(),
                                                    1.0F));
}

void MaterialResource::set_translation (const Vector3 &translation)
{
    _textures[_unit]._transform.set_translation(translation);
}

void MaterialResource::set_scale (const Vector3 &scale)
{
    _textures[_unit]._transform.set_scale(scale);
}

//==============================================================================
//==============================================================================

void MaterialResource::activate (void)
{
    //
    // Resource updating if necessary
    //
    
    if (_depth_stencil_state_dirty) {
    
        WARNING(_depth_stencil_resource == NULL, "Performance Problem: Recreating depth stencil state");
        
        _depth_stencil_resource = System::renderer()->create_depth_stencil_state(_depth_stencil_state);
        _depth_stencil_state_dirty = false;
    }

    if (_blend_state_dirty) {
    
        WARNING(_blend_state_resource == NULL, "Performance Problem: Recreating blend state");

        _blend_state_resource = System::renderer()->create_blend_state(_blend_state);
        _blend_state_dirty = false;
    }

    if (_rasterizer_state_dirty) {
    
        WARNING(_rasterizer_resource == NULL, "Performance Problem: Recreating rasterizer state");

        _rasterizer_resource = System::renderer()->create_rasterizer_state(_rasterizer_state);
        _rasterizer_state_dirty = false;
    }
    
    for (DTint i = 0; i < ARRAY_SIZE(_textures); ++i) {
    
        TextureSlot &ts = _textures[i];
    
        if (ts._sampler_state_dirty) {
        
            WARNING(ts._sampler_state_resource == NULL, "Performance Problem: Recreating texture sampler state");

            ts._sampler_state_resource = System::renderer()->create_sampler_state(ts._sampler_state);
            ts._sampler_state_dirty = false;
        }
    
    }
    
    //
    // Activation
    //
    
    System::renderer()->attach_depth_stencil_state(_depth_stencil_resource);
    System::renderer()->attach_blend_state(_blend_state_resource);
    System::renderer()->attach_rasterizer_state(_rasterizer_resource);

    for (DTint i = 0; i < ARRAY_SIZE(_textures); ++i) {
    
        TextureSlot &ts = _textures[i];
        
        if (ts._texture_2D) {
            ts._texture_2D->activate(i);
            System::renderer()->attach_sampler(i,ts._sampler_state_resource);
        } else if (ts._texture_3D) {
            ts._texture_3D->activate(i);
            System::renderer()->attach_sampler(i,ts._sampler_state_resource);
        } else if (ts._texture_cube) {
            ts._texture_cube->activate(i);
            System::renderer()->attach_sampler(i,ts._sampler_state_resource);
        }
        
    }
    
}

//==============================================================================
//==============================================================================

} // DT3


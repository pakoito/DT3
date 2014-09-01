//==============================================================================
///	
///	File: ShaderResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Resources/Importers/ImporterShader.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"
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

IMPLEMENT_FACTORY_CREATION(ShaderResource)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,ShaderResource::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,ShaderResource::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                              ShaderResource::_shader_map_lock;
std::map<std::string, std::shared_ptr<ShaderResource>>  ShaderResource::_shader_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ShaderResource::ShaderResource (void)
    :   _needs_recompile              (false)
{

    for (DTint i = 0; i < ARRAY_SIZE(_standard_uniforms); ++i) {
        _standard_uniforms[i] = -1;
    }

    for (DTint i = 0; i < ARRAY_SIZE(_standard_attribs); ++i) {
        _standard_attribs[i] = -1;
    }

}
				
ShaderResource::~ShaderResource (void)
{ 
    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<ShaderResource> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<ShaderResource> &r)
{
	std::string path;
	s >> path;
	
	r = ShaderResource::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void ShaderResource::reload_if_changed (void)
{
    FOR_EACH (i,_shader_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<ShaderResource> ShaderResource::import_resource (const FilePath &pathname, std::string args)
{	
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_shader_map_lock);
    
    auto i = _shader_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _shader_map.end()) {
                        
        std::shared_ptr<ShaderResource> res = ShaderResource::create();
        DTerr err;
        if ((err = res->import(pathname,args)) != DT3_ERR_NONE) {
            return NULL;
        }
                    
        _shader_map[pathname.full_path()] = res;
        return res;
    }
    
    return i->second;
}

//==============================================================================
//==============================================================================

void ShaderResource::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void ShaderResource::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

void ShaderResource::initialize (void)
{
	Resource::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this, &type::screen_closed));
}
    
//==============================================================================
//==============================================================================

void ShaderResource::screen_opened (DTuint width, DTuint height)
{
    LOG_MESSAGE << "ShaderResource::screen_opened";
}

void ShaderResource::screen_closed (void)
{
    LOG_MESSAGE << "ShaderResource::screen_closed";

    _shader.reset();

    for (std::size_t i = 0; i < _attribs.size(); ++i) {
        _attribs[i]._index = -1;
        _attribs[i]._attrib_buffer_resource.reset();
    }
    
    for (std::size_t i = 0; i < _uniforms.size(); ++i) {
        _uniforms[i]._index = -1;
        _uniforms[i]._uniform_resource.reset();
    }

    _needs_recompile = true;
}

//==============================================================================
//==============================================================================

DTerr ShaderResource::import (const FilePath &pathname, std::string args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
			
	FilePath original_path(path());
	std::string extension = original_path.file_ext();
	
	// Build a generic importer for the file type
	std::shared_ptr<BaseClass> generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isa(ImporterShader::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer 
	std::shared_ptr<ImporterShader> shader_importer = checked_static_cast<ImporterShader>(generic_importer);
    err = shader_importer->import(this, args);

	return err;
}

//==============================================================================
//==============================================================================

void ShaderResource::add_vertex_shader (const std::string &standard, const std::string &program)
{
    _vertex_shaders[standard].push_back(program);

    _needs_recompile = true;
}

void ShaderResource::add_geometry_shader (const std::string &standard, const std::string &program)
{
    _geometry_shaders[standard].push_back(program);

    _needs_recompile = true;
}

void ShaderResource::add_fragment_shader (const std::string &standard, const std::string &program)
{
    _fragment_shaders[standard].push_back(program);
    
    _needs_recompile = true;
}

//==============================================================================
//==============================================================================

void ShaderResource::add_attrib (   const std::string &name, DT3GLStandardAttrib standard_attrib)
{
    MappingAttribute m;
    m._name = name;
    m._index = -1;  // Figured out later
    m._standard_attrib = standard_attrib;
    
    _attribs.push_back(m);
    
    _needs_recompile = true;
}

void ShaderResource::add_uniform (  const std::string &name,
                                    DT3GLUniformFormat type,
                                    DTint array_size,
                                    DT3GLStandardUniform standard_uniform)
{
    MappingUniform m;
    m._name = name;
    m._index = -1;  // Figured out later
    m._array_size = array_size;
    m._type = type;
    m._standard_uniform = standard_uniform;

    _uniforms.push_back(m);
    
    _needs_recompile = true;
}

//==============================================================================
//==============================================================================

DTint ShaderResource::attrib_slot (const std::string &name)
{
    recompile_if_needed();

	for (DTint i = 0; i < _attribs.size(); ++i)
		if (_attribs[i]._name == name)
			return i;
			
	return -1;
}

DTint ShaderResource::attrib_slot (DT3GLStandardAttrib standard_attrib)
{
    recompile_if_needed();

//	for (DTint i = 0; i < _attribs.size(); ++i)
//		if (_attribs[i]._standard_attrib == standard_attrib)
//			return i;
//			
//	return -1;

    return _standard_attribs[standard_attrib];
}

//==============================================================================
//==============================================================================

void ShaderResource::set_attribute_buffer (DTint attribute_slot, std::shared_ptr<DT3GLAttribBufferResource> res)
{
    MappingAttribute &m = _attribs[attribute_slot];
    
    m._attrib_buffer_resource = res;
}

//==============================================================================
//==============================================================================

DTint ShaderResource::uniform_slot (const std::string &name)
{
    recompile_if_needed();

	for (DTint i = 0; i < (DTsize) _uniforms.size(); ++i)
		if (_uniforms[i]._name == name)
			return i;
			
	return -1;
}

DTint ShaderResource::uniform_slot (DT3GLStandardUniform standard_uniform)
{
    recompile_if_needed();

//	for (DTint i = 0; i < _uniforms.size(); ++i)
//		if (_uniforms[i]._standard_uniform == standard_uniform)
//			return i;
//
//	return -1;

    return _standard_uniforms[standard_uniform];
}

//==============================================================================
//==============================================================================

void ShaderResource::attach_attribute_buffer (DTint attribute_slot, std::shared_ptr<DT3GLAttribBufferResource> res)
{
    MappingAttribute &m = _attribs[attribute_slot];
    System::renderer()->attach_attribute_buffer(_shader, m._index, res);
}

void ShaderResource::attach_uniform_buffer (DTint uniform_slot, std::shared_ptr<DT3GLUniformResource> res)
{
    MappingUniform &m = _uniforms[uniform_slot];
    System::renderer()->attach_uniform_buffer (_shader, m._index, res);
}

//==============================================================================
//==============================================================================

void ShaderResource::set_uniform_value (DTint uniform_slot, DTint buffer_data)
{
    if (uniform_slot < 0)
        return;

    recompile_if_needed();
    
    MappingUniform &m = _uniforms[uniform_slot];
    
    if (m._uniform_resource) {
        // Update uniform data
        System::renderer()->update_uniform(m._uniform_resource, buffer_data);
    
    } else {
        // Attach uniform to shader
        m._uniform_resource = System::renderer()->create_uniform(buffer_data);
        attach_uniform_buffer (uniform_slot, m._uniform_resource);
    }

}

void ShaderResource::set_uniform_value (DTint uniform_slot, const Vector4 &buffer_data)
{
    if (uniform_slot < 0)
        return;

    recompile_if_needed();

    MappingUniform &m = _uniforms[uniform_slot];
    
    if (m._uniform_resource) {
        // Update uniform data
        System::renderer()->update_uniform(m._uniform_resource, buffer_data);
    
    } else {
        // Attach uniform to shader
        m._uniform_resource = System::renderer()->create_uniform(buffer_data);
        attach_uniform_buffer (uniform_slot, m._uniform_resource);
    }

}

void ShaderResource::set_uniform_value (DTint uniform_slot, const std::vector<Vector4> &buffer_data)
{
    if (uniform_slot < 0)
        return;

    recompile_if_needed();

    MappingUniform &m = _uniforms[uniform_slot];
    
    if (m._uniform_resource) {
        // Update uniform data
        System::renderer()->update_uniform(m._uniform_resource, buffer_data);
    
    } else {
        // Attach uniform to shader
        m._uniform_resource = System::renderer()->create_uniform(buffer_data);
        attach_uniform_buffer (uniform_slot, m._uniform_resource);
    }
    
}

void ShaderResource::set_uniform_value (DTint uniform_slot, const Matrix4 &buffer_data)
{
    if (uniform_slot < 0)
        return;
    
    recompile_if_needed();

    MappingUniform &m = _uniforms[uniform_slot];
    
    if (m._uniform_resource) {
        // Update uniform data
        System::renderer()->update_uniform(m._uniform_resource, buffer_data);
    
    } else {
        // Attach uniform to shader
        m._uniform_resource = System::renderer()->create_uniform(buffer_data);
        attach_uniform_buffer (uniform_slot, m._uniform_resource);
    }

}

void ShaderResource::set_uniform_value (DTint uniform_slot, const std::vector<Matrix4> &buffer_data)
{
    if (uniform_slot < 0)
        return;

    recompile_if_needed();

    MappingUniform &m = _uniforms[uniform_slot];
    
    if (m._uniform_resource) {
        // Update uniform data
        System::renderer()->update_uniform(m._uniform_resource, buffer_data);
    
    } else {
        // Attach uniform to shader
        m._uniform_resource = System::renderer()->create_uniform(buffer_data);
        attach_uniform_buffer (uniform_slot, m._uniform_resource);
    }

}

void ShaderResource::set_uniform_value (DTint uniform_slot, const Color4f &buffer_data)
{
    if (uniform_slot < 0)
        return;

    recompile_if_needed();

    MappingUniform &m = _uniforms[uniform_slot];
    
    if (m._uniform_resource) {
        // Update uniform data
        System::renderer()->update_uniform(m._uniform_resource, buffer_data);
    
    } else {
        // Attach uniform to shader
        m._uniform_resource = System::renderer()->create_uniform(buffer_data);
        attach_uniform_buffer (uniform_slot, m._uniform_resource);
    }

}

//==============================================================================
//==============================================================================

void ShaderResource::recompile_if_needed (void)
{
    if (_needs_recompile) {
        _needs_recompile = false;

        _shader = System::renderer()->create_shader();
        
#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID
        std::string standard = "GLES2";
#elif DT3_OS == DT3_MACOSX
        std::string standard = "GLSL";
#endif
    
        // Recompile vertex and fragment programs
        std::vector<std::string> &vertex_shaders = _vertex_shaders[standard];
        for (std::size_t i = 0; i < _vertex_shaders.size(); ++i) {
            std::shared_ptr<DT3GLVertexShaderResource> vertex_shader;
            vertex_shader = System::renderer()->create_vertex_shader (vertex_shaders[i]);
            System::renderer()->attach_vertex_shader (_shader, vertex_shader);
        }
        
        std::vector<std::string> &geometry_shaders = _geometry_shaders[standard];
        for (std::size_t i = 0; i < geometry_shaders.size(); ++i) {
            std::shared_ptr<DT3GLGeometryShaderResource> geometry_shader;
            geometry_shader = System::renderer()->create_geometry_shader (geometry_shaders[i]);
            System::renderer()->attach_geometry_shader (_shader, geometry_shader);
        }
        
        std::vector<std::string> &fragment_shaders = _fragment_shaders[standard];
        for (std::size_t i = 0; i < fragment_shaders.size(); ++i) {
            std::shared_ptr<DT3GLFragmentShaderResource> fragment_shader;
            fragment_shader = System::renderer()->create_fragment_shader (fragment_shaders[i]);
            System::renderer()->attach_fragment_shader (_shader, fragment_shader);
        }
        
        System::renderer()->link_shader(_shader);
        
        
        // Clear standard attribs and uniforms
        for (DTint i = 0; i < ARRAY_SIZE(_standard_uniforms); ++i) {
            _standard_uniforms[i] = -1;
        }

        for (DTint i = 0; i < ARRAY_SIZE(_standard_attribs); ++i) {
            _standard_attribs[i] = -1;
        }


        // Get Attribute locations
        for (std::size_t i = 0; i < _attribs.size(); ++i) {
            _attribs[i]._index = System::renderer()->attribute_slot(_shader, _attribs[i]._name);
            
            if (_attribs[i]._standard_attrib >= 0) {
                _standard_attribs[_attribs[i]._standard_attrib] = i;
            }
        }
        
		// Get uniform locations
        for (std::size_t i = 0; i < _uniforms.size(); ++i) {
            _uniforms[i]._index = System::renderer()->uniform_slot(_shader, _uniforms[i]._name );
            
            if (_uniforms[i]._standard_uniform >= 0) {
                _standard_uniforms[_uniforms[i]._standard_uniform] = i;
            }
		}
        
        // Setup texture slots
        for (DTint i = 0; i < 16; ++i) {
            set_uniform_value(uniform_slot( (DT3GLStandardUniform) (DT3GL_UNIFORM_TEX0 + i) ), i);
        }
		
    }

}

void ShaderResource::activate (void)
{
    recompile_if_needed();

    
    //
    // Activate the shader
    //
    
    System::renderer()->attach_shader(_shader);
    
    
}

//==============================================================================
//==============================================================================

} // DT3


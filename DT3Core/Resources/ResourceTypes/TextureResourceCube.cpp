//==============================================================================
///	
///	File: TextureResourceCube.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/TextureResourceCube.hpp"
#include "DT3Core/Resources/Importers/ImporterImage.hpp"
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

IMPLEMENT_FACTORY_CREATION(TextureResourceCube)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,TextureResourceCube::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,TextureResourceCube::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                                      TextureResourceCube::_texture_map_lock;
std::map<std::string, std::shared_ptr<TextureResourceCube>>    TextureResourceCube::_texture_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextureResourceCube::TextureResourceCube (void)
	:	_format				(DT3GL_FORMAT_RGBA),
		_width				(0),
		_height				(0),
		_mipmapped			(false),
        _flags              (0)
{

}
			
TextureResourceCube::~TextureResourceCube (void)
{ 
    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<TextureResourceCube> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<TextureResourceCube> &r)
{
	std::string path;
	s >> path;
	
	r = TextureResourceCube::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void TextureResourceCube::reload_if_changed (void)
{
    FOR_EACH (i,_texture_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<TextureResourceCube> TextureResourceCube::import_resource (const FilePath &pathname, std::string args)
{    
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_texture_map_lock);
    
    auto i = _texture_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _texture_map.end()) {
                        
        std::shared_ptr<TextureResourceCube> res = TextureResourceCube::create();
        DTerr err;
        if ((err = res->import(pathname,args)) != DT3_ERR_NONE) {
            return NULL;
        }
                    
        _texture_map[pathname.full_path()] = res;
        return res;
    }
    
    return i->second;
}

//==============================================================================
//==============================================================================

void TextureResourceCube::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void TextureResourceCube::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

void TextureResourceCube::initialize (void)
{
	Resource::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

void TextureResourceCube::screen_opened (DTuint width, DTuint height)
{
    LOG_MESSAGE << "TextureResourceCube::screen_opened";

    if (_textels_front ||
        _textels_back ||
        _textels_left ||
        _textels_right ||
        _textels_top ||
        _textels_bottom ) {
        
        _resource_cube = System::renderer()->create_texture_cube(   _width, _height,
                                                                    _textels_front.get(),
                                                                    _textels_back.get(),
                                                                    _textels_left.get(),
                                                                    _textels_right.get(),
                                                                    _textels_top.get(),
                                                                    _textels_bottom.get(),
                                                                    _format, _mipmapped, _flags);
    
    } else {
        reload();
    }
}

void TextureResourceCube::screen_closed (void)
{
    LOG_MESSAGE << "TextureResourceCube::screen_closed";

    _resource_cube.reset();
}

//==============================================================================
//==============================================================================

DTerr TextureResourceCube::import (const FilePath &pathname, std::string args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
			
	FilePath original_path(path());
	std::string extension = original_path.file_ext();
    
	// Build a generic importer for the file type
	std::shared_ptr<BaseClass> generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isa(ImporterImage::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of an image importer
	std::shared_ptr<ImporterImage> image_importer = checked_static_cast<ImporterImage>(generic_importer);
    err = image_importer->import(this, args);

	return err;
}

//==============================================================================
//==============================================================================

void TextureResourceCube::set_textels_cube (const DTint width, const DTint height,
                                            const std::shared_ptr<DTubyte> &textels_front,
                                            const std::shared_ptr<DTubyte> &textels_back,
                                            const std::shared_ptr<DTubyte> &textels_left,
                                            const std::shared_ptr<DTubyte> &textels_right,
                                            const std::shared_ptr<DTubyte> &textels_top,
                                            const std::shared_ptr<DTubyte> &textels_bottom,
                                            DT3GLTextelFormat format,
                                            DTboolean mipmapped,
                                            DTuint flags)
{
    _textels_front = textels_front;
    _textels_back = textels_back;
    _textels_left = textels_left;
    _textels_right = textels_right;
    _textels_top = textels_top;
    _textels_bottom = textels_bottom;

	_format = format;
    
	_width = width;
	_height = height;
    
    _mipmapped = mipmapped;
    _flags = flags;

    // Build the resource
    _resource_cube = System::renderer()->create_texture_cube(    _width, _height,
                                                textels_front.get(),
                                                textels_back.get(),
                                                textels_left.get(),
                                                textels_right.get(),
                                                textels_top.get(),
                                                textels_bottom.get(),
                                                _format,
                                                _mipmapped,
                                                _flags);

}

//==============================================================================
//==============================================================================

void TextureResourceCube::activate (DTuint texture_slot)
{
    if (_resource_cube)  System::renderer()->attach_texture(texture_slot, _resource_cube);
}

//==============================================================================
//==============================================================================

} // DT3


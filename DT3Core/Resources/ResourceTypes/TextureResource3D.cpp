//==============================================================================
///	
///	File: TextureResource3D.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/TextureResource3D.hpp"
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

IMPLEMENT_FACTORY_CREATION(TextureResource3D)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,TextureResource3D::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,TextureResource3D::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                                  TextureResource3D::_texture_map_lock;
std::map<std::string, std::shared_ptr<TextureResource3D>>  TextureResource3D::_texture_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextureResource3D::TextureResource3D (void)
	:	_format				(DT3GL_FORMAT_RGBA),
		_width				(0),
		_height				(0),
        _depth              (0),
		_mipmapped			(false),
        _flags              (0)
{

}
			
TextureResource3D::~TextureResource3D (void)
{ 
    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<TextureResource3D> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<TextureResource3D> &r)
{
	std::string path;
	s >> path;
	
	r = TextureResource3D::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void TextureResource3D::reload_if_changed (void)
{
    FOR_EACH (i,_texture_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<TextureResource3D> TextureResource3D::import_resource (const FilePath &pathname, std::string args)
{    
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_texture_map_lock);
    
    auto i = _texture_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _texture_map.end()) {
                        
        std::shared_ptr<TextureResource3D> res = TextureResource3D::create();
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

void TextureResource3D::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void TextureResource3D::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

void TextureResource3D::initialize (void)
{
	Resource::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

void TextureResource3D::screen_opened (DTuint width, DTuint height)
{
    LOG_MESSAGE << "TextureResource3D::screen_opened";

    if (_textels) {
        _resource_3D = System::renderer()->create_texture_3D(_width, _height, _depth, _textels.get(), _format, _mipmapped, _flags);
        
    } else {
        reload();
    }
}

void TextureResource3D::screen_closed (void)
{
    LOG_MESSAGE << "TextureResource3D::screen_closed";

    _resource_3D.reset();
}

//==============================================================================
//==============================================================================

DTerr TextureResource3D::import (const FilePath &pathname, std::string args)
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

void TextureResource3D::set_textels (const DTint width, const DTint height, const DTint depth, std::shared_ptr<DTubyte> &textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags)
{
    _textels = textels;

	_format = format;
    
	_width = width;
	_height = height;
	_depth = 0;
    
    _mipmapped = mipmapped;
    _flags = flags;

    // Build the resource
    _resource_3D = System::renderer()->create_texture_3D(_width, _height, _depth, _textels.get(), _format, _mipmapped, _flags);
}

//==============================================================================
//==============================================================================

void TextureResource3D::activate (DTuint texture_slot)
{
    if (_resource_3D)   System::renderer()->attach_texture(texture_slot, _resource_3D);
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: TextureResource2D.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Resources/Importers/ImporterImage.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"

#include "DT3Core/Types/Media/PNGWriter.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(TextureResource2D)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,TextureResource2D::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,TextureResource2D::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                                  TextureResource2D::_texture_map_lock;
std::map<std::string, std::shared_ptr<TextureResource2D>>  TextureResource2D::_texture_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextureResource2D::TextureResource2D (void)
	:	_format				(DT3GL_FORMAT_RGBA),
		_width				(0),
		_height				(0),
		_mipmapped			(false),
        _flags              (0)
{

}
			
TextureResource2D::~TextureResource2D (void)
{ 
    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<TextureResource2D> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<TextureResource2D> &r)
{
	std::string path;
	s >> path;
	
	r = TextureResource2D::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void TextureResource2D::reload_if_changed (void)
{
    FOR_EACH (i,_texture_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<TextureResource2D> TextureResource2D::import_resource (const FilePath &pathname, std::string args)
{    
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_texture_map_lock);
    
    auto i = _texture_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _texture_map.end()) {
                        
        std::shared_ptr<TextureResource2D> res = TextureResource2D::create();
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

void TextureResource2D::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void TextureResource2D::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

void TextureResource2D::initialize (void)
{
	Resource::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

void TextureResource2D::screen_opened (DTuint width, DTuint height)
{
    LOG_MESSAGE << "TextureResource2D::screen_opened";

    if (_textels.get()) {
        _resource_2D = System::renderer()->create_texture_2D(_width, _height, _textels.get(), _format, _mipmapped, _flags);
    } else {
        reload();
    }
}

void TextureResource2D::screen_closed (void)
{
    LOG_MESSAGE << "TextureResource2D::screen_closed";

    _resource_2D.reset();
}

//==============================================================================
//==============================================================================

DTerr TextureResource2D::import (const FilePath &pathname, std::string args)
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

void TextureResource2D::allocate_rgba_textels (const DTint width, const DTint height, DTboolean mipmapped)
{
	_format = DT3GL_FORMAT_RGBA;
    
	_width = width;
	_height = height;
	_depth = 0;
    
    _mipmapped = mipmapped;
    _flags = DT3GL_ACCESS_CPU_READ | DT3GL_ACCESS_CPU_WRITE | DT3GL_ACCESS_GPU_READ;
    
    _textels = std::shared_ptr<DTubyte>(new DTubyte[_width * _height * sizeof(Color4b)]);

    // Build the resource
    _resource_2D = System::renderer()->create_texture_2D(_width, _height, _textels.get(), _format, _mipmapped, _flags);
    
    _dirty_rectangle.clear();
}

void TextureResource2D::set_textels (const DTint width, const DTint height, std::shared_ptr<DTubyte> &textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags)
{
    _textels = textels;

	_format = format;
    
	_width = width;
	_height = height;
	_depth = 0;
    
    _mipmapped = mipmapped;
    _flags = flags;

    // Build the resource
    _resource_2D = System::renderer()->create_texture_2D(_width, _height, _textels.get(), _format, _mipmapped, _flags);
    
    _dirty_rectangle.clear();
}

//==============================================================================
//==============================================================================

void TextureResource2D::set_textels_compressed (const DTint width, const DTint height, std::shared_ptr<DTubyte> &textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags)
{
    ASSERT ( (_flags & (DT3GL_ACCESS_CPU_WRITE | DT3GL_ACCESS_CPU_READ)) == 0);

    _textels = textels;

	_format = format;
    
	_width = width;
	_height = height;
	_depth = 0;
    
    _mipmapped = mipmapped;
    _flags = flags;
    
    // Build the resource
    _resource_2D = System::renderer()->create_texture_2D(_width, _height, _textels.get(), _format, _mipmapped, _flags);

    _dirty_rectangle.clear();
}

//==============================================================================
//==============================================================================

Color4b TextureResource2D::pixel(DTint x, DTint y)
{
    ASSERT(_flags & DT3GL_ACCESS_CPU_READ);
    ASSERT( _format == DT3GL_FORMAT_RGBA ||
            _format == DT3GL_FORMAT_RGBA_5551 ||
            _format == DT3GL_FORMAT_RGBA_4444 ||
            _format == DT3GL_FORMAT_RGB_565 ||
            _format == DT3GL_FORMAT_LUM_8 ||
            _format == DT3GL_FORMAT_RGB);
    
    DTubyte *buffer = _textels.get();
        
    switch (_format) {
        case DT3GL_FORMAT_RGBA: {
            DTubyte *base = buffer + ((_width * y) + x) * 4;
            return Color4b(base[0],base[1],base[2],base[3]);
        }
        
        case DT3GL_FORMAT_RGBA_5551: {
            DTubyte *base = buffer + ((_width * y) + x) * 2;
            DTushort base_short = *((DTushort *) base);
            return Color4b( MoreMath::convert_5_to_8_bit( (base_short >> 11) & 0x1F ),
                            MoreMath::convert_5_to_8_bit( (base_short >> 6) & 0x1F ),
                            MoreMath::convert_5_to_8_bit( (base_short >> 1) & 0x1F ),
                            (DTubyte) ( (base_short & 0x1 ) * 255) );
        }
        
        case DT3GL_FORMAT_RGBA_4444: {
            DTubyte *base = buffer + ((_width * y) + x) * 2;
            DTushort base_short = *((DTushort *) base);
            return Color4b( MoreMath::convert_4_to_8_bit( (base_short >> 12) & 0x0F ),
                            MoreMath::convert_4_to_8_bit( (base_short >> 8) & 0x0F ),
                            MoreMath::convert_4_to_8_bit( (base_short >> 4) & 0x0F ),
                            MoreMath::convert_4_to_8_bit( (base_short >> 0) & 0x0F ) );
        }

        case DT3GL_FORMAT_RGB_565: {
            DTubyte *base = buffer + ((_width * y) + x) * 2;
            DTushort base_short = *((DTushort *) base);
            return Color4b( MoreMath::convert_5_to_8_bit( (base_short >> 11) & 0x1F ),
                            MoreMath::convert_5_to_8_bit( (base_short >> 5) & 0x3F ),
                            MoreMath::convert_5_to_8_bit( (base_short >> 0) & 0x1F ), 
                            255 );
        }

        case DT3GL_FORMAT_LUM_8: {
            DTubyte *base = buffer + (_width*1 * y) + (x*1);
            return Color4b(*base,*base,*base,(DTubyte)255);
        }
        
        case DT3GL_FORMAT_RGB: {
            DTubyte *base = buffer + ((_width * y) + x) * 3;
            return Color4b(base[0],base[1],base[2],255);
        }

        default:
            return Color4b((DTubyte)0,(DTubyte)0,(DTubyte)0,(DTubyte)0);
    };

}

DTubyte* TextureResource2D::pixel_addr (DTint x, DTint y)
{
    ASSERT(_flags & DT3GL_ACCESS_CPU_READ);
    ASSERT( _format == DT3GL_FORMAT_RGBA ||
            _format == DT3GL_FORMAT_RGBA_5551 ||
            _format == DT3GL_FORMAT_RGBA_4444 ||
            _format == DT3GL_FORMAT_RGB_565 ||
            _format == DT3GL_FORMAT_LUM_8 ||
            _format == DT3GL_FORMAT_RGB);

    DTubyte *buffer = _textels.get();
 
    // Have to set all dirty
    _dirty_rectangle.set(0, _width, 0, _height);

    switch (_format) {
        case DT3GL_FORMAT_RGBA:       return buffer + ((_width * y) + x) * 4;
        case DT3GL_FORMAT_RGBA_5551:  return buffer + ((_width * y) + x) * 2;
        case DT3GL_FORMAT_RGBA_4444:  return buffer + ((_width * y) + x) * 2;
        case DT3GL_FORMAT_RGB_565:    return buffer + ((_width * y) + x) * 2;
        case DT3GL_FORMAT_LUM_8:      return buffer + ((_width * y) + x) * 1;
        case DT3GL_FORMAT_RGB:        return buffer + ((_width * y) + x) * 3;
        default:
            return NULL;
    };
    
    return NULL;
}

void TextureResource2D::set_pixel (DTint x, DTint y, const Color4b &pixel)
{
    ASSERT(_flags & DT3GL_ACCESS_CPU_WRITE);
    ASSERT(_format == DT3GL_FORMAT_RGBA);
    
    DTubyte *buffer = _textels.get();
    
    Color4b *c = (Color4b*) (buffer + ((_width * y) + x) * sizeof(Color4b));
    *c = pixel;
    
    _dirty_rectangle = Rectangle::calc_union(_dirty_rectangle, Vector2(x,y));
}

//==============================================================================
//==============================================================================

void TextureResource2D::replace_sub_rectangle (DTint dst_x, DTint dst_y, const DTubyte *b, DTint src_width, DTint src_height, DTint rowbytes)
{
    ASSERT(_flags & DT3GL_ACCESS_CPU_WRITE);
    ASSERT(_format == DT3GL_FORMAT_RGBA || _format == DT3GL_FORMAT_BGRA);
    
    DTubyte *buffer = _textels.get();

    DTint clip_src_width = MoreMath::min(_width - dst_x, src_width);
    DTint clip_src_height = MoreMath::min(_height - dst_x, src_height);
    
    if (clip_src_width <= 0)    return;
    if (clip_src_height <= 0)   return;
    
    DTint num_bytes = clip_src_width * 4;
    if (rowbytes == 0)  rowbytes = src_width * 4;
    
    DTint i,j;
    for (i = dst_y, j = 0; i < dst_y + clip_src_height; ++i, ++j) {
        DTubyte *dst = (buffer + ((_width * i) + dst_x) * 4);
        const DTubyte *src = (b + (rowbytes * j));
        
        ::memcpy(dst, src, num_bytes);
    }
    
    _dirty_rectangle = Rectangle::calc_union(_dirty_rectangle, Rectangle(dst_x, dst_x+src_width, dst_y, dst_x + src_height));
}

//==============================================================================
//==============================================================================

void TextureResource2D::activate (DTuint texture_slot)
{
    // Check if resource needs updating
    if (!_dirty_rectangle.is_clear()) {
        
        // Actually just update the whole thing so we don't have to copy buffers
        System::renderer()->update_texture_2D(_resource_2D, 0, 0, _width, _height, _textels.get());
        
        _dirty_rectangle.clear();
    }

    if (_resource_2D)
        System::renderer()->attach_texture(texture_slot, _resource_2D);
}

//==============================================================================
//==============================================================================

} // DT3


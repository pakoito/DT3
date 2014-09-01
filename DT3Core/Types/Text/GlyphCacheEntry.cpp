//==============================================================================
///	
///	File: GlyphCacheEntry.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Text/GlyphCacheEntry.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

extern "C" {
    extern unsigned int hashlittle( const void *key, unsigned int length, unsigned int initval);
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GlyphCacheEntry::GlyphCacheEntry (void)
    :   _hash               (0),
        _c                  (0),
        _font               (NULL),
        _size               (0),
        _resolution_scale   (1.0F),
        _transform          (   1.0F, 0.0F, 0.0F, 0.0F,
                                0.0F, 1.0F, 0.0F, 0.0F,
                                0.0F, 0.0F, 1.0F, 0.0F,
                                0.0F, 0.0F, 0.0F, 1.0F),
        _scale              (1.0F,1.0F,1.0F),
        _advance_x          (0),
        _advance_y          (0),
        _glyph              (NULL)
{

}

GlyphCacheEntry::GlyphCacheEntry (const GlyphCacheEntry &rhs)
    :   BaseClass           (rhs),
        _hash               (0),
        _advance_x          (0),
        _advance_y          (0),
        _glyph              (NULL)
{
    _font = rhs._font;
    _c = rhs._c;
    _size = rhs._size;
    _resolution_scale = rhs._resolution_scale;
    _transform = rhs._transform;
    _scale = rhs._scale;
}

GlyphCacheEntry& GlyphCacheEntry::operator = (const GlyphCacheEntry &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {		
        clear_glyph();
        
        BaseClass::operator=(rhs);

        _font = rhs._font;
        _c = rhs._c;
        _size = rhs._size;
        _resolution_scale = rhs._resolution_scale;
        _transform = rhs._transform;
        _scale = rhs._scale;
    }
    
    return (*this);
}

GlyphCacheEntry::~GlyphCacheEntry (void)
{
    clear_glyph();
}

//==============================================================================
//==============================================================================

DTboolean GlyphCacheEntry::operator == (const GlyphCacheEntry& rhs) const
{
    return  (_font == rhs._font) &&
            (_c == rhs._c) &&
            (_size == rhs._size) &&
            (_resolution_scale == rhs._resolution_scale) &&
            (_transform == rhs._transform) &&
            (_scale == rhs._scale);
}

DTboolean GlyphCacheEntry::operator != (const GlyphCacheEntry& rhs) const
{
    return  (_font != rhs._font) ||
            (_c != rhs._c) ||
            (_size != rhs._size) ||
            (_resolution_scale != rhs._resolution_scale) ||
            (_transform != rhs._transform) ||
            (_scale != rhs._scale);
}

//==============================================================================
//==============================================================================

DTboolean GlyphCacheEntry::is_same_family (const std::shared_ptr<GlyphCacheEntry> &rhs)
{
    return  (_font == rhs->_font) &&
            (_size == rhs->_size) &&
            (_resolution_scale == rhs->_resolution_scale) &&
            (_transform == rhs->_transform) &&
            (_scale == rhs->_scale);
}

//==============================================================================
//==============================================================================

DTuint GlyphCacheEntry::hash (void) const
{
    if (_hash == 0) {
        _hash = hashlittle( (unsigned char *) &_font, (unsigned int) sizeof(_font), _hash);
        _hash = hashlittle( (unsigned char *) &_c, (unsigned int) sizeof(_c), _hash);
        _hash = hashlittle( (unsigned char *) &_size, (unsigned int) sizeof(_size), _hash);
        _hash = hashlittle( (unsigned char *) &_resolution_scale, (unsigned int) sizeof(_resolution_scale), _hash);
        _hash = hashlittle( (unsigned char *) &_transform, (unsigned int) sizeof(_transform), _hash);
        _hash = hashlittle( (unsigned char *) &_scale, (unsigned int) sizeof(_scale), _hash);
    }
    
    return _hash;
}

//==============================================================================
//==============================================================================

void GlyphCacheEntry::set_font (const std::shared_ptr<FontResource> &font)
{
    clear_glyph();
    _font = font;
}

void GlyphCacheEntry::set_character (DTuint c)
{
    clear_glyph();
    _c = c;
}

void GlyphCacheEntry::set_size (DTuint size)
{
    clear_glyph();
    _size = size;
}

void GlyphCacheEntry::set_resolution_scale   (DTfloat resolution_scale)
{
    clear_glyph();
    _resolution_scale = resolution_scale;
}


void GlyphCacheEntry::set_transform (const Matrix4 &transform)
{
    clear_glyph();
    _transform = transform;
}

void GlyphCacheEntry::set_scale (const Vector3 &scale)
{
    clear_glyph();
    _scale = scale;
}
//==============================================================================
//==============================================================================

FT_Glyph GlyphCacheEntry::glyph (void) const
{   
    if (!_font)
        return NULL;

    if (!_glyph) {
        FT_Face face = _font->typeface();
        
        FT_Error error = FT_Set_Char_Size(
            face,               // handle to face object          
            0,                  // char_width in 1/64th of points 
            _size*64,           // char_height in 1/64th of points
            static_cast<FT_UInt>(_resolution_scale*_scale.x*72),   // horizontal device resolution
            static_cast<FT_UInt>(_resolution_scale*_scale.y*72) ); // vertical device resolution
        if (error) 
            return NULL;
   
        FT_Matrix matrix;
        matrix.xx = (FT_Fixed)( _transform.x_axis().x * 0x10000L );
        matrix.yx = (FT_Fixed)( _transform.x_axis().y * 0x10000L );
        matrix.xy = (FT_Fixed)( _transform.y_axis().x * 0x10000L );
        matrix.yy = (FT_Fixed)( _transform.y_axis().y * 0x10000L );

        FT_Vector start;
        start.x = static_cast<FT_Pos>(_transform.translation().x * 64.0F);
        start.y = static_cast<FT_Pos>(_transform.translation().y * 64.0F);
        
        DTuint glyph_index = ::FT_Get_Char_Index( face, _c);

        // A function used to load a single glyph into the glyph slot of a face object.
        error = ::FT_Load_Glyph( face, glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING );
        if (error) 
            return NULL;

        // A function used to extract a glyph image from a slot.
        error = ::FT_Get_Glyph( face->glyph, &_glyph );
        if ( error ) 
            return NULL;   
            
        // transform copy (this will also translate it to the correct position
        ::FT_Glyph_Transform( _glyph, &matrix, &start );

//        // convert glyph image to bitmap (destroy the glyph copy!)
//        error = FT_Glyph_To_Bitmap(
//                  &_glyph,
//                  FT_RENDER_MODE_NORMAL,
//                  0,            // no additional translation
//                  1 );          // destroy copy in "image" 
//        if ( error ) 
//            return NULL;   
    
        _advance_x = face->glyph->advance.x / 64.0F;
        _advance_y = face->glyph->advance.y / 64.0F;

    }
    
    return _glyph;
}

//==============================================================================
//==============================================================================

void GlyphCacheEntry::clear_glyph (void)
{
    if (_glyph) {
        FT_Done_Glyph(_glyph);
        _glyph = NULL;
    }
    
    _advance_x = 0;
    _advance_y = 0;
    _hash = 0;
}

//==============================================================================
//==============================================================================

} // DT3

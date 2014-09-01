//==============================================================================
///	
///	File: TextCharacter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Text/TextCharacter.hpp"
#include "DT3Core/Types/Text/GlyphCacheEntry.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextCharacter::TextCharacter (void)
    :   _justification  (JUSTIFY_FULL),
        _tab_size       (15),
        _baseline       (0.0F),
        _kerning        (0.0F),
        _color          (0.0F,0.0F,0.0F,1.0F),
        _outline_color  (1.0F,1.0F,1.0F,1.0F),
        _outline_size   (0.0F),
        _advance_x      (0.0F),
        _advance_y      (0.0F),
        _direction      (DIR_HORZ_LEFT_TO_RIGHT),
        _word_wrap      (WRAP_WORD)
{

}

TextCharacter::TextCharacter (const TextCharacter &rhs)
{
    _justification = rhs._justification;
    _tab_size = rhs._tab_size;
    _baseline = rhs._baseline;
    _kerning = rhs._kerning;
    _color = rhs._color;

    _outline_color = rhs._outline_color;
    _outline_size = rhs._outline_size;
    
    _advance_x = rhs._advance_x;
    _advance_y = rhs._advance_y;
    
    _glyph = rhs._glyph;
    
    _direction = rhs._direction;
    _word_wrap = rhs._word_wrap;
}

TextCharacter& TextCharacter::operator = (const TextCharacter &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {		        
        _justification = rhs._justification;
        _tab_size = rhs._tab_size;
        _baseline = rhs._baseline;
        _kerning = rhs._kerning;
        _color = rhs._color;

        _outline_color = rhs._outline_color;
        _outline_size = rhs._outline_size;

        _advance_x = rhs._advance_x;
        _advance_y = rhs._advance_y;

        _glyph = rhs._glyph;

        _direction = rhs._direction;
        _word_wrap = rhs._word_wrap;
    }
    
    return (*this);
}

TextCharacter::~TextCharacter (void)
{

}

//==============================================================================
//==============================================================================

void TextCharacter::set_tab_size (DTuint size)
{
    _tab_size = size;
}

void TextCharacter::set_baseline_shift (DTuint bl)
{
    _baseline = bl;
}

void TextCharacter::set_kern (DTfloat k)
{
    _kerning = k;
}

void TextCharacter::set_word_wrap (Wrap word_wrap)
{
    _word_wrap = word_wrap;
}

void TextCharacter::set_glyph_entry (std::shared_ptr<GlyphCacheEntry> &glyph)
{
    _glyph = glyph;
}

void TextCharacter::set_direction    (Direction direction)
{
    _direction = direction;
}

//==============================================================================
//==============================================================================

DTfloat TextCharacter::extent (void) const
{
    if (_direction == DIR_HORZ_LEFT_TO_RIGHT || _direction == DIR_HORZ_RIGHT_TO_LEFT) {
        return (_advance_x == 0.0F) ? (_glyph->size() * _glyph->scale().x * _glyph->resolution_scale() + 2.0F * outline_size()): _advance_x;
    } else {
        return (_advance_y == 0.0F) ? (_glyph->size() * _glyph->scale().y * _glyph->resolution_scale() + 2.0F * outline_size()) : _advance_y;
    }
}

DTfloat TextCharacter::extent_perp (void) const
{
    if (_direction == DIR_HORZ_LEFT_TO_RIGHT || _direction == DIR_HORZ_RIGHT_TO_LEFT) {
        return (_advance_y == 0.0F) ? (_glyph->size() * _glyph->scale().y * _glyph->resolution_scale() + 2.0F * outline_size()) : _advance_y;
    } else {
        return (_advance_x == 0.0F) ? (_glyph->size() * _glyph->scale().x * _glyph->resolution_scale() + 2.0F * outline_size()) : _advance_x;
    }
}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: TextLines.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Text/TextLines.hpp"
#include "DT3Core/Types/Text/GlyphCache.hpp"
#include "DT3Core/Types/Text/GlyphCacheEntry.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Resources/ResourceTypes/FontResource.hpp"
#include "DT3Core/System/Globals.hpp"

extern "C" {
    #include "ft2build.h"
    #include "freetype/freetype.h"
    #include "freetype/ftmodapi.h"
    #include "freetype/ftglyph.h"
    #include "freetype/ftoutln.h"
    #include "freetype/ftstroke.h"
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextLine::TextLine (void)
    :   _extents_offset     (0.0F),
        _extents_perp_offset(0.0F),
        _auto_split         (false)
{

}

TextLine::TextLine (const TextLine &rhs)
{
    _glyphs = rhs._glyphs;
    _extents_offset = rhs._extents_offset;
    _extents_perp_offset = rhs._extents_perp_offset;
    _auto_split = rhs._auto_split;
}

TextLine& TextLine::operator = (const TextLine &rhs)
{
    _glyphs = rhs._glyphs;
    _extents_offset = rhs._extents_offset;
    _extents_perp_offset = rhs._extents_perp_offset;
    _auto_split = rhs._auto_split;
    
    return (*this);
}

TextLine::~TextLine (void)
{

}

DTboolean TextLine::is_blank(void) const
{
    for (auto &g : _glyphs) {
        DTuint c = g.glyph_entry()->character();
    
        if (c >= 128 || ::isalpha(c)) 
            return false;
    }
    
    return true;

}

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextLines::TextLines (void)
{

}

TextLines::TextLines (const TextLines &rhs)
    :   _lines                  (rhs._lines)
{
    
}

TextLines& TextLines::operator = (const TextLines &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {		
        _lines = rhs._lines;
    }
    
    return (*this);
}

TextLines::~TextLines (void)
{

}

//==============================================================================
//==============================================================================

void TextLines::add_line (  GlyphCache &cache,
                            const Matrix4 &transform,
                            const Vector3 &scale,
                            DTfloat resolution_scale,
                            const std::string &text_raw,
                            const std::shared_ptr<FontResource> &font,
                            DTuint font_size,
                            const Color4f &color,
                            TextCharacter::Justification justification)
{
    //
    // Extract text
    //
    
    std::string text = Globals::substitute_global(text_raw);

    //
    // Remove all control characters
    //
    
    text = MoreStrings::clean_ctrl_characters(text);
    
    // Default Cache Entry
    GlyphCacheEntry ce;
    ce.set_transform(transform);
    ce.set_scale(scale);
    ce.set_font(font);
    ce.set_size(font_size);
    ce.set_resolution_scale(resolution_scale);

    // Default TextCharacter
    TextCharacter   ct;
    ct.set_justification(justification);
    ct.set_color(color);
    
    _lines.resize(_lines.size() + 1);
    TextLine        &line = _lines.back();
    
    DTuint          incr;

    for (DTuint i = 0; i < text.size(); i += incr) {
        DTuint c = text[i];
        
        if (c > 127) {
            MoreStrings::extract_unicode (&text[i], incr, c);
        } else {
            incr = 1;
        }

        // Check for escaping
        if (c == '\\') {
            ++i;
            c = text[i];

            // Convert to control characters if necessary
            if (c == 't')       c = '\t';
            else if (c == 'n')  c = '\n';
        }
    
        // Set the character in the glyph cache entry
        ce.set_character(c);
    
        // Add the entry        
        std::shared_ptr<GlyphCacheEntry> &e = cache.entry(ce);
        
        // Set the glyph cache entry
        ct.set_glyph_entry(e);
        
        // makes sure glyph is loaded
        e->glyph();
        
        //
        // Append the character
        //
        
        FT_Int kern_pair_dx = 0;
        FT_Int kern_pair_dy = 0;
        
        // Check for kerning pairs
        if (line._glyphs.size() > 0) {
            TextCharacter &c_prev = line._glyphs.back();
            
            if (c_prev.glyph_entry()->is_same_family( ct.glyph_entry() )) {
            
                FT_Face face = ct.glyph_entry()->font()->typeface();
                
                FT_Int last_glyph_index = ::FT_Get_Char_Index( face, c_prev.glyph_entry()->character());
                FT_Int glyph_index = ::FT_Get_Char_Index( face, ct.glyph_entry()->character());
                
                FT_Bool use_kerning = FT_HAS_KERNING(face);
                if (use_kerning) {
                    FT_Vector  d;
                    
                    // Return the kerning vector between two glyphs of a same face.
                    ::FT_Get_Kerning( face, last_glyph_index, glyph_index,  FT_KERNING_DEFAULT, &d );
                    kern_pair_dx = (FT_Int) d.x >> 6;
                    kern_pair_dy = (FT_Int) d.y >> 6;
                }
            }
        }
        
        // Add character
        ct.set_advance_X( kern_pair_dx + ct.glyph_entry()->advance_X() );
        ct.set_advance_Y( kern_pair_dy + ct.glyph_entry()->advance_Y() );
        
        line._glyphs.push_back(ct);
    }
    

}

void TextLines::add_line (GlyphCache &cache, const Matrix4 &transform, const Vector3 &scale, DTfloat resolution_scale, const std::string &text_raw)
{
    //
    // Extract text
    //
    
    std::string text = Globals::substitute_global(text_raw);
        
    //
    // Remove all control characters
    //
    
    text = MoreStrings::clean_ctrl_characters(text);
    
    // Default Cache Entry
    GlyphCacheEntry ce;
    ce.set_transform(transform);
    ce.set_scale(scale);
    ce.set_resolution_scale(resolution_scale);
    
    // Default TextCharacter
    TextCharacter   ct;

    _lines.resize(_lines.size() + 1);
    TextLine        &line = _lines.back();

    DTuint          incr;

    for (DTuint i = 0; i < text.size(); i += incr) {
        DTuint c = text[i];
        
        if (c > 127) {
            MoreStrings::extract_unicode (&text[i], incr, c);
        } else {
            incr = 1;
        }

        // Check for escaping
        if (c == '\\') {
            ++i;
            c = text[i];

            // Convert to control characters if necessary
            if (c == 't')       c = '\t';
            else if (c == 'n')  c = '\n';
        
        // Check for formatting change
        } else if (c == '[') {
            std::string command;
            
            // Get matching ']'
            ++i;
            while ( (c = text[i]) != ']' && i < text.size() ) {
                command += static_cast<std::string::value_type>(c);
                ++i;
            }
                                
            if (command == "j>") {     
                ct.set_justification(TextCharacter::JUSTIFY_RIGHT);
                continue;
            } else if (command == "j<") {
                ct.set_justification(TextCharacter::JUSTIFY_LEFT);
                continue;
            } else if (command == "j><") {  
                ct.set_justification(TextCharacter::JUSTIFY_CENTER);
                continue;
            } else if (command == "j<>") { 
                ct.set_justification(TextCharacter::JUSTIFY_FULL);
                continue;
            }
            
            if (command == "vlr") { 
                ct.set_direction(TextCharacter::DIR_VERT_LEFT_TO_RIGHT);
                continue;
            } else if (command == "hlr") { 
                ct.set_direction(TextCharacter::DIR_HORZ_LEFT_TO_RIGHT);
                continue;
            } else if (command == "vrl") { 
                ct.set_direction(TextCharacter::DIR_VERT_RIGHT_TO_LEFT);
                continue;
            } else if (command == "hrl") { 
                ct.set_direction(TextCharacter::DIR_HORZ_RIGHT_TO_LEFT);
                continue;
            }
            
            if (command == "ww") {
                ct.set_word_wrap(TextCharacter::WRAP_WORD);         // Word wrapping
                continue;
            } else if (command == "cw") {
                ct.set_word_wrap(TextCharacter::WRAP_CHARACTER);    // Character wrapping
                continue;
            } else if (command == "nw") {
                ct.set_word_wrap(TextCharacter::WRAP_NONE);         // No wrapping
                continue;
            }
                            
            // Parameterized commands
            std::vector<std::string> params = MoreStrings::split(command,"=");
            if (params.size() == 2) {
                
                // Glyph Entry stuff
                if (params[0] == "size") {
                    ce.set_size(MoreStrings::cast_from_string<DTuint>(params[1]));
                    continue;
                }
                
                if (params[0] == "kern") {
                    ct.set_kern(MoreStrings::cast_from_string<DTfloat>(params[1]));
                    continue;
                }
                
                if (params[0] == "font")    {   
                    std::shared_ptr<FontResource> font = FontResource::import_resource(FilePath(params[1]));
                    ce.set_font(font);
                    continue;
                }

                // Text character stuff
                if (params[0] == "color") {
                    ct.set_color(MoreStrings::cast_from_string<Color4f>(params[1]));
                    continue;
                }

                if (params[0] == "outlinecolor") {
                    ct.set_outline_color(MoreStrings::cast_from_string<Color4f>(params[1]));
                    continue;
                }

                if (params[0] == "outlinesize") {
                    ct.set_outline_size(MoreStrings::cast_from_string<DTuint>(params[1]));
                    continue;
                }

                if (params[0] == "tab") {
                    ct.set_tab_size(MoreStrings::cast_from_string<DTuint>(params[1]));
                    continue;
                }
                
                if (params[0] == "baseline") {
                    ct.set_baseline_shift(MoreStrings::cast_from_string<DTuint>(params[1]));
                    continue;
                }
                                
                // Unicode character
                if (params[0] == "U") {
                    c = MoreStrings::cast_from_string<DTuint>(params[1]);
                }
                
                                
                // More later I'm sure
            }

            
        }
    
        // Set the character in the glyph cache entry
        ce.set_character(c);

    
        // Add the entry        
        if (ce.font()) {
            std::shared_ptr<GlyphCacheEntry> &e = cache.entry(ce);
            
            // Set the glyph cache entry
            ct.set_glyph_entry(e);
            
            // makes sure glyph is loaded
            e->glyph();
            
            //
            // Append the character
            //
            
            FT_Int kern_pair_dx = 0;
            FT_Int kern_pair_dy = 0;
            
            // Check for kerning pairs
            if (line._glyphs.size() > 0) {
                TextCharacter &c_prev = line._glyphs.back();
                
                if (c_prev.glyph_entry()->is_same_family( ct.glyph_entry() )) {
                
                    FT_Face face = ct.glyph_entry()->font()->typeface();
                    
                    FT_Int last_glyph_index = ::FT_Get_Char_Index( face, c_prev.glyph_entry()->character());
                    FT_Int glyph_index = ::FT_Get_Char_Index( face, ct.glyph_entry()->character());
                    
                    FT_Bool use_kerning = FT_HAS_KERNING(face);
                    if (use_kerning) {
                        FT_Vector  d;
                        
                        // Return the kerning vector between two glyphs of a same face.
                        ::FT_Get_Kerning( face, last_glyph_index, glyph_index,  FT_KERNING_DEFAULT, &d );
                        kern_pair_dx = (FT_Int) d.x >> 6;
                        kern_pair_dy = (FT_Int) d.y >> 6;
                    }
                }
            }
                        
            // Add character
            ct.set_advance_X( kern_pair_dx + ct.glyph_entry()->advance_X() +
                            ((ct.direction() == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || ct.direction() == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) ? ct.kern() * ct.glyph_entry()->size() : 0.0F) );
            ct.set_advance_Y( kern_pair_dy + ct.glyph_entry()->advance_Y() +
                            ((ct.direction() == TextCharacter::DIR_VERT_LEFT_TO_RIGHT || ct.direction() == TextCharacter::DIR_VERT_RIGHT_TO_LEFT) ? ct.kern() * ct.glyph_entry()->size() : 0.0F) );
            
            line._glyphs.push_back(ct);

        }
    }

}

void TextLines::clear (void)
{
    _lines.clear();
}

//==============================================================================
//==============================================================================

void TextLines::wrap (DTfloat width, DTfloat height, Alignment alignment)
{
    
    //
    // Split the lines
    //
    
    FOR_EACH (i, _lines) {
        std::list<TextCharacter> &glyphs = i->_glyphs;
        
        // Skip empty lines
        if (glyphs.size() <= 0)
            continue;
        
        // Scan the line
        auto split = glyphs.end();
        TextCharacter::Direction direction = glyphs.front().direction(); // Use direction from first glyph
        
        DTfloat extent = glyphs.front().outline_size();
        DTint character_count = 0;
               
        DTfloat max_extent = (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) ? width : height;
        max_extent -= glyphs.front().outline_size();
        
        for (auto j = glyphs.begin(); j != glyphs.end(); ++j, ++character_count) {
            TextCharacter &ct = *j;
            
            // If we hit a newline, split after here and add a new line
            if (ct.glyph_entry()->character() == '\n') {
                TextLine new_line;
                new_line._glyphs.splice(new_line._glyphs.begin(), i->_glyphs, std::next(j), i->_glyphs.end());
                
                _lines.insert(std::next(i), new_line);    // Set ourselves up to continue on the next line
                break;
            }
            
            // If we hit a space, it's a nice place to break so we remember it
            if (ct.glyph_entry()->character() == ' ') {
                split = j;
            }
            if (ct.word_wrap() == TextCharacter::WRAP_CHARACTER) {
                split = j;
            }
            
            extent += ct.extent();
            
            // If our extents bring us out of bounds
            if (extent > max_extent) {
                
                if (ct.word_wrap() != TextCharacter::WRAP_NONE) {
                
                    // We have a place to split
                    if (split != glyphs.end()) {
                        TextLine new_line;
                        
                        if (split->glyph_entry()->character() == ' ' || split == glyphs.begin())
                            new_line._glyphs.splice(new_line._glyphs.begin(), i->_glyphs, std::next(split), i->_glyphs.end());
                        else
                            new_line._glyphs.splice(new_line._glyphs.begin(), i->_glyphs, split, i->_glyphs.end());
                        
                        i->_auto_split = true;
                    
                        _lines.insert(std::next(i), new_line);    // Set ourselves up to continue on the next line
                        break;
                    
                    // No place to split, so we split before this character (or after if it's the only one)
                    } else {
                        TextLine new_line;
                    
                        if (character_count > 0)
                            new_line._glyphs.splice(new_line._glyphs.begin(), i->_glyphs, j, i->_glyphs.end());
                        else
                            new_line._glyphs.splice(new_line._glyphs.begin(), i->_glyphs, std::next(j), i->_glyphs.end());
                        
                        i->_auto_split = true;

                        _lines.insert(std::next(i), new_line);    // Set ourselves up to continue on the next line
                        break;
                    
                    }
                    
                }
            
            }
        
        }
    }
    
    //
    // Position all of the lines
    //
    
    if (_lines.size() == 0 || _lines.size() == 0)
        return;
        
    TextCharacter::Direction direction = _lines.front().direction();

    DTfloat extent_perp_offset = 0.0F;
    DTfloat extent_perp_max = (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) ? height : width;
    DTfloat extent_max = (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) ? width : height;
    
    switch (alignment) {
        case ALIGN_TOP:
            if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)         extent_perp_offset = extent_perp_max;
            else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)    extent_perp_offset = 0.0F;
            else                                                            extent_perp_offset = extent_perp_max;
                
            break;
            
        case ALIGN_MIDDLE: {
            DTfloat extent = extents_perp();
            if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)         extent_perp_offset = extent_perp_max - (extent_perp_max - extent) * 0.5F;
            else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)    extent_perp_offset = (extent_perp_max - extent) * 0.5F;
            else                                                            extent_perp_offset = extent_perp_max - (extent_perp_max - extent) * 0.5F;

            } break;
            
        case ALIGN_BOTTOM: {
            DTfloat extent = extents_perp();
            if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)         extent_perp_offset = extent_perp_max - (extent_perp_max - extent);
            else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)    extent_perp_offset = extent_perp_max - extent;
            else                                                            extent_perp_offset = extent_perp_max - (extent_perp_max - extent);

            } break;
    }
    
    for (auto &line : _lines) {
        std::list<TextCharacter> &glyphs = line._glyphs;

        // Skip empty lines
        if (glyphs.size() <= 0)
            continue;

        TextCharacter::Justification justify = line.justification();
        
        DTfloat extent_offset = 0.0F;

        switch (justify) {
            case TextCharacter::JUSTIFY_LEFT: {
                    if (direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)         extent_offset = line_extents(line);
                    else if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)    extent_offset = extent_max - glyphs.back().extent();
                    else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)    extent_offset = extent_max - glyphs.back().extent();
                    else                                                            extent_offset = 0.0F;
                } break;
            
            case TextCharacter::JUSTIFY_RIGHT: {
                    if (direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)         extent_offset = extent_max;
                    else if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)    extent_offset = line_extents(line) - glyphs.back().extent();
                    else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)    extent_offset = line_extents(line) - glyphs.back().extent();
                    else                                                            extent_offset = extent_max - line_extents(line);
                } break;
            
            case TextCharacter::JUSTIFY_CENTER: {
                    if (direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)         extent_offset = extent_max - (extent_max - line_extents(line)) * 0.5F;
                    else if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)    extent_offset = extent_max - (extent_max - line_extents(line)) * 0.5F - glyphs.back().extent() * 0.5F;
                    else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)    extent_offset = extent_max - (extent_max - line_extents(line)) * 0.5F - glyphs.back().extent() * 0.5F;
                    else                                                            extent_offset = (extent_max - line_extents(line)) * 0.5F;
                } break;
            
            case TextCharacter::JUSTIFY_FULL: {
                    DTfloat actual_extent = line_extents(line);
                    
                    if (direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)         extent_offset = extent_max;
                    else if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)    extent_offset = extent_max - glyphs.front().extent();
                    else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)    extent_offset = extent_max - glyphs.front().extent();
                    else                                                            extent_offset = 0.0F;
                    
                    if (line._auto_split) {
                    
                        // Build list of spaces
                        std::vector<TextCharacter*> spaces;
                        DTboolean                   found_char = false;
                        
                        for (auto j = glyphs.rbegin(); j != glyphs.rend(); ++j) {
                            TextCharacter &ct = *j;
                            
                            if (ct.glyph_entry()->character() != ' ' && ct.glyph_entry()->character() != '\n')
                                found_char = true;
                            
                            if (found_char && ct.glyph_entry()->character() == ' ') {
                                spaces.push_back(&ct);
                            }
                        }

                        // Expand the spaces
                        if (spaces.size() > 0) {
                            for (DTuint i = 0; actual_extent < extent_max; ++i) {
                                TextCharacter *space = spaces[i%spaces.size()];
                                
                                if (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)
                                    space->set_advance_X(space->advance_X() + 1);
                                else
                                    space->set_advance_Y(space->advance_Y() + 1);

                                actual_extent += 1;
                            }
                        }
                        
                    }
                                        
                } break;
        };
        
        
        // Increment in direction of ExtentsPerp
        if (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)         
            extent_perp_offset = extent_perp_offset - line_extents_perp(line);
        else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)                                                     
            extent_perp_offset = extent_perp_offset + line_extents_perp(line);
        else if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)                                                     
            extent_perp_offset = extent_perp_offset - line_extents_perp(line);

        line.set_extents_offset(extent_offset);
        line.set_extents_perp_offset(extent_perp_offset);

        // Increment in direction of ExtentsPerp
        if (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)         
            extent_perp_offset = extent_perp_offset - line_baseline_shift(line);
        else if (direction == TextCharacter::DIR_VERT_LEFT_TO_RIGHT)                                                     
            extent_perp_offset = extent_perp_offset + line_baseline_shift(line);
        else if (direction == TextCharacter::DIR_VERT_RIGHT_TO_LEFT)                                                     
            extent_perp_offset = extent_perp_offset - line_baseline_shift(line);
    }
    
}

//==============================================================================
//==============================================================================

std::list<TextLines> TextLines::paginate (DTfloat width, DTfloat height)
{

    TextCharacter::Direction direction = _lines.front().direction();
    DTfloat extent_perp_max = (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) ? height : width;

    std::list<TextLines> lines;

    TextLines new_lines;
    DTfloat new_lines_extents_perp = extent_perp_max;
    
    for (auto &line : _lines) {
        DTfloat line_extent_perp = line_extents_perp(line);
        DTfloat line_baseline = line_baseline_shift(line);
        
        if ( new_lines_extents_perp - line_extent_perp - line_baseline < 0.0F ) {
                
            lines.push_back(new_lines);
            new_lines.clear();
            new_lines_extents_perp = extent_perp_max;
        }
        
        // Skip blank lines on top
        if (new_lines._lines.size() == 0 && line.is_blank())
            continue;
        
        // Subtract line perp
        new_lines_extents_perp -= line_extent_perp;
        
        // Add the line to the new set
        new_lines._lines.push_back(line);
        new_lines._lines.back().set_extents_perp_offset(new_lines_extents_perp);
        new_lines_extents_perp -= line_baseline;
    }   
    
    // Finish off last page 
    if (new_lines._lines.size() > 0)
        lines.push_back(new_lines);
    
    return lines;
}

//==============================================================================
//==============================================================================

DTfloat TextLines::extents (void) const
{
    DTfloat width = 0.0F;

    for (auto &line : _lines) {
        width = MoreMath::max(width, line_extents(line));
    }

    return width;
}


DTfloat TextLines::extents_perp (void) const
{
    DTfloat height = 0.0F;

    for (auto &line : _lines) {
        height = height + line_extents_perp(line) + line_baseline_shift(line);
    }
    
    return height;
}

//==============================================================================
//==============================================================================

DTfloat TextLines::line_extents (const TextLine &line) const
{
    const std::list<TextCharacter> &glyphs = line._glyphs;
    DTfloat e = 0.0F;
    
    for (auto &ct : glyphs) {
        if (ct.glyph_entry()->character() == '\t') {
            e += ct.tab_size() * ct.glyph_entry()->scale().x;
        } else {
            e += ct.extent();
        }
        
    }
    
    // Subtract trailing spaces and newlines
    for (auto i = glyphs.rbegin(); i != glyphs.rend(); ++i) {
        const TextCharacter &ct = *i;

        if (ct.glyph_entry()->character() != ' ' && ct.glyph_entry()->character() != '\n')
            break;
            
        e -= ct.extent();
    }
    
    return e;
}

DTfloat TextLines::line_extents_perp (const TextLine &line) const
{
    const std::list<TextCharacter> &glyphs = line._glyphs;
    DTfloat e = 0;

    for (auto &ct : glyphs) {
        e = MoreMath::max(e, ct.extent_perp());
    }
                
    return e;
}

DTfloat TextLines::line_baseline_shift (const TextLine &line) const
{
    const std::list<TextCharacter> &glyphs = line._glyphs;
    DTfloat max_shift = 0;
    
    for (auto &ct : glyphs) {
        max_shift = MoreMath::max(max_shift, ct.baseline_shift());
    }
    
    return max_shift;
}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: TextRenderer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Text/TextRenderer.hpp"
#include "DT3Core/Types/Text/GlyphCacheEntry.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {
                                            
//==============================================================================
//==============================================================================

void TextRenderer::raster_callback(const int y, const int count, const FT_Span * const spans, void * const user)
{
    RasterInfo *raster_info = (RasterInfo*) user;
    
    DTint y_offset = y + (DTint) raster_info->_offset.y;
    
    // Check if outsize of image
    if (y_offset < 0)
        return;

    // Check if outsize of image
    if (y_offset >= raster_info->_destination->height())
        return;
    
    for (int i = 0; i < count; ++i) {
    
        // Clip span
        DTint x_min = (DTint) raster_info->_offset.x + spans[i].x;
        DTint x_max = x_min + spans[i].len - 1;
        
        if (x_min < 0)
            x_min = 0;
        if (x_max >= raster_info->_destination->width())
            x_max = raster_info->_destination->width()-1;
        if (x_max < x_min)
            continue;
        
        Color4f c = raster_info->_color * Color4f(255,255,255,spans[i].coverage);
        Color4b* p = (Color4b*) raster_info->_destination->pixel_addr(x_min, y_offset);
    
        // Fill span
        for (DTint x = x_min; x <= x_max; ++x) {
            DTfloat src_a = c.a_as_float();
            DTfloat one_minus_src_a = 1.0F - src_a;
            DTfloat dst_a = p->a_as_float();
            
            // This compositing code from Porter et al.
            
            DTfloat a_float = src_a + dst_a * one_minus_src_a;
            if (a_float <= 0.0F) {
                *p = Color4b( (DTubyte) 0, (DTubyte) 0, (DTubyte) 0, (DTubyte) 0);
                
            } else {
                DTfloat denom = 1.0F / a_float;
                
                p->set_r((c.r * src_a + p->r * one_minus_src_a * dst_a) * denom);
                p->set_g((c.g * src_a + p->g * one_minus_src_a * dst_a) * denom);
                p->set_b((c.b * src_a + p->b * one_minus_src_a * dst_a) * denom);
                p->set_a(a_float);
            }
            

            ++p;
        }
    }

}

//==============================================================================
//==============================================================================

void TextRenderer::begin_render (std::shared_ptr<TextureResource2D> &destination)
{
    FT_Int  width = destination->width();
    FT_Int  height = destination->height();
    DTubyte *buffer = destination->buffer();
    
    ASSERT(destination->format() == DT3GL_FORMAT_RGBA);
    
    if (destination->format() == DT3GL_FORMAT_RGBA) {
        ::memset(buffer,0,width * height * 4);  // Clear to black
    }
}

void TextRenderer::end_render (std::shared_ptr<TextureResource2D> &destination)
{

}

//==============================================================================
//==============================================================================

Rectangle TextRenderer::render_lines (  std::shared_ptr<TextureResource2D>  &destination,
                                        TextLines                           &lines,
                                        const Vector2                       &offset)
{
    if (lines.lines().size() <= 0)
        return Rectangle(0.0F,0.0F,0.0F,0.0F);

    FT_Int  width = destination->width();
    FT_Int  height = destination->height();
        
    Rectangle rect(0.0F,0.0F,0.0F,0.0F);
    DTuint num_chars = 0;

    for (auto &line : lines.lines()) {
        
        TextCharacter::Direction direction = line.direction();
    
        DTfloat x_pos = (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) ? line.extents_offset() : line.extents_perp_offset();
        DTfloat y_pos = (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT || direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) ? line.extents_perp_offset() : line.extents_offset();
        
        const std::list<TextCharacter> &glyphs = line.glyphs();
                
        for (auto &ct : glyphs) {
            
            Vector2 p;

            if (direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT) {
                p = ct.glyph_entry()->transform() * Vector2(x_pos - ct.extent() + ct.outline_size(),y_pos + ct.outline_size());
            } else {
                p = ct.glyph_entry()->transform() * Vector2(x_pos + ct.outline_size(),y_pos + ct.outline_size());
            }
                    
            FT_Glyph glyph = ct.glyph_entry()->glyph();
            if (glyph && ct.glyph_entry()->character() != '\n') {
                
                // check bounding box; if the transformed glyph image   
                // is not in our target surface, we can avoid rendering it
                FT_BBox bbox;
                ::FT_Glyph_Get_CBox( glyph, FT_GLYPH_BBOX_PIXELS, &bbox );
                
                FT_Vector total_offset = { (FT_Pos) (p.x + offset.x),
                                           (FT_Pos) (p.y + offset.y) };
                
                if ( (bbox.xMax + total_offset.x) <= 0 || (bbox.xMin + total_offset.x) >= width  ||
                     (bbox.yMax + total_offset.y) <= 0 || (bbox.yMin + total_offset.y) >= height ) {
                    // Do nothing
                } else {
                
                    RasterInfo raster_info;
                    raster_info._destination = destination.get();
                    raster_info._offset = total_offset;

                    FT_Raster_Params params;
                    memset(&params, 0, sizeof(params));
                    params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
                    params.gray_spans = TextRenderer::raster_callback;
                    params.user = &raster_info;
                    
                    FT_Outline *o;

                    //
                    // Outline
                    //

                    if (ct.outline_size() > 0.0F) {
                        
                        raster_info._color = ct.outline_color();
                        raster_info._alpha_blend = false;

                        // Set up a stroker.
                        FT_Stroker stroker;
                        FT_Stroker_New(FontResource::library(), &stroker);
                        FT_Stroker_Set(stroker,
                                       (DTint)(ct.outline_size() * 64),
                                       FT_STROKER_LINECAP_ROUND,
                                       FT_STROKER_LINEJOIN_ROUND,
                                       0);

                        FT_Glyph glyph_outline = glyph;
                        FT_Glyph_StrokeBorder(&glyph_outline, stroker, 0, 0);

                        o = &reinterpret_cast<FT_OutlineGlyph>(glyph_outline)->outline;
                        FT_Outline_Render(FontResource::library(), o, &params);

                        FT_Stroker_Done(stroker);
                        FT_Done_Glyph(glyph_outline);
                    }

                    //
                    // Fill
                    //
                    
                    raster_info._color = ct.color();
                    raster_info._alpha_blend = (ct.outline_size() > 0.0F);

                    o = &reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
                    FT_Outline_Render(FontResource::library(), o, &params);
                    
                }
                
                // Update bounding box
                if (num_chars == 0) {
                    rect.set(   bbox.xMin + total_offset.x, bbox.xMax + total_offset.x,
                                bbox.yMin + total_offset.y, bbox.yMax + total_offset.y);
                } else {
                    Rectangle r(bbox.xMin + total_offset.x, bbox.xMax + total_offset.x,
                                bbox.yMin + total_offset.y, bbox.yMax + total_offset.y);
                    
                    rect = Rectangle::calc_union(rect, r);
                }
                
                ++num_chars;
            }
                
            if (direction == TextCharacter::DIR_HORZ_LEFT_TO_RIGHT)         x_pos += ct.extent();
            else if (direction == TextCharacter::DIR_HORZ_RIGHT_TO_LEFT)    x_pos -= ct.extent();
            else                                                            y_pos -= ct.extent();
            
        }
        
    }
    
    return rect;
}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: DrawUtils.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Graphics/DrawUtils.hpp"
#include "DT3Core/Types/Graphics/BitFont.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Objects/CameraObject.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

void DrawUtils::draw_quad_stretch_center_3x3 (  DrawBatcher &draw_batcher,
                                            const std::shared_ptr<CameraObject> &camera,
                                            const std::shared_ptr<MaterialResource> &material,
                                            const std::shared_ptr<ShaderResource> &shader,
                                            const Color4f &color,
                                            const Matrix4 &transform,
                                            DTfloat xpos,
                                            DTfloat ypos,
                                            DTfloat xsize,
                                            DTfloat ysize,
                                            DTfloat corner_width,
                                            DTfloat corner_height)
{
	
	// Adjust corner width
	if (corner_width > xsize * 0.5F)
		corner_width = xsize * 0.5F;
		
	if (corner_height > ysize * 0.5F)
		corner_height = ysize * 0.5F;
	
		
	// Makes the corners of the image. This means we have to draw nine rectangles.
	DTfloat one_third_x = 1.0F / 3.0F;
	DTfloat two_thirds_x = 1.0F - one_third_x;
	DTfloat one_third_y = 1.0F / 3.0F;
	DTfloat two_thirds_y = 1.0F - one_third_y;
	
	const DTfloat minus_x = xpos;
	const DTfloat plus_x = xpos + xsize;

	const DTfloat minus_y = ypos;
	const DTfloat plus_y = ypos + ysize;
	
	DTfloat inner_left = minus_x + corner_width;
	DTfloat inner_right = plus_x - corner_width;

	DTfloat inner_top = plus_y - corner_height;
	DTfloat inner_bottom = minus_y + corner_height;
		
	if (inner_left > inner_right)	inner_left = inner_right = (minus_x + plus_x) * 0.5F;
	if (inner_bottom > inner_top)	inner_bottom = inner_top = (minus_y + plus_y) * 0.5F;
					
	draw_batcher.batch_begin(camera, material, shader, transform, DT3GL_PRIM_TRI_STRIP, DrawBatcher::FMT_V | DrawBatcher::FMT_T0 | DrawBatcher::FMT_C);
    
    // Bottom row
    draw_batcher.add().v(minus_x,		inner_bottom,	0.0F)   .t0(0.0F,			one_third_y)    .c(color);
    draw_batcher.add().v(minus_x,       minus_y,        0.0F)   .t0(0.0F,           0.0F)           .c(color);
    draw_batcher.add().v(inner_left,	inner_bottom,	0.0F)   .t0(one_third_x,	one_third_y)    .c(color);
    draw_batcher.add().v(inner_left,    minus_y,		0.0F)   .t0(one_third_x,    0.0F)           .c(color);
    draw_batcher.add().v(inner_right,	inner_bottom,	0.0F)   .t0(two_thirds_x,	one_third_y)    .c(color);
    draw_batcher.add().v(inner_right,	minus_y,		0.0F)   .t0(two_thirds_x,	0.0F)           .c(color);
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,			one_third_y)    .c(color);
	draw_batcher.add().v(plus_x,		minus_y,		0.0F)   .t0(1.0F,			0.0F)           .c(color);
    
	draw_batcher.add().v(plus_x,		minus_y,		0.0F)   .t0(1.0F,			0.0F)           .c(color);  // Degenerate
	draw_batcher.add().v(plus_x,		minus_y,		0.0F)   .t0(1.0F,			0.0F)           .c(color);  // Degenerate
    
    // Middle row
	draw_batcher.add().v(minus_x,		inner_top,		0.0F)   .t0(0.0F,			two_thirds_y)   .c(color);
	draw_batcher.add().v(minus_x,		inner_bottom,	0.0F)   .t0(0.0F,			one_third_y)    .c(color);
	draw_batcher.add().v(inner_left,	inner_top,		0.0F)   .t0(one_third_x,	two_thirds_y)   .c(color);
	draw_batcher.add().v(inner_left,	inner_bottom,	0.0F)   .t0(one_third_x,	one_third_y)    .c(color);
    draw_batcher.add().v(inner_right,	inner_top,		0.0F)   .t0(two_thirds_x,	two_thirds_y)   .c(color);
	draw_batcher.add().v(inner_right,	inner_bottom,	0.0F)   .t0(two_thirds_x,	one_third_y)    .c(color);
    draw_batcher.add().v(plus_x,		inner_top,		0.0F)   .t0(1.0F,			two_thirds_y)   .c(color);
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,			one_third_y)    .c(color);
    
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,			one_third_y)    .c(color);  // Degenerate
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,			one_third_y)    .c(color);  // Degenerate
	
    // Top row
	draw_batcher.add().v(minus_x,		plus_y,			0.0F)   .t0(0.0F,			1.0F)           .c(color);
	draw_batcher.add().v(minus_x,		inner_top,		0.0F)   .t0(0.0F,			two_thirds_y)   .c(color);
	draw_batcher.add().v(inner_left,	plus_y,			0.0F)   .t0(one_third_x,	1.0F)           .c(color);
	draw_batcher.add().v(inner_left,	inner_top,		0.0F)   .t0(one_third_x,	two_thirds_y)   .c(color);
    draw_batcher.add().v(inner_right,	plus_y,			0.0F)   .t0(two_thirds_x,	1.0F)           .c(color);
	draw_batcher.add().v(inner_right,	inner_top,		0.0F)   .t0(two_thirds_x,	two_thirds_y)   .c(color);
	draw_batcher.add().v(plus_x,		plus_y,			0.0F)   .t0(1.0F,			1.0F)           .c(color);
	draw_batcher.add().v(plus_x,		inner_top,		0.0F)   .t0(1.0F,			two_thirds_y)   .c(color);

	draw_batcher.batch_end();
    draw_batcher.flush();
}

//==============================================================================
//==============================================================================

void DrawUtils::draw_quad_stretch_center_2x2 (  DrawBatcher &draw_batcher,
                                            const std::shared_ptr<CameraObject> &camera,
                                            const std::shared_ptr<MaterialResource> &material,
                                            const std::shared_ptr<ShaderResource> &shader,
                                            const Color4f &color,
                                            const Matrix4 &transform,
                                            DTfloat xpos,
                                            DTfloat ypos,
                                            DTfloat xsize,
                                            DTfloat ysize,
                                            DTfloat corner_width,
                                            DTfloat corner_height)
{
	
	// Adjust corner width
	if (corner_width > xsize * 0.5F)
		corner_width = xsize * 0.5F;
		
	if (corner_height > ysize * 0.5F)
		corner_height = ysize * 0.5F;
	
	const DTfloat minus_x = xpos;
	const DTfloat plus_x = xpos + xsize;

	const DTfloat minus_y = ypos;
	const DTfloat plus_y = ypos + ysize;
	
	DTfloat inner_left = minus_x + corner_width;
	DTfloat inner_right = plus_x - corner_width;

	DTfloat inner_top = plus_y - corner_height;
	DTfloat inner_bottom = minus_y + corner_height;
		
	if (inner_left > inner_right)	inner_left = inner_right = (minus_x + plus_x) * 0.5F;
	if (inner_bottom > inner_top)	inner_bottom = inner_top = (minus_y + plus_y) * 0.5F;
					
	draw_batcher.batch_begin(camera, material, shader, transform, DT3GL_PRIM_TRI_STRIP, DrawBatcher::FMT_V | DrawBatcher::FMT_T0 | DrawBatcher::FMT_C);
    
    // Bottom row
    draw_batcher.add().v(minus_x,		inner_bottom,	0.0F)   .t0(0.0F,   0.5F)   .c(color);
    draw_batcher.add().v(minus_x,       minus_y,        0.0F)   .t0(0.0F,   0.0F)   .c(color);
    draw_batcher.add().v(inner_left,	inner_bottom,	0.0F)   .t0(0.5F,	0.5F)   .c(color);
    draw_batcher.add().v(inner_left,    minus_y,		0.0F)   .t0(0.5F,   0.0F)   .c(color);
    draw_batcher.add().v(inner_right,	inner_bottom,	0.0F)   .t0(0.5F,	0.5F)   .c(color);
    draw_batcher.add().v(inner_right,	minus_y,		0.0F)   .t0(0.5F,	0.0F)   .c(color);
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,   0.5F)   .c(color);
	draw_batcher.add().v(plus_x,		minus_y,		0.0F)   .t0(1.0F,   0.0F)   .c(color);
    
	draw_batcher.add().v(plus_x,		minus_y,		0.0F)   .t0(1.0F,   0.0F)   .c(color);  // Degenerate
	draw_batcher.add().v(plus_x,		minus_y,		0.0F)   .t0(1.0F,   0.0F)   .c(color);  // Degenerate
    
    // Middle row
	draw_batcher.add().v(minus_x,		inner_top,		0.0F)   .t0(0.0F,   0.5F)   .c(color);
	draw_batcher.add().v(minus_x,		inner_bottom,	0.0F)   .t0(0.0F,   0.5F)   .c(color);
	draw_batcher.add().v(inner_left,	inner_top,		0.0F)   .t0(0.5F,	0.5F)   .c(color);
	draw_batcher.add().v(inner_left,	inner_bottom,	0.0F)   .t0(0.5F,	0.5F)   .c(color);
    draw_batcher.add().v(inner_right,	inner_top,		0.0F)   .t0(0.5F,	0.5F)   .c(color);
	draw_batcher.add().v(inner_right,	inner_bottom,	0.0F)   .t0(0.5F,	0.5F)   .c(color);
    draw_batcher.add().v(plus_x,		inner_top,		0.0F)   .t0(1.0F,   0.5F)   .c(color);
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,   0.5F)   .c(color);
    
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,   0.5F)   .c(color);  // Degenerate
	draw_batcher.add().v(plus_x,		inner_bottom,	0.0F)   .t0(1.0F,   0.5F)   .c(color);  // Degenerate
	
    // Top row
	draw_batcher.add().v(minus_x,		plus_y,			0.0F)   .t0(0.0F,   1.0F)   .c(color);
	draw_batcher.add().v(minus_x,		inner_top,		0.0F)   .t0(0.0F,   0.5F)   .c(color);
	draw_batcher.add().v(inner_left,	plus_y,			0.0F)   .t0(0.5F,	1.0F)   .c(color);
	draw_batcher.add().v(inner_left,	inner_top,		0.0F)   .t0(0.5F,	0.5F)   .c(color);
    draw_batcher.add().v(inner_right,	plus_y,			0.0F)   .t0(0.5F,	1.0F)   .c(color);
	draw_batcher.add().v(inner_right,	inner_top,		0.0F)   .t0(0.5F,	0.5F)   .c(color);
	draw_batcher.add().v(plus_x,		plus_y,			0.0F)   .t0(1.0F,   1.0F)   .c(color);
	draw_batcher.add().v(plus_x,		inner_top,		0.0F)   .t0(1.0F,   0.5F)   .c(color);

	draw_batcher.batch_end();
    draw_batcher.flush();
}

//==============================================================================
//==============================================================================

void DrawUtils::draw_selection (    DrawBatcher &draw_batcher,
                                    const std::shared_ptr<CameraObject> &camera,
                                    const std::shared_ptr<MaterialResource> &material,
                                    const std::shared_ptr<ShaderResource> &shader,
                                    const Matrix4 &transform,
                                    const Color4b &color,
                                    DTfloat radius)
{
    const DTfloat SEL_size = radius * 0.2F;

    // Draw box around object
    draw_batcher.batch_begin(camera, material, shader, transform, DT3GL_PRIM_LINES, DrawBatcher::FMT_V | DrawBatcher::FMT_C);
    
    // - - -
    draw_batcher.add().v(-radius,-radius,-radius)           .c(color);
    draw_batcher.add().v(-radius+SEL_size,-radius,-radius)  .c(color);
    draw_batcher.add().v(-radius,-radius,-radius)           .c(color);
    draw_batcher.add().v(-radius,-radius+SEL_size,-radius)  .c(color);
    draw_batcher.add().v(-radius,-radius,-radius)           .c(color);
    draw_batcher.add().v(-radius,-radius,-radius+SEL_size)  .c(color);
    
    // - - +
    draw_batcher.add().v(-radius,-radius,+radius)           .c(color);
    draw_batcher.add().v(-radius+SEL_size,-radius,+radius)  .c(color);
    draw_batcher.add().v(-radius,-radius,+radius)           .c(color);
    draw_batcher.add().v(-radius,-radius+SEL_size,+radius)  .c(color);
    draw_batcher.add().v(-radius,-radius,+radius)           .c(color);
    draw_batcher.add().v(-radius,-radius,+radius-SEL_size)  .c(color);

    // - + -
    draw_batcher.add().v(-radius,+radius,-radius)           .c(color);
    draw_batcher.add().v(-radius+SEL_size,+radius,-radius)  .c(color);
    draw_batcher.add().v(-radius,+radius,-radius)           .c(color);
    draw_batcher.add().v(-radius,+radius-SEL_size,-radius)  .c(color);
    draw_batcher.add().v(-radius,+radius,-radius)           .c(color);
    draw_batcher.add().v(-radius,+radius,-radius+SEL_size)  .c(color);

    // - + +
    draw_batcher.add().v(-radius,+radius,+radius)           .c(color);
    draw_batcher.add().v(-radius+SEL_size,+radius,+radius)  .c(color);
    draw_batcher.add().v(-radius,+radius,+radius)           .c(color);
    draw_batcher.add().v(-radius,+radius-SEL_size,+radius)  .c(color);
    draw_batcher.add().v(-radius,+radius,+radius)           .c(color);
    draw_batcher.add().v(-radius,+radius,+radius-SEL_size)  .c(color);

    // + - -
    draw_batcher.add().v(+radius,-radius,-radius)           .c(color);
    draw_batcher.add().v(+radius-SEL_size,-radius,-radius)  .c(color);
    draw_batcher.add().v(+radius,-radius,-radius)           .c(color);
    draw_batcher.add().v(+radius,-radius+SEL_size,-radius)  .c(color);
    draw_batcher.add().v(+radius,-radius,-radius)           .c(color);
    draw_batcher.add().v(+radius,-radius,-radius+SEL_size)  .c(color);

    // + - +
    draw_batcher.add().v(+radius,-radius,+radius)           .c(color);
    draw_batcher.add().v(+radius-SEL_size,-radius,+radius)  .c(color);
    draw_batcher.add().v(+radius,-radius,+radius)           .c(color);
    draw_batcher.add().v(+radius,-radius+SEL_size,+radius)  .c(color);
    draw_batcher.add().v(+radius,-radius,+radius)           .c(color);
    draw_batcher.add().v(+radius,-radius,+radius-SEL_size)  .c(color);

    // + + -
    draw_batcher.add().v(+radius,+radius,-radius)           .c(color);
    draw_batcher.add().v(+radius-SEL_size,+radius,-radius)  .c(color);
    draw_batcher.add().v(+radius,+radius,-radius)           .c(color);
    draw_batcher.add().v(+radius,+radius-SEL_size,-radius)  .c(color);
    draw_batcher.add().v(+radius,+radius,-radius)           .c(color);
    draw_batcher.add().v(+radius,+radius,-radius+SEL_size)  .c(color);

    // + + +
    draw_batcher.add().v(+radius,+radius,+radius)           .c(color);
    draw_batcher.add().v(+radius-SEL_size,+radius,+radius)  .c(color);
    draw_batcher.add().v(+radius,+radius,+radius)           .c(color);
    draw_batcher.add().v(+radius,+radius-SEL_size,+radius)  .c(color);
    draw_batcher.add().v(+radius,+radius,+radius)           .c(color);
    draw_batcher.add().v(+radius,+radius,+radius-SEL_size)  .c(color);

    draw_batcher.batch_end();
    draw_batcher.flush();
}

//==============================================================================
//==============================================================================

void DrawUtils::draw_cube ( DrawBatcher &draw_batcher,
                            const std::shared_ptr<CameraObject> &camera,
                            const std::shared_ptr<MaterialResource> &material,
                            const std::shared_ptr<ShaderResource> &shader,
                            const Matrix4 &transform,
                            const Color4b &color,
                            DTfloat size)
{
    draw_batcher.batch_begin(camera, material, shader, transform, DT3GL_PRIM_TRI_STRIP, DrawBatcher::FMT_V | DrawBatcher::FMT_C);
    
    // Sized
    draw_batcher.add().v(-size,size,-size).c(color);
    draw_batcher.add().v(-size,-size,-size).c(color);

    draw_batcher.add().v(size,size,-size).c(color);
    draw_batcher.add().v(size,-size,-size).c(color);

    draw_batcher.add().v(size,size,size).c(color);
    draw_batcher.add().v(size,-size,size).c(color);

    draw_batcher.add().v(-size,size,size).c(color);
    draw_batcher.add().v(-size,-size,size).c(color);

    draw_batcher.add().v(-size,size,-size).c(color);
    draw_batcher.add().v(-size,-size,-size).c(color);

    draw_batcher.add().v(-size,-size,-size).c(color);   // Degenerate

    // Top
    draw_batcher.add().v(-size,size,-size).c(color);
    draw_batcher.add().v(-size,size,size).c(color);
    draw_batcher.add().v(size,size,-size).c(color);
    draw_batcher.add().v(size,size,size).c(color);

    draw_batcher.add().v(size,size,size).c(color);   // Degenerate

    // Bottom
    draw_batcher.add().v(-size,-size,size).c(color);
    draw_batcher.add().v(-size,-size,-size).c(color);
    draw_batcher.add().v(size,-size,size).c(color);
    draw_batcher.add().v(size,-size,-size).c(color);
    
    draw_batcher.batch_end();
    draw_batcher.flush();
}

//==============================================================================
//==============================================================================

void DrawUtils::draw_cone(  DrawBatcher &draw_batcher,
                            const std::shared_ptr<CameraObject> &camera,
                            const std::shared_ptr<MaterialResource> &material,
                            const std::shared_ptr<ShaderResource> &shader,
                            const Matrix4 &transform,
                            const Color4b &color,
                            DTfloat radius,
                            DTfloat length)
{
    draw_batcher.batch_begin(camera, material, shader, transform, DT3GL_PRIM_TRI_STRIP, DrawBatcher::FMT_V | DrawBatcher::FMT_C);
    
    const DTint MAX_SEGMENTS = 16;
    for (DTint i = 0; i < MAX_SEGMENTS; ++i) {
        DTfloat angle = static_cast<DTfloat>(i) / MAX_SEGMENTS * 2.0F * PI;
        DTfloat angle_plus_1 = static_cast<DTfloat>(i+1) / MAX_SEGMENTS * 2.0F * PI;
        
        draw_batcher.add().v(radius * std::cos(angle), -length, radius * std::sin(angle)).c(color);
        draw_batcher.add().v(radius * std::cos(angle_plus_1), -length, radius * std::sin(angle_plus_1)).c(color);
        draw_batcher.add().v(0.0F,length,0.0F).c(color);
    }
    
    draw_batcher.batch_end();
    draw_batcher.flush();
}

//==============================================================================
//==============================================================================

void DrawUtils::draw_ring ( DrawBatcher &draw_batcher,
                            const std::shared_ptr<CameraObject> &camera,
                            const std::shared_ptr<MaterialResource> &material,
                            const std::shared_ptr<ShaderResource> &shader,
                            const Matrix4 &transform,
                            const Color4b &color,
                            DTfloat size)
{
    draw_batcher.batch_begin(camera, material, shader, transform, DT3GL_PRIM_LINE_LOOP, DrawBatcher::FMT_V | DrawBatcher::FMT_C);
    
    const DTfloat MAX_SEGMENTS = 64.0F;
    for (DTuint i = 0; i <= MAX_SEGMENTS; ++i) {
        DTfloat angle = static_cast<DTfloat>(i) / MAX_SEGMENTS * 2.0F * PI;
        draw_batcher.add().v(size * std::cos(angle), 0.0F, size * std::sin(angle)).c(color);
    }
    
    draw_batcher.batch_end();
    draw_batcher.flush();
}

//==============================================================================
//==============================================================================

void DrawUtils::draw_bit_font ( const std::shared_ptr<CameraObject> &camera,
                                const std::shared_ptr<MaterialResource> &material,
                                const std::shared_ptr<ShaderResource> &shader,
                                const Vector2 &translation,
                                const std::string &text)
{
    DTfloat x = translation.x;
    
    for (std::string::size_type i = 0; i < text.size(); ++i) {
//        DTcharacter c = text[i];
        
//        const DTubyte* bytes = BitFont::character_bitmap(c);
//        System::renderer()->draw_bitmap(material, Vector2(x, translation.y), 8, 8, bytes);
        
        x += 8;
    }

}

//==============================================================================
//==============================================================================

void DrawUtils::activate_state (    const std::shared_ptr<CameraObject> &camera,
                                    const std::shared_ptr<MaterialResource> &material,
                                    const std::shared_ptr<ShaderResource> &shader,
                                    const Matrix4 &transform)
{
    if (camera) {
        camera->calculate_frustum();
        
        shader->set_uniform_value(  shader->uniform_slot(DT3GL_UNIFORM_MODELVIEW), camera->modelview() * transform );
        shader->set_uniform_value(  shader->uniform_slot(DT3GL_UNIFORM_PROJECTION), camera->projection() );
    }
    
    
    // Set some shader parameters that were set in the material
    for (DTuint i = 0; i < 16; ++i) {
        material->set_current_unit(i);
        shader->set_uniform_value(  shader->uniform_slot((DT3GLStandardUniform) (DT3GL_UNIFORM_TEX_MATRIX0 + i)), material->transform());
    }

    material->activate();
    shader->activate();
}

//==============================================================================
//==============================================================================

} // DT3

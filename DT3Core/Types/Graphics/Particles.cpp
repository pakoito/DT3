//==============================================================================
///	
///	File: Particles.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(Particles)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Particles::Particles (void)
	:   _default_velocity	(0.0F,0.0F,0.0F),
		_default_size		(1.0F),
		_default_lifetime	(0.0F),
		_default_rotation	(0.0F),
		_default_color		(1.0F,1.0F,1.0F,1.0F),
		_active_start		(0),
		_active_end			(0),
        _stream_size        (0)
{

}
		
Particles::~Particles (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<Particles> &r)
{
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<Particles> &r)
{
	return s;
}

//==============================================================================
//==============================================================================

void Particles::screen_opened (DTuint width, DTuint height)
{

}

void Particles::screen_closed (void)
{
    _translations_resource.reset();
    _texcoords_resource.reset();
    _color_resource.reset();
}

//==============================================================================
//==============================================================================

void Particles::build_velocity_stream	(void)
{
    _velocities.resize(_translations.size(), _default_velocity);
}

void Particles::build_sizes_stream		(void)
{
    _sizes.resize(_translations.size(), _default_size);
}

void Particles::build_lifetimes_stream	(void)
{
    _lifetimes.resize(_translations.size(), _default_lifetime);
}

void Particles::build_colors_stream		(void)
{
    _colors.resize(_translations.size(), _default_color);
}

void Particles::build_rotations_stream	(void)
{
    _rotations.resize(_translations.size(), _default_rotation);
}

//==============================================================================
//==============================================================================

void Particles::resize (DTuint size)
{
	if (_translations.size() != size) {
    
		_translations.resize(size, Vector3(0.0F,0.0F,0.0F));
	
		// Build optional streams
		if (_velocities.size() > 0)     build_velocity_stream();
		if (_sizes.size() > 0)			build_sizes_stream();
		if (_lifetimes.size() > 0)		build_lifetimes_stream();
		if (_colors.size() > 0)         build_colors_stream();
		if (_rotations.size() > 0)      build_rotations_stream();
	}
}

//==============================================================================
//==============================================================================

void Particles::draw (const std::shared_ptr<CameraObject> &camera, DTint first, DTint num)
{

    if (_translations.size() <= 0)
        return;
    
//    DTsize num_particles = _translations.size();
//    
//    
//    std::shared_ptr<ArrayBlock<DTubyte>> vertex_stream = makeSmartPtr(ArrayBlock<DTubyte>::create());
//    vertex_stream->resize(sizeof(Vector3) * 6 * num_particles);
//    Vector3 *vertices = (Vector3*) vertex_stream->getBuffer();
//
//    std::shared_ptr<ArrayBlock<DTubyte>> texcoords_stream = makeSmartPtr(ArrayBlock<DTubyte>::create());
//    texcoords_stream->resize(sizeof(Texcoord2) * 6 * num_particles);
//    Texcoord2 *texcoords = (Texcoord2*) texcoords_stream->getBuffer();
//
//    std::shared_ptr<ArrayBlock<DTubyte>> colors_stream = makeSmartPtr(ArrayBlock<DTubyte>::create());
//    colors_stream->resize(sizeof(Color) * 6 * num_particles);
//    Color *colors = (Color*) colors_stream->getBuffer();
//    
//    
//    Vector3 x_axis = camera->getRightAxis();
//    Vector3 y_axis = camera->getUpAxis();
//    
//    
//    // Fill in the streams
//    for (DTint i = 0; i < num_particles; ++i) {
//        const Vector3 translation = Vector3(_translations[i]);
//
//        // Default texcoords
//        Vector2 tc0(0.0F,0.0F);
//        Vector2 tc1(1.0F,0.0F);
//        Vector2 tc2(1.0F,1.0F);
//        Vector2 tc3(0.0F,1.0F);
//
//        // Default vector offsets
//        Vector3 vp0 = -x_axis - y_axis;
//        Vector3 vp1 = x_axis - y_axis;
//        Vector3 vp2 = x_axis + y_axis;
//        Vector3 vp3 = -x_axis + y_axis;
//
//        // Change size if need be
//        if (_sizes.isValid()) {
//            DTfloat size = _sizes[i];
//
//            vp0 *= size;
//            vp1 *= size;
//            vp2 *= size;
//            vp3 *= size;
//        }
//        
//        // Change rotation if need be
//        if (_rotations.isValid()) {
//            DTfloat rot = _rotations[i];
//
//            DTfloat cosa = std::cos(rot);
//            DTfloat sina = std::sin(rot);
//
//            Matrix2 rotm = Matrix2( cosa,-sina,
//                                    sina,cosa   );
//
//            tc0 -= Vector2(0.5F,0.5F);
//            tc0 = rotm * tc0;
//            tc0 += Vector2(0.5F,0.5F);
//
//            tc1 -= Vector2(0.5F,0.5F);
//            tc1 = rotm * tc1;
//            tc1 += Vector2(0.5F,0.5F);
//
//            tc2 -= Vector2(0.5F,0.5F);
//            tc2 = rotm * tc2;
//            tc2 += Vector2(0.5F,0.5F);
//
//            tc3 -= Vector2(0.5F,0.5F);
//            tc3 = rotm * tc3;
//            tc3 += Vector2(0.5F,0.5F);
//        }
//
//        vp0 += translation;
//        vp1 += translation;
//        vp2 += translation;
//        vp3 += translation;
//
//        vertices[i*6+0] = vp0;
//        vertices[i*6+1] = vp1;
//        vertices[i*6+2] = vp3;
//        vertices[i*6+3] = vp1;
//        vertices[i*6+4] = vp2;
//        vertices[i*6+5] = vp3;
//
//        texcoords[i*6+0] = Texcoord2(tc0.x,tc0.y);
//        texcoords[i*6+1] = Texcoord2(tc1.x,tc1.y);
//        texcoords[i*6+2] = Texcoord2(tc3.x,tc3.y);
//        texcoords[i*6+3] = Texcoord2(tc1.x,tc1.y);
//        texcoords[i*6+4] = Texcoord2(tc2.x,tc2.y);
//        texcoords[i*6+5] = Texcoord2(tc3.x,tc3.y);
//        
//        colors[i*6+0] = _colors[i];
//        colors[i*6+1] = _colors[i];
//        colors[i*6+2] = _colors[i];
//        colors[i*6+3] = _colors[i];
//        colors[i*6+4] = _colors[i];
//        colors[i*6+5] = _colors[i];
//    }
//
//
//    // Lets see if we need to reallocate the buffers
//    if (_stream_size != _translations.size() ||
//        _translations_resource.isNull() ||
//        _texcoords_resource.isNull() ||
//        _color_resource.isNull() ) {
//        
//        _stream_size = _translations.size();
//    
//        System::getRenderer()->createBuffer(_translations_resource, vertex_stream, DT3GL_ACCESS_CPU_WRITE | DT3GL_ACCESS_GPU_READ);
//        System::getRenderer()->createBuffer(_texcoords_resource, texcoords_stream, DT3GL_ACCESS_CPU_WRITE | DT3GL_ACCESS_GPU_READ);
//        System::getRenderer()->createBuffer(_color_resource, colors_stream, DT3GL_ACCESS_CPU_WRITE | DT3GL_ACCESS_GPU_READ);
//    } else {
//        System::getRenderer()->updateBuffer(_translations_resource, vertex_stream,0);
//        System::getRenderer()->updateBuffer(_texcoords_resource, texcoords_stream,0);
//        System::getRenderer()->updateBuffer(_color_resource, colors_stream,0);
//    }
//
//
//    // Time to draw
//    System::getRenderer()->activateAttributeBuffer(DT3GL_ATTRIB_POSITION, _translations_resource);
//    System::getRenderer()->activateAttributeBuffer(DT3GL_ATTRIB_TEXCOORD0, _texcoords_resource);
//    System::getRenderer()->activateAttributeBuffer(DT3GL_ATTRIB_COLOR, _color_resource);
//    
//    System::getRenderer()->drawArraysRanged(DT3GL_PRIM_TRIS, first*6, num*6);

}
		
//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: GUIAnimKey.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/GUI/GUIAnimKey.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"
#include "DT3Core/Objects/GUIObject.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUIAnimKey::GUIAnimKey (void)
	:	_repeat_flag			(false),
		_color_flag				(false),
		_scale_flag				(false),
		_width_flag				(false),
		_height_flag			(false),
		_translation_flag		(false),
		_orientation_flag		(false),
		_spline_flag			(false),
		_label_flag				(false),
        _busy                   (false),
        _latent_call            (NULL),
		_time					(0.0F),
		_delay					(0.0F),
		_duration				(0.0F),
		_width					(1.0F),
		_cache_width			(1.0F),
		_height					(1.0F),
		_cache_height			(1.0F)
{

}
	
GUIAnimKey::GUIAnimKey (const GUIAnimKey &rhs)
	:	_repeat_flag			(rhs._repeat_flag),
		_color_flag				(rhs._color_flag),
		_scale_flag				(rhs._scale_flag),
		_width_flag				(rhs._width_flag),
		_height_flag			(rhs._height_flag),
		_translation_flag		(rhs._translation_flag),
		_orientation_flag		(rhs._orientation_flag),
		_spline_flag			(rhs._spline_flag),
		_label_flag				(rhs._label_flag),
        _busy                   (rhs._busy),
        _latent_call            (rhs._latent_call),
		_time					(0.0F),
		_delay					(rhs._delay),
		_duration				(rhs._duration),
		_orientation			(rhs._orientation),
		_cache_orientation		(rhs._cache_orientation),
		_color					(rhs._color),
		_cache_color			(rhs._cache_color),
		_scale					(rhs._scale),
		_cache_scale			(rhs._cache_scale),
		_width					(rhs._width),
		_cache_width			(rhs._cache_width),
		_height					(rhs._height),
		_cache_height			(rhs._cache_height),
		_translation            (rhs._translation),
		_cache_translation      (rhs._cache_translation),
		_label					(rhs._label),
		_cache_label			(rhs._cache_label)
{

}

GUIAnimKey& GUIAnimKey::operator = (const GUIAnimKey &rhs)
{
	_repeat_flag = rhs._repeat_flag;
	_color_flag = rhs._color_flag;
	_scale_flag = rhs._scale_flag;
    _width_flag = rhs._width_flag;
    _height_flag = rhs._height_flag;

	_translation_flag = rhs._translation_flag;
	_orientation_flag = rhs._orientation_flag;
	_spline_flag = rhs._spline_flag;
	_label_flag = rhs._label_flag;
    _busy = rhs._busy;
		
    _latent_call = rhs._latent_call;

	_time = 0.0F;
	_delay = rhs._delay;
	_duration = rhs._duration;
	_orientation = rhs._orientation;
	_cache_orientation = rhs._cache_orientation;
	_color = rhs._color;
	_cache_color = rhs._cache_color;
	_scale = rhs._scale;
	_cache_scale = rhs._cache_scale;
    _width = rhs._width;
    _cache_width = rhs._cache_width;
    _height = rhs._height;
    _cache_height = rhs._cache_height;
	_translation = rhs._translation;
	_cache_translation = rhs._cache_translation;
	_label = rhs._label;
	_cache_label = rhs._cache_label;

	return *this;
}
		
GUIAnimKey::~GUIAnimKey (void)
{

}

//==============================================================================
//==============================================================================

void GUIAnimKey::cache_frame_state (GUIObject *frame)
{
	_cache_orientation = frame->orientation();
	_cache_color = frame->color();
	_cache_translation = frame->translation_local();
	_cache_scale = frame->scale();
	_cache_width = frame->width();
	_cache_height = frame->height();
	_cache_label = frame->label();
}

//==============================================================================
//==============================================================================

GUIAnimKey& GUIAnimKey::set_delay (DTfloat delay)
{	
	_delay = delay;	
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_duration (DTfloat duration)
{	
	_duration = duration;			
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_repeat (DTboolean s)
{
	_repeat_flag = s;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_spline (DTboolean s)
{
	_spline_flag = s;
	return *this;
}

GUIAnimKey& GUIAnimKey::set_call (std::shared_ptr<LatentCall> latent_call)
{
    _latent_call = latent_call;
    return *this;
}

GUIAnimKey& GUIAnimKey::set_translation_local (const Vector3 &translation)
{	
	_translation = translation;	
	_translation_flag = true;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_orientation (const Matrix3 &orientation)
{	
	_orientation = orientation;					
	_orientation_flag = true;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_color (const Color4f &color)
{	
	_color = color;					
	_color_flag = true;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_scale (const Vector3 &scale)
{	
	_scale = scale;					
	_scale_flag = true;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_width (const DTfloat width)
{	
	_width = width;					
	_width_flag = true;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_height (const DTfloat height)
{	
	_height = height;					
	_height_flag = true;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_label (const std::string &label)
{
	_label = label;					
	_label_flag = true;
	return *this;	
}

GUIAnimKey& GUIAnimKey::set_bounds (DTfloat left, DTfloat bottom, DTfloat width, DTfloat height)
{
    set_translation_local(Vector3(left + width/2.0F, bottom + height/2.0F,0.0F));
    set_width(width);
    set_height(height);

	return *this;	
}

//==============================================================================
//==============================================================================
		
DTfloat GUIAnimKey::interp(DTfloat v0, DTfloat v1, DTfloat t)
{
	return v0 + (v1 - v0) * t;
}
		
Vector3 GUIAnimKey::interp(Vector3 v0, Vector3 v1, DTfloat t)
{
	return v0 + (v1 - v0) * t;
}
		
DTboolean GUIAnimKey::animate (DTfloat dt, GUIObject *frame)
{
    _busy = true;
    
    if (_latent_call && _time == 0.0F)
        _latent_call->fire();
    
	_time += dt;
	DTfloat t = 0.0F;
    if (_duration <= 0.0F) {
        
        if (_time >= _delay)    t = 1.0F;
        else                    t = 0.0F;
    
    } else {
        t = (_time - _delay) / _duration;
    }
     	
	// Clamp t
	if (t < 0.0F)		t = 0.0F;
	else if (t > 1.0F)	t = 1.0F;
	
	// Do spline
	if (_spline_flag)
		t = (3.0F - 2.0F * t) * (t * t);
	
	// Check animate Rect
	if (_translation_flag) {
		Vector3 v;
		
		v = interp(_cache_translation, _translation, t);        
		frame->set_translation_local(v);
	}
	
	// Check animate Rotation
	if (_orientation_flag) {
		
        Matrix3 m0(_cache_orientation);
        Matrix3 m1(_orientation);
        
        Quaternion q0(m0);
        Quaternion q1(m1);
		
        Quaternion qr = Quaternion::slerp(q0,q1,t);
		frame->set_orientation(Matrix3(qr));
	}
	
	// Check animate Color
	if (_color_flag) {
		Color4f v;
		
		v.r = interp(_cache_color.r, _color.r, t);
		v.g = interp(_cache_color.g, _color.g, t);
		v.b = interp(_cache_color.b, _color.b, t);
		v.a = interp(_cache_color.a, _color.a, t);
		
		frame->set_color(v);
	}
	
	// Check animate scale
	if (_scale_flag) {
		Vector3 scale;
		
		scale = interp(_cache_scale, _scale, t);
		frame->set_scale(scale);
	}
    
	// Check animate scale
	if (_width_flag) {
		DTfloat width;
		
		width = interp(_cache_width, _width, t);
		frame->set_width(width);
	}
    
	// Check animate scale
	if (_height_flag) {
		DTfloat height;
		
		height = interp(_cache_height, _height, t);
		frame->set_height(height);
	}
    
    // Check label
    if (_label_flag) {
        frame->set_label(_label);
    }
    
    _busy = false;
	
	return _time < (_delay + _duration);
}
	
//==============================================================================
//==============================================================================

} // DT3


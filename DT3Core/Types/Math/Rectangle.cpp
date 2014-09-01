//==============================================================================
///	
///	File: Rectangle.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Rectangle.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include <limits>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Rectangle::Rectangle (void)
    :   _plus_x     (-std::numeric_limits<DTfloat>::infinity()),
        _minus_x    (std::numeric_limits<DTfloat>::infinity()),
        _plus_y     (-std::numeric_limits<DTfloat>::infinity()),
        _minus_y    (std::numeric_limits<DTfloat>::infinity())
{

}

Rectangle::Rectangle (const DTfloat mx, const DTfloat px, const DTfloat my, const DTfloat py)
    : 	_minus_x    (mx),
        _plus_x     (px),
        _minus_y    (my),
        _plus_y     (py)
{

}

Rectangle::Rectangle (const Vector2 &v)
{
    _minus_x = _plus_x = v.x;
    _minus_y = _plus_y = v.y;
}

Rectangle::Rectangle (const Rectangle &rhs)
    : 	_minus_x(rhs._minus_x),
        _plus_x(rhs._plus_x),
        _minus_y(rhs._minus_y),
        _plus_y(rhs._plus_y)
{
    
}
            
Rectangle& Rectangle::operator = (const Rectangle &rhs)
{	
    // Make sure we are not assigning the class to itself
	_minus_x = rhs._minus_x;
	_plus_x = rhs._plus_x;
	_minus_y = rhs._minus_y;
	_plus_y = rhs._plus_y;
    return (*this);
}	

Rectangle::~Rectangle (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Rectangle&v)
{
	s	<< v.minus_x() << Stream::fs
		<< v.plus_x() << Stream::fs
		<< v.minus_y() << Stream::fs
		<< v.plus_y();
	return s;
}

Stream& operator >>(Stream &s, Rectangle&v)
{
	DTfloat minus_x, plus_x;
	DTfloat minus_y, plus_y;
	s >> minus_x >> plus_x >> minus_y >> plus_y;
	
	v.set_minus_x(minus_x);
	v.set_plus_x(plus_x);
	v.set_minus_y(minus_y);
	v.set_plus_y(plus_y);
	
	return s;
}

//==============================================================================
//==============================================================================

DTboolean Rectangle::operator == (const Rectangle& rhs) const
{
    return  (_minus_x == rhs._minus_x) &&
            (_plus_x == rhs._plus_x) &&
            (_minus_y == rhs._minus_y) &&
            (_plus_y == rhs._plus_y);
}
		
DTboolean Rectangle::operator != (const Rectangle& rhs) const
{
    return  (_minus_x != rhs._minus_x) ||
            (_plus_x != rhs._plus_x) ||
            (_minus_y != rhs._minus_y) ||
            (_plus_y != rhs._plus_y);
}

//==============================================================================
//==============================================================================
            
void Rectangle::set (	const DTfloat minus_x,
                        const DTfloat plus_x,
                        const DTfloat minus_y,
                        const DTfloat plus_y)
{
    set_X_extents(minus_x, plus_x);
    set_Y_extents(minus_y, plus_y);
}

//==============================================================================
//==============================================================================

void Rectangle::clear (void)
{
   _plus_x = -std::numeric_limits<DTfloat>::infinity();
   _minus_x = std::numeric_limits<DTfloat>::infinity();
   _plus_y = -std::numeric_limits<DTfloat>::infinity();
   _minus_y = std::numeric_limits<DTfloat>::infinity();
}

//==============================================================================
//==============================================================================

void Rectangle::offset (const Vector2 &offset)
{
    _minus_x += offset.x;
    _plus_x += offset.x;
    _minus_y += offset.y;
    _plus_y += offset.y;
}

//==============================================================================
//==============================================================================

Rectangle Rectangle::calc_union (const Rectangle &a, const Rectangle &b)
{
    Rectangle r;
    
    r._minus_x = MoreMath::min(a._minus_x, b._minus_x);
    r._plus_x = MoreMath::max(a._plus_x, b._plus_x);

    r._minus_y = MoreMath::min(a._minus_y, b._minus_y);
    r._plus_y = MoreMath::max(a._plus_y, b._plus_y);

    return r;
}

Rectangle Rectangle::calc_union (const Rectangle &a, const Vector2 &b)
{
    Rectangle r;
    
    r._minus_x = MoreMath::min(a._minus_x, b.x);
    r._plus_x = MoreMath::max(a._plus_x, b.x);

    r._minus_y = MoreMath::min(a._minus_y, b.y);
    r._plus_y = MoreMath::max(a._plus_y, b.y);

    return r;
}

Rectangle Rectangle::calc_intersection (const Rectangle &a, const Rectangle &b)
{
    Rectangle r;

    r._minus_x = MoreMath::max(a._minus_x, b._minus_x);
    r._minus_y = MoreMath::max(a._minus_y, b._minus_y);

    r._plus_x = MoreMath::min(a._plus_x, b._plus_x);
    r._plus_y = MoreMath::min(a._plus_y, b._plus_y);
    
    if (r._minus_x > r._plus_x)     r._minus_x = r._plus_x;
    if (r._minus_y > r._plus_y)     r._minus_y = r._plus_y;

    return r;
}

//==============================================================================
//==============================================================================

DTboolean Rectangle::is_touching (const Rectangle &other)
{
    if (other._minus_x > _plus_x ||
        other._plus_x < _minus_x ||
        other._minus_y > _plus_y ||
        other._plus_y < _minus_y)
        return false;
    else
        return true;
}

//==============================================================================
//==============================================================================

DTboolean Rectangle::is_touching (const Vector2 &pt) const
{
    if (_minus_x <= pt.x && _plus_x >= pt.x &&
        _minus_y <= pt.y && _plus_y >= pt.y)
        return true;
    else
        return false;
	
}

//==============================================================================
//==============================================================================

DTfloat Rectangle::distance_to_point (const Vector2 &pt) const
{
    DTfloat xdist, ydist;
    
    if (pt.x > _plus_x)         xdist = pt.x - _plus_x;
    else if (pt.x < _minus_x)	xdist = _minus_x - pt.x;
    else                        xdist = 0.0F;

    if (pt.y > _plus_y)         ydist = pt.y - _plus_y;
    else if (pt.y < _minus_y)	ydist = _minus_y - pt.y;
    else                        ydist = 0.0F;
    
    DTfloat abs = Vector2(xdist,ydist).abs();
    
    return abs;
}

//==============================================================================
//==============================================================================

const Vector2 Rectangle::closest_point (const Vector2 &pt) const
{
    DTfloat x, y;
    
    if (pt.x > _plus_x)         x = _plus_x;
    else if (pt.x < _minus_x)	x = _minus_x;
    else                        x = pt.x;

    if (pt.y > _plus_y)         y = _plus_y;
    else if (pt.y < _minus_y)	y = _minus_y;
    else                        y = pt.y;
        
    return Vector2(x,y);
}

//==============================================================================
//==============================================================================

} // DT3

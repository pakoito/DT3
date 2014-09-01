//==============================================================================
///	
///	File: Box.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Box.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include <limits>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Box::Box (void)
    :   _plus_z     (-std::numeric_limits<DTfloat>::infinity()),
        _minus_z    (std::numeric_limits<DTfloat>::infinity())
{

}

Box::Box (const Box &rhs)
    : 	Rectangle(rhs),
        _minus_z    (rhs._minus_z),
        _plus_z     (rhs._plus_z)
{

}

Box::Box (  const DTfloat mx, const DTfloat px,
            const DTfloat my, const DTfloat py,
            const DTfloat mz, const DTfloat pz )
{
    set(mx, px, my, py, mz, pz);
}
            
Box& Box::operator = (const Box &rhs)
{	
	Rectangle::operator = (rhs);
        
	_minus_z = rhs._minus_z;
	_plus_z = rhs._plus_z;

    return (*this);
}	

Box::~Box (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Box&v)
{
	s <<	v.minus_x() << Stream::fs << v.plus_x() << Stream::fs <<
			v.minus_y() << Stream::fs << v.plus_y() << Stream::fs <<
			v.minus_z() << Stream::fs << v.plus_z();
	return s;
}

Stream& operator >>(Stream &s, Box&v)
{
	DTfloat mx,my,mz,px,py,pz;
	s >> mx >> px >> my >> py >> mz >> pz;
	
	v.set_minus_x(mx);
	v.set_plus_x(px);
	v.set_minus_y(my);
	v.set_plus_y(py);
	v.set_minus_z(mz);
	v.set_plus_z(pz);

	return s;
}
    
//==============================================================================
//==============================================================================

DTboolean Box::operator == (const Box& rhs) const
{
    return  Rectangle::operator==(rhs) &&
            (_minus_z == rhs._minus_z) &&
            (_plus_z == rhs._plus_z);
}
		
DTboolean Box::operator != (const Box& rhs) const
{
    return  Rectangle::operator!=(rhs) ||
            (_minus_z != rhs._minus_z) ||
            (_plus_z != rhs._plus_z);
}

//==============================================================================
//==============================================================================
            
void Box::set (	const DTfloat minus_x, const DTfloat plus_x, const DTfloat minus_y,
                const DTfloat plus_y, const DTfloat minus_z, const DTfloat plus_z)
{
    Rectangle::set(minus_x, plus_x, minus_y, plus_y);
    set_Z_extents(minus_z, plus_z);
}

//==============================================================================
//==============================================================================

void Box::clear (void)
{
    Rectangle::clear();
    
   _plus_z = -std::numeric_limits<DTfloat>::infinity();
   _minus_z = std::numeric_limits<DTfloat>::infinity();
}

//==============================================================================
//==============================================================================

void Box::offset (const Vector3 &offset)
{
    Rectangle::offset(Vector2(offset));
    
    _minus_z += offset.z;
    _plus_z += offset.z;
}

//==============================================================================
//==============================================================================

Box Box::calc_union (const Box &a, const Box &b)
{
    Box r;
    
    r._minus_x = MoreMath::min(a._minus_x, b._minus_x);
    r._plus_x = MoreMath::max(a._plus_x, b._plus_x);

    r._minus_y = MoreMath::min(a._minus_y, b._minus_y);
    r._plus_y = MoreMath::max(a._plus_y, b._plus_y);

    r._minus_z = MoreMath::min(a._minus_z, b._minus_z);
    r._plus_z = MoreMath::max(a._plus_z, b._plus_z);
    
    return r;
}

Box Box::calc_union (const Box &a, const Vector3 &b)
{
    Box r;
    
    r._minus_x = MoreMath::min(a._minus_x, b.x);
    r._plus_x = MoreMath::max(a._plus_x, b.x);

    r._minus_y = MoreMath::min(a._minus_y, b.y);
    r._plus_y = MoreMath::max(a._plus_y, b.y);

    r._minus_z = MoreMath::min(a._minus_z, b.z);
    r._plus_z = MoreMath::max(a._plus_z, b.z);

    return r;
}

Box Box::calc_intersection (const Box &a, const Box &b)
{
    Box r;

    r._minus_x = MoreMath::max(a._minus_x, b._minus_x);
    r._minus_y = MoreMath::max(a._minus_y, b._minus_y);
    r._minus_z = MoreMath::max(a._minus_z, b._minus_z);

    r._plus_x = MoreMath::min(a._plus_x, b._plus_x);
    r._plus_y = MoreMath::min(a._plus_y, b._plus_y);
    r._plus_z = MoreMath::min(a._plus_z, b._plus_z);
    
    if (r._minus_x > r._plus_x)     r._minus_x = r._plus_x;
    if (r._minus_y > r._plus_y)     r._minus_y = r._plus_y;
    if (r._minus_z > r._plus_z)     r._minus_z = r._plus_z;

    return r;
}

//==============================================================================
//==============================================================================

DTboolean Box::is_touching (const Box &other)
{
    if (!Rectangle::is_touching(other) ||
        other._minus_z > _plus_z ||
        other._plus_z < _minus_z)
        return false;
    else
        return true;
}

//==============================================================================
//==============================================================================

DTboolean Box::is_touching (const Vector3 &pt) const
{
    if (_minus_x <= pt.x && _plus_x >= pt.x &&
        _minus_y <= pt.y && _plus_y >= pt.y &&
        _minus_z <= pt.z && _plus_z >= pt.z)
        return true;
    else
        return false;
	
}

//==============================================================================
//==============================================================================

DTboolean Box::is_touching (const Vector3 &from, const Vector3 &to, const DTfloat r)
{
    DTfloat   plus_x = _plus_x + r;
    DTfloat   plus_y = _plus_y + r;
    DTfloat   plus_z = _plus_z + r;
    DTfloat   minus_x = _minus_x - r;
    DTfloat   minus_y = _minus_y - r;
    DTfloat   minus_z = _minus_z - r;

    // page 577 of Real-Time rendering 2nd edition
    Vector3 c,w;
    
    // Calculate Center vector
    c = (from + to) * 0.5F;
    
    // Calculate Half Vector3
    w = c - from;
    
    
    DTfloat hx = (plus_x - minus_x) / 2.0F;
    DTfloat cx = c.x - minus_x - hx;
    DTfloat vx = std::abs(w.x);

    if (std::abs(cx) > vx + hx)     return false;
    
    DTfloat hy = (plus_y - minus_y) / 2.0F;
    DTfloat cy = c.y - minus_y - hy;    
    DTfloat vy = std::abs(w.y);
    
    if (std::abs(cy) > vy + hy)     return false;

    DTfloat hz = (plus_z - minus_z) / 2.0F;
    DTfloat cz = c.z - minus_z - hz;
    DTfloat vz = std::abs(w.z);

    if (std::abs(cz) > vz + hz)     return false;
    
    if (std::abs(cy * w.z - cz * w.y) > (hy * vz + hz * vy)) return false;
    if (std::abs(cx * w.z - cz * w.x) > (hx * vz + hz * vx)) return false;
    if (std::abs(cx * w.y - cy * w.x) > (hx * vy + hy * vx)) return false;
    
    return true;
}

//==============================================================================
//==============================================================================


DTboolean Box::is_touching (const Vector3 &from, const Vector3 &to)
{
    // page 577 of Real-Time rendering 2nd edition
    Vector3 c,w;
    
    // Calculate Center vector
    c = (from + to) * 0.5F;
    
    // Calculate Half Vector3
    w = c - from;
    
    
    DTfloat hx = (_plus_x - _minus_x) * 0.5F;
    DTfloat cx = c.x - _minus_x - hx;
    DTfloat vx = std::abs(w.x);

    if (std::abs(cx) > vx + hx)     return false;
    
    DTfloat hy = (_plus_y - _minus_y) * 0.5F;
    DTfloat cy = c.y - _minus_y - hy;
    DTfloat vy = std::abs(w.y);
    
    if (std::abs(cy) > vy + hy)     return false;

    DTfloat hz = (_plus_z - _minus_z) * 0.5F;
    DTfloat cz = c.z - _minus_z - hz;
    DTfloat vz = std::abs(w.z);

    if (std::abs(cz) > vz + hz)     return false;
    
    if (std::abs(cy * w.z - cz * w.y) > (hy * vz + hz * vy)) return false;
    if (std::abs(cx * w.z - cz * w.x) > (hx * vz + hz * vx)) return false;
    if (std::abs(cx * w.y - cy * w.x) > (hx * vy + hy * vx)) return false;
    
    return true;
}

//==============================================================================
/// Calculates the distance to a box
//==============================================================================

DTfloat Box::distance_to_point (const Vector3 &point) const
{
    DTfloat xdist, ydist, zdist;
    
    if (point.x > _plus_x)          xdist = point.x - _plus_x;
    else if (point.x < _minus_x)	xdist = _minus_x - point.x;
    else                            xdist = 0.0F;

    if (point.y > _plus_y)          ydist = point.y - _plus_y;
    else if (point.y < _minus_y)	ydist = _minus_y - point.y;
    else                            ydist = 0.0F;

    if (point.z > _plus_z)			zdist = point.z - _plus_z;
    else if (point.z < _minus_z)    zdist = _minus_z - point.z;
    else                            zdist = 0.0F;
    
    DTfloat abs = Vector3(xdist,ydist,zdist).abs();
    
    return abs;
}
    
//==============================================================================
//==============================================================================

const Vector3 Box::closest_point (const Vector3 &pt) const
{
    DTfloat x, y, z;
    
    if (pt.x > _plus_x)         x = _plus_x;
    else if (pt.x < _minus_x)	x = _minus_x;
    else                        x = pt.x;

    if (pt.y > _plus_y)         y = _plus_y;
    else if (pt.y < _minus_y)	y = _minus_y;
    else                        y = pt.y;

    if (pt.z > _plus_z)         z = _plus_z;
    else if (pt.z < _minus_z)	z = _minus_z;
    else                        z = pt.z;
    
    return Vector3(x,y,z);
}

//==============================================================================
//==============================================================================

} // DT3

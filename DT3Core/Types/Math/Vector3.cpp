//==============================================================================
///	
///	File: Vector3.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

Vector3::Vector3 (const Vector2 &rhs, DTfloat z_)
{  
	x = rhs.x; 
	y = rhs.y;		
	z = z_;		
}

Vector3::Vector3 (const Vector4 &rhs)		
{   
	x = rhs.x; 
	y = rhs.y;		
	z = rhs.z;		
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Vector3&v)
{
	s << v.x << Stream::fs << v.y << Stream::fs << v.z;
	return s;
}

Stream& operator >>(Stream &s, Vector3&v)
{
	DTfloat x=v.x,y=v.y,z=v.z;
	s >> x >> y >> z;
	
	v.x = (x);
	v.y = (y);
	v.z = (z);
	
	return s;
}

//==============================================================================
//==============================================================================

Vector3 Vector3::cross (const Vector3& a, const Vector3& b)
{
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
    
}
		
//==============================================================================
//==============================================================================

void Vector3::displace (const Vector3 &a, const Vector3 &n, DTfloat d, Vector3 &r)
{
    DTfloat x = a.x + n.x * d;
    DTfloat y = a.y + n.y * d;
    DTfloat z = a.z + n.z * d;

    r.x = x;
    r.y = y;
    r.z = z;
}

//==============================================================================
//==============================================================================

void Vector3::reflect (const Vector3 &dir, const Vector3 &n, const DTfloat restitution, Vector3 &r)
{
    Vector3 delta;
    DTfloat dot;
    
    dot = Vector3::dot(dir, n);
    r = dir + n * (-(dot + dot * restitution));
}

//==============================================================================
//==============================================================================
		
} // DT3


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

#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

Vector4::Vector4 (const Vector2 &rhs, DTfloat z_, DTfloat w_)
{  
	x = rhs.x; 
	y = rhs.y;		
	z = z_;		
}

Vector4::Vector4 (const Vector3 &rhs, DTfloat w_)
{  
	x = rhs.x; 
	y = rhs.y;		
	z = rhs.z;	
	w = w_;		
}

Vector4::Vector4 (const Vector4 &rhs)		
{   
	x = rhs.x; 
	y = rhs.y;		
	z = rhs.z;		
	w = rhs.w;		
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Vector4&v)
{
	s << v.x << Stream::fs << v.y << Stream::fs << v.z << Stream::fs << v.w;
	return s;
}

Stream& operator >>(Stream &s, Vector4&v)
{
	DTfloat x,y,z,w;
	s >> x >> y >> z >> w;
	
	v.x = (x);
	v.y = (y);
	v.z = (z);
	v.w = (w);
	
	return s;
}

//==============================================================================
//==============================================================================
		
} // DT3


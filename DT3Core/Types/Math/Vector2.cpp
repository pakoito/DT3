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

#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

Vector2::Vector2 (const Vector3 &rhs)		
{   
	x = rhs.x; 
	y = rhs.y;		
}

Vector2::Vector2 (const Vector4 &rhs)		
{   
	x = rhs.x; 
	y = rhs.y;		
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Vector2 &v)
{
	s << v.x << Stream::fs << v.y;
	return s;
}

Stream& operator >>(Stream &s, Vector2 &v)
{
	s >> v.x >> v.y;
	return s;
}

//==============================================================================
//==============================================================================
		
} // DT3


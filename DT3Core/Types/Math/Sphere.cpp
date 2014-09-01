//==============================================================================
///	
///	File: Sphere.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Sphere.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Sphere::Sphere (void)
{

}

Sphere::Sphere (DTfloat radius)
    : 	_radius(radius)
{

}

Sphere::Sphere (const Sphere &rhs)
    : 	_radius(rhs._radius)
{

}
            
Sphere& Sphere::operator = (const Sphere &rhs)
{	
	_radius = rhs._radius;
    return (*this);
}

Sphere::~Sphere (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Sphere &v)
{
	s << v.radius();
	return s;
}

Stream& operator >>(Stream &s, Sphere &v)
{
	DTfloat radius;
	s >> radius;
	
	v.set_radius(radius);

	return s;
}

//==============================================================================
//==============================================================================

} // DT3

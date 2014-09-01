//==============================================================================
///	
///	File: Triangle.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Triangle.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Triangle::Triangle (const DTushort v0_, const DTushort v1_, const DTushort v2_)
{
	v[0] = v0_;
	v[1] = v1_;
	v[2] = v2_;
}


Triangle::Triangle (const Triangle &rhs)
{
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
}
            
Triangle& Triangle::operator = (const Triangle &rhs)
{	
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];

    return (*this);
}	

Triangle::~Triangle (void)
{

}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: BinaryStream.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/BinaryStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

BinaryStream::BinaryStream (void)
{

}
		
BinaryStream::BinaryStream (const BinaryStream &rhs)
{

}

BinaryStream & BinaryStream::operator = (const BinaryStream &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {	

    }
    return (*this);
}
	
BinaryStream::~BinaryStream (void)
{

}

//==============================================================================
//==============================================================================
		

} // DT3

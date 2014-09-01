//==============================================================================
///	
///	File: TextStream.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/TextStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextStream::TextStream (void)
{

}
		
TextStream::TextStream (const TextStream &rhs)
{

}

TextStream & TextStream::operator = (const TextStream &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {	

    }
    return (*this);
}
	
TextStream::~TextStream (void)
{

}

//==============================================================================
//==============================================================================
		

} // DT3

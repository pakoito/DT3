//==============================================================================
///	
///	File: Progress.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Progress.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Progress::Progress (void)
{

}
		
			
Progress::Progress (const Progress &rhs)
    :   BaseClass				(rhs)
{   

}

Progress & Progress::operator = (const Progress &rhs)
{
	BaseClass::operator = (rhs);
    return (*this);
}

Progress::~Progress (void)
{

}

//==============================================================================
//==============================================================================
		
} // DT3

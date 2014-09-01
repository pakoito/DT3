//==============================================================================
///	
///	File: DisplayMode.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/DisplayMode.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DisplayMode)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DisplayMode::DisplayMode (void)
	:	_width          (0),
		_height         (0),
        _refresh_rate   (0)
{

}

DisplayMode::DisplayMode (const DisplayMode &rhs)
	:	_width          (rhs._width),
		_height         (rhs._height),
        _refresh_rate   (rhs._refresh_rate)
{

}
		
DisplayMode& DisplayMode::operator = (const DisplayMode &rhs)
{	
	_width = rhs._width;
	_height = rhs._height;
    _refresh_rate = rhs._refresh_rate;
    
    return (*this);
}	

DisplayMode::~DisplayMode (void)
{

}

//==============================================================================
//==============================================================================

} // DT3

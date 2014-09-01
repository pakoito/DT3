//==============================================================================
///	
///	File: TransitionBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "TransitionBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TransitionBase::TransitionBase (void)
{

}
			
TransitionBase::~TransitionBase (void)
{

}

//==============================================================================
//==============================================================================

DTfloat TransitionBase::remap_zero_one (DTfloat value, DTfloat begin, DTfloat end)
{
	return (value - begin) / (end - begin);
}

//==============================================================================
//==============================================================================

} // DT3

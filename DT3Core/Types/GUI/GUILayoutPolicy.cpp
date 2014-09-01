//==============================================================================
///	
///	File: GUILayoutPolicy.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/GUI/GUILayoutPolicy.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUILayoutPolicy::GUILayoutPolicy (void)
    :   _policy (POLICY_WEIGHTED_SIZE),
        _value  (1.0F)
{  

}
	
GUILayoutPolicy::GUILayoutPolicy (const GUILayoutPolicy &rhs)
    :   _policy (rhs._policy),
        _value  (rhs._value)
{
    
}

GUILayoutPolicy& GUILayoutPolicy::operator = (const GUILayoutPolicy &rhs)
{
    _policy = rhs._policy;
    _value = rhs._value;

	return *this;
}
		
GUILayoutPolicy::~GUILayoutPolicy (void)
{

}
	
//==============================================================================
//==============================================================================

} // DT3


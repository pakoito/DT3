//==============================================================================
///	
///	File: GUILayout.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/GUI/GUILayout.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUILayout::GUILayout (void)
{  

}
	
GUILayout::GUILayout (const GUILayout &rhs)
    :   BaseClass   (rhs)
{

}

GUILayout& GUILayout::operator = (const GUILayout &rhs)
{
    BaseClass::operator=(rhs);
	return *this;
}
		
GUILayout::~GUILayout (void)
{

}
	
//==============================================================================
//==============================================================================

} // DT3


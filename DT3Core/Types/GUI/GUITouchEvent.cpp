//==============================================================================
///	
///	File: GUITouchEvent.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/GUI/GUITouchEvent.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUITouchEvent::GUITouchEvent (void)
    :   _focused            (NULL),
        _initial_position   (0.0F,0.0F),
        _previous_position  (0.0F,0.0F),
        _position           (0.0F,0.0F),
        _velocity           (0.0F,0.0F),
        _state              (TouchEvent::STATE_NONE)
{

}
	
GUITouchEvent::GUITouchEvent (const GUITouchEvent &rhs)
    :   _focused            (rhs._focused),
        _initial_position   (rhs._initial_position),
        _previous_position  (rhs._previous_position),
        _position           (rhs._position),
        _velocity           (rhs._velocity),
        _state              (rhs._state)
{

}

GUITouchEvent& GUITouchEvent::operator = (const GUITouchEvent &rhs)
{
    _focused = rhs._focused;
    _initial_position = rhs._initial_position;
    _previous_position = rhs._previous_position;
    _position = rhs._position;
    _velocity = rhs._velocity;
    _state = rhs._state;
    
	return *this;
}
		
GUITouchEvent::~GUITouchEvent (void)
{

}
	
//==============================================================================
//==============================================================================

} // DT3


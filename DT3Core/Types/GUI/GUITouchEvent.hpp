#ifndef DT3_GUITOUCHEVENT
#define DT3_GUITOUCHEVENT
//==============================================================================
///	
///	File: GUITouchEvent.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Utility/TouchEvent.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward Declarations
//==============================================================================

class GUIObject;

//==============================================================================
/// Class
//==============================================================================

class GUITouchEvent {
    public:
        DEFINE_TYPE_BASE(GUITouchEvent)
        		
                                    GUITouchEvent       (void);	
                                    GUITouchEvent       (const GUITouchEvent &rhs);
        GUITouchEvent &             operator =          (const GUITouchEvent &rhs);
		virtual                     ~GUITouchEvent      (void);
  
    public:
        DEFINE_ACCESSORS(initial_position, set_initial_position, Vector2, _initial_position)
        DEFINE_ACCESSORS(position, set_position, Vector2, _position)
        DEFINE_ACCESSORS(previous_position, set_previous_position, Vector2, _previous_position)
        DEFINE_ACCESSORS(velocity, set_velocity, Vector2, _velocity)
        DEFINE_ACCESSORS(focused, set_focused, GUIObject*, _focused)

        DEFINE_ACCESSORS(state, set_state, TouchEvent::State, _state)

    private:
        GUIObject                   *_focused;
        Vector2                     _initial_position;
        Vector2                     _previous_position;
        Vector2                     _position;
        Vector2                     _velocity;
        TouchEvent::State           _state;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

//==============================================================================
///	
///	File: TouchEvent.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/TouchEvent.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TouchEvent::TouchEvent (void)
{
    clear();
}

TouchEvent::TouchEvent (const TouchEvent &rhs)
{
    clear();
    
    for (DTuint i = 0; i < ARRAY_SIZE(touches); ++i) {
        touches[i] = rhs.touches[i];
    }
}
		
TouchEvent& TouchEvent::operator = (const TouchEvent &rhs)
{	
    clear();
    
    for (DTuint i = 0; i < ARRAY_SIZE(touches); ++i) {
        touches[i] = rhs.touches[i];
    }

    return (*this);
}	

TouchEvent::~TouchEvent (void)
{

}

//==============================================================================
//==============================================================================

void TouchEvent::clear (void)
{
    
    for (DTuint i = 0; i < MAX_NUM_TOUCHES; ++i) {
        touches[i].state = STATE_NONE;
        touches[i].pos = Vector2(0.0F,0.0F);
        touches[i].previous_pos = Vector2(0.0F,0.0F);
        touches[i].first_pos = Vector2(0.0F,0.0F);

        touches[i].delta = Vector2(0.0F,0.0F);
        touches[i].velocity = Vector2(0.0F,0.0F);
    }
    
}

//==============================================================================
//==============================================================================

Vector2 TouchEvent::normalize_input (Vector2 pos)
{
    DTfloat width = System::renderer()->screen_width();
    DTfloat height = System::renderer()->screen_height();

    return Vector2( pos.x / width, 1.0F - pos.y / height);
}

Vector2 TouchEvent::normalize_input_vel (Vector2 pos)
{
    DTfloat width = System::renderer()->screen_width();
    DTfloat height = System::renderer()->screen_height();

    return Vector2( pos.x / width, -pos.y / height);
}

Vector2 TouchEvent::normalize_input_ndc (Vector2 pos)
{
    pos = normalize_input(pos);
    
    pos.x = (pos.x - 0.5F) * 2.0F;
    pos.y = (pos.y - 0.5F) * 2.0F;
    
    return pos;
}

//==============================================================================
//==============================================================================

void TouchEvent::cleanup_releases (void)
{
    for (DTuint i = 0; i < MAX_NUM_TOUCHES; ++i) {
        if (touches[i].state == STATE_RELEASED)
            touches[i].state = STATE_NONE;
    }
}

//==============================================================================
//==============================================================================

} // DT3

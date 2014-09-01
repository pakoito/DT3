//==============================================================================
///	
///	File: EdLevelToolEvent.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelToolEvent.hpp"

// Qt include

// Engine includes
#include "DT3Core/Objects/CameraObject.hpp"


//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

const DTint EdLevelToolEvent::MODIFIER_SHIFT = 1 << 0;
const DTint EdLevelToolEvent::MODIFIER_CONTROL = 1 << 1;
const DTint EdLevelToolEvent::MODIFIER_OPTION_ALT = 1 << 2;
const DTint EdLevelToolEvent::MODIFIER_META = 1 << 3;

//==============================================================================
//==============================================================================

EdLevelToolEvent::EdLevelToolEvent (void)
    :   _event_type     (NONE),
        _ray_src        (0.0F,0.0F,0.0F),
        _ray_dest       (0.0F,0.0F,0.0F),
        _control_id     (0),
        _mouse_x        (0),
        _mouse_y        (0),
        _key_code       (0),
        _modifiers      (0),
        _window_width   (0),
        _window_height  (0),
        _camera         (NULL)
{

}

EdLevelToolEvent::EdLevelToolEvent		(const EdLevelToolEvent &rhs)
    :   _event_type     (rhs._event_type),
        _ray_src        (rhs._ray_src),
        _ray_dest       (rhs._ray_dest),
        _control_id     (rhs._control_id),
        _mouse_x        (rhs._mouse_x),
        _mouse_y        (rhs._mouse_y),
        _key_code       (rhs._key_code),
        _modifiers      (rhs._modifiers),
        _window_width   (rhs._window_width),
        _window_height  (rhs._window_height),
        _camera         (NULL)
{

}

EdLevelToolEvent& EdLevelToolEvent::operator = (const EdLevelToolEvent &rhs)
{
    _event_type = rhs._event_type;
    _ray_src = rhs._ray_src;
    _ray_dest = rhs._ray_dest;
    _control_id = rhs._control_id;
    _mouse_x = rhs._mouse_x;
    _mouse_y = rhs._mouse_y;
    _key_code = rhs._key_code;
    _modifiers = rhs._modifiers;
    _window_width = rhs._window_width;
    _window_height = rhs._window_height;
    _camera = rhs._camera;
    
    return (*this);
}

EdLevelToolEvent::~EdLevelToolEvent (void)
{

}

//==============================================================================
//==============================================================================


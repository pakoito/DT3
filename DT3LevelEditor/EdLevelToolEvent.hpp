#ifndef EDLEVELTOOLEVENT
#define EDLEVELTOOLEVENT
//==============================================================================
///	
///	File: EdLevelToolEvent.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include

// Qt include

// Engine includes
#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class CameraObject;
}

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelToolEvent
{
	public:
                                                EdLevelToolEvent        (void);
                                                EdLevelToolEvent		(const EdLevelToolEvent &rhs);
        EdLevelToolEvent &                      operator =              (const EdLevelToolEvent &rhs);
        virtual                                 ~EdLevelToolEvent       (void);
        
    public:
    
        Vector3                                 getRaySource            (void) const    {   return _ray_src;        }
        Vector3                                 getRayDestination       (void) const    {   return _ray_dest;       }
    
        DTuint                                  mouseX                  (void) const    {   return _mouse_x;        }
        DTuint                                  mouseY                  (void) const    {   return _mouse_y;        }

        const std::shared_ptr<CameraObject>&    getCamera               (void) const    {   return _camera;         }
    
        DTfloat                                 getGrid                 (void) const    {   return _grid;           }
    
        DTuint                                  keyCode                 (void) const    {   return _key_code;       }
        DTuint                                  modifiers               (void) const    {   return _modifiers;      }

    public:
        static const DTint MODIFIER_SHIFT;
        static const DTint MODIFIER_CONTROL;
        static const DTint MODIFIER_OPTION_ALT;
        static const DTint MODIFIER_META;
       
        enum EventType {
            NONE,
            MOUSE_DOWN,
            MOUSE_DRAG,
            MOUSE_UP,
            KEY_DOWN
        };  
        EventType                       _event_type;
        
        std::shared_ptr<CameraObject>   _camera;
        Vector3                         _ray_src;
        Vector3                         _ray_dest;
        
        DTuint                          _control_id;
        
        DTuint                          _mouse_x;
        DTuint                          _mouse_y;
        
        DTuint                          _key_code;
        DTuint                          _modifiers;
        
        DTuint                          _window_width;
        DTuint                          _window_height;
    
        DTfloat                         _grid;
};

//==============================================================================
//==============================================================================

#endif

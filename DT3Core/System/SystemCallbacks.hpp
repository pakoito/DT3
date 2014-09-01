#ifndef DT3_SYSTEMCALLBACKS
#define DT3_SYSTEMCALLBACKS
//==============================================================================
///	
///	File: SystemCallbacks.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/CallbackList.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class ComponentBase;
class World;
class WorldNode;
class TouchEvent;
class Group;
class Event;
class PlugBase;
class Vector3;

//==============================================================================
/// Class
//==============================================================================

class SystemCallbacks {
	
    private:
										SystemCallbacks             (void);
										SystemCallbacks             (const SystemCallbacks &rhs);
        SystemCallbacks &               operator =					(const SystemCallbacks &rhs);	
        virtual							~SystemCallbacks            (void);

	public:	
	
        static CallbackList<>&                                      screen_inited_cb                (void)  {  return _screenInitedCB;      }
        static CallbackList<DTuint,DTuint>&                         screen_opened_cb                (void)  {  return _screenOpenedCB;      }
        static CallbackList<>&                                      screen_closed_cb                (void)  {  return _screenClosedCB;      }
        static CallbackList<>&                                      screen_swap_cb                  (void)  {  return _screenSwapCB;        }
        static CallbackList<DTuint,DTuint>&                         screen_changed_cb               (void)  {  return _screenChangedCB;     }
        
        static CallbackList<World*, ComponentBase*>&                add_component_cb                (void)  {  return _addComponentCB;      }
        static CallbackList<World*, ComponentBase*>&                remove_component_cb             (void)  {  return _removeComponentCB;   }
    
        static CallbackList<World*, WorldNode*, WorldNode*, WorldNode*>&    reparent_node_cb        (void)  {  return _reparentNodeCB;      }
        static CallbackList<World*, WorldNode*>&                            add_node_cb             (void)  {  return _addNodeCB;           }
        static CallbackList<World*, WorldNode*>&                            remove_node_cb          (void)  {  return _removeNodeCB;        }

        static CallbackList<World*, Group*>&                        add_group_cb                    (void)  {  return _addGroupCB;          }
        static CallbackList<World*, Group*>&                        remove_group_cb                 (void)  {  return _removeGroupCB;       }
    
        static CallbackList<PlugBase*,PlugBase*>&                   connect_plug_cb                 (void)  {  return _connectPlugCB;       }
        static CallbackList<PlugBase*,PlugBase*>&                   disconnect_plug_cb              (void)  {  return _disconnectPlugCB;    }
        static CallbackList<Event*,Event*>&                         connect_event_cb                (void)  {  return _connectEventCB;      }
        static CallbackList<Event*,Event*>&                         disconnect_event_cb             (void)  {  return _disconnectEventCB;   }
        
        static CallbackList<const TouchEvent*>&                     touch_event_cb                  (void)  {  return _touchEventCB;            }
        static CallbackList<DTuint,DTushort>&                       key_down_event_cb               (void)  {  return _keyDownEventCB;          }
        static CallbackList<DTuint,DTushort>&                       key_up_event_cb                 (void)  {  return _keyUpEventCB;            }
        static CallbackList<>&                                      back_button_event_cb            (void)  {  return _backButtonEventCB;       }
        static CallbackList<>&                                      menu_button_event_cb            (void)  {  return _menuButtonEventCB;       }

        static CallbackList<>&                                      begin_capture_mouse_cb          (void)  {  return _beginCaptureMouseCB;     }
        static CallbackList<>&                                      end_capture_mouse_cb            (void)  {  return _endCaptureMouseCB;       }
    
        static CallbackList<const Vector3 &>&                       acceleration_cb                 (void)  {  return _accelerationEventCB;     }
        static CallbackList<const Vector3 &>&                       gyro_cb                         (void)  {  return _gyroEventCB;             }
        static CallbackList<const Vector3 &>&                       magnetometer_cb                 (void)  {  return _magnetometerEventCB;     }
    
        static CallbackList<>&                                      reload_resources_cb             (void)  {  return _reloadResourcesCB;       }
        static CallbackList<>&                                      emergency_free_memory_cb        (void)  {  return _emergencyFreeMemoryCB;   }

        static CallbackList<const std::string &>&                   error_cb                        (void)  {  return _errorCB;     }
        static CallbackList<const std::string &>&                   message_cb                      (void)  {  return _messageCB;   }
        static CallbackList<const std::string &>&                   debug_cb                        (void)  {  return _debugCB;     }

    private:
    
        // Screen callbacks        
        static CallbackList<>                                       _screenInitedCB;
        static CallbackList<DTuint,DTuint>                          _screenOpenedCB;
        static CallbackList<>                                       _screenClosedCB;
        static CallbackList<>                                       _screenSwapCB;
        static CallbackList<DTuint,DTuint>                          _screenChangedCB;

        // Component Callbacks        
        static CallbackList<World*, ComponentBase*>                 _addComponentCB;
        static CallbackList<World*, ComponentBase*>                 _removeComponentCB;

        // Node Callbacks        
        static CallbackList<World*, WorldNode*, WorldNode*, WorldNode*> _reparentNodeCB;
        static CallbackList<World*, WorldNode*>                         _addNodeCB;
        static CallbackList<World*, WorldNode*>                         _removeNodeCB;

        // Group Callbacks
        static CallbackList<World*, Group*>                         _addGroupCB;
        static CallbackList<World*, Group*>                         _removeGroupCB;

        // Connection Callbacks
        static CallbackList<PlugBase*,PlugBase*>                    _connectPlugCB;
        static CallbackList<PlugBase*,PlugBase*>                    _disconnectPlugCB;
        static CallbackList<Event*,Event*>                          _connectEventCB;
        static CallbackList<Event*,Event*>                          _disconnectEventCB;

        static CallbackList<const TouchEvent*>                      _touchEventCB;
        static CallbackList<DTuint,DTushort>                        _keyDownEventCB;
        static CallbackList<DTuint,DTushort>                        _keyUpEventCB;
        static CallbackList<>                                       _backButtonEventCB;
        static CallbackList<>                                       _menuButtonEventCB;

        static CallbackList<>                                       _beginCaptureMouseCB;
        static CallbackList<>                                       _endCaptureMouseCB;

        static CallbackList<const Vector3 &>                        _accelerationEventCB;
        static CallbackList<const Vector3 &>                        _gyroEventCB;
        static CallbackList<const Vector3 &>                        _magnetometerEventCB;
        
        static CallbackList<>                                       _reloadResourcesCB;
        static CallbackList<>                                       _emergencyFreeMemoryCB;

        static CallbackList<const std::string &>                    _errorCB;
        static CallbackList<const std::string &>                    _messageCB;
        static CallbackList<const std::string &>                    _debugCB;

};

//==============================================================================
//==============================================================================


} // DT3

#endif

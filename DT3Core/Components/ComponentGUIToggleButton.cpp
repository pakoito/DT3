//==============================================================================
///	
///	File: ComponentGUIToggleButton.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentGUIToggleButton.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/GUI/GUITouchEvent.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Objects/GUIObject.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Resources/ResourceTypes/SoundResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentGUIToggleButton,"GUIBehaviour","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentGUIToggleButton)

IMPLEMENT_EVENT_INFO_INDEX(_clicked_on)
IMPLEMENT_EVENT_INFO_INDEX(_clicked_off)
IMPLEMENT_EVENT_INFO_INDEX(_toggled)
IMPLEMENT_EVENT_INFO_INDEX(_force_click_on)
IMPLEMENT_EVENT_INFO_INDEX(_force_click_off)

IMPLEMENT_PLUG_INFO_INDEX(_is_on)
IMPLEMENT_PLUG_INFO_INDEX(_sound_on)
IMPLEMENT_PLUG_INFO_INDEX(_sound_off)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentGUIToggleButton)

    PLUG_INIT(_sound_on, "Sound_On")
		.set_input(true);

    PLUG_INIT(_sound_off, "Sound_Off")
		.set_input(true);

    PLUG_INIT(_is_on, "Is_On")
		.set_output(true);

	EVENT_INIT(_clicked_on,"Clicked_On")
        .set_output(true);

	EVENT_INIT(_clicked_off,"Clicked_Off")
        .set_output(true);

	EVENT_INIT(_toggled,"Toggled")
        .set_output(true);

	EVENT_INIT(_force_click_on,"Force_Click_On")
        .set_input(true)
        .set_event(&ComponentGUIToggleButton::force_click_on);

	EVENT_INIT(_force_click_off,"Force_Click_Off")
        .set_input(true)
        .set_event(&ComponentGUIToggleButton::force_click_off);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentGUIToggleButton::ComponentGUIToggleButton (void)
    :   _sound_on           (PLUG_INFO_INDEX(_sound_on)),
        _sound_off          (PLUG_INFO_INDEX(_sound_off)),
        _is_on              (PLUG_INFO_INDEX(_is_on), false),
        _clicked_on         (EVENT_INFO_INDEX(_clicked_on)),
        _clicked_off        (EVENT_INFO_INDEX(_clicked_off)),
        _toggled            (EVENT_INFO_INDEX(_toggled)),
        _force_click_on     (EVENT_INFO_INDEX(_force_click_on)),
        _force_click_off    (EVENT_INFO_INDEX(_force_click_off))
{

}
		
ComponentGUIToggleButton::ComponentGUIToggleButton (const ComponentGUIToggleButton &rhs)
    :   ComponentBase       (rhs),
        _sound_on           (rhs._sound_on),
        _sound_off          (rhs._sound_off),
        _is_on              (rhs._is_on),
        _clicked_on         (EVENT_INFO_INDEX(_clicked_on)),
        _clicked_off        (EVENT_INFO_INDEX(_clicked_off)),
        _toggled            (EVENT_INFO_INDEX(_toggled)),
        _force_click_on     (EVENT_INFO_INDEX(_force_click_on)),
        _force_click_off    (EVENT_INFO_INDEX(_force_click_off))
{

}

ComponentGUIToggleButton & ComponentGUIToggleButton::operator = (const ComponentGUIToggleButton &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
        _sound_on = rhs._sound_on;
        _sound_off = rhs._sound_off;
        _is_on = rhs._is_on;
    }
    return (*this);
}
			
ComponentGUIToggleButton::~ComponentGUIToggleButton (void)
{

}

//==============================================================================
//==============================================================================

void ComponentGUIToggleButton::initialize (void)
{
	ComponentBase::initialize();
}

//==============================================================================
//==============================================================================

void ComponentGUIToggleButton::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

	archive->push_domain (class_id ());
        		
    *archive << ARCHIVE_PLUG(_sound_on, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_sound_off, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentGUIToggleButton::add_to_owner (ObjectBase *owner)
{
    ComponentBase::add_to_owner(owner);
    
    GUIObject *gui = checked_cast<GUIObject*>(owner);
    if (gui) {
        gui->touches_began_callbacks().add(make_callback(this, &type::touches_began));
        gui->touches_moved_callbacks().add(make_callback(this, &type::touches_moved));
        gui->touches_ended_callbacks().add(make_callback(this, &type::touches_ended));
        gui->touches_cancelled_callbacks().add(make_callback(this, &type::touches_cancelled));
    }
}

void ComponentGUIToggleButton::remove_from_owner (void)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (gui) {
        gui->touches_began_callbacks().remove(make_callback(this, &type::touches_began));
        gui->touches_moved_callbacks().remove(make_callback(this, &type::touches_moved));
        gui->touches_ended_callbacks().remove(make_callback(this, &type::touches_ended));
        gui->touches_cancelled_callbacks().remove(make_callback(this, &type::touches_cancelled));
    }

    ComponentBase::remove_from_owner();
}


//==============================================================================
//==============================================================================

void ComponentGUIToggleButton::force_click_on (PlugNode *sender)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;

    if (gui->state() == GUIObject::STATE_NORMAL) {
        gui->set_state(GUIObject::STATE_FOCUSED);
        
        _is_on = true;
        _clicked_on.send(gui);      // GUIObject is the originator of the event
        _toggled.send(gui);
    }
}

void ComponentGUIToggleButton::force_click_off (PlugNode *sender)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;

    if (gui->state() == GUIObject::STATE_FOCUSED) {
        gui->set_state(GUIObject::STATE_NORMAL);
        
        _is_on = false;
        _clicked_off.send(gui);     // GUIObject is the originator of the event
        _toggled.send(gui);
    }
}

//==============================================================================
//==============================================================================

void ComponentGUIToggleButton::touches_began (GUITouchEvent *event)
{
//    GUIObject *gui = checked_cast<GUIObject*>(owner());
//    if (!gui)
//        return;
}

void ComponentGUIToggleButton::touches_moved (GUITouchEvent *event)
{
//    GUIObject *gui = checked_cast<GUIObject*>(owner());
//    if (!gui)
//        return;
}

void ComponentGUIToggleButton::touches_ended (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
        
    //
    // Check if still within bounds
    //
    
    if (gui->state() != GUIObject::STATE_DISABLED) {
        // Transform touches into widget coordinates
        Vector2 pos = gui->position_to_object_coord(event->position());

        if (gui->rectangle().is_touching(pos)) {

            if (gui->state() == GUIObject::STATE_FOCUSED) {
            
                if (_sound_off.as_ref())
                    System::audio_renderer()->play_quick(_sound_off, owner()->world());
                gui->set_state(GUIObject::STATE_NORMAL);
                _is_on = false;

                // Call callback too
                if (_button_pressed_latent_call) {
                    _button_pressed_latent_call->fire();
                }

                _clicked_off.send(gui);     // GUIObject is the originator of the event
                _toggled.send(gui);
                
            } else {
            
                if (_sound_on.as_ref())
                    System::audio_renderer()->play_quick(_sound_on, owner()->world());
                gui->set_state(GUIObject::STATE_FOCUSED);
                _is_on = true;

                // Call callback too
                if (_button_pressed_latent_call) {
                    _button_pressed_latent_call->fire();
                }

                _clicked_on.send(gui);      // GUIObject is the originator of the event
                _toggled.send(gui);
               
            }

        }
    }
        
}

void ComponentGUIToggleButton::touches_cancelled (GUITouchEvent *event)
{
//    GUIObject *gui = checked_cast<GUIObject*>(owner());
//    if (!gui)
//        return;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ComponentGUIButton.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentGUIButton.hpp"
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

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentGUIButton,"GUIBehaviour","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentGUIButton)

IMPLEMENT_EVENT_INFO_INDEX(_clicked)
IMPLEMENT_PLUG_INFO_INDEX(_sound)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentGUIButton)

    PLUG_INIT(_sound, "Sound")
		.set_input(true);

	EVENT_INIT(_clicked,"Clicked")
        .set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentGUIButton::ComponentGUIButton (void)
    :   _sound                      (PLUG_INFO_INDEX(_sound)),
        _clicked                    (EVENT_INFO_INDEX(_clicked))
{

}
		
ComponentGUIButton::ComponentGUIButton (const ComponentGUIButton &rhs)
    :   ComponentBase               (rhs),
        _button_pressed_latent_call (rhs._button_pressed_latent_call),
        _sound                      (rhs._sound),
        _clicked                    (rhs._clicked)
{

}

ComponentGUIButton & ComponentGUIButton::operator = (const ComponentGUIButton &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
        _button_pressed_latent_call = rhs._button_pressed_latent_call;
        _sound = rhs._sound;
    }
    return (*this);
}
			
ComponentGUIButton::~ComponentGUIButton (void)
{

}

//==============================================================================
//==============================================================================

void ComponentGUIButton::initialize (void)
{
	ComponentBase::initialize();
}

//==============================================================================
//==============================================================================

void ComponentGUIButton::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

	archive->push_domain (class_id());
        	
    *archive << ARCHIVE_PLUG(_sound, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentGUIButton::add_to_owner (ObjectBase *owner)
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

void ComponentGUIButton::remove_from_owner (void)
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

void ComponentGUIButton::touches_began (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;

    if (gui->state() != GUIObject::STATE_DISABLED) {
        gui->set_state(GUIObject::STATE_FOCUSED);
    }
}

void ComponentGUIButton::touches_moved (GUITouchEvent *event)
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
            gui->set_state(GUIObject::STATE_FOCUSED);
        } else {
            gui->set_state(GUIObject::STATE_NORMAL);
        }
    }

}

void ComponentGUIButton::touches_ended (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
        
    if (gui->state() == GUIObject::STATE_FOCUSED) {
    
        if (_sound.as_ref())
            System::audio_renderer()->play_quick(_sound, owner()->world());
        
        gui->set_state(GUIObject::STATE_NORMAL);
     
        _clicked.send(gui);    // GUIObject is the originator of the event
        
        // Call callback too
        if (_button_pressed_latent_call) {
            _button_pressed_latent_call->fire();
        }

    }
}

void ComponentGUIButton::touches_cancelled (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;

    if (gui->state() != GUIObject::STATE_DISABLED) {
        gui->set_state(GUIObject::STATE_NORMAL);
    }
}

//==============================================================================
//==============================================================================

} // DT3


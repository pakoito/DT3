#ifndef DT3_COMPONENTGUITOGGLEBUTTON
#define DT3_COMPONENTGUITOGGLEBUTTON
//==============================================================================
///	
///	File: ComponentGUIToggleButton.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class GUITouchEvent;
class SoundResource;

//==============================================================================
/// GUI behaviour for a toggle button.
//==============================================================================

class ComponentGUIToggleButton: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentGUIToggleButton,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentGUIToggleButton	(void);	
									ComponentGUIToggleButton	(const ComponentGUIToggleButton &rhs);
        ComponentGUIToggleButton &  operator =                  (const ComponentGUIToggleButton &rhs);	
        virtual                     ~ComponentGUIToggleButton	(void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize                  (void);
        
		/// Returns the component type. This defines which slot the component is
        /// put into on the object.
		/// \return Component type
        virtual ComponentType       component_type              (void)  {   return COMPONENT_TOUCH;  }

		/// Called when this component is added to the owner. Note that this will
        /// only be called if the owner is added to a world already. If not it 
        /// will be called when it is added to the World.
		/// \param owner Pointer to the owner
		virtual void                add_to_owner                (ObjectBase *owner);

		/// Called when this component is removed from its owner.
		virtual void                remove_from_owner           (void);


		/// Callback called when the component is getting a touch begin event
		/// \param event Touch events
        void                        touches_began               (GUITouchEvent *event);

		/// Callback called when the component is getting a touch move event
		/// \param event Touch events
        void                        touches_moved               (GUITouchEvent *event);

		/// Callback called when the component is getting a touch end event
		/// \param event Touch events
        void                        touches_ended               (GUITouchEvent *event);

		/// Callback called when the component is getting a touch cancelled event
		/// \param event Touch events
        void                        touches_cancelled           (GUITouchEvent *event);


		/// Forces the Toggle button on
		/// \param sender sender of the event
        void                        force_click_on              (PlugNode *sender);

		/// Forces the Toggle button off
		/// \param sender sender of the event
        void                        force_click_off             (PlugNode *sender);
        
        /// Defines standard accessors for the sound that is played when clicked on
		DEFINE_ACCESSORS (sound_on, set_sound_on, std::shared_ptr<SoundResource>, _sound_on);

        /// Defines standard accessors for the sound that is played when clicked off
		DEFINE_ACCESSORS (sound_off, set_sound_off, std::shared_ptr<SoundResource>, _sound_off);

        /// Defines standard accessors for the optional callback when this button is clicked
        DEFINE_ACCESSORS (button_pressed_latent_call,set_button_pressed_latent_call,std::shared_ptr<LatentCall>, _button_pressed_latent_call)

    private:
        std::shared_ptr<LatentCall>             _button_pressed_latent_call;

        Plug<std::shared_ptr<SoundResource> >   _sound_on;
        Plug<std::shared_ptr<SoundResource> >   _sound_off;

        Plug<DTboolean>                         _is_on;

        Event                                   _clicked_on;
        Event                                   _clicked_off;
        Event                                   _toggled;

        Event                                   _force_click_on;
        Event                                   _force_click_off;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

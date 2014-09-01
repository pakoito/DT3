#ifndef DT3_COMPONENTGUICLICKBUTTON
#define DT3_COMPONENTGUICLICKBUTTON
//==============================================================================
///	
///	File: ComponentGUIButton.hpp
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
#include "DT3Core/Types/Utility/LatentCall.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class GUITouchEvent;
class SoundResource;

//==============================================================================
/// GUI behaviour for a button.
//==============================================================================

class ComponentGUIButton: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentGUIButton,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentGUIButton      (void);
									ComponentGUIButton      (const ComponentGUIButton &rhs);
        ComponentGUIButton &        operator =              (const ComponentGUIButton &rhs);
        virtual                     ~ComponentGUIButton     (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize              (void);
        
		/// Returns the component type. This defines which slot the component is
        /// put into on the object.
		/// \return Component type
        virtual ComponentType       component_type          (void)  {   return COMPONENT_TOUCH;  }

		/// Called when this component is added to the owner. Note that this will
        /// only be called if the owner is added to a world already. If not it 
        /// will be called when it is added to the World.
		/// \param owner Pointer to the owner
		virtual void                add_to_owner            (ObjectBase *owner);

		/// Called when this component is removed from its owner.
		virtual void                remove_from_owner       (void);


		/// Callback called when the component is getting a touch begin event
		/// \param event Touch events
        void                        touches_began           (GUITouchEvent *event);

		/// Callback called when the component is getting a touch move event
		/// \param event Touch events
        void                        touches_moved           (GUITouchEvent *event);

		/// Callback called when the component is getting a touch end event
		/// \param event Touch events
        void                        touches_ended           (GUITouchEvent *event);

		/// Callback called when the component is getting a touch cancelled event
		/// \param event Touch events
        void                        touches_cancelled       (GUITouchEvent *event);


        /// Defines standard accessors for the sound that is played when this button is clicked
		DEFINE_ACCESSORS (sound, set_sound, std::shared_ptr<SoundResource>, _sound);
        
        /// Defines standard accessors for the optional callback when this button is clicked
        DEFINE_ACCESSORS (button_pressed_latent_call,set_button_pressed_latent_call,std::shared_ptr<LatentCall>, _button_pressed_latent_call)
 
    private:
        std::shared_ptr<LatentCall>                 _button_pressed_latent_call;
    
        Plug<std::shared_ptr<SoundResource> >       _sound;
        Event                                       _clicked;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

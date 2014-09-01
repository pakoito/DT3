#ifndef DT3_GUICONTROLLER
#define DT3_GUICONTROLLER
//==============================================================================
///	
///	File: GUIController.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Types/GUI/GUITouchEvent.hpp"
#include "DT3Core/Types/Utility/CallbackList.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;
class Color4f;

//==============================================================================
/// Class
//==============================================================================

class GUIController: public PlaceableObject {
    public:
        DEFINE_TYPE(GUIController,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
   		DEFINE_PLUG_NODE
     
                                        GUIController               (void);
                                        GUIController               (const GUIController &rhs);
        GUIController &                 operator =                  (const GUIController &rhs);
        virtual                         ~GUIController              (void);
                
        virtual void                    archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		void                            initialize                  (void);

		/// Callback called when the component is getting a touch event
		/// \param event Touch events
		/// \param camera The current camera
		/// \param handled reference to the currently handled widget
        void                            touch_gui                   (const TouchEvent *event, const std::shared_ptr<CameraObject> &camera, WorldNode* &handled);
        
		/// Draw Callback for component
		/// \param camera Camera used for drawing
        void                            draw_gui                    (const std::shared_ptr<CameraObject> &camera, const DTfloat lag);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                    add_to_world                (World *world);

		/// Object was removed from a world
        virtual void                    remove_from_world           (void);
    
		/// Set the focused object
		/// \param new_focus focused object
        void                            set_focus                   (GUIObject *new_focus);

        DEFINE_ACCESSORS(use_stencil, set_use_stencil, DTboolean, _use_stencil)
    private:
        GUITouchEvent                   _touch;
        DTboolean                       _use_stencil;

        void                            draw_gui_recursive          (PlaceableObject *root, const std::shared_ptr<CameraObject> &camera, const Color4f &parent_color, DTint stencil);
    
        std::shared_ptr<CameraObject>   _gui_camera;
};  

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_COMPONENTGUISCROLLER
#define DT3_COMPONENTGUISCROLLER
//==============================================================================
///	
///	File: ComponentGUIScroller.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"
#include "DT3Core/Types/Node/Plug.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class GUITouchEvent;
class GUIObject;
class CameraObject;
class Color4f;
class MaterialResource;
class ShaderResource;

//==============================================================================
/// GUI behaviour for a scroll zone. It scrolls its children objects.
//==============================================================================

class ComponentGUIScroller: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentGUIScroller,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentGUIScroller	(void);	
									ComponentGUIScroller	(const ComponentGUIScroller &rhs);
        ComponentGUIScroller &      operator =              (const ComponentGUIScroller &rhs);	
        virtual                     ~ComponentGUIScroller	(void);
    
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


		/// Callback called when overriding hit testing
        void                        hit_test                (GUITouchEvent *event, GUIObject* &hit_object, const Color4f &parent_color);

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
        
        
		/// Callback called when the component is getting a tick
		/// \param dt delta time
        void                        tick                    (DTfloat dt);

		/// Draw Callback for component
		/// \param camera Camera used for drawing
        void                        draw                    (const std::shared_ptr<CameraObject> &camera, const Color4f &parent_color);

		/// Forces the scroller to scroll to the top
        void                        scroll_to_top           (void)  {   _scroll_to_top = true;  }

		/// Forces the scroller to scroll to the bottom
        void                        scroll_to_bottom        (void)  {   _scroll_to_bottom = true;  }
        
		/// Forces the scroller to scroll to the left
        void                        scroll_to_left          (void)  {   _scroll_to_left = true;  }

		/// Forces the scroller to scroll to the right
        void                        scroll_to_right         (void)  {   _scroll_to_right = true;  }
        

		/// Called while an item is being dragged
        void                        calculate_contect_rect  (void);

		/// Called while an item is being dragged
        const Rectangle &           contect_rect            (void) const    {   return _contents;   }

        /// Defines standard accessors for the material
  		DEFINE_ACCESSORS (material, set_material, std::shared_ptr<MaterialResource>, _material);

		/// Get and retrieve the shader for this widget
  		DEFINE_ACCESSORS (shader, set_shader, std::shared_ptr<ShaderResource>, _shader);

        /// Defines standard accessors for the scroll horizontal flag
  		DEFINE_ACCESSORS (scroll_horz, set_scroll_horz, DTboolean, _scroll_horz);

        /// Defines standard accessors for the scroll vertical flag
  		DEFINE_ACCESSORS (scroll_vert, set_scroll_vert, DTboolean, _scroll_vert);

        /// Defines standard accessors for the scroll horizontal pad
  		DEFINE_ACCESSORS (pad_horz, set_pad_horz, DTfloat, _pad_horz);

        /// Defines standard accessors for the scroll vertical pad
  		DEFINE_ACCESSORS (pad_vert, set_pad_vert, DTfloat, _pad_vert);

        /// Defines standard accessors for auto centering
  		DEFINE_ACCESSORS (auto_center_horz, set_auto_center_horz, DTboolean, _auto_center_horz);
        
        /// Defines standard accessors for auto centering
  		DEFINE_ACCESSORS (auto_center_vert, set_auto_center_vert, DTboolean, _auto_center_vert);
    
     private:
        static const DTfloat VELOCITY_DECAY;
        static const DTfloat BORDER_BOUNCE;

		Plug<std::shared_ptr<MaterialResource> >    _material;
  		Plug<std::shared_ptr<ShaderResource> >      _shader;

        Plug<DTfloat>               _horz_scroll_percentage_fill;
        Plug<DTfloat>               _horz_scroll_position;
        Plug<DTfloat>               _vert_scroll_percentage_fill;
        Plug<DTfloat>               _vert_scroll_position;
        
        Rectangle                   _contents;

        DTboolean                   _scroll_horz;
        DTboolean                   _scroll_vert;

        DTboolean                   _auto_center_horz;
        DTboolean                   _auto_center_vert;
        
        DTboolean                   _scroll_to_top;
        DTboolean                   _scroll_to_bottom;
        DTboolean                   _scroll_to_left;
        DTboolean                   _scroll_to_right;
        
        DTfloat                     _vel_x;
        DTfloat                     _vel_y;

        DTfloat                     _pad_horz;
        DTfloat                     _pad_vert;
        
        DTfloat                     _velocity_decay;
        
        GUIObject                   *_focused;    
};

//==============================================================================
//==============================================================================

} // DT3

#endif

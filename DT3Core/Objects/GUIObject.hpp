#ifndef DT3_GUIOBJECT
#define DT3_GUIOBJECT
//==============================================================================
///	
///	File: GUIObject.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"
#include "DT3Core/Types/Utility/CallbackList.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class GUIAnimKey;
class GUIController;
class GUITouchEvent;
class CameraObject;
class MaterialResource;
class ShaderResource;

//==============================================================================
/// Base object for the different GUI objects in the engine.
//==============================================================================

class GUIObject: public PlaceableObject {
    public:
        DEFINE_TYPE(GUIObject,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    GUIObject                   (void);
                                    GUIObject                   (const GUIObject &rhs);
        GUIObject &                 operator =					(const GUIObject &rhs);
        virtual                     ~GUIObject                  (void);
  
        virtual void				archive                     (const std::shared_ptr<Archive> &archive);
	
		/// Called to initialize the object
		virtual void				initialize                  (void);

		/// Called to animate the object
        void                        animate                     (DTfloat dt);


        enum State {
            STATE_NORMAL = 0,
            STATE_FOCUSED = 1,
            STATE_DISABLED = 2
        };
        
		/// Returns the state of the object.
		/// \return Object state
		State						state                       (void) const                    {   return _state;                  }

		/// Sets the state of the object.
		/// \param state object state
        void						set_state                   (State state)                   {   _state = state;                 }
    

		/// Returns the bounds of the object.
		/// \param left left side of object
		/// \param bottom bottom side of object
		/// \param width width of object
		/// \param height height of object
		/// \param z z depth
        void                        set_bounds                  (DTfloat left, DTfloat bottom, DTfloat width, DTfloat height, DTfloat z = 0.0F);

		/// Returns the width of the object.
		/// \return width
		DTfloat						width                       (void) const                    {   return _width;                  }

		/// Sets the width of the object.
		/// \param width width
        void						set_width                   (const DTfloat width)           {   _width = width;                 }
        

		/// Returns the height of the object.
		/// \return height
		DTfloat						height                      (void) const                    {   return _height;                 }

		/// Sets the height of the object.
		/// \param height height
        void						set_height                  (const DTfloat height)          {   _height = height;           }
        
        
        enum {
            SIZE_MODE_NO_CHANGE = 0,
            SIZE_MODE_WIDTH_CONSTANT = 1,
            SIZE_MODE_HEIGHT_CONSTANT = 2,
            SIZE_MODE_PIXELS = 3
        };
        
		/// Returns the size mode for the object. The size mode is how the object interprets its width and height.
		/// \return size mode
		DTint                       size_mode                   (void) const                    {   return _size_mode;          }

		/// Sets the size mode for the object.
		/// \param size_mode size mode
        void						set_size_mode               (const DTint size_mode)         {   _size_mode = size_mode;     }
        
        
        enum {
            PIVOT_CENTER = 0,
            PIVOT_TOP_LEFT = 1,
            PIVOT_TOP_RIGHT = 2,
            PIVOT_BOTTOM_LEFT = 3,
            PIVOT_BOTTOM_RIGHT = 4
        };
        
		/// Returns the pivot for the object.
		/// \return pivot
		DTint						pivot                       (void) const                    {   return _pivot;              }

		/// Sets the pivot for the object.
		/// \param pivot pivot
        void						set_pivot                   (const DTint pivot)             {   _pivot = pivot;             }
        

        
		/// Returns the rect of the object.
		/// \return object rectangle
        Rectangle                   rectangle                   (void) const;

		/// Returns the rect of the object.
		/// \return object rectangle
        Rectangle                   screen_rectangle            (void) const;

		/// Returns the draw transform of the object.
		/// \return draw transform
        Matrix4                     draw_transform              (void) const;


		/// Returns the rect of all of the content.
		/// \return content rect
        Rectangle                   content_rectangle           (void) const;

		/// Returns the rect of all of the children.
		/// \return children content rect
        Rectangle                   children_content_rectangle  (void) const;

		/// Scrolls the childrens
		/// \param dx delta x
		/// \param dy delta y
        void                        scroll_contents             (DTfloat dx, DTfloat dy);
        
		/// Checks to see if there are animations queued on this object or its children
		/// \return is animating
        DTboolean                   is_contents_animating       (void) const;

		/// Returns the color of the object.
		/// \return color
        const Color4f&              color                       (void) const                    {   return _color.as_ref();  }

		/// Sets the color of the object.
		/// \param c color
        void                        set_color                   (const Color4f &c)              {   _color = c;     }
        
		/// Returns the label of the object.
		/// \return object label
        const std::string&          label                       (void) const                    {   return _label.as_ref();  }

		/// Sets the label of the object.
		/// \param l label
        void                        set_label                   (const std::string &l)          {   _label = l;     }
                
		/// Accessor for the no focus flag. No focus means that clicks will pass right through it.
        DEFINE_ACCESSORS(no_focus, set_no_focus, DTboolean, _no_focus)
        

		// Animation

		/// Add an animation key to the object
		/// \return animation key
		GUIAnimKey&					add_anim_key                (void);
    
		/// Add an animation key to the object
		/// \param key animation key
		void                        add_anim_key                (const GUIAnimKey& key);
    
		/// Checks if there are animations queued on this object
		/// \return has animations
		DTboolean					has_anim_key                (void) const        {	return _anim_keys.size() > 0;	}
	
		/// Removes all of the animations from this object
		void						clear_anims                 (void);

		/// Removes the "repeating" flag from any queued animations
		void						cancel_repeating_anims		(void);

        //
        // From GUI Controller
        //
        
		/// Gets the controller associated with this object
		/// \return returns controller
        GUIController*              controller              (void);
        
        //
        // Gui events
        //
        
		/// Called to hit test the object
		/// \param event event for hit testing
		/// \param hit_object object hit by the hit test
		/// \param parent_color parent object color (since colors are inherited)
        void                        hit_test                    (GUITouchEvent *event, GUIObject* &hit_object, const Color4f &parent_color);
    
		/// Touch began event
		/// \param event touch event
        void                        touches_began               (GUITouchEvent *event);

		/// Touch moved event
		/// \param event touch event
        void                        touches_moved               (GUITouchEvent *event);

		/// Touch ended event
		/// \param event touch event
        void                        touches_ended               (GUITouchEvent *event);

		/// Touch cancelled event
		/// \param event touch event
        void                        touches_cancelled           (GUITouchEvent *event);
        
        
		/// Draw Mask Callback for component
		/// \param camera Camera used for drawing
        void                        draw_gui_mask               (const std::shared_ptr<CameraObject> &camera);

		/// Draw Mask Callback for component
		/// \param camera Camera used for drawing
        void                        draw_gui_unmask             (const std::shared_ptr<CameraObject> &camera);

		/// Draw Callback for component
		/// \param camera Camera used for drawing
		/// \param parent_color Color of the parent GUI Object
        void                        draw_gui                    (const std::shared_ptr<CameraObject> &camera, const Color4f& parent_color);

		/// Post Draw Callback for the component
		/// \param camera Camera used for drawing
		/// \param parent_color Color of the parent GUI Object
        void                        post_draw_gui               (const std::shared_ptr<CameraObject> &camera, const Color4f& parent_color);

        
        // 
        // Utility
        // 
        
		/// Convert an input position to the gui object coordinate system
		/// \param pixel_pos position
		/// \return object coord position
        Vector2                     position_to_object_coord    (Vector2 pixel_pos);

		/// Convert an input velocity to the gui object coordinate system
		/// \param pixel_velocity velocity
		/// \return object coord velocity
        Vector2                     velocity_to_object_coord    (Vector2 pixel_velocity);

        
        //
        // World
        //


		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world           (void);


        //
        // Callbacks
        //

		/// Gets the Draw callbacks list
		/// \return Callbacks list
        CallbackList<const std::shared_ptr<CameraObject> &, const Color4f&>&            draw_gui_callbacks          (void)  {  return _draw_gui_callbacks;          }

		/// Gets the Post Draw callbacks list
		/// \return Callbacks list
        CallbackList<const std::shared_ptr<CameraObject> &, const Color4f&>&            post_draw_gui_callbacks     (void)  {  return _post_draw_gui_callbacks;     }

		/// Gets the Hit callbacks list
		/// \return Callbacks list
        CallbackList<GUITouchEvent*, GUIObject*&, const Color4f &>&                     hit_object_callbacks        (void)  {  return _hit_object_callbacks;        }

		/// Gets the Touches Began callbacks list
		/// \return Callbacks list
        CallbackList<GUITouchEvent *>&                                                  touches_began_callbacks     (void)  {  return _touches_began_callbacks;     }

		/// Gets the Touches Moved callbacks list
		/// \return Callbacks list
        CallbackList<GUITouchEvent *>&                                                  touches_moved_callbacks     (void)  {  return _touches_moved_callbacks;     }

		/// Gets the Touches Ended callbacks list
		/// \return Callbacks list
        CallbackList<GUITouchEvent *>&                                                  touches_ended_callbacks     (void)  {  return _touches_ended_callbacks;     }

		/// Gets the Touches Cancelled callbacks list
		/// \return Callbacks list
        CallbackList<GUITouchEvent *>&                                                  touches_cancelled_callbacks (void)  {  return _touches_cancelled_callbacks; }


	private:
        DrawBatcher                 _b;
    
        Plug<DTfloat>				_width;
		Plug<DTfloat>				_height;
        
		Plug<Color4f>               _color;
		Plug<std::string>           _label;

        State                       _state;
                
        DTint                       _size_mode;
        DTint                       _pivot;
        
        DTboolean                   _no_focus;

		std::list<GUIAnimKey>       _anim_keys;
		DTfloat						_animation_time;
                
        // 
        // Callbacks
        //
        
        CallbackList<const std::shared_ptr<CameraObject> &, const Color4f&>         _draw_gui_callbacks;
        CallbackList<const std::shared_ptr<CameraObject> &, const Color4f&>         _post_draw_gui_callbacks;
    
        CallbackList<GUITouchEvent*, GUIObject*&, const Color4f &>                  _hit_object_callbacks;
        CallbackList<GUITouchEvent*>                                                _touches_began_callbacks;
        CallbackList<GUITouchEvent*>                                                _touches_moved_callbacks;
        CallbackList<GUITouchEvent*>                                                _touches_ended_callbacks;
        CallbackList<GUITouchEvent*>                                                _touches_cancelled_callbacks;
    
        std::shared_ptr<MaterialResource>                                           _mask_material;
        std::shared_ptr<MaterialResource>                                           _unmask_material;
        std::shared_ptr<ShaderResource>                                             _shader;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

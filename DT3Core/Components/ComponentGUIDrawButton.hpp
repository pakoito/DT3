#ifndef DT3_COMPONENTGUIDRAWBUTTON
#define DT3_COMPONENTGUIDRAWBUTTON
//==============================================================================
///	
///	File: ComponentGUIDrawButton.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class MaterialResource;
class TextureResource2D;
class FontResource;

//==============================================================================
/// GUI drawing for a button.
//==============================================================================

class ComponentGUIDrawButton: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentGUIDrawButton,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentGUIDrawButton	(void);	
									ComponentGUIDrawButton	(const ComponentGUIDrawButton &rhs);
        ComponentGUIDrawButton &    operator =              (const ComponentGUIDrawButton &rhs);	
        virtual                     ~ComponentGUIDrawButton (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize              (void);
        
		/// Returns the component type. This defines which slot the component is
        /// put into on the object.
		/// \return Component type
        virtual ComponentType       component_type          (void)  {   return COMPONENT_DRAW;  }

		/// Draw Callback for component
		/// \param camera Camera used for drawing
        void                        draw                    (const std::shared_ptr<CameraObject> &camera, const Color4f &parent_color);
    
		/// Called when this component is added to the owner. Note that this will
        /// only be called if the owner is added to a world already. If not it 
        /// will be called when it is added to the World.
		/// \param owner Pointer to the owner
		virtual void                add_to_owner            (ObjectBase *owner);

		/// Called when this component is removed from its owner.
		virtual void                remove_from_owner       (void);

		/// Screen created callback.
        void                        screen_opened           (DTuint width, DTuint height);
        
		/// Screen destroyed callback.
        void                        screen_closed           (void);
        
        /// Defines standard accessors for the material
  		DEFINE_ACCESSORS (material, set_material, std::shared_ptr<MaterialResource>, _material);

        /// Defines standard accessors for the pressed material
  		DEFINE_ACCESSORS (pressed_material, set_pressed_material, std::shared_ptr<MaterialResource>, _pressed_material);

        /// Defines standard accessors for the disabled material
  		DEFINE_ACCESSORS (disabled_material, set_disabled_material, std::shared_ptr<MaterialResource>, _disabled_material);
    
        /// Defines standard accessors for the font material
  		DEFINE_ACCESSORS (font_material, set_font_material, std::shared_ptr<MaterialResource>, _font_material);

		/// Get and retrieve the shader for this image
  		DEFINE_ACCESSORS (shader, set_shader, std::shared_ptr<ShaderResource>, _shader);

        /// Defines standard accessors for the corner width
  		DEFINE_ACCESSORS_RANGED (corner_width, set_corner_width, DTfloat, _corner_width, 0.0F, 1.0F);

        /// Defines standard accessors for the corner height
  		DEFINE_ACCESSORS_RANGED (corner_height, set_corner_height, DTfloat, _corner_height, 0.0F, 1.0F);
      
        /// Defines standard accessors for the x offset
  		DEFINE_ACCESSORS (offset_x, set_offset_x, DTfloat, _offset_x);

        /// Defines standard accessors for the y offset
  		DEFINE_ACCESSORS (offset_y, set_offset_y, DTfloat, _offset_y);
    
        enum {
            DRAW_STYLE_STRETCH_CENTER_3X3 = 0,
            DRAW_STYLE_STRETCH_CENTER_2X2 = 1,
            DRAW_STYLE_RECTANGLE = 2
        };

        /// Defines standard accessors for the draw style
  		DEFINE_ACCESSORS (draw_style, set_draw_style, DTuint, _draw_style);
      
    private:
        DrawBatcher                                         _b;
    
		Plug<std::shared_ptr<MaterialResource> >            _material;
		Plug<std::shared_ptr<MaterialResource> >            _pressed_material;
		Plug<std::shared_ptr<MaterialResource> >            _disabled_material;
    
		Plug<std::shared_ptr<MaterialResource> >            _font_material;
    
  		Plug<std::shared_ptr<ShaderResource> >              _shader;
    
    
        DTuint                                              _draw_style;
    
        DTfloat                                             _corner_width;
        DTfloat                                             _corner_height;
        
        DTfloat                                             _offset_x;
        DTfloat                                             _offset_y;
                
        Rectangle                                           _bounds;
        std::string                                         _label;
    
        DTboolean                                           _needs_render;
        DTboolean                                           _can_render;
        
        std::shared_ptr<TextureResource2D>                  _text_image;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

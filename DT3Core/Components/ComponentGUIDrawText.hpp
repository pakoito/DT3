#ifndef DT3_COMPONENTGUIDRAWTEXT
#define DT3_COMPONENTGUIDRAWTEXT
//==============================================================================
///	
///	File: ComponentGUIDrawText.hpp
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
/// GUI drawing for text that supports different fonts and styles using
/// a special markup language. The formatting options supported are:
/// \\t		Tab
/// \\n		Newline
/// [j<]	Left justify text
/// [j>]	Right justify text
/// [j><]	Center justify text
/// [j<>]	Full justify text
/// [vlr]	Vertical text, left to right
/// [vrl]	Vertical text, right to left
/// [hrl]	Horizontal text, right to left
/// [hlr]	Horizontal text, left to right	(Default)
/// [ww]	Word Wrap
/// [cw]	Character wrap
/// [font="{somefont.ttf}"]	Set the font
/// [size=##]	Set the font size
/// [kern=##]	Set the font kern
/// [color=R G B A]	Set the font color
/// [outlinecolor=R G B A]	Set the outline color
/// [outlinesize=##]	Set the outline size
/// [color=R G B A]	Set the font color
/// [tab=##]	Set the tab size
/// [baseline=##]	Set the baseline shift
/// [U=##]	Insert a unicode character
//==============================================================================

class ComponentGUIDrawText: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentGUIDrawText,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentGUIDrawText	(void);
                                    ComponentGUIDrawText	(const ComponentGUIDrawText &rhs);
        ComponentGUIDrawText &      operator =              (const ComponentGUIDrawText &rhs);
        virtual                     ~ComponentGUIDrawText   (void);
    
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
		/// \param parent_color Color of the parent GUI Object
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
                
        /// Defines standard accessors for the font material
  		DEFINE_ACCESSORS (font_material, set_font_material, std::shared_ptr<MaterialResource>, _font_material);

		/// Get and retrieve the shader for this image
  		DEFINE_ACCESSORS (shader, set_shader, std::shared_ptr<ShaderResource>, _shader);

        /// Defines standard accessors for the vertical centering flag
  		DEFINE_ACCESSORS (center_vertically, set_center_vertically, DTboolean, _center_vertically);
        
        /// Defines standard accessors for state colors
  		DEFINE_ACCESSORS (normal_color, set_normal_color, Color4f, _normal_color);
  		DEFINE_ACCESSORS (pressed_color, set_pressed_color, Color4f, _pressed_color);
    
    private:
        DrawBatcher                                 _b;
        
		Plug<std::shared_ptr<MaterialResource> >    _font_material;
  		Plug<std::shared_ptr<ShaderResource> >      _shader;

        Rectangle                                   _bounds;
        std::string                                 _label;
        DTboolean                                   _needs_render;
        DTboolean                                   _can_render;
        DTboolean                                   _center_vertically;
        
        Color4f                                     _normal_color;
        Color4f                                     _pressed_color;
        
        std::shared_ptr<TextureResource2D>          _text_image;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

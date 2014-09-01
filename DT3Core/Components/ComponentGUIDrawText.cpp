//==============================================================================
///	
///	File: ComponentGUIDrawText.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentGUIDrawText.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/GUI/GUITouchEvent.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Text/TextRenderer.hpp"
#include "DT3Core/Types/Text/GlyphCache.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Objects/GUIObject.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Resources/ResourceTypes/SoundResource.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentGUIDrawText,"GUI","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentGUIDrawText)

IMPLEMENT_PLUG_INFO_INDEX(_font_material)
IMPLEMENT_PLUG_INFO_INDEX(_shader)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentGUIDrawText)
                        
    PLUG_INIT(_font_material, "Font_Material")
		.set_input(true);
    
    PLUG_INIT(_shader, "Shader")
		.set_input(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentGUIDrawText::ComponentGUIDrawText (void)
    :   _font_material      (PLUG_INFO_INDEX(_font_material)),
        _shader             (PLUG_INFO_INDEX(_shader)),
        _bounds             (0.0F,0.0F,0.0F,0.0F),
        _needs_render       (true),
        _can_render         (true),
        _center_vertically  (false),
        _normal_color       (1.0F,1.0F,1.0F,1.0F),
        _pressed_color      (1.0F,1.0F,1.0F,1.0F)
{

}
		
ComponentGUIDrawText::ComponentGUIDrawText (const ComponentGUIDrawText &rhs)
    :   ComponentBase       (rhs),
        _font_material      (rhs._font_material),
        _shader             (PLUG_INFO_INDEX(_shader)),
        _bounds             (rhs._bounds),
        _needs_render       (true),
        _can_render         (true),
        _center_vertically  (rhs._center_vertically),
        _normal_color       (rhs._normal_color),
        _pressed_color      (rhs._pressed_color)
{

}

ComponentGUIDrawText & ComponentGUIDrawText::operator = (const ComponentGUIDrawText &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
                
        _font_material = rhs._font_material;
        _shader = rhs._shader;
        
        _bounds = rhs._bounds;
        _needs_render = true;
        _can_render = true;
        _center_vertically = rhs._center_vertically;
        _normal_color = rhs._normal_color;
        _pressed_color = rhs._pressed_color;
    }
    return (*this);
}
			
ComponentGUIDrawText::~ComponentGUIDrawText (void)
{
    SystemCallbacks::screen_opened_cb().remove(make_callback(this,&type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this,&type::screen_closed));
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawText::initialize (void)
{
	ComponentBase::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this,&type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this,&type::screen_closed));
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawText::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

	archive->push_domain (class_id ());
        		    
	*archive << ARCHIVE_PLUG(_font_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_shader, DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_DATA(_center_vertically, DATA_PERSISTENT | DATA_SETTABLE);
    
    *archive << ARCHIVE_DATA(_normal_color, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_pressed_color, DATA_PERSISTENT | DATA_SETTABLE);
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawText::screen_opened (DTuint width, DTuint height)
{
    // If for some reason the render context is created, we have to rerender
    
    _can_render = true;
    _needs_render = true;
}
        
void ComponentGUIDrawText::screen_closed (void)
{
    // If for some reason the render context is lost, we have to clean up

    _can_render = false;
    _needs_render = true;
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawText::draw (const std::shared_ptr<CameraObject> &camera, const Color4f &parent_color)
{        
    if (!_font_material.as_ref() || !_shader.as_ref())
        return;

    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
    
    if (gui->color().a_as_float() * parent_color.a_as_float() <= 0.0F)
        return;
    
    if (gui->scale() == Vector3(0.0F,0.0F,0.0F))
        return;

    Rectangle local_rect = gui->rectangle();
    DTfloat screen_width = System::renderer()->viewport_width();
    //DTfloat screen_height = System::getRenderer()->getViewportHeight();
    
    //
    // Check screen space rectangle to see if it has changed
    //
    Matrix4 transform = gui->draw_transform();

    Vector2 p0 = transform * Vector2(local_rect.minus_x(), local_rect.minus_y());
    Vector2 p1 = transform * Vector2(local_rect.plus_x(), local_rect.minus_y());
    Vector2 p2 = transform * Vector2(local_rect.minus_x(), local_rect.plus_y());
    Vector2 p3 = transform * Vector2(local_rect.plus_x(), local_rect.plus_y());

    Rectangle bounds;
    bounds.set_minus_x( MoreMath::min(p0.x, p1.x, p2.x, p3.x) );
    bounds.set_plus_x( MoreMath::max(p0.x, p1.x, p2.x, p3.x) );
    bounds.set_minus_y( MoreMath::min(p0.y, p1.y, p2.y, p3.y) );
    bounds.set_plus_y( MoreMath::max(p0.y, p1.y, p2.y, p3.y) );
    
    DTfloat new_width = bounds.width();
    DTfloat new_height = bounds.height();

    DTfloat width = _bounds.width();
    DTfloat height = _bounds.height();
    
    _bounds = bounds;

    const DTfloat TOLERANCE = 1.0F / screen_width;
    std::string full_label = Globals::substitute_global(gui->label());

    if ((   (fabsf(new_width-width) > TOLERANCE) || 
            (fabsf(new_height-height) > TOLERANCE) || 
            _label != full_label || _needs_render) && _can_render && _font_material.as_ref()) {

        _needs_render = false;
        _label = full_label;
    
        DTint tex_width = (_bounds.plus_x() - _bounds.minus_x()) * System::renderer()->screen_width();
        DTint tex_height = (_bounds.plus_y() - _bounds.minus_y()) * System::renderer()->screen_height();
        
        if (!_text_image || (_text_image->width() != tex_width) || (_text_image->height() != tex_height) ) {
        
            if (!_text_image) {
                _text_image = std::shared_ptr<TextureResource2D>(TextureResource2D::create());
            }
                
            _text_image->allocate_rgba_textels (tex_width, tex_height, false);
        }
        
        // Build local transform for bounds rectangle
        Matrix4 tex_transform = gui->transform();
        tex_transform.set_scale(Vector3(1.0F,1.0F,1.0F));
        tex_transform._m14 = 0.0F;
        tex_transform._m24 = 0.0F;

        DTfloat screen_width = System::renderer()->screen_width();
        DTfloat screen_height = System::renderer()->screen_height();
        DTfloat resolution_mul = (screen_width > screen_height) ? screen_height / 768.0F : 1.0F;

        TextRenderer::begin_render (_text_image);

        GlyphCache cache;
        TextLines lines;
        
        lines.add_line(cache, Matrix4(tex_transform), gui->scale(), resolution_mul, _label);
        
        lines.wrap(tex_width, tex_height, _center_vertically ? TextLines::ALIGN_MIDDLE : TextLines::ALIGN_TOP);
        TextRenderer::render_lines(_text_image, lines);

        TextRenderer::end_render (_text_image);
    }
    
    if (_text_image) {
    
        Color4f c;
        
        if (gui->state() == GUIObject::STATE_FOCUSED)
            c = gui->color() * parent_color * _pressed_color;
        else
            c = gui->color() * parent_color * _normal_color;
        
        (*_font_material)->set_current_unit(0);
        (*_font_material)->set_texture_2D(_text_image);

        _b.batch_begin(  camera,
                        _font_material,
                        _shader,
                        Matrix4::identity(),
                        DT3GL_PRIM_TRI_STRIP,
                        DrawBatcher::FMT_V | DrawBatcher::FMT_T0 | DrawBatcher::FMT_C);
        
        _b.add().v(_bounds.minus_x(),_bounds.plus_y(),0.0F)  .t0(0.0F,1.0F)  .c(c);
        _b.add().v(_bounds.minus_x(),_bounds.minus_y(),0.0F) .t0(0.0F,0.0F)  .c(c);
        _b.add().v(_bounds.plus_x(),_bounds.plus_y(),0.0F)   .t0(1.0F,1.0F)  .c(c);
        _b.add().v(_bounds.plus_x(),_bounds.minus_y(),0.0F)  .t0(1.0F,0.0F)  .c(c);
        
        _b.batch_end();
        _b.flush();
    }
    
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawText::add_to_owner (ObjectBase *owner)
{
    ComponentBase::add_to_owner(owner);
    
    GUIObject *gui = checked_cast<GUIObject*>(owner);
    if (gui) {
        gui->draw_gui_callbacks().add(make_callback(this, &type::draw));
    }
}

void ComponentGUIDrawText::remove_from_owner (void)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (gui) {
        gui->draw_gui_callbacks().remove(make_callback(this, &type::draw));
    }

    ComponentBase::remove_from_owner();
}

//==============================================================================
//==============================================================================

} // DT3


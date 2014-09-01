//==============================================================================
///	
///	File: ComponentGUIDrawIcon.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentGUIDrawIcon.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Graphics/DrawUtils.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Objects/GUIObject.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentGUIDrawIcon,"GUI","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentGUIDrawIcon)

IMPLEMENT_PLUG_INFO_INDEX(_material)
IMPLEMENT_PLUG_INFO_INDEX(_pressed_material)
IMPLEMENT_PLUG_INFO_INDEX(_shader)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentGUIDrawIcon)
        
    PLUG_INIT(_material, "Material")
		.set_input(true);
        
    PLUG_INIT(_pressed_material, "Pressed_Material")
		.set_input(true);

    PLUG_INIT(_shader, "Shader")
		.set_input(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentGUIDrawIcon::ComponentGUIDrawIcon (void)
    :   _material           (PLUG_INFO_INDEX(_material)),
        _pressed_material   (PLUG_INFO_INDEX(_pressed_material)),
        _shader             (PLUG_INFO_INDEX(_shader)),
        _draw_style         (2),
        _corner_width       (0.0F),
        _corner_height      (0.0F)
{

}
		
ComponentGUIDrawIcon::ComponentGUIDrawIcon (const ComponentGUIDrawIcon &rhs)
    :   ComponentBase       (rhs),
        _material           (rhs._material),
        _pressed_material   (rhs._pressed_material),
        _shader             (rhs._shader),
        _draw_style         (rhs._draw_style),
        _corner_width       (rhs._corner_width),
        _corner_height      (rhs._corner_height)
{

}

ComponentGUIDrawIcon & ComponentGUIDrawIcon::operator = (const ComponentGUIDrawIcon &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
        _material = rhs._material;
        _pressed_material = rhs._pressed_material;
        _shader = rhs._shader;
        _draw_style = rhs._draw_style;
        
        _corner_width = rhs._corner_width;
        _corner_height = rhs._corner_height;
    }
    return (*this);
}
			
ComponentGUIDrawIcon::~ComponentGUIDrawIcon (void)
{

}

//==============================================================================
//==============================================================================

void ComponentGUIDrawIcon::initialize (void)
{
	ComponentBase::initialize();
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawIcon::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

    archive->push_domain (class_id ());
	
    *archive << ARCHIVE_DATA_ACCESSORS("Material", ComponentGUIDrawIcon::material, ComponentGUIDrawIcon::set_material, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Pressed_Material", ComponentGUIDrawIcon::pressed_material, ComponentGUIDrawIcon::set_pressed_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Shader", ComponentGUIDrawIcon::shader, ComponentGUIDrawIcon::set_shader, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA_ACCESSORS("Draw_Style", ComponentGUIDrawIcon::draw_style, ComponentGUIDrawIcon::set_draw_style, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Stretch Center 3x3")
        .add_enum("Stretch Center 2x2")
        .add_enum("Rectangle");

	*archive << ARCHIVE_DATA(_corner_width, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_corner_height, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawIcon::draw (const std::shared_ptr<CameraObject> &camera, const Color4f &parent_color)
{
    if (!_material.as_ref() || !_shader.as_ref())
        return;

    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
    
    if (gui->color().a_as_float() * parent_color.a_as_float() <= 0.0F)
        return;
    
    if (gui->scale() == Vector3(0.0F,0.0F,0.0F))
        return;

    // Get Alternate materials
    Color4f c = gui->color() * parent_color;
    std::shared_ptr<MaterialResource> m = _material;
    
    if (gui->state() == GUIObject::STATE_FOCUSED) {
        if (_pressed_material.as_ref()) {
            m = _pressed_material;
        } else {
            c = gui->color() * 0.5F;
        }
    }


    Rectangle local_rect = gui->rectangle();

    // Draw background
    if ( (_corner_width > 0.0F || _corner_height > 0.0F) && _draw_style == DRAW_STYLE_STRETCH_CENTER_3X3) {
        DrawUtils::draw_quad_stretch_center_3x3 (
            _b, camera, m, _shader,
            c,
            gui->draw_transform(),
            local_rect.minus_x(), local_rect.minus_y(),
            local_rect.width(), local_rect.height(),
            _corner_width, _corner_height * System::renderer()->screen_aspect());
        
    } else if ( (_corner_width > 0.0F || _corner_height > 0.0F) && _draw_style == DRAW_STYLE_STRETCH_CENTER_2X2) {
        DrawUtils::draw_quad_stretch_center_2x2 (
            _b, camera, m, _shader,
            c,
            gui->draw_transform(),
            local_rect.minus_x(), local_rect.minus_y(),
            local_rect.width(), local_rect.height(),
            _corner_width, _corner_height * System::renderer()->screen_aspect());
        
    } else {
        _b.batch_begin(  camera,
                        m,
                        _shader,
                        gui->draw_transform(),
                        DT3GL_PRIM_TRI_STRIP,
                        DrawBatcher::FMT_V | DrawBatcher::FMT_T0 | DrawBatcher::FMT_C);
        
        _b.add().v(local_rect.minus_x(),local_rect.plus_y(),0.0F)  .t0(0.0F,1.0F)  .c(c);
        _b.add().v(local_rect.minus_x(),local_rect.minus_y(),0.0F) .t0(0.0F,0.0F)  .c(c);
        _b.add().v(local_rect.plus_x(),local_rect.plus_y(),0.0F)   .t0(1.0F,1.0F)  .c(c);
        _b.add().v(local_rect.plus_x(),local_rect.minus_y(),0.0F)  .t0(1.0F,0.0F)  .c(c);
    
        _b.batch_end();
        _b.flush();
    }


//    DrawBatcher b;
//    b.batch_begin(  camera,
//                    _material,
//                    _shader,
//                    gui->draw_transform(),
//                    DT3GL_PRIM_TRI_STRIP,
//                    DrawBatcher::FMT_V | DrawBatcher::FMT_T0 | DrawBatcher::FMT_C,
//                    4);
//    
//    Rectangle rect = gui->rectangle();
//
//    b.add().v(rect.minus_x(),rect.plus_y(),0.0F).t0(0.0F,1.0F).c(c);
//    b.add().v(rect.minus_x(),rect.minus_y(),0.0F).t0(0.0F,0.0F).c(c);
//    b.add().v(rect.plus_x(),rect.plus_y(),0.0F).t0(1.0F,1.0F).c(c);
//    b.add().v(rect.plus_x(),rect.minus_y(),0.0F).t0(1.0F,0.0F).c(c);
//    
//    b.batch_end();
    
}

//==============================================================================
//==============================================================================

void ComponentGUIDrawIcon::add_to_owner (ObjectBase *owner)
{
    ComponentBase::add_to_owner(owner);
    
    GUIObject *gui = checked_cast<GUIObject*>(owner);
    if (gui) {
        gui->draw_gui_callbacks().add(make_callback(this, &type::draw));
    }
}

void ComponentGUIDrawIcon::remove_from_owner (void)
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


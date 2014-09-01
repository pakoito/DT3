//==============================================================================
///	
///	File: ComponentGUIScroller.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentGUIScroller.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"
#include "DT3Core/Types/GUI/GUITouchEvent.hpp"
#include "DT3Core/Objects/GUIObject.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

const DTfloat ComponentGUIScroller::VELOCITY_DECAY = 0.85F;
const DTfloat ComponentGUIScroller::BORDER_BOUNCE = 0.8F;

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentGUIScroller,"GUIBehaviour","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentGUIScroller)

IMPLEMENT_PLUG_INFO_INDEX(_material)
IMPLEMENT_PLUG_INFO_INDEX(_shader)
IMPLEMENT_PLUG_INFO_INDEX(_horz_scroll_percentage_fill)
IMPLEMENT_PLUG_INFO_INDEX(_horz_scroll_position)
IMPLEMENT_PLUG_INFO_INDEX(_vert_scroll_percentage_fill)
IMPLEMENT_PLUG_INFO_INDEX(_vert_scroll_position)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentGUIScroller)

    PLUG_INIT(_material, "Material")
		.set_input(true);

    PLUG_INIT(_shader, "Shader")
		.set_input(true);

    PLUG_INIT(_horz_scroll_percentage_fill, "Horz_Percentage")
		.set_output(true);

    PLUG_INIT(_horz_scroll_position, "Horz_Position")
		.set_output(true);

    PLUG_INIT(_vert_scroll_percentage_fill, "Vert_Percentage")
		.set_output(true);

    PLUG_INIT(_vert_scroll_position, "Vert_Position")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentGUIScroller::ComponentGUIScroller (void)
    :   _material                   (PLUG_INFO_INDEX(_material)),
        _shader                     (PLUG_INFO_INDEX(_shader)),
        _horz_scroll_percentage_fill(PLUG_INFO_INDEX(_horz_scroll_percentage_fill), 0.0F),
        _horz_scroll_position       (PLUG_INFO_INDEX(_horz_scroll_position), 0.0F),
        _vert_scroll_percentage_fill(PLUG_INFO_INDEX(_vert_scroll_percentage_fill), 0.0F),
        _vert_scroll_position       (PLUG_INFO_INDEX(_vert_scroll_position), 0.0F),
        _scroll_horz                (true),
        _scroll_vert                (true),
        _auto_center_horz           (true),
        _auto_center_vert           (true),
        _vel_x                      (0.0F),
        _vel_y                      (0.0F),
        _pad_horz                   (0.0F),
        _pad_vert                   (0.0F),
        _scroll_to_top              (false),
        _scroll_to_bottom           (false),
        _scroll_to_left             (false),
        _scroll_to_right            (false),
        _velocity_decay             (VELOCITY_DECAY),
        _focused                    (NULL),
        _contents                   (0.0F,0.0F,0.0F,0.0F)
{  

}
		
ComponentGUIScroller::ComponentGUIScroller (const ComponentGUIScroller &rhs)
    :   ComponentBase               (rhs),
        _material                   (rhs._material),
        _shader                     (rhs._shader),
        _horz_scroll_percentage_fill(rhs._horz_scroll_percentage_fill),
        _horz_scroll_position       (rhs._horz_scroll_position),
        _vert_scroll_percentage_fill(rhs._vert_scroll_percentage_fill),
        _vert_scroll_position       (rhs._vert_scroll_position),
        _scroll_horz                (rhs._scroll_horz),
        _scroll_vert                (rhs._scroll_vert),
        _auto_center_horz           (rhs._auto_center_horz),
        _auto_center_vert           (rhs._auto_center_vert),
        _vel_x                      (rhs._vel_x),
        _vel_y                      (rhs._vel_y),
        _pad_horz                   (rhs._pad_horz),
        _pad_vert                   (rhs._pad_vert),
        _scroll_to_top              (rhs._scroll_to_top),
        _scroll_to_bottom           (rhs._scroll_to_bottom),
        _scroll_to_left             (rhs._scroll_to_left),
        _scroll_to_right            (rhs._scroll_to_right),
        _velocity_decay             (rhs._velocity_decay),
        _focused                    (rhs._focused),
        _contents                   (rhs._contents)
{

}

ComponentGUIScroller & ComponentGUIScroller::operator = (const ComponentGUIScroller &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
        _material = rhs._material;
        _shader = rhs._shader;
        
        _horz_scroll_percentage_fill = rhs._horz_scroll_percentage_fill;
        _horz_scroll_position = rhs._horz_scroll_position;

        _vert_scroll_percentage_fill = rhs._vert_scroll_percentage_fill;
        _vert_scroll_position = rhs._vert_scroll_position;

        _scroll_horz = rhs._scroll_horz;
        _scroll_vert = rhs._scroll_vert;

        _auto_center_horz = rhs._auto_center_horz;
        _auto_center_vert = rhs._auto_center_vert;
        
        _vel_x = rhs._vel_x;
        _vel_y = rhs._vel_y;
        
        _pad_horz = rhs._pad_horz;
        _pad_vert = rhs._pad_vert;

        _scroll_to_top = rhs._scroll_to_top;
        _scroll_to_bottom = rhs._scroll_to_bottom;
        _scroll_to_left = rhs._scroll_to_left;
        _scroll_to_right = rhs._scroll_to_right;

        _velocity_decay = rhs._velocity_decay;
        
        _focused = rhs._focused;
        
        _contents = rhs._contents;
        
        _focused = rhs._focused;
    }
    return (*this);
}
			
ComponentGUIScroller::~ComponentGUIScroller (void)
{

}

//==============================================================================
//==============================================================================

void ComponentGUIScroller::initialize (void)
{
	ComponentBase::initialize();
}

//==============================================================================
//==============================================================================

void ComponentGUIScroller::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_shader, DATA_PERSISTENT | DATA_SETTABLE);
    
	*archive << ARCHIVE_DATA(_scroll_horz, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_scroll_vert, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA(_auto_center_horz, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_auto_center_vert, DATA_PERSISTENT | DATA_SETTABLE);
    
    *archive << ARCHIVE_DATA(_pad_horz, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_pad_vert, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA(_velocity_decay, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentGUIScroller::add_to_owner (ObjectBase *owner)
{
    ComponentBase::add_to_owner(owner);
    
    GUIObject *gui = checked_cast<GUIObject*>(owner);
    if (gui) {
        
        gui->hit_object_callbacks().add(make_callback(this, &ComponentGUIScroller::hit_test));

        gui->touches_began_callbacks().add(make_callback(this, &type::touches_began));
        gui->touches_moved_callbacks().add(make_callback(this, &type::touches_moved));
        gui->touches_ended_callbacks().add(make_callback(this, &type::touches_ended));
        gui->touches_cancelled_callbacks().add(make_callback(this, &type::touches_cancelled));

        gui->post_draw_gui_callbacks().add(make_callback(this, &ComponentGUIScroller::draw));
    }
    
    World *w = owner->world();
    w->register_for_tick(owner, make_callback(this, &ComponentGUIScroller::tick));
}

void ComponentGUIScroller::remove_from_owner (void)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (gui) {
        gui->hit_object_callbacks().remove(make_callback(this, &ComponentGUIScroller::hit_test));

        gui->touches_began_callbacks().remove(make_callback(this, &type::touches_began));
        gui->touches_moved_callbacks().remove(make_callback(this, &type::touches_moved));
        gui->touches_ended_callbacks().remove(make_callback(this, &type::touches_ended));
        gui->touches_cancelled_callbacks().remove(make_callback(this, &type::touches_cancelled));

        gui->post_draw_gui_callbacks().remove(make_callback(this, &ComponentGUIScroller::draw));
    }

    World *w = owner()->world();
    w->unregister_for_tick(owner(), make_callback(this, &type::tick));

    ComponentBase::remove_from_owner();
}

//==============================================================================
//==============================================================================

void ComponentGUIScroller::calculate_contect_rect(void)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    _contents = gui->children_content_rectangle();
}

//==============================================================================
//==============================================================================

void ComponentGUIScroller::tick (DTfloat dt)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
    
    // Ramp velocity
    if (_vel_x != 0.0F || _vel_y != 0.0F) {
    
        gui->scroll_contents (_vel_x * dt, _vel_y * dt);
        _contents.offset(Vector2(_vel_x * dt, _vel_y * dt));

        _vel_x *= _velocity_decay;
        _vel_y *= _velocity_decay;
        
    }
        
    // Limit scroll bounds
    Rectangle rect = gui->rectangle();
        
    if (_scroll_vert) {
        if (_contents.height() > rect.height()) {
        
            if (_contents.plus_y() < (rect.plus_y() - _pad_vert)) {
            
                DTfloat delta = (rect.plus_y() - _pad_vert) - _contents.plus_y();
                gui->scroll_contents (0, delta * BORDER_BOUNCE);
                _contents.offset(Vector2(0, delta * BORDER_BOUNCE));
             
                //LOG_MESSAGE << "Top Bound hit " << contents.plus_y() << " " << rect.plus_y();
                
            } else if (_contents.minus_y() > (rect.minus_y() + _pad_vert)) {
                
                DTfloat delta = (rect.minus_y() + _pad_vert) - _contents.minus_y();
                gui->scroll_contents (0, delta * BORDER_BOUNCE);
                _contents.offset(Vector2(0, delta * BORDER_BOUNCE));
            
                //LOG_MESSAGE << "Bottom Bound hit " << contents.minus_y() << " " << rect.minus_y();

            } 

            if (_scroll_to_top) {
                DTfloat delta = rect.plus_y() - _contents.plus_y();
                gui->scroll_contents (0, delta);
                _contents.offset(Vector2(0, delta));
            }
            if (_scroll_to_bottom) {  
                DTfloat delta = rect.minus_y() - _contents.minus_y();
                gui->scroll_contents (0, delta);
                _contents.offset(Vector2(0, delta));
            }
                        
        } else {
        
            if (_auto_center_vert) {
                DTfloat delta = ( (rect.plus_y() + rect.minus_y())*0.5F - (_contents.plus_y() + _contents.minus_y())*0.5F );
                gui->scroll_contents (0, delta * BORDER_BOUNCE);
                _contents.offset(Vector2(0, delta * BORDER_BOUNCE));
            } else {
                DTfloat delta = rect.plus_y() - _contents.plus_y();
                gui->scroll_contents (0, delta * BORDER_BOUNCE);
                _contents.offset(Vector2(0, delta * BORDER_BOUNCE));
            }
            
        }
        
        _vert_scroll_percentage_fill = (rect.plus_y() - rect.minus_y()) / (_contents.plus_y() - _contents.minus_y());
        if (_vert_scroll_percentage_fill > 1.0) _vert_scroll_percentage_fill = 1.0F;
        
    }

    if (_scroll_horz) {
        if (_contents.width() > rect.width()) {
            if ( _contents.plus_x() < (rect.plus_x() - _pad_horz)) {
            
                DTfloat delta = (rect.plus_x() - _pad_horz) - _contents.plus_x();
                gui->scroll_contents (delta * BORDER_BOUNCE,0);
                _contents.offset(Vector2(delta * BORDER_BOUNCE,0));
            
                //LOG_MESSAGE << "Right Bound hit " << contents.plus_x() << " " << rect.plus_x();
                
            } else if ( _contents.minus_x() > (rect.minus_x() + _pad_horz) ) {
                
                DTfloat delta = (rect.minus_x() + _pad_horz) -  _contents.minus_x();
                gui->scroll_contents (delta * BORDER_BOUNCE,0);
                _contents.offset(Vector2(delta * BORDER_BOUNCE,0));
            
                //LOG_MESSAGE << "Left Bound hit " << contents.getMinusX() << " " << rect.getMinusX();

            }
            
            if (_scroll_to_left) {
                DTfloat delta = rect.minus_x() - _contents.minus_x();
                gui->scroll_contents (delta,0);
                _contents.offset(Vector2(delta,0));
            }
            if (_scroll_to_right) {  
                DTfloat delta = rect.plus_x() - _contents.plus_x();
                gui->scroll_contents (delta,0);
                _contents.offset(Vector2(delta,0));
            }

        } else {
        
            if (_auto_center_horz) {
                DTfloat delta = ( (rect.plus_x() + rect.minus_x())*0.5F - (_contents.plus_x() + _contents.minus_x())*0.5F );
                gui->scroll_contents (delta * BORDER_BOUNCE, 0);
                _contents.offset(Vector2(delta * BORDER_BOUNCE,0));
            } else {
                DTfloat delta = rect.minus_x() - _contents.minus_x();
                gui->scroll_contents (delta * BORDER_BOUNCE, 0);
                _contents.offset(Vector2(delta * BORDER_BOUNCE,0));
            }
            
        }
    }
    
    // If animating, keep flags on
    if (!gui->has_anim_key()) {
        _scroll_to_top = false;
        _scroll_to_bottom = false;
        _scroll_to_left = false;
        _scroll_to_right = false;
    }
    
}

//==============================================================================
//==============================================================================

void ComponentGUIScroller::draw (const std::shared_ptr<CameraObject> &camera, const Color4f &parent_color)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
    
    if (gui->color().a_as_float() * parent_color.a_as_float() <= 0.0F)
        return;
    
    if (gui->scale() == Vector3(0.0F,0.0F,0.0F))
        return;
    
    //
    // Scroll bars
    //

    Color4f c = gui->color() * parent_color;

    if (_material.as_ref()) {
        const DTfloat H_SIZE = 6.0F/1024.0F;
        const DTfloat V_SIZE = H_SIZE / System::renderer()->screen_aspect();
                    
        // Limit scroll bounds
        Rectangle rect = gui->rectangle();
            
        if (_scroll_vert && (_contents.plus_y() > rect.plus_y() || _contents.minus_y() < rect.minus_y())) {
            
            DTfloat top_gap = 1.0F - ( _contents.plus_y() - rect.plus_y() ) / _contents.height();
            DTfloat bottom_gap = ( rect.minus_y() - _contents.minus_y() ) / _contents.height();
            
            if (top_gap > 1.0F)     top_gap = 1.0F;
            if (bottom_gap < 0.0F)  bottom_gap = 0.0F;
                    
            DrawBatcher b;
            b.batch_begin(  camera,
                _material,
                _shader,
                gui->draw_transform(),
                DT3GL_PRIM_TRI_STRIP,
                DrawBatcher::FMT_V | DrawBatcher::FMT_C | DrawBatcher::FMT_T0);

            b.add().v(rect.plus_x() - H_SIZE,  rect.minus_y() + rect.height() * bottom_gap,             0.0F)   .t0(0.0F,0.0F)  .c(c);
            b.add().v(rect.plus_x(),           rect.minus_y() + rect.height() * bottom_gap,             0.0F)   .t0(1.0F,0.0F)  .c(c);
            b.add().v(rect.plus_x() - H_SIZE,  rect.minus_y() + rect.height() * bottom_gap + V_SIZE,    0.0F)   .t0(0.0F,0.5F)  .c(c);
            b.add().v(rect.plus_x(),           rect.minus_y() + rect.height() * bottom_gap + V_SIZE,    0.0F)   .t0(1.0F,0.5F)  .c(c);

            b.add().v(rect.plus_x() - H_SIZE,  rect.minus_y() + rect.height() * top_gap - V_SIZE,       0.0F)   .t0(0.0F,0.5F)  .c(c);
            b.add().v(rect.plus_x(),           rect.minus_y() + rect.height() * top_gap - V_SIZE,       0.0F)   .t0(1.0F,0.5F)  .c(c);
            b.add().v(rect.plus_x() - H_SIZE,  rect.minus_y() + rect.height() * top_gap,                0.0F)   .t0(0.0F,1.0F)  .c(c);
            b.add().v(rect.plus_x(),           rect.minus_y() + rect.height() * top_gap,                0.0F)   .t0(1.0F,1.0F)  .c(c);
            
            b.batch_end();
            b.flush();
        }

        if (_scroll_horz && (_contents.plus_x() > rect.plus_x() || _contents.minus_x() < rect.minus_x())) {
            
            DTfloat right_gap = 1.0F - ( _contents.plus_x() - rect.plus_x() ) / _contents.width();
            DTfloat left_gap = ( rect.minus_x() - _contents.minus_x() ) / _contents.width();
            
            if (right_gap > 1.0F)   right_gap = 1.0F;
            if (left_gap < 0.0F)    left_gap = 0.0F;
            
            
            DrawBatcher b;
            b.batch_begin(  camera,
                _material,
                _shader,
                gui->draw_transform(),
                DT3GL_PRIM_TRI_STRIP,
                DrawBatcher::FMT_V | DrawBatcher::FMT_C | DrawBatcher::FMT_T0);
            
            b.add().v(rect.minus_x() + rect.width() * left_gap,            rect.minus_y(),            0.0F)   .t0(0.0F,1.0F)  .c(c);
            b.add().v(rect.minus_x() + rect.width() * left_gap,            rect.minus_y() - V_SIZE,   0.0F)   .t0(0.0F,0.0F)  .c(c);
            b.add().v(rect.minus_x() + rect.width() * left_gap + H_SIZE,   rect.minus_y(),            0.0F)   .t0(0.5F,1.0F)  .c(c);
            b.add().v(rect.minus_x() + rect.width() * left_gap + H_SIZE,   rect.minus_y() - V_SIZE,   0.0F)   .t0(0.5F,0.0F)  .c(c);

            b.add().v(rect.minus_x() + rect.width() * right_gap - H_SIZE,  rect.minus_y(),            0.0F)   .t0(0.5F,1.0F)  .c(c);
            b.add().v(rect.minus_x() + rect.width() * right_gap,           rect.minus_y(),            0.0F)   .t0(1.0F,1.0F)  .c(c);
            b.add().v(rect.minus_x() + rect.width() * right_gap - H_SIZE,  rect.minus_y() - V_SIZE,   0.0F)   .t0(0.5F,0.0F)  .c(c);
            b.add().v(rect.minus_x() + rect.width() * right_gap,           rect.minus_y() - V_SIZE,   0.0F)   .t0(1.0F,0.0F)  .c(c);
            
            b.batch_end();
            b.flush();
        }
    }
        
}

//==============================================================================
//==============================================================================

void ComponentGUIScroller::hit_test (GUITouchEvent *event, GUIObject* &hit_object, const Color4f &parent_color)
{   
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
        
    if (gui->color().a_as_float() * parent_color.a_as_float() <= 0.0F)
        return;

    Color4f c = gui->color() * parent_color;

    Vector2 object_pos = gui->position_to_object_coord (event->position());
    if (gui->rectangle().is_touching(object_pos)) {
        
        hit_object = gui;

        std::list<PlaceableObject*> children = gui->children();
        
        for (auto &i : children) {
            GUIObject *gui_child = checked_cast<GUIObject*>(i);
            if (!gui_child)
                continue;
                
            gui_child->hit_test(event, _focused, c);
            if (_focused)
                break;
            
        }   

    }
}

void ComponentGUIScroller::touches_began (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
    
    if (_focused)
        _focused->touches_began(event);
    
    // Do scroll
    if (event->focused() == gui) {
        _vel_x = 0.0F;
        _vel_y = 0.0F;
    }
    
}

void ComponentGUIScroller::touches_moved (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
        
    if (event->position() == event->previous_position())
        return;
    
    // If the touch is associated with another gui object
    if (_focused) {
        
        if ( (event->position() - event->initial_position()).abs() > 10.0F ) {
            event->set_focused(gui); // Scroller steals focus
            
            // If the scroller was forwarding touch events to another widget
            _focused->touches_cancelled(event);
            _focused = NULL;
        }
        
    }
    
    // If dragging to rearrange
    if (!_focused) {
        
        // Do scroll
        if (event->focused() == gui) {
            // Handle scrolling
            _vel_x = 0.0F;
            _vel_y = 0.0F;

            // Transform touches into widget coordinates
            Vector2 delta = gui->velocity_to_object_coord(event->position() - event->previous_position());
            gui->scroll_contents (delta.x * _scroll_horz, delta.y * _scroll_vert);
            _contents.offset(Vector2(delta.x * _scroll_horz, delta.y * _scroll_vert));
            
            _focused = NULL;
        }
        
        
    }

    
}

void ComponentGUIScroller::touches_ended (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
        
    if (_focused) {
        _focused->touches_ended(event);
        _focused = NULL;
    }
    
    // Do scroll
    if (event->focused() == gui) {
        Vector2 vel = gui->velocity_to_object_coord(event->velocity());
                    
        // Transform touches into widget coordinates
        _vel_x = vel.x * _scroll_horz;
        _vel_y = vel.y * _scroll_vert;
    }

}

void ComponentGUIScroller::touches_cancelled (GUITouchEvent *event)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;

}

//==============================================================================
//==============================================================================

} // DT3


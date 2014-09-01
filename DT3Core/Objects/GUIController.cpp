//==============================================================================
///	
///	File: GUIController.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/GUIController.hpp"
#include "DT3Core/Objects/GUIObject.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with gui factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(GUIController,"GUI","EdPlaceableObjectAdapter")
IMPLEMENT_PLUG_NODE(GUIController)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(GUIController)
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUIController::GUIController (void)
    :   _use_stencil    (true)
{

}
		
GUIController::GUIController (const GUIController &rhs)
    :   PlaceableObject (rhs),
        _use_stencil    (rhs._use_stencil)
{

}

GUIController & GUIController::operator = (const GUIController &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		PlaceableObject::operator = (rhs);
    }
    return (*this);
}
			
GUIController::~GUIController (void)
{

}

//==============================================================================
//==============================================================================

void GUIController::archive (const std::shared_ptr<Archive> &archive)
{
    PlaceableObject::archive(archive);
    
    archive->push_domain (class_id());
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void GUIController::initialize (void)
{
	PlaceableObject::initialize();
    
    _gui_camera = CameraObject::create();
    _gui_camera->set_ortho(0.0F,1.0F,0.0F,1.0F,1000.0F,-1000.0F);
}

//==============================================================================
//==============================================================================

namespace {
    
    struct CompareDraw {
        DTint operator() (PlaceableObject *a, PlaceableObject *b) const {
          return (a->translation().z < b->translation().z);
        }
    };

    struct CompareTouch {
        DTint operator() (PlaceableObject *a, PlaceableObject *b) const {
          return (a->translation().z > b->translation().z);
        }
    };

};

//==============================================================================
//==============================================================================

void GUIController::touch_gui (const TouchEvent *event, const std::shared_ptr<CameraObject> &camera, WorldNode* &handled)
{        
    if (handled)
        return;

    // Scan for first event
    for (DTuint i = 0; i < TouchEvent::MAX_NUM_TOUCHES; ++i) {
        if ( event->touches[i].state != TouchEvent::STATE_NONE ) {
            _touch.set_initial_position(event->touches[i].first_pos);
            _touch.set_previous_position(event->touches[i].previous_pos);
            _touch.set_position(event->touches[i].pos);
            _touch.set_velocity(event->touches[i].velocity);
            _touch.set_state(event->touches[i].state);

            break;
        }
    }
    
    // If this is a new event, then we find the target widget
    if (_touch.state() == TouchEvent::STATE_PRESSED) {
        
        // Find first hit object
        std::list<PlaceableObject*> c = children();
        
        c.sort(CompareTouch());
        
        for (auto i = c.rbegin(); i != c.rend(); ++i) {
            GUIObject *gui = checked_cast<GUIObject*>(*i);
            if (!gui)
                continue;
        
            GUIObject *hit_object = NULL;
            gui->hit_test(&_touch, hit_object, Color4f(1.0F,1.0F,1.0F,1.0F));
            
            if (hit_object) {
                _touch.set_focused(hit_object);
                break;
            }

        }
        
        // Start touches
        if (_touch.focused())
            _touch.focused()->touches_began(&_touch);

        handled = _touch.focused();
            
    } else if (_touch.state() == TouchEvent::STATE_DOWN) {
    
        if (_touch.focused())
            _touch.focused()->touches_moved(&_touch);

        handled = _touch.focused();

    } else if (_touch.state() == TouchEvent::STATE_RELEASED) {
    
        if (_touch.focused())
            _touch.focused()->touches_ended(&_touch);
 
        handled = _touch.focused();

        // Reset touch
        _touch.set_state(TouchEvent::STATE_NONE);
        _touch.set_focused(NULL);
    } else if (_touch.state() == TouchEvent::STATE_HOVER) {
    
        // Do nothing

    } else {
        handled = _touch.focused();
    }
    
}

void GUIController::draw_gui_recursive (PlaceableObject *root, const std::shared_ptr<CameraObject> &camera, const Color4f &parent_color, DTint stencil)
{
    GUIObject *gui = checked_cast<GUIObject*>(root);
    Rectangle gui_rect(0.0F,1.0F,0.0F,1.0F);
    DTboolean use_stencil = false;

    std::list<PlaceableObject*> c = root->children();

    if (gui) {

        // Check for transparent
        Color4f color = parent_color * gui->color();
        if (color.a <= 0.0F)
            return;

        // Check for no scale
        if (gui->scale() == Vector3(0.0F,0.0F,0.0F))
            return;

        // Get Rectangle
        gui_rect = gui->screen_rectangle();

        // Check if we need to stencil
        use_stencil = c.size() > 0;

        if (use_stencil) {
            ++stencil;
            gui->draw_gui_mask(camera);
        }

        System::renderer()->set_stencil_ref(stencil);
        gui->draw_gui(camera, parent_color);
    }


    c.sort(CompareDraw());
    
    for (auto &i : c) {
        GUIObject *gui_child = checked_cast<GUIObject*>(i);

        if (gui_child) {
        
            // Get Rectangle
            Rectangle gui_child_rect = gui_child->screen_rectangle();

            if (!gui_rect.is_touching(gui_child_rect))
                continue;

            draw_gui_recursive(gui_child, camera, parent_color * gui_child->color(), stencil );
        } else {
            draw_gui_recursive(i, camera, parent_color, stencil);
        }
    }

    if (gui) {
        System::renderer()->set_stencil_ref(stencil);
        gui->post_draw_gui(camera, parent_color);

        if (use_stencil) {
            gui->draw_gui_unmask(camera);
        }
        
    }
}

void GUIController::draw_gui (const std::shared_ptr<CameraObject> &camera, const DTfloat lag)
{
#ifdef DT3_EDITOR
    if (camera->is_picking())
        return;
#endif
    
    System::renderer()->clear_viewport(false,false,true);

    draw_gui_recursive(this, _gui_camera, Color4f(1.0F,1.0F,1.0F,1.0F), 0);    // GUIController goes first
}

//==============================================================================
//==============================================================================

void GUIController::set_focus (GUIObject *new_focus)
{
    // Cancel touches
    if (_touch.focused())
        _touch.focused()->touches_cancelled(&_touch);

    _touch.set_focused(new_focus);

    // Start touches
    if (_touch.focused())
        _touch.focused()->touches_began(&_touch);
}

//==============================================================================
//==============================================================================

void GUIController::add_to_world(World *world)
{
    PlaceableObject::add_to_world(world);
    
    world->register_for_touch(this, make_callback(this, &GUIController::touch_gui));
    world->register_for_draw(this, make_callback(this, &type::draw_gui));
}

void GUIController::remove_from_world (void)
{
    world()->unregister_for_touch(this, make_callback(this, &GUIController::touch_gui));
    world()->unregister_for_draw(this, make_callback(this, &type::draw_gui));

    PlaceableObject::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


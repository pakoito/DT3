
//==============================================================================
///	
///	File: GUIObject.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/GUIObject.hpp"
#include "DT3Core/Objects/GUIController.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/GUI/GUIAnimKey.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/World/World.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(GUIObject,"GUI","EdManipGUIObject")
IMPLEMENT_PLUG_NODE(GUIObject)

IMPLEMENT_PLUG_INFO_INDEX(_width)
IMPLEMENT_PLUG_INFO_INDEX(_height)
IMPLEMENT_PLUG_INFO_INDEX(_color)
IMPLEMENT_PLUG_INFO_INDEX(_label)
IMPLEMENT_PLUG_INFO_INDEX(_skin)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(GUIObject)        

    PLUG_INIT(_width, "Width")
		.set_input(true)
		.set_output(true);

    PLUG_INIT(_height, "Height")
		.set_input(true)
		.set_output(true);
        
    PLUG_INIT(_color, "Color")
		.set_input(true)
		.set_output(true);
        
    PLUG_INIT(_label, "Label")
		.set_input(true)
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUIObject::GUIObject (void)
    :   _width          (PLUG_INFO_INDEX(_width), 0.1F),
        _height         (PLUG_INFO_INDEX(_height), 0.1F),
        _size_mode      (SIZE_MODE_NO_CHANGE),
        _pivot          (PIVOT_CENTER),
        _no_focus       (false),
        _color          (PLUG_INFO_INDEX(_color), Color4f(1.0F,1.0F,1.0F,1.0F)),
        _label          (PLUG_INFO_INDEX(_label), ""),
        _state          (STATE_NORMAL)
{
    
}
		
GUIObject::GUIObject (const GUIObject &rhs)
	:	PlaceableObject      (rhs),
        _width          (rhs._width),
        _height         (rhs._height),
        _size_mode      (rhs._size_mode),
        _pivot          (rhs._pivot),
        _no_focus       (rhs._no_focus),
        _color          (rhs._color),
        _label          (rhs._label),
        _state          (rhs._state)
{

}

GUIObject & GUIObject::operator = (const GUIObject &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		PlaceableObject::operator = (rhs);

        _width = rhs._width;
        _height = rhs._height;
        _size_mode = rhs._size_mode;
        _pivot = rhs._pivot;
        _no_focus = rhs._no_focus;
        _color = rhs._color;
        _label = rhs._label;
        _state = rhs._state;
    }
    return (*this);
}
			
GUIObject::~GUIObject (void)
{

}

//==============================================================================
//==============================================================================

void GUIObject::archive (const std::shared_ptr<Archive> &archive)
{
    PlaceableObject::archive(archive);

    archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_width, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_height, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Size_Mode", GUIObject::size_mode, GUIObject::set_size_mode, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("No Change")
        .add_enum("Width Constant")
        .add_enum("Height Constant")
        .add_enum("Pixels");
    *archive << ARCHIVE_DATA_ACCESSORS("Pivot", GUIObject::pivot, GUIObject::set_pivot, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Center")
        .add_enum("Top Left")
        .add_enum("Top Right")
        .add_enum("Bottom Left")
        .add_enum("Bottom Right");
        
	*archive << ARCHIVE_PLUG(_color, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_label, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_no_focus, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void GUIObject::initialize (void)
{
	PlaceableObject::initialize();
    
	_shader = ShaderResource::import_resource(FilePath("{SYS_MASK_SHADER}"));

    _mask_material = MaterialResource::create();
    _mask_material->set_blend_enable(false);
    _mask_material->set_depth_enable(false);
    _mask_material->set_depth_write_mask(false);
    _mask_material->set_cull_mode(DT3GL_CULL_NONE);
    _mask_material->set_render_target_write_mask_r(false);
    _mask_material->set_render_target_write_mask_g(false);
    _mask_material->set_render_target_write_mask_b(false);
    _mask_material->set_render_target_write_mask_a(false);
    _mask_material->set_stencil_enable(true);
    _mask_material->set_stencil_write_mask(0xFFFFFFFF);
    _mask_material->set_stencil_read_mask(0xFFFFFFFF);
    _mask_material->set_front_face_stencil_fail_op(DT3GL_STENCILOP_INCR);
    _mask_material->set_front_face_stencil_depth_fail_op(DT3GL_STENCILOP_KEEP);
    _mask_material->set_front_face_stencil_pass_op(DT3GL_STENCILOP_KEEP);
    _mask_material->set_front_face_stencil_func(DT3GL_COMPARE_NEVER);
    _mask_material->set_back_face_stencil_fail_op(DT3GL_STENCILOP_INCR);
    _mask_material->set_back_face_stencil_depth_fail_op(DT3GL_STENCILOP_KEEP);
    _mask_material->set_back_face_stencil_pass_op(DT3GL_STENCILOP_KEEP);
    _mask_material->set_front_face_stencil_func(DT3GL_COMPARE_NEVER);

    _unmask_material = MaterialResource::create();
    _unmask_material->set_blend_enable(false);
    _unmask_material->set_depth_enable(false);
    _unmask_material->set_depth_write_mask(false);
    _unmask_material->set_cull_mode(DT3GL_CULL_NONE);
    _unmask_material->set_render_target_write_mask_r(false);
    _unmask_material->set_render_target_write_mask_g(false);
    _unmask_material->set_render_target_write_mask_b(false);
    _unmask_material->set_render_target_write_mask_a(false);
    _unmask_material->set_stencil_enable(true);
    _unmask_material->set_stencil_write_mask(0xFFFFFFFF);
    _unmask_material->set_stencil_read_mask(0xFFFFFFFF);
    _unmask_material->set_front_face_stencil_fail_op(DT3GL_STENCILOP_DECR);
    _unmask_material->set_front_face_stencil_depth_fail_op(DT3GL_STENCILOP_KEEP);
    _unmask_material->set_front_face_stencil_pass_op(DT3GL_STENCILOP_KEEP);
    _unmask_material->set_front_face_stencil_func(DT3GL_COMPARE_NEVER);
    _unmask_material->set_back_face_stencil_fail_op(DT3GL_STENCILOP_DECR);
    _unmask_material->set_back_face_stencil_depth_fail_op(DT3GL_STENCILOP_KEEP);
    _unmask_material->set_back_face_stencil_pass_op(DT3GL_STENCILOP_KEEP);
    _unmask_material->set_front_face_stencil_func(DT3GL_COMPARE_NEVER);

}

//==============================================================================
//==============================================================================

GUIController* GUIObject::controller (void)
{
    PlaceableObject *cur = checked_static_cast<PlaceableObject*>(this);
    
    while (cur && !cur->isa(GUIController::kind())) {
        cur = cur->parent();
    }
    
    // Check result
    if (cur->isa(GUIController::kind()))
        return checked_static_cast<GUIController*>(cur);
    else   
        return NULL;
}

//==============================================================================
//==============================================================================

void GUIObject::draw_gui_mask (const std::shared_ptr<CameraObject> &camera)
{
    _b.batch_begin( camera,
                    _mask_material,
                    _shader,
                    draw_transform(),
                    DT3GL_PRIM_TRI_STRIP,
                    DrawBatcher::FMT_V,
                    4);
    
    Rectangle rect = rectangle();
    
    _b.add().v(rect.plus_x(),rect.plus_y(),0.0F);
    _b.add().v(rect.minus_x(),rect.plus_y(),0.0F);
    _b.add().v(rect.plus_x(),rect.minus_y(),0.0F);
    _b.add().v(rect.minus_x(),rect.minus_y(),0.0F);
    
    _b.batch_end();
    _b.flush();
}

void GUIObject::draw_gui_unmask (const std::shared_ptr<CameraObject> &camera)
{
    _b.batch_begin( camera,
                    _unmask_material,
                    _shader,
                    draw_transform(),
                    DT3GL_PRIM_TRI_STRIP,
                    DrawBatcher::FMT_V,
                    4);
    
    Rectangle rect = rectangle();
    
    _b.add().v(rect.plus_x(),rect.plus_y(),0.0F);
    _b.add().v(rect.minus_x(),rect.plus_y(),0.0F);
    _b.add().v(rect.plus_x(),rect.minus_y(),0.0F);
    _b.add().v(rect.minus_x(),rect.minus_y(),0.0F);
    
    _b.batch_end();
    _b.flush();
}

void GUIObject::draw_gui (const std::shared_ptr<CameraObject> &camera, const Color4f& parent_color)
{
    _draw_gui_callbacks.fire(camera, parent_color);
}

void GUIObject::post_draw_gui (const std::shared_ptr<CameraObject> &camera, const Color4f& parent_color)
{
    _post_draw_gui_callbacks.fire(camera, parent_color);
}

//==============================================================================
//==============================================================================

void GUIObject::set_bounds (DTfloat left, DTfloat bottom, DTfloat width, DTfloat height, DTfloat z)
{
    set_translation_local(Vector3(left + width/2.0F, bottom + height/2.0F, z));
    set_width(width);
    set_height(height);
}

//==============================================================================
//==============================================================================

Vector2 GUIObject::position_to_object_coord  (Vector2 pixel_pos)
{
    // Transform position into widget coordinates
    DTfloat x = (DTfloat) pixel_pos.x / System::renderer()->screen_width();
    DTfloat y = 1.0F - (DTfloat) pixel_pos.y / System::renderer()->screen_height();
    
    Vector2 pos = draw_transform().inversed() * Vector2(x,y);
    return pos;
}

Vector2 GUIObject::velocity_to_object_coord  (Vector2 pixel_velocity)
{
    // Transform position into widget coordinates
    DTfloat dx = (DTfloat) pixel_velocity.x / System::renderer()->screen_width();
    DTfloat dy = -(DTfloat) pixel_velocity.y / System::renderer()->screen_height();
    
    Vector2 vel = draw_transform().orientation().inversed() * Vector2(dx,dy);
    return vel;
}

//==============================================================================
//==============================================================================

Rectangle GUIObject::rectangle (void) const
{
    DTfloat half_width = _width/2.0F;
    DTfloat half_height = _height/2.0F;
    
    Rectangle r;
    r.set (-half_width, half_width, -half_height, half_height);
    return r;
}

Rectangle GUIObject::screen_rectangle (void) const
{
    Rectangle rect = rectangle();
    Matrix4 transform = draw_transform();

    Vector2 p0 = transform * Vector2(rect.minus_x(), rect.minus_y());
    Vector2 p1 = transform * Vector2(rect.plus_x(), rect.minus_y());
    Vector2 p2 = transform * Vector2(rect.minus_x(), rect.plus_y());
    Vector2 p3 = transform * Vector2(rect.plus_x(), rect.plus_y());

    Rectangle bounds;
    bounds.set_minus_x( MoreMath::min(p0.x, p1.x, p2.x, p3.x) );
    bounds.set_plus_x( MoreMath::max(p0.x, p1.x, p2.x, p3.x) );
    bounds.set_minus_y( MoreMath::min(p0.y, p1.y, p2.y, p3.y) );
    bounds.set_plus_y( MoreMath::max(p0.y, p1.y, p2.y, p3.y) );

    return bounds;
}

Matrix4 GUIObject::draw_transform (void) const
{
    DTfloat aspect = System::renderer()->screen_aspect();
    
    DTfloat half_width = _width/2.0F;
    DTfloat half_height = _height/2.0F;
    
    
    //
    // Calculate offset
    // 
    
    Matrix4 offset;
    switch (_pivot) {
        case PIVOT_TOP_LEFT:
            offset = Matrix4(Vector3(half_width,-half_height,0.0F));
            break;
        case PIVOT_TOP_RIGHT:
            offset = Matrix4(Vector3(-half_width,-half_height,0.0F));
            break;
        case PIVOT_BOTTOM_LEFT:
            offset = Matrix4(Vector3(half_width,half_height,0.0F));
            break;
        case PIVOT_BOTTOM_RIGHT:
            offset = Matrix4(Vector3(-half_width,half_height,0.0F));
            break;
        default:
            offset = Matrix4::identity();
            break;
    };

    //
    // Calculate size
    //

    switch (_size_mode) {
    
        case SIZE_MODE_NO_CHANGE: {
                return transform();
            } break;
        
        case SIZE_MODE_WIDTH_CONSTANT: {
                Matrix4 t = transform();
                Vector3 translation = t.translation();
            
                Matrix3 distortion( 1.0F,   0.0F,   0.0F,
                                    0.0F,   aspect, 0.0F,
                                    0.0F,   0.0F,   1.0F    );
            
                return Matrix4(distortion * t.orientation(), translation, t.scale()) * offset;
            } break;

        case SIZE_MODE_HEIGHT_CONSTANT: {
                Matrix4 t = transform();
                Vector3 translation = t.translation();
                
                Matrix3 distortion( 1.0F/aspect,0.0F,   0.0F,
                                    0.0F,       1.0F,   0.0F,
                                    0.0F,       0.0F,   1.0F    );
            
                return Matrix4(distortion * t.orientation(), translation, t.scale()) * offset;
            } break;

        case SIZE_MODE_PIXELS: {
                Matrix4 t = transform();
                Vector3 translation = t.translation();

                DTfloat width = 1.0F / System::renderer()->screen_width();
                DTfloat height = 1.0F / System::renderer()->screen_height();
                
                Matrix3 distortion( width,  0.0F,   0.0F,
                                    0.0F,   height, 0.0F,
                                    0.0F,   0.0F,   1.0F    );
            
                return Matrix4(distortion * t.orientation(), translation, t.scale()) * offset;

            } break;

        default:
            return transform() * offset;
            break;

    };
    
}

//==============================================================================
//==============================================================================

Rectangle GUIObject::content_rectangle (void) const
{
    Rectangle r(rectangle());
    r = Rectangle::calc_union(children_content_rectangle(), r);
    return r;
}


Rectangle GUIObject::children_content_rectangle (void) const
{
    std::list<PlaceableObject*> c = children();

    if (c.size() == 0) {
        return rectangle();
    } else {
        
        Matrix4 transform = draw_transform().inversed();
        Rectangle r;
        r.clear();
        
        DTuint num = 0;
        
        for (auto &i : c) {
            GUIObject *gui = checked_cast<GUIObject*>(i);
            if (!gui)
                continue;
                
            if (gui->color().r_as_float() <= 0.0F)
                continue;
        
            Rectangle child_rect = gui->content_rectangle();
            Matrix4 gui_transform = transform * gui->draw_transform();
        
            // 4 corners
            Vector2 p0 = Vector2( gui_transform * Vector3(child_rect.minus_x(), child_rect.plus_y(), 0.0F));
            Vector2 p1 = Vector2( gui_transform * Vector3(child_rect.plus_x(), child_rect.plus_y(), 0.0F));
            Vector2 p2 = Vector2( gui_transform * Vector3(child_rect.minus_x(), child_rect.minus_y(), 0.0F));
            Vector2 p3 = Vector2( gui_transform * Vector3(child_rect.plus_x(), child_rect.minus_y(), 0.0F));
            
            r = Rectangle::calc_union(r, p0);
            r = Rectangle::calc_union(r, p1);
            r = Rectangle::calc_union(r, p2);
            r = Rectangle::calc_union(r, p3);
            
            ++num;
        }
        
        if (num == 0) {
            r.set(0.0F,0.0F,0.0F,0.0F);
        }
    
        return r;
    }
}

//==============================================================================
//==============================================================================

void GUIObject::scroll_contents (DTfloat dx, DTfloat dy)
{
    std::list<PlaceableObject*> c = children();
    
    for (auto &i : c)
        i->set_translation( i->translation() + Vector3(dx, dy, 0.0F) );
}

DTboolean GUIObject::is_contents_animating (void) const
{
    if (has_anim_key())
        return true;
 
    std::list<PlaceableObject*> c = children();

    for (auto &i : c) {
        GUIObject *gui = checked_cast<GUIObject*>(i);
        if (!gui)
            continue;
    
        if ( gui->is_contents_animating() )
            return true;
    }
            
    return false;
}

//==============================================================================
//==============================================================================

void GUIObject::animate (DTfloat dt)
{
	// If there are animation pending
	if (_anim_keys.size() > 0) {
	
		GUIAnimKey &front_key = _anim_keys.front();
		
		// Animate the frame
		if (!front_key.animate(dt,this)) {

			// Resubmit keyframe if repeated
			if (front_key.is_repeat())
				_anim_keys.push_back(front_key);

			// Load in the next key frame
			_anim_keys.pop_front();
			
			// Cache current state for next keyframe
			if (_anim_keys.size() > 0) {
				GUIAnimKey &next_front_key = _anim_keys.front();
				next_front_key.cache_frame_state(this);
			} else {
                // Stop getting ticks
                world()->unregister_for_tick(this, make_callback(this, &type::animate));
            }
		}
	}
}

GUIAnimKey& GUIObject::add_anim_key (void)
{
	if (_anim_keys.size() == 0) {

        // Start getting ticks
        world()->register_for_tick(this, make_callback(this, &type::animate));
    }

	GUIAnimKey new_key;
	new_key.cache_frame_state(this);
    _anim_keys.push_back(new_key);
	
	return _anim_keys.back();
}

void GUIObject::add_anim_key (const GUIAnimKey& key)
{
    GUIAnimKey &new_key = add_anim_key();
    new_key = key;
    new_key.cache_frame_state(this);  // recache
}

void GUIObject::cancel_repeating_anims (void)
{
	for (auto &i : _anim_keys) {
		i.set_repeat(false);
	}
}

//==============================================================================
//==============================================================================

void GUIObject::clear_anims (void)
{	    
	for (auto i = _anim_keys.begin(); i != _anim_keys.end();) {
    
        // Skip if keyframe is running. This can be called in a callback and bad stuff would happen
        if (i->is_busy())   ++i;
        else                i = _anim_keys.erase(i);
            
    }
    
}

//==============================================================================
//==============================================================================

namespace {
    
    struct CompareTouch {
        DTint operator() (PlaceableObject *a, PlaceableObject *b) const {
          return (a->translation().z > b->translation().z);
        }
    };

};


//==============================================================================
//==============================================================================

void GUIObject::hit_test (GUITouchEvent *event, GUIObject* &hit_object, const Color4f &parent_color)
{
    Vector3 s = scale();
    if (s.x == 0.0F || s.y == 0.0F || s.z == 0.0F)
        return;

    if (_no_focus)
        return;

    Color4f c = parent_color * color();

    if (_hit_object_callbacks.size() == 0) {
    
        if (c.a_as_float() == 0)
            return;
    
        Vector2 object_pos = position_to_object_coord (event->position());
        if (rectangle().is_touching(object_pos)) {
            
            std::list<PlaceableObject*> cc = children();
            
            cc.sort(CompareTouch());

            for (auto &i : cc) {
                GUIObject *gui = checked_cast<GUIObject*>(i);
                if (!gui)
                    continue;
                    
                gui->hit_test(event, hit_object, c);
                if (hit_object)
                    return;
            }   

            hit_object = this;
        }
     
    } else {
        _hit_object_callbacks.fire(event, hit_object, c);
    }
}
        
void GUIObject::touches_began (GUITouchEvent *event)
{
    _touches_began_callbacks.fire(event);
}

void GUIObject::touches_moved (GUITouchEvent *event)
{
    _touches_moved_callbacks.fire(event);
}

void GUIObject::touches_ended (GUITouchEvent *event)
{
    _touches_ended_callbacks.fire(event);
}

void GUIObject::touches_cancelled (GUITouchEvent *event)
{
    _touches_cancelled_callbacks.fire(event);
}

//==============================================================================
//==============================================================================

void GUIObject::add_to_world(World *world)
{
    PlaceableObject::add_to_world(world);
}

void GUIObject::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::animate));

    PlaceableObject::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


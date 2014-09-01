//==============================================================================
///	
///	File: PlaceableObject.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(PlaceableObject,"Object","EdPlaceableObjectAdapter")
IMPLEMENT_PLUG_NODE(PlaceableObject)

IMPLEMENT_PLUG_INFO_INDEX(_radius)
IMPLEMENT_PLUG_INFO_INDEX(_velocity)
IMPLEMENT_PLUG_INFO_INDEX(_angular_velocity)
IMPLEMENT_PLUG_INFO_INDEX(_translation)
IMPLEMENT_PLUG_INFO_INDEX(_orientation)
IMPLEMENT_PLUG_INFO_INDEX(_scale)
IMPLEMENT_PLUG_INFO_INDEX(_transform)
IMPLEMENT_PLUG_INFO_INDEX(_transform_inversed)
IMPLEMENT_PLUG_INFO_INDEX(_world_translation)
IMPLEMENT_PLUG_INFO_INDEX(_world_orientation)
IMPLEMENT_PLUG_INFO_INDEX(_world_scale)
IMPLEMENT_PLUG_INFO_INDEX(_world_transform)
IMPLEMENT_PLUG_INFO_INDEX(_world_transform_inversed)
IMPLEMENT_PLUG_INFO_INDEX(_parent_transform)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(PlaceableObject)

    PLUG_INIT(_radius, "Radius")
		.set_input(true)
		.set_output(true);
		
    PLUG_INIT(_velocity, "Velocity")
		.set_input(true)
		.set_output(true);

    PLUG_INIT(_angular_velocity, "Angular_Velocity")
		.set_input(true)
		.set_output(true);
		
		
    PLUG_INIT(_translation, "Translation")
		.affects(PLUG_INFO_INDEX(_world_translation))
		.affects(PLUG_INFO_INDEX(_world_orientation))
		.affects(PLUG_INFO_INDEX(_world_scale))
		.affects(PLUG_INFO_INDEX(_world_transform))
		.affects(PLUG_INFO_INDEX(_world_transform_inversed))
		.affects(PLUG_INFO_INDEX(_transform))
		.affects(PLUG_INFO_INDEX(_transform_inversed))
		.set_input(true)
		.set_output(true);
		
    PLUG_INIT(_orientation, "Orientation")
		.affects(PLUG_INFO_INDEX(_world_translation))
		.affects(PLUG_INFO_INDEX(_world_orientation))
		.affects(PLUG_INFO_INDEX(_world_scale))
		.affects(PLUG_INFO_INDEX(_world_transform))
		.affects(PLUG_INFO_INDEX(_world_transform_inversed))
		.affects(PLUG_INFO_INDEX(_transform))
		.affects(PLUG_INFO_INDEX(_transform_inversed))
		.set_input(true)
		.set_output(true);
		
    PLUG_INIT(_scale, "Scale")
		.affects(PLUG_INFO_INDEX(_world_translation))
		.affects(PLUG_INFO_INDEX(_world_orientation))
		.affects(PLUG_INFO_INDEX(_world_scale))
		.affects(PLUG_INFO_INDEX(_world_transform))
		.affects(PLUG_INFO_INDEX(_world_transform_inversed))
		.affects(PLUG_INFO_INDEX(_transform))
		.affects(PLUG_INFO_INDEX(_transform_inversed))
		.set_input(true)
		.set_output(true);

    PLUG_INIT(_transform, "Transform")
		.set_output(true);

    PLUG_INIT(_transform_inversed, "Transform_Inversed")
		.set_output(true);


    PLUG_INIT(_world_translation, "World_Translation")
		.set_output(true);
		
    PLUG_INIT(_world_orientation, "World_Orientation")
		.set_output(true);
		
    PLUG_INIT(_world_scale, "World_Scale")
		.set_output(true);

    PLUG_INIT(_world_transform, "World_Transform")
		.set_output(true);
        
    PLUG_INIT(_world_transform_inversed, "World_Transform_Inversed")
		.set_output(true);
        

    PLUG_INIT(_parent_transform, "Parent_Transform")
		.affects(PLUG_INFO_INDEX(_world_translation))
		.affects(PLUG_INFO_INDEX(_world_orientation))
		.affects(PLUG_INFO_INDEX(_world_scale))
		.affects(PLUG_INFO_INDEX(_world_transform))
		.affects(PLUG_INFO_INDEX(_world_transform_inversed))
		.affects(PLUG_INFO_INDEX(_transform))
		.affects(PLUG_INFO_INDEX(_transform_inversed))
		.set_no_draw(true)
		.set_input(true);
		
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

PlaceableObject::PlaceableObject (void)
    :  	_pickable				(true),
		_world_translation		(PLUG_INFO_INDEX(_world_translation), Vector3(0.0F,0.0F,0.0F)),
        _world_orientation		(PLUG_INFO_INDEX(_world_orientation), Matrix3::identity()),
		_world_scale			(PLUG_INFO_INDEX(_world_scale), Vector3(1.0F,1.0F,1.0F)),
		_world_transform		(PLUG_INFO_INDEX(_world_transform), Matrix4::identity()),
		_world_transform_inversed(PLUG_INFO_INDEX(_world_transform_inversed), Matrix4::identity()),
		_radius					(PLUG_INFO_INDEX(_radius), 1.0F),
        _velocity				(PLUG_INFO_INDEX(_velocity), Vector3(0.0F,0.0F,0.0F)),
        _angular_velocity		(PLUG_INFO_INDEX(_angular_velocity), Vector3(0.0F,0.0F,0.0F)),
		_translation			(PLUG_INFO_INDEX(_translation), Vector3(0.0F,0.0F,0.0F)),
        _orientation			(PLUG_INFO_INDEX(_orientation), Matrix3::identity()),
		_scale                  (PLUG_INFO_INDEX(_scale), Vector3(1.0F,1.0F,1.0F)),
		_transform              (PLUG_INFO_INDEX(_transform), Matrix4::identity()),
		_transform_inversed     (PLUG_INFO_INDEX(_transform_inversed), Matrix4::identity()),
        _parent_transform		(PLUG_INFO_INDEX(_parent_transform), Matrix4::identity())
{  

}
		
PlaceableObject::PlaceableObject (const PlaceableObject &rhs)
    :   ObjectBase				(rhs),
		_pickable				(rhs._pickable),
		_world_translation		(PLUG_INFO_INDEX(_world_translation), Vector3(0.0F,0.0F,0.0F)),
        _world_orientation		(PLUG_INFO_INDEX(_world_orientation), Matrix3::identity()),
		_world_scale			(PLUG_INFO_INDEX(_world_scale), Vector3(1.0F,1.0F,1.0F)),
        _world_transform		(rhs._world_transform),
        _world_transform_inversed(rhs._world_transform_inversed),
		_radius					(rhs._radius),
		_velocity				(rhs._velocity),
		_angular_velocity		(rhs._angular_velocity),
		_translation			(rhs._translation),
        _orientation			(rhs._orientation),
 		_scale                  (rhs._scale),
 		_transform              (rhs._transform),
 		_transform_inversed     (rhs._transform_inversed),
        _parent_transform		(PLUG_INFO_INDEX(_parent_transform), Matrix4::identity())
{   

}

PlaceableObject & PlaceableObject::operator = (const PlaceableObject &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) { 
		ObjectBase::operator = (rhs);
		
		_pickable = rhs._pickable;

		_radius = rhs._radius;
		_velocity = rhs._velocity;
		_angular_velocity = rhs._angular_velocity;
		
		_translation = rhs._translation;
        _orientation = rhs._orientation;
        _scale = rhs._scale;

 		_transform = rhs._transform;
 		_transform_inversed = rhs._transform_inversed;

		_world_transform = rhs._world_transform;
        _world_transform_inversed = rhs._world_transform_inversed;
		
		// reset parent transforms
		_parent_transform = Matrix4::identity();

		// Remove all children
		unparent_children();

    }
    return (*this);
}
			
PlaceableObject::~PlaceableObject (void)
{

}

//==============================================================================
//==============================================================================

void PlaceableObject::initialize (void)
{
	ObjectBase::initialize();
}

void PlaceableObject::uninitialize (void)
{
	ObjectBase::uninitialize();

	unparent_children();
}

//==============================================================================
//==============================================================================

void PlaceableObject::archive (const std::shared_ptr<Archive> &archive)
{
    ObjectBase::archive(archive);

	archive->push_domain (class_id ());
        	
	*archive << ARCHIVE_DATA_ACCESSORS("Pickable_In_Editor", PlaceableObject::is_pickable, PlaceableObject::set_pickable, DATA_PERSISTENT | DATA_SETTABLE);
	
	*archive << ARCHIVE_PLUG(_translation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_radius, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_orientation, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_scale, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_velocity, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_angular_velocity, DATA_PERSISTENT | DATA_SETTABLE);
			               								  
    archive->pop_domain ();
	
}

//==============================================================================
//==============================================================================

PlaceableObject* PlaceableObject::parent (void) const
{
	if (_parent_transform.has_incoming_connection())
		return checked_static_cast<PlaceableObject*>(_parent_transform.incoming_connection()->owner());
	else
		return NULL;
}

PlaceableObject* PlaceableObject::top_parent (void) const
{
	PlaceableObject *parent = const_cast<PlaceableObject*>(this);
	
	while (parent->parent()) {
		parent = parent->parent();
	}
	
	return parent;
}

void PlaceableObject::add_child (PlaceableObject *child)
{
	ASSERT(child);
    
    PlaceableObject *old_parent = child->parent();
	
	_world_transform.add_outgoing_connection(&child->_parent_transform);
    
    SystemCallbacks::reparent_node_cb().fire (world(), child, old_parent, this);
}

void PlaceableObject::add_child (const std::shared_ptr<PlaceableObject> &child)
{
    add_child(child.get());
}

void PlaceableObject::unparent_child (PlaceableObject *child)
{
	ASSERT(child);
	
    PlaceableObject *old_parent = child->parent();

	_world_transform.remove_outgoing_connection(&child->_parent_transform);

    SystemCallbacks::reparent_node_cb().fire (world(), child, old_parent, this);
}

void PlaceableObject::unparent_child (const std::shared_ptr<PlaceableObject> &child)
{
    unparent_child(child.get());
}

void PlaceableObject::unparent_children (void)
{
    std::list<PlaceableObject*> c = children();
    
    FOR_EACH (i,c)
        unparent_child(*i);
}

void PlaceableObject::remove_child (PlaceableObject *child)
{
	ASSERT(child);
	
    PlaceableObject *old_parent = child->parent();

	_world_transform.remove_outgoing_connection(&child->_parent_transform);

    SystemCallbacks::reparent_node_cb().fire (world(), child, old_parent, NULL);

    if (world())
        world()->remove_node(checked_cast<WorldNode>(child->shared_from_this()));
}

void PlaceableObject::remove_child (const std::shared_ptr<PlaceableObject> &child)
{
    remove_child(child.get());
}


void PlaceableObject::remove_children (void)
{
    std::list<PlaceableObject*> c = children();
    
    FOR_EACH (i,c) {
        remove_child(*i);
    }
}

std::list<PlaceableObject*> PlaceableObject::children (void) const
{
	std::list<PlaceableObject*> children;

	const std::vector<PlugBase*>& pos = _world_transform.outgoing_connections();
		
	FOR_EACH (pos_iter,pos) {
		if ( (**pos_iter).name() == "Parent_Transform" )
			children.push_back( checked_static_cast<PlaceableObject*>((**pos_iter).owner()) );
	}
	
	return children;
}

//==============================================================================
//==============================================================================

Vector3 PlaceableObject::scale (void) const
{
	if (has_parent()) {
		return _world_scale;
	} else {
		return _scale;
	}
}

void PlaceableObject::set_scale (const Vector3 &scale)
{
	if (has_parent()) {
		set_scale_local(scale / _parent_transform->scale());
	} else {
		set_scale_local(scale);
	}
}

Vector3 PlaceableObject::scale_local (void) const
{	
    return _scale;				
}

void PlaceableObject::set_scale_local (const Vector3 &scale)
{	
    _scale = scale;
}

//==============================================================================
//==============================================================================

const Vector3 & PlaceableObject::translation (void) const
{
	if (has_parent()) {
		return _world_translation.as_ref();
	} else {
		return _translation.as_ref();
	}
}

void PlaceableObject::set_translation (const Vector3 &translation)
{
	if (has_parent()) {
		set_translation_local(_parent_transform->inversed() * translation);
	} else {
		set_translation_local(translation);
	}
}

//==============================================================================
//==============================================================================

const Matrix3 & PlaceableObject::orientation (void) const
{
	if (has_parent()) {
		return _world_orientation.as_ref();
	} else {
		return _orientation.as_ref();
	}
}

void PlaceableObject::set_orientation (const Matrix3 &orientation)
{
	if (has_parent()) {
		set_orientation_local(_parent_transform->orientation().inversed() * orientation);
	} else {
		set_orientation_local(orientation);
	}
}

//==============================================================================
//==============================================================================

void PlaceableObject::set_transform (const Matrix4 &transform)
{
	if (has_parent()) {
        set_transform_local(_parent_transform->inversed() * transform);
	} else {
        set_transform_local(transform);
	}
}

void PlaceableObject::set_transform_local (const Matrix4 &transform)
{
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();
    Vector3 scale = transform.scale();
	
	set_orientation_local(orientation);
	set_translation_local(translation);
	set_scale_local(scale);
}

//==============================================================================
/// Converts a point in global coordinates to the local coordinate system.
//==============================================================================

Vector3 PlaceableObject::global_to_local_point (const Vector3 &rg) const
{
    Vector3 pt;
    
    pt = rg - translation();
    
    return orientation().inversed() * pt;
}

//==============================================================================
/// Converts a point in local coordinates to the global coordinate system.
//==============================================================================

Vector3 PlaceableObject::local_to_global_point (const Vector3 &rl) const
{
    Vector3 pt = orientation() * rl;
    pt = pt + translation();
    
    return pt;
}

//==============================================================================
//==============================================================================

Vector3 PlaceableObject::local_to_global_velocity (const Vector3 &rl) const
{
    return velocity() + Vector3::cross(angular_velocity(), rl);
}

//==============================================================================
//==============================================================================

void PlaceableObject::rotate_around_X (const DTfloat angle)
{	
	set_orientation(_orientation * Matrix3::set_rotation_x(angle));
}

//==============================================================================
//==============================================================================

void PlaceableObject::rotate_around_Y (const DTfloat angle)
{	
	set_orientation(_orientation * Matrix3::set_rotation_y(angle));
}

//==============================================================================
//==============================================================================

void PlaceableObject::rotate_around_Z (const DTfloat angle)
{	
	set_orientation(_orientation * Matrix3::set_rotation_z(angle));
}

//==============================================================================
//==============================================================================

Vector3 PlaceableObject::forwards    (void) const
{
    Vector3 z_axis = -transform().z_axis();
    return z_axis;
}

Vector3 PlaceableObject::backwards   (void) const
{
    Vector3 z_axis = transform().z_axis();
    return z_axis;
}

Vector3 PlaceableObject::left_axis	(void) const
{
    Vector3 x_axis = -transform().x_axis();
    return x_axis;
}

Vector3 PlaceableObject::right_axis       (void) const
{
    Vector3 x_axis = transform().x_axis();
    return x_axis;
}

Vector3 PlaceableObject::up_axis	(void) const
{
    Vector3 y_axis = transform().y_axis();
    return y_axis;
}

Vector3 PlaceableObject::down_axis    (void) const
{
    Vector3 y_axis = -transform().y_axis();
    return y_axis;
}

//==============================================================================
//==============================================================================

void PlaceableObject::set_orientation_relative_angle_X	(PlaceableObject *obj1, DTfloat angle)
{   
    Matrix3	body_orientation = obj1->orientation();
    Vector3	body_x = body_orientation.x_axis();

    Matrix3	rot = Matrix3::set_rotation_around(body_x, angle);
    
    // Apply the rotation
    rot = rot * body_orientation;
    set_orientation(rot);
}

//==============================================================================
//==============================================================================

void PlaceableObject::set_orientation_relative_angle_Y	(PlaceableObject *obj1, DTfloat angle)
{   
    Matrix3	body_orientation = obj1->orientation();
    Vector3	body_y = body_orientation.y_axis();

    Matrix3	rot = Matrix3::set_rotation_around(body_y, angle);
    
    // Apply the rotation
    rot = rot * body_orientation;
    set_orientation(rot);
}

//==============================================================================
//==============================================================================

void PlaceableObject::set_orientation_relative_angle_Z	(PlaceableObject *obj1, DTfloat angle)
{   
    Matrix3	body_orientation = obj1->orientation();
    Vector3	body_z = body_orientation.z_axis();

    Matrix3	rot = Matrix3::set_rotation_around(body_z, angle);
    
    // Apply the rotation
    rot = rot * body_orientation;
    set_orientation(rot);
}

//==============================================================================
//==============================================================================

void PlaceableObject::set_translation_relative (PlaceableObject *obj1, Vector3 pos)
{
    set_translation(obj1->transform() * pos);
}

//==============================================================================
//==============================================================================

Sphere PlaceableObject::bounding_sphere (void) const
{  
    Vector3 s = scale();
    return Sphere( radius() * MoreMath::max(s.x,s.y,s.z) );
}

//==============================================================================
//==============================================================================

DTboolean PlaceableObject::compute (const PlugBase *plug)
{
    if (super_type::compute(plug))  return true;

	if (plug == &_world_translation ||
		plug == &_world_orientation ||
		plug == &_world_scale) {
		
		if (has_parent()) {
			Matrix4 transform = _parent_transform;
			
			_world_translation = transform * translation_local();
			_world_orientation = transform.orientation() * orientation_local();
			_world_scale = transform.scale() * scale_local();
		} else {
			_world_translation = _translation;
			_world_orientation = _orientation;
			_world_scale = _scale;
		}
		
		_world_translation.set_clean();
		_world_orientation.set_clean();
		_world_scale.set_clean();
		return true;
        
    } else if (plug == &_transform) {
        _transform = Matrix4(_orientation, _translation, _scale);
        _transform.set_clean();
		return true;
        
    } else if (plug == &_transform_inversed) {
        _transform_inversed = (*_transform).inversed();
        _transform_inversed.set_clean();
		return true;
    
	} else if (plug == &_world_transform) {
        if (has_parent())   _world_transform = _parent_transform * Matrix4(_orientation, _translation, _scale);
        else                _world_transform =  Matrix4(_orientation, _translation, _scale);
        _world_transform.set_clean();
		return true;
        
    } else if (plug == &_world_transform_inversed) {
        _world_transform_inversed = (*_world_transform).inversed();
        _world_transform_inversed.set_clean();
		return true;
        
    }
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


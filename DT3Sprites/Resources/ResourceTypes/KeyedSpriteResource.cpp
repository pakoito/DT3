//==============================================================================
///	
///	File: KeyedSpriteResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteResource.hpp"
#include "Archive.hpp"
#include "ArchiveProcessPointersRef.hpp"
#include "ArchiveProcessPointersWeakRef.hpp"

#include "ResourceManager.hpp"
#include "Archive.hpp"
#include "System.hpp"
#include "Factory.hpp"
#include "Assert.hpp"
#include "ImporterKeyedSprite.hpp"
#include "BinaryStream.hpp"
#include "TextStream.hpp"
#include "FilePath.hpp"
#include "Algorithm.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(KeyedSpriteResource)
IMPLEMENT_RESOURCE_CREATION(KeyedSpriteResource)
IMPLEMENT_STANDARD_RESOURCE_HANDLERS(KeyedSpriteResource,_keyed_sprite_map,_keyed_sprite_map_lock)

//==============================================================================
//==============================================================================

SpinLock                                        KeyedSpriteResource::_keyed_sprite_map_lock;
std::map<std::string, std::shared_ptr<KeyedSpriteResource> >			KeyedSpriteResource::_keyed_sprite_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteResource::KeyedSpriteResource (void)
{

}
					
KeyedSpriteResource::~KeyedSpriteResource (void)
{ 
	clear_joints();
}


//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, KeyedSpriteResource* &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, KeyedSpriteResource* &r)
{
	std::string path;
	s >> path;
	
	r = KeyedSpriteResource::keyed_sprite(FilePath(path)).get();
	return s;
}

Stream& operator <<(Stream &s, const std::shared_ptr<KeyedSpriteResource> &r)
{
	if (r.is_valid()) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<KeyedSpriteResource> &r)
{
	std::string path;
	s >> path;
	
	r = KeyedSpriteResource::keyed_sprite(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

std::shared_ptr<KeyedSpriteResource> KeyedSpriteResource::keyed_sprite(const FilePath &pathname, std::string args)
{	
	return getImpl<KeyedSpriteResource>::go(_keyed_sprite_map, pathname, args, _keyed_sprite_map_lock);
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::archiveRead (Archive *archive)
{
	Resource::archiveRead(archive);

    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA_ACCESSORS("Material", KeyedSpriteResource::getMaterialProperty, KeyedSpriteResource::setMaterialProperty, DATA_PERSISTENT | DATA_SETTABLE);

	DTsize jointCount;
	*archive << ARCHIVE_DATA(jointCount,DATA_PERSISTENT);
	_joints.resize(jointCount);

	for (DTuint j = 0; j < _joints.size(); ++j) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_joints[j]));
	}

	DTsize childrenCount;
	*archive << ARCHIVE_DATA(childrenCount,DATA_PERSISTENT);
	_children.resize(childrenCount);
	
	FOR_EACH (i, _children) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_WEAK_REFERENCED(archive,*i));
	}
	
    archive->popDomain ();
}

void KeyedSpriteResource::archiveWrite (Archive *archive)
{
	Resource::archiveWrite(archive);

    archive->pushDomain (getClassID ());
    
	*archive << ARCHIVE_DATA_ACCESSORS("Material", KeyedSpriteResource::getMaterialProperty, KeyedSpriteResource::setMaterialProperty, DATA_PERSISTENT | DATA_SETTABLE);

	DTsize jointCount = _joints.size();
	*archive << ARCHIVE_DATA(jointCount,DATA_PERSISTENT);

	for (DTuint j = 0; j < _joints.size(); ++j) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_joints[j]));
	}
	
	DTsize childrenCount = _children.size();
	*archive << ARCHIVE_DATA(childrenCount,DATA_PERSISTENT);

	FOR_EACH (i, _children) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_WEAK_REFERENCED(archive,*i));
	}

    archive->popDomain ();
}
        
//==============================================================================
//==============================================================================

DTerr KeyedSpriteResource::import (const FilePath &pathname, String args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
		
	//if ((err = reload ()) != DT3_ERR_NONE)
	//	return err;
	
	return DT3_ERR_NONE;
}

DTerr KeyedSpriteResource::recache (void)
{
	FilePath original_path(getPath());
	String extension = original_path.getFileExt();
	
	// Build a generic importer for the file type
	BaseClass *generic_importer = Factory::createImporter(extension);
	if (!generic_importer || !generic_importer->isA(ImporterKeyedSprite::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of an image importer
	ImporterKeyedSprite *sprite_importer = checkedStaticCast<ImporterKeyedSprite*>(generic_importer);
	DTerr err = sprite_importer->import(this, getArgs());
	
	return err;
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::copy (const KeyedSpriteResource &rhs)
{
	clear_joints();
	
	_material = rhs._material;
	
	// copy the joints of the object
	_joints.resize(rhs._joints.size());
	for (DTuint i = 0; i < rhs._joints.size(); ++i) {
		_joints[i] = rhs._joints[i]->clone();
	}

	// set top Level Children
	_children.resize(rhs._children.size());
	for (DTuint i = 0; i < _children.size(); ++i) {
        DTint idx = (DTint) rhs._joints.indexOf(rhs._children[i]);
		_children[i] = _joints[idx];
	}

	// set Other Children
	for (DTuint j = 0; j < _joints.size(); ++j) {
    
		// set parent
		if (rhs._joints[j]->_parent) {
            DTint idx = (DTint) rhs._joints.indexOf(rhs._joints[j]->_parent);
            _joints[j]->_parent = _joints[ idx ];
        } else {
        	_joints[j]->_parent = NULL;
        }
        
		// set Children
		std::vector<KeyedSpriteAnimationPoseJoint*> &joint_children = _joints[j]->_children;
		std::vector<KeyedSpriteAnimationPoseJoint*> &rhs_joint_children = rhs._joints[j]->_children;

		joint_children.resize(rhs_joint_children.size());

		for (DTuint i = 0; i < joint_children.size(); ++i) {
            DTint idx = (DTint) rhs._joints.indexOf(rhs_joint_children[i]);
            joint_children[i] = _joints[ idx ];
		}
	}
}

//==============================================================================
//==============================================================================

KeyedSpriteAnimationPoseJoint* KeyedSpriteResource::joint_by_name (const String &joint_name)
{
	for (DTuint i = 0; i < _joints.size(); ++i) {
		if (_joints[i]->getName() == joint_name)
			return _joints[i];
	}

	return NULL;
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::clear_joints (void)
{
	_joints.clear();
}

//==============================================================================
//==============================================================================

KeyedSpriteAnimationPoseJoint* KeyedSpriteResource::parent_of_joint (KeyedSpriteAnimationPoseJoint *joint)
{
	return joint->_parent;
}

std::vector<KeyedSpriteAnimationPoseJoint*>& KeyedSpriteResource::children_of_joint (KeyedSpriteAnimationPoseJoint *joint)
{
	if (joint)	return joint->_children;
	else		return _children;
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::reparent_joint (KeyedSpriteAnimationPoseJoint *joint, KeyedSpriteAnimationPoseJoint *parent)
{
	// Check for cycles
	KeyedSpriteAnimationPoseJoint *check_joint = joint->_parent;
	while (check_joint) {
		if (joint == check_joint)
			return;
		check_joint = check_joint->_parent;
	}

	// Remove from parent
	if (joint->_parent)	joint->_parent->_children.erase(joint->_parent->_children.find(joint));
	else				_children.erase(_children.find(joint));
	
	// add to new Parent
	if (parent)			parent->_children.append(joint);
	else				_children.append(joint);
	
	joint->_parent = parent;
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::recalculate_order (void)
{
    DTfloat order =  -_joints.size() / 2.0F;
    FOR_EACH(i, _joints) {
        (**i).setOrder(order);
        order += 1.0F;
    }
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::delete_joint (KeyedSpriteAnimationPoseJoint *joint)
{
	// Remove from parent
	if (joint->_parent)	joint->_parent->_children.erase(joint->_parent->_children.find(joint));
	else				_children.erase(_children.find(joint));

	// delete all objects from the list of joints
	std::list<KeyedSpriteAnimationPoseJoint*> joints_to_delete;
	joints_to_delete.pushBack(joint);
	
	while (joints_to_delete.size() > 0) {
		KeyedSpriteAnimationPoseJoint *joint_to_delete = joints_to_delete.front();
		
		for (DTuint i = 0; i < joint_to_delete->_children.size(); ++i) {
			joints_to_delete.pushBack(joint_to_delete->_children[i]);
		}
		
		// delete the joint from the list of objects. Gotta use find because indexes get mixed up.
		auto i = _joints.find(joint_to_delete);
		if (i != _joints.end()) {
			(**i)._parent = NULL;
			_joints.erase(i);
		}
		
		joints_to_delete.popFront();
	}

}

void KeyedSpriteResource::add_joint (KeyedSpriteAnimationPoseJoint *joint, KeyedSpriteAnimationPoseJoint *parent)
{
	Assert(joint->_parent == NULL);

	// add to the list of all of the joints
	_joints.append(joint);
	
	// add to top level of hierarchy
	if (parent)		parent->_children.append(joint);
	else			_children.append(joint);

	joint->_parent = parent;	

    recalculate_order();
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::move_to_front (KeyedSpriteAnimationPoseJoint *joint)
{
	auto i = _joints.find(joint);
	if (i != _joints.end()) {
		KeyedSpriteAnimationPoseJoint *joint = *i;
		_joints.erase(i);
		_joints.push_back(joint);
    }
    
    recalculate_order();
}

void KeyedSpriteResource::move_to_back (KeyedSpriteAnimationPoseJoint *joint)
{
	auto i = _joints.find(joint);
	if (i != _joints.end()) {
		KeyedSpriteAnimationPoseJoint *joint = *i;
		_joints.erase(i);
		_joints.push_front(joint);
	}
    
    recalculate_order();
}

//==============================================================================
//==============================================================================

void KeyedSpriteResource::update_transforms_recursive (const Matrix3 *parent_transform, KeyedSpriteAnimationPoseJoint *joint)
{
	Vector2 translation = joint->translation();
	DTfloat scale = joint->scale();
	DTfloat rotation = joint->rotation();

	DTfloat sin_rotation = std::sin(rotation);
	DTfloat cos_rotation = std::cos(rotation);

	Matrix3 joint_to_parent = Matrix3(	scale * cos_rotation,	scale * sin_rotation,	translation.x,
										scale * -sin_rotation,  scale * cos_rotation,	translation.y,
										0.0F,					0.0F,					1.0F    );
																									
	Matrix3 transform;
	if (parent_transform)	transform = (*parent_transform) * joint_to_parent;
	else					transform = joint_to_parent;

	joint->set_joint_to_object_transform(transform);
	
	for (DTuint i = 0; i < joint->_children.size(); ++i) {
		update_transforms_recursive(&transform, joint->_children[i]);
	}
}

void KeyedSpriteResource::update_transforms (void)
{
	for (DTuint i = 0; i < _children.size(); ++i) {
		update_transforms_recursive(NULL, _children[i]);
	}
}

//==============================================================================
//==============================================================================

DTint KeyedSpriteResource::comparison (const KeyedSpriteAnimationPoseJoint *a, const KeyedSpriteAnimationPoseJoint *b)
{
    if (a->_order < b->_order)  return -1;
    if (a->_order > b->_order)  return 1;
    return 0;
}

void KeyedSpriteResource::update_order (void)
{   
    Algorithm::bubblesort(_joints, &KeyedSpriteResource::comparison);
}   

//==============================================================================
//==============================================================================

} // DT3


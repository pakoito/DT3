//==============================================================================
///	
///	File: KeyedSpriteAnimationPoseJoint.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimationPoseJoint.hpp"
#include "Archive.hpp"
#include "PoolAllocator.hpp"
#include "MaterialResource.hpp"
#include "ArchiveProcessPointersWeakRef.hpp"
#include "Factory.hpp"
#include "MoreMath.hpp"
#include "DrawBatcherQuads.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(KeyedSpriteAnimationPoseJoint)
IMPLEMENT_POOL_ALLOCATOR(KeyedSpriteAnimationPoseJoint)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimationPoseJoint::KeyedSpriteAnimationPoseJoint (void)
	:	_name_hash                  (0),
		_parent						(NULL),
		_joint_to_object_transform	(	1.0F, 0.0F, 0.0F,
										0.0F,	1.0F,	0.0F,
										0.0F,	0.0F,	1.0F),
        _translation                (0.0F,0.0F),
		_rotation					(0.0F),
		_scale						(1.0F),
		_plus_x						(0.1F),
		_minus_x					(0.1F),
		_plus_y						(0.1F),
		_minus_y					(0.1F),
		_plus_u						(1.0F),
		_minus_u					(0.0F),
		_plus_v						(1.0F),
		_minus_v					(0.0F),
		_visible					(true),
        _order                      (0.0F),
        _groups                     (0x0001),
        _grid_size_x                (0),
        _grid_size_y                (0)
{
    setName("Part");
}

KeyedSpriteAnimationPoseJoint::KeyedSpriteAnimationPoseJoint (const KeyedSpriteAnimationPoseJoint &rhs)
    :   BaseClass					(rhs),
		_name						(rhs._name),
        _name_hash                  (rhs._name_hash),
		_parent						(NULL),
		_joint_to_object_transform	(rhs._joint_to_object_transform),
        _translation				(rhs._translation),
		_rotation					(rhs._rotation),
		_scale						(rhs._scale),
		_plus_x						(rhs._plus_x),
		_minus_x					(rhs._minus_x),
		_plus_y						(rhs._plus_y),
		_minus_y					(rhs._minus_y),
		_plus_u						(rhs._plus_u),
		_minus_u					(rhs._minus_u),
		_plus_v						(rhs._plus_v),
		_minus_v					(rhs._minus_v),
		_visible					(rhs._visible),
        _order                      (rhs._order),
        _groups                     (rhs._groups),
        _grid_size_x                (rhs._grid_size_x),
        _grid_size_y                (rhs._grid_size_y),
        _grid                       (rhs._grid)
{

}	
            
KeyedSpriteAnimationPoseJoint& KeyedSpriteAnimationPoseJoint::operator = (const KeyedSpriteAnimationPoseJoint &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {		
		BaseClass::operator = (rhs);

		_name = rhs._name;
        _name_hash = rhs._name_hash;

		_parent = NULL;
		_children.clear();
		_joint_to_object_transform = rhs._joint_to_object_transform;
        
		_translation = rhs._translation;
		_rotation = rhs._rotation;
		_scale = rhs._scale;
		_plus_x = rhs._plus_x;
		_minus_x = rhs._minus_x;
		_plus_y = rhs._plus_y;
		_minus_y = rhs._minus_y;
		_plus_u = rhs._plus_u;
		_minus_u = rhs._minus_u;
		_plus_v = rhs._plus_v;
		_minus_v = rhs._minus_v;
        
		_visible = rhs._visible;
        _order = rhs._order;
        
        _groups = rhs._groups;

        _grid_size_x = rhs._grid_size_x;
        _grid_size_y = rhs._grid_size_y;
        _grid = rhs._grid;
    }
    return (*this);
}	

KeyedSpriteAnimationPoseJoint::~KeyedSpriteAnimationPoseJoint (void)
{

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPoseJoint::archiveRead (Archive *archive)
{    
    BaseClass::archiveRead(archive);

    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA_ACCESSORS("Name", KeyedSpriteAnimationPoseJoint::getName, KeyedSpriteAnimationPoseJoint::setName, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_joint_to_object_transform,DATA_PERSISTENT);
	
	DTuint childrenCount;
	*archive << ARCHIVE_DATA(childrenCount,DATA_PERSISTENT);
	_children.resize(childrenCount);
	
	FOR_EACH (i,_children) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_WEAK_REFERENCED(archive,*i));
	}
	
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_WEAK_REFERENCED(archive,_parent));

	*archive << ARCHIVE_DATA(_translation,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_rotation,DATA_PERSISTENT);
	*archive << ARCHIVE_DATA_ACCESSORS("Rotation", KeyedSpriteAnimationPoseJoint::getRotationDegrees, KeyedSpriteAnimationPoseJoint::setRotationDegrees, DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_visible,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_scale,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_v,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_v,DATA_PERSISTENT | DATA_SETTABLE);
    
    if (archive->getVersion() >= 1364) {
        *archive << ARCHIVE_DATA(_order,DATA_PERSISTENT | DATA_SETTABLE);
    }
    
    if (archive->getVersion() >= 1365) {
        *archive << ARCHIVE_DATA(_groups,DATA_PERSISTENT | DATA_SETTABLE);
    }

    if (archive->getVersion() >= 1358) {
        *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_X", KeyedSpriteAnimationPoseJoint::getGridSizeX, KeyedSpriteAnimationPoseJoint::setGridSizeX, DATA_PERSISTENT | DATA_SETTABLE);
        *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_Y", KeyedSpriteAnimationPoseJoint::getGridSizeY, KeyedSpriteAnimationPoseJoint::setGridSizeY, DATA_PERSISTENT | DATA_SETTABLE);
    
        for (DTint i = 0; i < _grid.size(); ++i) {
            *archive << ARCHIVE_DATA_RAW(_grid[i], DATA_PERSISTENT);
        }
    
    }

	archive->popDomain ();
}

void KeyedSpriteAnimationPoseJoint::archiveWrite (Archive *archive)
{    
    BaseClass::archiveWrite(archive);

    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA_ACCESSORS("Name", KeyedSpriteAnimationPoseJoint::getName, KeyedSpriteAnimationPoseJoint::setName, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_joint_to_object_transform,DATA_PERSISTENT);
	
	DTuint childrenCount = (DTuint) _children.size();
	*archive << ARCHIVE_DATA(childrenCount,DATA_PERSISTENT);

	FOR_EACH (i,_children) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_WEAK_REFERENCED(archive,*i));
	}
	
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_WEAK_REFERENCED(archive,_parent));

	*archive << ARCHIVE_DATA(_translation,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_rotation,DATA_PERSISTENT);
	*archive << ARCHIVE_DATA_ACCESSORS("Rotation", KeyedSpriteAnimationPoseJoint::getRotationDegrees, KeyedSpriteAnimationPoseJoint::setRotationDegrees, DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_visible,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_scale,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_v,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_v,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_DATA(_order,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_groups,DATA_PERSISTENT | DATA_SETTABLE)
        .setBitfield(true);

    *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_X", KeyedSpriteAnimationPoseJoint::getGridSizeX, KeyedSpriteAnimationPoseJoint::setGridSizeX, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_Y", KeyedSpriteAnimationPoseJoint::getGridSizeY, KeyedSpriteAnimationPoseJoint::setGridSizeY, DATA_PERSISTENT | DATA_SETTABLE);

    for (DTint i = 0; i < _grid.size(); ++i) {
        *archive << ARCHIVE_DATA_RAW(_grid[i], DATA_PERSISTENT);
    }

	archive->popDomain ();
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPoseJoint::setName (const String &name)
{   
    _name_hash = name.hash();	
    _name = name;	
}

//==============================================================================
//==============================================================================

DTfloat KeyedSpriteAnimationPoseJoint::getRotationDegrees (void) const
{
	return _rotation * RAD_TO_DEG;
}

void KeyedSpriteAnimationPoseJoint::setRotationDegrees (const DTfloat rotation)
{
	_rotation = rotation * DEG_TO_RAD;
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPoseJoint::setWithKey (const KeyedSpriteAnimationKeyframe& key)
{
    _translation = key._translation;
    _rotation = key._rotation;
    _visible = key._visible;
    _scale = key._scale;
    _plus_x = key._plus_x;
    _minus_x = key._minus_x;
    _plus_y = key._plus_y;
    _minus_y = key._minus_y;
    _plus_u = key._plus_u;
    _minus_u = key._minus_u;
    _plus_v = key._plus_v;
    _minus_v = key._minus_v;
    
    _order = key._order;

    _grid_size_x = key._grid_size_x;
    _grid_size_y = key._grid_size_y;
    _grid = key._grid;
}

KeyedSpriteAnimationKeyframe KeyedSpriteAnimationPoseJoint::getAsKey (void) const
{
    KeyedSpriteAnimationKeyframe key;
    
    key._translation = _translation;
    key._rotation = _rotation;
    key._visible = _visible;
    key._scale = _scale;
    key._plus_x = _plus_x;
    key._minus_x = _minus_x;
    key._plus_y = _plus_y;
    key._minus_y = _minus_y;
    key._plus_u = _plus_u;
    key._minus_u = _minus_u;
    key._plus_v = _plus_v;
    key._minus_v = _minus_v;

    key._order = _order;
    
    key._grid_size_x = _grid_size_x;
    key._grid_size_y = _grid_size_y;
    key._grid = _grid;

    return key;
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPoseJoint::setGridSizeX (DTint size_x)
{
    _grid_size_x = (size_x < 2) ? 0 : size_x;
    allocateGrid();
}
        
DTint KeyedSpriteAnimationPoseJoint::getGridSizeX (void) const
{
    return _grid_size_x;
}

void KeyedSpriteAnimationPoseJoint::setGridSizeY (DTint size_y)
{
    _grid_size_y = (size_y < 2) ? 0 : size_y;
    allocateGrid();
}
        
DTint KeyedSpriteAnimationPoseJoint::getGridSizeY (void) const
{
    return _grid_size_y;
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPoseJoint::allocateGrid (void)
{
    DTint desired_size = _grid_size_x * _grid_size_y;
    
    if (desired_size != _grid.size()) {
        _grid.resize(desired_size);
        
        resetGrid();
    }
    
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPoseJoint::resetGrid (void)
{
    for (DTint x = 0; x < _grid_size_x; ++x) {
        DTfloat tx = (DTfloat) x / (_grid_size_x - 1);
    
        for (DTint y = 0; y < _grid_size_y; ++y) {
            DTfloat ty = (DTfloat) y / (_grid_size_y - 1);
            
            DTint i = y * _grid_size_x + x;

            _grid[i] = Vector2(tx,ty);
        }
    }
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPoseJoint::draw (DrawBatcherQuads &b)
{    
    Vector2 p0 = _joint_to_object_transform * Vector2(-_minus_x, -_minus_y);
    Vector2 p1 = _joint_to_object_transform * Vector2(_plus_x, -_minus_y);
    Vector2 p2 = _joint_to_object_transform * Vector2(_plus_x, _plus_y);
    Vector2 p3 = _joint_to_object_transform * Vector2(-_minus_x, _plus_y);

    if (_grid.size() == 0) {
        
        // No grid warp
        
        b.vertex(Vector3(p0.x,p0.y,0.0F), Texcoord2(_minus_u, _minus_v));
        b.vertex(Vector3(p1.x,p1.y,0.0F), Texcoord2(_plus_u, _minus_v));
        b.vertex(Vector3(p2.x,p2.y,0.0F), Texcoord2(_plus_u, _plus_v));
        b.vertex(Vector3(p3.x,p3.y,0.0F), Texcoord2(_minus_u, _plus_v));
        
    } else {
    
        // Grid warp
    
        for (DTint x = 0; x < (_grid_size_x-1); ++x) {
        
            for (DTint y = 0; y < (_grid_size_y-1); ++y) {
            
                DTint i00 = y * _grid_size_x + x;
                DTint i10 = i00 + 1;
                DTint i01 = i00 + _grid_size_y;
                DTint i11 = i01 + 1;
                
                Vector2 &v00 = _grid[i00];
                Vector2 &v10 = _grid[i10];
                Vector2 &v01 = _grid[i01];
                Vector2 &v11 = _grid[i11];
                
                Vector2 p1p0 = p1-p0;
                Vector2 p2p3 = p2-p3;

                DTfloat du = _plus_u - _minus_u;
                DTfloat dv = _minus_v - _plus_v;
                
                DTfloat rx = 1.0F / (_grid_size_x-1);
                DTfloat ry = 1.0F / (_grid_size_y-1);
                
                b.vertex( Vector3((p0 + p1p0*v00.x) * v00.y + (p3 + p2p3*v00.x) * (1.0F - v00.y)) , Texcoord2( (x+0) * rx * du + _minus_u, (y+0) * ry * dv + _plus_v ));
                b.vertex( Vector3((p0 + p1p0*v10.x) * v10.y + (p3 + p2p3*v10.x) * (1.0F - v10.y)) , Texcoord2( (x+1) * rx * du + _minus_u, (y+0) * ry * dv + _plus_v ));
                b.vertex( Vector3((p0 + p1p0*v11.x) * v11.y + (p3 + p2p3*v11.x) * (1.0F - v11.y)) , Texcoord2( (x+1) * rx * du + _minus_u, (y+1) * ry * dv + _plus_v ));
                b.vertex( Vector3((p0 + p1p0*v01.x) * v01.y + (p3 + p2p3*v01.x) * (1.0F - v01.y)) , Texcoord2( (x+0) * rx * du + _minus_u, (y+1) * ry * dv + _plus_v ));
            
            }
        }
    
    }

}

//==============================================================================
//==============================================================================

} // DT3

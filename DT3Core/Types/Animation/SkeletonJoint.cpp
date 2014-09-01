//==============================================================================
///	
///	File: SkeletonJoint.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Animation/SkeletonJoint.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SkeletonJoint::SkeletonJoint (void)
	:	_index						(0)
{

}

SkeletonJoint::SkeletonJoint (const SkeletonJoint &rhs)
	:	_name						(rhs._name),
		_name_hash					(rhs._name_hash),
		_index						(rhs._index),
		_local_transform			(rhs._local_transform),
		_local_transform_inverse	(rhs._local_transform_inverse),
		_world_transform			(rhs._world_transform),
		_world_transform_inverse	(rhs._world_transform_inverse),
		_children					(rhs._children)
{

}
            
SkeletonJoint::SkeletonJoint (SkeletonJoint &&rhs)
	:	_name						(std::move(rhs._name)),
		_name_hash					(rhs._name_hash),
		_index						(rhs._index),
		_local_transform			(rhs._local_transform),
		_local_transform_inverse	(rhs._local_transform_inverse),
		_world_transform			(rhs._world_transform),
		_world_transform_inverse	(rhs._world_transform_inverse),
		_children					(std::move(rhs._children))
{

}
            
SkeletonJoint& SkeletonJoint::operator = (const SkeletonJoint &rhs)
{	
	_name = rhs._name;
	_name_hash = rhs._name_hash;
	_index = rhs._index;
	_local_transform = rhs._local_transform;
	_local_transform_inverse = rhs._local_transform_inverse;
	_world_transform = rhs._world_transform;
	_world_transform_inverse = rhs._world_transform_inverse;
	_children = rhs._children;
    return (*this);
}	

SkeletonJoint& SkeletonJoint::operator = (SkeletonJoint &&rhs)
{	
	_name = std::move(rhs._name);
	_name_hash = rhs._name_hash;
	_index = rhs._index;
	_local_transform = rhs._local_transform;
	_local_transform_inverse = rhs._local_transform_inverse;
	_world_transform = rhs._world_transform;
	_world_transform_inverse = rhs._world_transform_inverse;
	_children = std::move(rhs._children);
    return (*this);
}	

SkeletonJoint::~SkeletonJoint (void)
{

}

//==============================================================================
//==============================================================================

void SkeletonJoint::archive (const std::shared_ptr<Archive> &archive)
{
    *archive << ARCHIVE_DATA_RAW(_name, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_RAW(_name_hash, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_RAW(_index, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_RAW(_local_transform, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_RAW(_local_transform_inverse, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_RAW(_world_transform, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_RAW(_world_transform_inverse, DATA_PERSISTENT);
    
    DTsize children_size = _children.size();
    *archive << ARCHIVE_DATA_RAW(children_size, DATA_PERSISTENT);
    _children.resize(children_size);

    for (auto &child : _children) {
        child.archive(archive);
    }
}

//==============================================================================
//==============================================================================
		
} // DT3

//==============================================================================
///	
///	File: AnimationPoseJoint.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Animation/AnimationPoseJoint.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

AnimationPoseJoint::AnimationPoseJoint (void)
	:	_name_hash		(0),
		_keyframe_cache	(0)
{

}

AnimationPoseJoint::AnimationPoseJoint (const AnimationPoseJoint &rhs)
	:	_name_hash		(rhs._name_hash),
		_keyframe_cache	(rhs._keyframe_cache),
		_orientation	(rhs._orientation),
		_translation	(rhs._translation)
{

}
            
AnimationPoseJoint& AnimationPoseJoint::operator = (const AnimationPoseJoint &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {	
		_name_hash = rhs._name_hash;
		_keyframe_cache = rhs._keyframe_cache;
		_orientation = rhs._orientation;
		_translation = rhs._translation;
    }
    return (*this);
}	

AnimationPoseJoint::~AnimationPoseJoint (void)
{

}

//==============================================================================
//==============================================================================

void AnimationPoseJoint::set_transform (const Matrix4 &transform)
{
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();
	
	_orientation = Quaternion(orientation);
	_translation = translation;
}

void AnimationPoseJoint::set_transform (const Quaternion &orientation, const Vector3 &translation)
{
	_orientation = orientation;
	_translation = translation;
}

//==============================================================================
//==============================================================================
	
		
		
		
		
} // DT3

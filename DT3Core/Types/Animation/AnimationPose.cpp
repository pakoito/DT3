//==============================================================================
///	
///	File: AnimationPose.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Animation/AnimationPose.hpp"
#include "DT3Core/Types/Animation/AnimationTrack.hpp"
#include "DT3Core/Resources/ResourceTypes/AnimationResource.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

AnimationPose::AnimationPose (void)
{

}

AnimationPose::AnimationPose (const AnimationPose &rhs)
	:	BaseClass		(rhs),
        _joints         (rhs._joints)
{

}

AnimationPose::AnimationPose (AnimationPose &&rhs)
	:	BaseClass		(rhs),
        _joints         (std::move(rhs._joints))
{
    
}

            
AnimationPose& AnimationPose::operator = (const AnimationPose &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);
        _joints = rhs._joints;
	}
    return (*this);
}	

AnimationPose& AnimationPose::operator = (AnimationPose &&rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);
        _joints = std::move(rhs._joints);
	}
    return (*this);
}	

AnimationPose::~AnimationPose (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, AnimationPose* &r)
{
	return s;
}

Stream& operator >>(Stream &s, AnimationPose* &r)
{
	return s;
}

Stream& operator <<(Stream &s, const std::shared_ptr<AnimationPose> &r)
{
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<AnimationPose> &r)
{
	return s;
}

//==============================================================================
//==============================================================================

void AnimationPose::update (const DTfloat time, const std::shared_ptr<AnimationResource> &animation)
{
	if (!animation)
		return;
		
	// resize Pose for correct number of tracks
	if (_joints.size() != animation->num_tracks())
		_joints.clear();
		
	for (DTuint i = 0; i < animation->num_tracks(); ++i) {
		const std::shared_ptr<AnimationTrack> &track = animation->track(i);
		track->update_joint(time, _joints[track->name_hash()]);
	}
}

//==============================================================================
//==============================================================================

DTboolean AnimationPose::joint_transform (DTuint joint_name_hash, Matrix4 &transform) const
{
	std::map<DTuint, AnimationPoseJoint>::const_iterator i = _joints.find(joint_name_hash);
	if (i == _joints.end()) {
		return false;
	} else {
		Quaternion	orientation = i->second.orientation();
		Vector3		translation = i->second.translation();
		
		transform = Matrix4(Matrix3(orientation), translation);

		return true;
	}
}

//==============================================================================
//==============================================================================

} // DT3

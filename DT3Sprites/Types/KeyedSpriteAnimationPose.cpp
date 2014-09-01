//==============================================================================
///	
///	File: KeyedSpriteAnimationPose.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimationPose.hpp"
#include "Archive.hpp"
#include "KeyedSpriteAnimation.hpp"
#include "KeyedSpriteAnimationTrack.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimationPose::KeyedSpriteAnimationPose (void)
{

}

KeyedSpriteAnimationPose::KeyedSpriteAnimationPose (const KeyedSpriteAnimationPose &rhs)
	:	BaseClass		(rhs)
{

}
            
KeyedSpriteAnimationPose& KeyedSpriteAnimationPose::operator = (const KeyedSpriteAnimationPose &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);
	}
    return (*this);
}	

KeyedSpriteAnimationPose::~KeyedSpriteAnimationPose (void)
{

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPose::archiveRead (Archive *archive)
{    

}

void KeyedSpriteAnimationPose::archiveWrite (Archive *archive)
{    

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationPose::update (const DTfloat time, const KeyedSpriteAnimation *animation, std::shared_ptr<Callback2<const String&,const String&> > cb)
{
	if (!animation)
		return;
		
	// resize Pose for correct number of tracks
	if (_joints.size() != animation->getNumTracks())
		_joints.clear();
		
	for (DTuint i = 0; i < animation->getNumTracks(); ++i) {
		KeyedSpriteAnimationTrack *track = animation->getTrack(i);
        
        JointState &state = _joints[track->getNameHash()];
        
        track->updateJoint(time, state._keyframe);
        state._updated = track->getKeyframes().size() > 0;
	}
    
    // Fire events
    animation->getEventTrack().fireEvents(time, cb);
}

//==============================================================================
//==============================================================================

DTboolean KeyedSpriteAnimationPose::getJointState	(DTuint joint_name_hash, KeyedSpriteAnimationPoseJoint &joint) const
{
	std::map<DTuint, JointState>::const_iterator i = _joints.find(joint_name_hash);
	if (i == _joints.end()) {
		return false;
	} else {
        if (i->second._updated)
            joint.setWithKey(i->second._keyframe);
		return true;
	}
}

//==============================================================================
//==============================================================================

} // DT3

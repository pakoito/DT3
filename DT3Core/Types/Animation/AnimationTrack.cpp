//==============================================================================
///	
///	File: AnimationTrack.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Animation/AnimationTrack.hpp"
#include "DT3Core/Types/Animation/AnimationPoseJoint.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

AnimationTrack::AnimationTrack (void)	
	:	_name_hash	(0)
{

}

AnimationTrack::AnimationTrack (const AnimationTrack &rhs)
	:	BaseClass	(rhs),
		_name		(rhs._name),
		_name_hash	(rhs._name_hash),
		_keyframes	(rhs._keyframes)
{

}

AnimationTrack::AnimationTrack (AnimationTrack &&rhs)
	:	BaseClass	(rhs),
		_name		(std::move(rhs._name)),
		_name_hash	(rhs._name_hash),
		_keyframes	(std::move(rhs._keyframes))
{

}

            
AnimationTrack& AnimationTrack::operator = (const AnimationTrack &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);
		
		_name = rhs._name;
		_name_hash = rhs._name_hash;
		_keyframes = rhs._keyframes;
    }
    return (*this);
}	

AnimationTrack& AnimationTrack::operator = (AnimationTrack &&rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);
		
		_name = std::move(rhs._name);
		_name_hash = rhs._name_hash;
		_keyframes = std::move(rhs._keyframes);
    }
    return (*this);
}	

AnimationTrack::~AnimationTrack (void)
{

}

//==============================================================================
//==============================================================================

void AnimationTrack::archive (const std::shared_ptr<Archive> &archive)
{    
	BaseClass::archive(archive);

    archive->push_domain (class_id ());
    
    *archive << ARCHIVE_DATA_RAW(_name, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_RAW(_name_hash, DATA_PERSISTENT);

    DTsize size = _keyframes.size();
    *archive << ARCHIVE_DATA_RAW(size, DATA_PERSISTENT);
    _keyframes.resize(size);
    
    for (auto &keyframe : _keyframes)
        keyframe.archive(archive);
		
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void AnimationTrack::update_joint (const DTfloat time, AnimationPoseJoint &joint) const
{
	
	//
	// set name
	//
	
	joint.set_name_hash(_name_hash);

	//
	// Search for the correct keyframe
	//

	// Handle too few keyframes cases
	if (_keyframes.size() == 0) {
		joint.set_transform(Matrix4::identity());
	} else if (_keyframes.size() == 1) {
		Matrix4 r(Matrix3(_keyframes[0].orientation()), _keyframes[0].translation());
		joint.set_transform(r);
	}
		
	// Clamp cached value
	DTint cur_keyframe_cache = joint.keyframe_cache();
	if (cur_keyframe_cache < 0)	cur_keyframe_cache = 0;
	else if (cur_keyframe_cache+1 >= (DTint) _keyframes.size())	cur_keyframe_cache = (DTint) _keyframes.size()-2;

	// Do the search
	while (1) {	
		if (time < _keyframes[cur_keyframe_cache].time()) {
			--cur_keyframe_cache;
			// Check limits
			if (cur_keyframe_cache < 0) {
				cur_keyframe_cache = 0;
				break;
			}
		} else if (time >= _keyframes[cur_keyframe_cache+1].time()) {
			++cur_keyframe_cache;
			// Check limits
			if (cur_keyframe_cache+1 >= (DTint) _keyframes.size()) {
				cur_keyframe_cache = (DTint) _keyframes.size()-2;
				break;
			}
		} else {
			break;
		}

	}
	
	joint.set_keyframe_cache(cur_keyframe_cache);
	
	//
	// interpolate Keyframes
	//
	
	DTfloat t0 = _keyframes[cur_keyframe_cache].time();
	DTfloat t1 = _keyframes[cur_keyframe_cache+1].time();
	
	DTfloat t = (time - t0) / (t1 - t0);
	t = MoreMath::clamp_zero_one(t);
	
	Quaternion q0 = _keyframes[cur_keyframe_cache].orientation();
	Quaternion q1 = _keyframes[cur_keyframe_cache+1].orientation();
	Quaternion qr(Quaternion::slerp (q0, q1, t));
	
	Vector3 v0 = _keyframes[cur_keyframe_cache].translation();
	Vector3 v1 = _keyframes[cur_keyframe_cache+1].translation();
	Vector3 vr;
	
	vr = (v1 * t) + (v0 * (1.0F-t));
		
	//joint.setTransform(qr,vr);
	joint.set_transform(q0,v0);

}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: KeyedSpriteAnimationTrack.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimationTrack.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"
#include "Archive.hpp"
#include "Factory.hpp"
#include "Algorithm.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(KeyedSpriteAnimationTrack)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimationTrack::KeyedSpriteAnimationTrack (void)	
    :   _id             (0)
{
    setName("Track");
}

KeyedSpriteAnimationTrack::KeyedSpriteAnimationTrack (const KeyedSpriteAnimationTrack &rhs)
	:	BaseClass		(rhs),
        _id             (rhs._id),
		_name			(rhs._name),
		_name_hash      (rhs._name_hash),
		_keyframes		(rhs._keyframes)
{

}
            
KeyedSpriteAnimationTrack& KeyedSpriteAnimationTrack::operator = (const KeyedSpriteAnimationTrack &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);

        _id = rhs._id;
		_name = rhs._name;
		_name_hash = rhs._name_hash;
		_keyframes = rhs._keyframes;
    }
    return (*this);
}	

KeyedSpriteAnimationTrack::~KeyedSpriteAnimationTrack (void)
{

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationTrack::archiveRead (Archive *archive)
{    
	BaseClass::archiveRead(archive);

    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA_ACCESSORS("Name", KeyedSpriteAnimationTrack::getName, KeyedSpriteAnimationTrack::setName, DATA_PERSISTENT | DATA_SETTABLE);

	DTsize keyframeCount;
	*archive << ARCHIVE_DATA(keyframeCount,DATA_PERSISTENT);
	_keyframes.resize(keyframeCount);
	
	for (DTuint i = 0; i < _keyframes.size(); ++i) {
		_keyframes[i].archiveRead(archive);
        _keyframes[i]._id = ++_id;
	}
	
    archive->popDomain ();
}

void KeyedSpriteAnimationTrack::archiveWrite (Archive *archive)
{    
	BaseClass::archiveWrite(archive);

    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA_ACCESSORS("Name", KeyedSpriteAnimationTrack::getName, KeyedSpriteAnimationTrack::setName, DATA_PERSISTENT | DATA_SETTABLE);

	// Override settable flag
	DTuint flags = archive->getFlagsMask();
	archive->setFlagsMask( flags & (~DATA_SETTABLE) );

	DTsize keyframeCount = _keyframes.size();
	*archive << ARCHIVE_DATA(keyframeCount,DATA_PERSISTENT);

	for (DTsize i = 0; i < _keyframes.size(); ++i) {
		_keyframes[i].archiveRead(archive);
	}

	// Restore flags
	archive->setFlagsMask(flags);

    archive->popDomain ();
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationTrack::setName (const String &name)
{   
    _name_hash = name.hash();	
    _name = name;	
}

//==============================================================================
//==============================================================================

DTint KeyedSpriteAnimationTrack::duplicate(DTuint k)
{
    KeyedSpriteAnimationKeyframe keyframe = _keyframes[k];
    keyframe._id = ++_id;
    keyframe._time += 0.1F;
    
    _keyframes.pushBack(keyframe);
    
    return (DTint) _keyframes.size()-1;
}

//==============================================================================
//==============================================================================

#define INTERPOLATE_PARAMETER(param)	\
	joint.param = min_keyframe.param * one_minus_t + max_keyframe.param * t

#define INTERPOLATE_PARAMETER_BOOL(param)	\
	joint.param = (t >= 1.0F) ? max_keyframe.param : min_keyframe.param;


void KeyedSpriteAnimationTrack::updateJoint (const DTfloat time, KeyedSpriteAnimationKeyframe &joint) const
{

	//
	// Search for the correct keyframe
	//

	// Handle too few keyframes cases
	if (_keyframes.size() == 0) {
        // Do nothing
        return;
	} else if (_keyframes.size() == 1) {
        joint = _keyframes[0];
        return;
	}

	// Clamp cached value
	DTint cur_keyframe_cache = joint.getKeyframeCache();
	if (cur_keyframe_cache < 0)	cur_keyframe_cache = 0;
	else if (cur_keyframe_cache+1 >= (DTint) _keyframes.size())	cur_keyframe_cache = (DTint) _keyframes.size()-2;

	// Do the search
	while (1) {	
		if (time < _keyframes[cur_keyframe_cache]._time) {
			--cur_keyframe_cache;
			// Check limits
			if (cur_keyframe_cache < 0) {
				cur_keyframe_cache = 0;
				break;
			}
		} else if (time >= _keyframes[cur_keyframe_cache+1]._time) {
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
	
	joint.setKeyframeCache(cur_keyframe_cache);
	
	//
	// Interpolate Keyframes
	//
    
    const KeyedSpriteAnimationKeyframe &min_keyframe = _keyframes[cur_keyframe_cache];
    const KeyedSpriteAnimationKeyframe &max_keyframe = _keyframes[cur_keyframe_cache+1];
    
	DTfloat t0 = min_keyframe._time;
	DTfloat t1 = max_keyframe._time;
	
	DTfloat t = (time - t0) / (t1 - t0);
    
	t = MoreMath::clampZeroOne(t);
    DTfloat one_minus_t = 1.0F - t;

    INTERPOLATE_PARAMETER(_translation);
    INTERPOLATE_PARAMETER(_rotation);
    INTERPOLATE_PARAMETER(_scale);
    INTERPOLATE_PARAMETER(_plus_x);
    INTERPOLATE_PARAMETER(_minus_x);
    INTERPOLATE_PARAMETER(_plus_y);
    INTERPOLATE_PARAMETER(_minus_y);
    INTERPOLATE_PARAMETER(_plus_u);
    INTERPOLATE_PARAMETER(_minus_u);
    INTERPOLATE_PARAMETER(_plus_v);
    INTERPOLATE_PARAMETER(_minus_v);
    INTERPOLATE_PARAMETER(_order);
    INTERPOLATE_PARAMETER_BOOL(_visible);
    
    // Make sure grid is the correct size
    if (joint._grid_size_x != min_keyframe._grid_size_x || joint._grid_size_y != min_keyframe._grid_size_y) {
        joint._grid_size_x = min_keyframe._grid_size_x;
        joint._grid_size_y = min_keyframe._grid_size_y;
        joint.allocateGrid();
    }
    
    if (min_keyframe._grid.size() == max_keyframe._grid.size()) {
        for (DTint i = 0; i < joint._grid.size(); ++i) {
            INTERPOLATE_PARAMETER(_grid[i]);
        }
    }

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationTrack::addKeyframe (const KeyedSpriteAnimationKeyframe &keyframe)
{

    // Search for existing keyframe
    for (DTuint i = 0; i < _keyframes.size(); ++i) {
    
        if (keyframe.getTime() == _keyframes[i].getTime()) {
            DTint old_id = _keyframes[i]._id;
            _keyframes[i] = keyframe;
            _keyframes[i]._id = old_id;
            return;
        }
    
    }

    // Slightly modified version of the keyframe
    KeyedSpriteAnimationKeyframe new_keyframe = keyframe;
    new_keyframe._id = ++_id;

    // Requires new keyframe
    _keyframes.pushBack(new_keyframe);
    
    updateKeyframes();
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationTrack::updateKeyframes (void)
{
    Algorithm::bubblesort(_keyframes);
}

//==============================================================================
//==============================================================================
		
} // DT3

//==============================================================================
///	
///	File: KeyedSpriteAnimationEventTrack.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimationEventTrack.hpp"

#include "Archive.hpp"
#include "Factory.hpp"
#include "Algorithm.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(KeyedSpriteAnimationEventTrack)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimationEventTrack::KeyedSpriteAnimationEventTrack (void)	
    :   _id             (0)
{

}

KeyedSpriteAnimationEventTrack::KeyedSpriteAnimationEventTrack (const KeyedSpriteAnimationEventTrack &rhs)
	:	BaseClass		(rhs),
        _id             (rhs._id),
		_keyframes		(rhs._keyframes)
{

}
            
KeyedSpriteAnimationEventTrack& KeyedSpriteAnimationEventTrack::operator = (const KeyedSpriteAnimationEventTrack &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);

        _id = rhs._id;
		_keyframes = rhs._keyframes;
    }
    return (*this);
}	

KeyedSpriteAnimationEventTrack::~KeyedSpriteAnimationEventTrack (void)
{

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationEventTrack::archiveRead (Archive *archive)
{    
	BaseClass::archiveRead(archive);

    archive->pushDomain (getClassID ());

	DTsize keyframeCount;
	*archive << ARCHIVE_DATA(keyframeCount,DATA_PERSISTENT);
	_keyframes.resize(keyframeCount);
	
	for (DTuint i = 0; i < _keyframes.size(); ++i) {
		_keyframes[i].archiveRead(archive);
        _keyframes[i]._id = ++_id;
	}
	
    archive->popDomain ();
}

void KeyedSpriteAnimationEventTrack::archiveWrite (Archive *archive)
{    
	BaseClass::archiveWrite(archive);

    archive->pushDomain (getClassID ());

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

DTint KeyedSpriteAnimationEventTrack::duplicate(DTuint k)
{
    KeyedSpriteAnimationEvent keyframe = _keyframes[k];
    keyframe._id = ++_id;
    keyframe._time += 0.1F;
    
    _keyframes.pushBack(keyframe);
    
    return (DTint) _keyframes.size()-1;
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationEventTrack::fireEvents (const DTfloat time, std::shared_ptr<Callback2<const String&,const String&> > cb) const
{
    // Search for existing keyframe
    for (DTuint i = 0; i < _keyframes.size(); ++i) {
        const KeyedSpriteAnimationEvent &keyframe = _keyframes[i];
    
        if (time < _keyframes[i].getTime()) {
            keyframe.setTriggered(false);
        } else {
            
            // Check for newly triggered events
            if (!keyframe.getTriggered()) {
                
                // Fire callback
                if (cb.isValid())
                    (*cb)(keyframe.getEventID(), keyframe.getEventParameter());
                
                keyframe.setTriggered(true);
            
            }
        
        }
    
    }

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationEventTrack::addKeyframe (const KeyedSpriteAnimationEvent &keyframe)
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
    KeyedSpriteAnimationEvent new_keyframe = keyframe;
    new_keyframe._id = ++_id;

    // Requires new keyframe
    _keyframes.pushBack(new_keyframe);
    
    updateKeyframes();
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationEventTrack::updateKeyframes (void)
{
    Algorithm::bubblesort(_keyframes);
}

//==============================================================================
//==============================================================================
		
} // DT3

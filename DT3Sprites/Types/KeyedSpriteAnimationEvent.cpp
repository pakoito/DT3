//==============================================================================
///	
///	File: KeyedSpriteAnimationEvent.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimationEvent.hpp"
#include "Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimationEvent::KeyedSpriteAnimationEvent (void)
	:   _id                         (0),
        _time                       (0.0F),
        _triggered                  (false)
{
    
}

KeyedSpriteAnimationEvent::KeyedSpriteAnimationEvent (const KeyedSpriteAnimationEvent &rhs)
	:	_id                         (rhs._id),
        _time                       (rhs._time),
		_event_id                   (rhs._event_id),
		_event_parameter            (rhs._event_parameter),
        _triggered                  (rhs._triggered)
{

}	
            
KeyedSpriteAnimationEvent& KeyedSpriteAnimationEvent::operator = (const KeyedSpriteAnimationEvent &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {		
        _id = rhs._id;
		_time = rhs._time;
		_event_id = rhs._event_id;
		_event_parameter = rhs._event_parameter;
        _triggered = rhs._triggered;

    }
    return (*this);
}	

KeyedSpriteAnimationEvent::~KeyedSpriteAnimationEvent (void)
{

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationEvent::archiveRead (Archive *archive)
{    
    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA(_time,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_event_id,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_event_parameter,DATA_PERSISTENT | DATA_SETTABLE);

	archive->popDomain ();
}

void KeyedSpriteAnimationEvent::archiveWrite (Archive *archive)
{    
    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA(_time,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_event_id,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_event_parameter,DATA_PERSISTENT | DATA_SETTABLE);

	archive->popDomain ();
}

//==============================================================================
//==============================================================================

} // DT3

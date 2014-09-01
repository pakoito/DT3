//==============================================================================
///	
///	File: KeyedSpriteAnimation.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimation.hpp"
#include "KeyedSpriteAnimationTrack.hpp"
#include "ArchiveProcessPointersRef.hpp"
#include "ArchiveProcessPointersWeakRef.hpp"
#include "Archive.hpp"
#include "Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(KeyedSpriteAnimation)
//IMPLEMENT_POOL_ALLOCATOR(KeyedSpriteAnimation)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimation::KeyedSpriteAnimation (void)	
	:	_name			("Animation"),
        _start_time     (0.0F),
        _stop_time      (10.0F),
        _speed          (1.0F),
        _loop           (true),
        _groups         (0xFFFF)
{

}

KeyedSpriteAnimation::KeyedSpriteAnimation (const KeyedSpriteAnimation &rhs)
	:	BaseClass		(rhs),
		_name			(rhs._name),
        _start_time     (rhs._start_time),
        _stop_time      (rhs._stop_time),
        _speed          (rhs._speed),
        _loop           (rhs._loop),
        _groups         (rhs._groups),
        _event_track    (rhs._event_track)
{
    
    _tracks.resize(rhs._tracks.size(), NULL);
    for (DTuint i = 0; i < _tracks.size(); ++i) {
        _tracks[i] = rhs._tracks[i]->clone();
    }

}
            
KeyedSpriteAnimation& KeyedSpriteAnimation::operator = (const KeyedSpriteAnimation &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);

		_name = rhs._name;
        _start_time = rhs._start_time;
        _stop_time = rhs._stop_time;
        _speed = rhs._speed;
        _loop = rhs._loop;
        _groups = rhs._groups;
        _event_track = rhs._event_track;

        _tracks.resize(rhs._tracks.size(), NULL);
        for (DTuint i = 0; i < _tracks.size(); ++i) {
            RELEASE(_tracks[i]);
            _tracks[i] = rhs._tracks[i]->clone();
        }

    }
    return (*this);
}	

KeyedSpriteAnimation::~KeyedSpriteAnimation (void)
{
    removeAllTracks();
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimation::archiveRead (Archive *archive)
{    
	BaseClass::archiveRead(archive);

    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA(_name,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_start_time,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_stop_time,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_speed,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_loop,DATA_PERSISTENT | DATA_SETTABLE);

    if (archive->getVersion() >= 1365) {
        *archive << ARCHIVE_DATA(_groups,DATA_PERSISTENT | DATA_SETTABLE);
    }
    
    if (archive->getVersion() >= 1366) {
        _event_track.archiveRead(archive);
    }

	DTsize trackCount;
	*archive << ARCHIVE_DATA(trackCount,DATA_PERSISTENT);
	_tracks.resize(trackCount);
    
	for (DTuint j = 0; j < _tracks.size(); ++j) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_tracks[j]));
	}
		
    archive->popDomain ();
}

void KeyedSpriteAnimation::archiveWrite (Archive *archive)
{    
	BaseClass::archiveWrite(archive);

    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA(_name,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_start_time,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_stop_time,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_speed,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_loop,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_DATA(_groups,DATA_PERSISTENT | DATA_SETTABLE)
        .setBitfield(true);

    _event_track.archiveWrite(archive);

	DTsize trackCount = _tracks.size();
	*archive << ARCHIVE_DATA(trackCount,DATA_PERSISTENT);

	for (DTuint j = 0; j < _tracks.size(); ++j) {
		archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_tracks[j]));
	}

    archive->popDomain ();
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimation::addTrack (KeyedSpriteAnimationTrack* track)
{
	RETAIN(track);
	_tracks.pushBack(track);
}

void KeyedSpriteAnimation::removeTrack (KeyedSpriteAnimationTrack* track)
{
	for (auto i = _tracks.begin(); i != _tracks.end();) {
		if (track == *i) {
			RELEASE(*i);
			i = _tracks.erase(i);
		} else {
			++i;
		}
	}
}

void KeyedSpriteAnimation::removeAllTracks	(void)
{
    FOR_EACH(i,_tracks) {
		RELEASE(*i);
	}
	
	_tracks.clear();
}

KeyedSpriteAnimationTrack* KeyedSpriteAnimation::getTrackByName (const String &track_name) const
{
    FOR_EACH(i,_tracks) {
        if ( (**i).getName() == track_name )
            return *i;
	}
    
    return NULL;
}

//==============================================================================
//==============================================================================
		
} // DT3

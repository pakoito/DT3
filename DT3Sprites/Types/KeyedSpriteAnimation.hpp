#ifndef DT3_KEYEDSPRITEANIMATION
#define DT3_KEYEDSPRITEANIMATION
//==============================================================================
///	
///	File: KeyedSpriteAnimation.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "BaseClass.hpp"
#include "Array.hpp"
#include "PoolAllocator.hpp"
#include "KeyedSpriteAnimationKeyframe.hpp"
#include "KeyedSpriteAnimationEventTrack.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Archive;
class KeyedSpriteAnimationTrack;
class KeyedSpriteAnimationPoseJoint;

//==============================================================================
//==============================================================================

class KeyedSpriteAnimation: public BaseClass {
    public:
        DEFINE_TYPE(KeyedSpriteAnimation, BaseClass)   
		DEFINE_CREATE_AND_CLONE
		//DEFINE_POOL_ALLOCATOR
	
												KeyedSpriteAnimation        (void);	
												KeyedSpriteAnimation        (const KeyedSpriteAnimation &rhs);
        KeyedSpriteAnimation&                   operator =					(const KeyedSpriteAnimation &rhs);		
		virtual									~KeyedSpriteAnimation       (void);
    
        void									archiveRead                 (Archive *archive);
        void									archiveWrite				(Archive *archive);

	public:	
		DEFINE_ACCESSORS_REFERENCED(getName, setName, String, _name);

		/// Description
		/// \param param description
		/// \return description
		void                                    addTrack			(KeyedSpriteAnimationTrack* track);

		/// Description
		/// \param param description
		/// \return description
		void                                    removeTrack         (KeyedSpriteAnimationTrack* track);

		/// Description
		/// \param param description
		/// \return description
		void                                    removeAllTracks     (void);
		
		
		/// Description
		/// \param param description
		/// \return description
		DTuint                                  getNumTracks		(void) const				{	return (DTuint) _tracks.size();	}

		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteAnimationTrack*              getTrack			(DTuint track) const		{	return _tracks[track];	}
        
    
		/// Description
		/// \param param description
		/// \return description
        KeyedSpriteAnimationEventTrack&         getEventTrack       (void)                      {   return _event_track;    }
        const KeyedSpriteAnimationEventTrack&   getEventTrack       (void) const                {   return _event_track;    }

    
        DEFINE_ACCESSORS(getStartTime, setStartTime, DTfloat, _start_time);
        DEFINE_ACCESSORS(getStopTime, setStopTime, DTfloat, _stop_time);
        DEFINE_ACCESSORS(getSpeed, setSpeed, DTfloat, _speed);
        DEFINE_ACCESSORS(getGroups, setGroups, DTushort, _groups);
        DEFINE_ACCESSORS(getLoop, setLoop, DTboolean, _loop);
	
		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteAnimationTrack*              getTrackByName      (const String &track_name) const;

        
	private:
		String                              _name;
        
        DTfloat                             _start_time;
        DTfloat                             _stop_time;
        DTfloat                             _speed;
        DTboolean                           _loop;
        
		Array<KeyedSpriteAnimationTrack*>   _tracks;
        KeyedSpriteAnimationEventTrack      _event_track;
    
        DTushort                            _groups;
};

//==============================================================================
//==============================================================================

} // DT3

#endif


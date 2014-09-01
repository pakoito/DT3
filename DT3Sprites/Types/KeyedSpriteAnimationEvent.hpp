#ifndef DT3_KEYEDSPRITEANIMATIONEVENT
#define DT3_KEYEDSPRITEANIMATIONEVENT
//==============================================================================
///	
///	File: KeyedSpriteAnimationEvent.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "Matrix.hpp"
#include "MaterialResource.hpp"
#include "Array.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Archive;

//==============================================================================
//==============================================================================

class KeyedSpriteAnimationEvent {
    public:
        DEFINE_TYPE_SIMPLE_BASE(KeyedSpriteAnimationEvent)   
	
         
										KeyedSpriteAnimationEvent       (void);	
										KeyedSpriteAnimationEvent       (const KeyedSpriteAnimationEvent &rhs);
        KeyedSpriteAnimationEvent &     operator =						(const KeyedSpriteAnimationEvent &rhs);		
										~KeyedSpriteAnimationEvent      (void);
    
        void							archiveRead                     (Archive *archive);
        void							archiveWrite					(Archive *archive);
		
	public:
        
		/// Returns a unique ID for this key
		/// \param k key index
		/// \return ID
        virtual DTint                   getKeyID                    (void) const                {   return _id;                         }


		DEFINE_ACCESSORS(getTime, setTime, DTfloat, _time);
	
		DEFINE_ACCESSORS(getEventID, setEventID, String, _event_id);
		DEFINE_ACCESSORS(getEventParameter, setEventParameter, String, _event_parameter);

        
		/// Description
		/// \param param description
		/// \return description
        void                            setTriggered                (DTboolean triggered) const {   _triggered = triggered; }
        DTboolean                       getTriggered                (void) const                {   return _triggered;      }

	private:
		friend class KeyedSpriteAnimationEventTrack;

        DTint                               _id;
    
        mutable DTboolean                   _triggered; // Bookkeeping variable

        String                              _event_id;
        String                              _event_parameter;
	
		DTfloat								_time;
};

//==============================================================================
//==============================================================================

inline DTboolean operator < (const KeyedSpriteAnimationEvent &a, const KeyedSpriteAnimationEvent &b)
{
    return a.getTime() < b.getTime();
}

inline DTboolean operator > (const KeyedSpriteAnimationEvent &a, const KeyedSpriteAnimationEvent &b)
{
    return a.getTime() > b.getTime();
}

//==============================================================================
//==============================================================================


} // DT3

#endif


#ifndef DT3_KEYEDSPRITEANIMATIONEVENTTRACK
#define DT3_KEYEDSPRITEANIMATIONEVENTTRACK
//==============================================================================
///	
///	File: KeyedSpriteAnimationEventTrack.hpp
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
#include "KeyedSpriteAnimationEvent.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Archive;

//==============================================================================
//==============================================================================

class KeyedSpriteAnimationEventTrack: public BaseClass {
    public:
        DEFINE_TYPE(KeyedSpriteAnimationEventTrack, BaseClass)   
		DEFINE_CREATE_AND_CLONE
		//DEFINE_POOL_ALLOCATOR
	
												KeyedSpriteAnimationEventTrack	(void);	
												KeyedSpriteAnimationEventTrack	(const KeyedSpriteAnimationEventTrack &rhs);
        KeyedSpriteAnimationEventTrack&         operator =                      (const KeyedSpriteAnimationEventTrack &rhs);		
		virtual									~KeyedSpriteAnimationEventTrack	(void);
    
        void									archiveRead                 (Archive *archive);
        void									archiveWrite				(Archive *archive);

	public:	
    
		/// Sets the keyframes for the track
		/// \param param description
		/// \return description
        void                                        addKeyframe             (const KeyedSpriteAnimationEvent &keyframe);

		/// Returns the keyframes for the track
		/// \param param description
		/// \return description
        const Array<KeyedSpriteAnimationEvent>&     getKeyframes            (void) const                                {   return _keyframes;					}

		/// Returns a unique ID for this key
		/// \param k key index
		/// \return ID
        DTint                                       getKeyID                (DTuint k) const                            {   return _keyframes[k]._id;           }

		/// Returns a time for this key
		/// \param k key index
		/// \return time
        DTfloat                                     getKeyTime              (DTuint k) const                            {   return _keyframes[k]._time;         }

		/// Sets a time for this key
		/// \param k key index
        void                                        setKeyTime              (DTuint k, DTfloat time)                    {   _keyframes[k]._time = time;         }
		
        /// Returns a time for this key
		/// \param k key index
        void                                        eraseKey                (DTuint k)                                  {   _keyframes.erase(k);                }

        /// Returns a time for this key
		/// \param k key index
		/// \return ID
        DTint                                       duplicate               (DTuint k);

		
		/// Description
		/// \param param description
		/// \return description
        void                                        updateKeyframes         (void);


		/// Description
		/// \param param description
		/// \return description
        void                                        fireEvents              (const DTfloat time, std::shared_ptr<Callback2<const String&,const String&> > cb) const;
                
	private:
        DTint                               _id;
                
		Array<KeyedSpriteAnimationEvent> _keyframes;
};

//==============================================================================
//==============================================================================

} // DT3

#endif


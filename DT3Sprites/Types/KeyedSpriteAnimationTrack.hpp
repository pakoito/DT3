#ifndef DT3_KEYEDSPRITEANIMATIONTRACK
#define DT3_KEYEDSPRITEANIMATIONTRACK
//==============================================================================
///	
///	File: KeyedSpriteAnimationTrack.hpp
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

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Archive;
class KeyedSpriteAnimationPoseJoint;

//==============================================================================
//==============================================================================

class KeyedSpriteAnimationTrack: public BaseClass {
    public:
        DEFINE_TYPE(KeyedSpriteAnimationTrack, BaseClass)   
		DEFINE_CREATE_AND_CLONE
		//DEFINE_POOL_ALLOCATOR
	
												KeyedSpriteAnimationTrack	(void);	
												KeyedSpriteAnimationTrack	(const KeyedSpriteAnimationTrack &rhs);
        KeyedSpriteAnimationTrack&				operator =					(const KeyedSpriteAnimationTrack &rhs);		
		virtual									~KeyedSpriteAnimationTrack	(void);
    
        void									archiveRead                 (Archive *archive);
        void									archiveWrite				(Archive *archive);

	public:	
    
		/// Sets the keyframes for the track
		/// \param param description
		/// \return description
        void                                        addKeyframe             (const KeyedSpriteAnimationKeyframe &keyframe);

		/// Returns the keyframes for the track
		/// \param param description
		/// \return description
        const Array<KeyedSpriteAnimationKeyframe>&  getKeyframes            (void) const                                {   return _keyframes;					}

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

		/// Sets the name of the bone for the track
		/// \param param description
		/// \return description
        void                                        setName                 (const String &name);

		/// Returns the name of the bone for the track
		/// \param param description
		/// \return description
        const String                                getName                 (void) const                                {   return _name;						}
		
		/// Returns the name hash of the object
		/// \param param description
		/// \return description
        DTuint                                      getNameHash             (void) const								{   return _name_hash;					}
		
		/// Description
		/// \param param description
		/// \return description
        void                                        updateKeyframes         (void);


		/// Description
		/// \param param description
		/// \return description
		void                                        updateJoint             (const DTfloat time, KeyedSpriteAnimationKeyframe &joint) const;
		
	private:
        DTint                               _id;
    
		String                              _name;
		DTuint                              _name_hash;
                
		Array<KeyedSpriteAnimationKeyframe> _keyframes;
};

//==============================================================================
//==============================================================================

} // DT3

#endif


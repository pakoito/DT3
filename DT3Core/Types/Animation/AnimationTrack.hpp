#ifndef DT3_ANIMATIONTRACK
#define DT3_ANIMATIONTRACK
//==============================================================================
///	
///	File: AnimationTrack.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Animation/AnimationKeyframe.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include <string>
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Archive;
class AnimationPoseJoint;

//==============================================================================
/// Class
//==============================================================================

class AnimationTrack: public BaseClass {
    public:
        DEFINE_TYPE(AnimationTrack, BaseClass)   
		DEFINE_CREATE_AND_CLONE
	
										AnimationTrack			(void);	
										AnimationTrack			(const AnimationTrack &rhs);
										AnimationTrack			(AnimationTrack &&rhs);
        AnimationTrack &				operator =				(const AnimationTrack &rhs);
        AnimationTrack &				operator =				(AnimationTrack &&rhs);
										~AnimationTrack			(void);
    
        void							archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Sets the keyframes for the track
		/// \param keyframes array of keyframes
        void                                    set_keyframes           (const std::vector<AnimationKeyframe> &keyframes)	{   _keyframes = keyframes;				}

		/// Returns the keyframes for the track
		/// \return keyframes
        const std::vector<AnimationKeyframe>&	keyframes               (void) const                {   return _keyframes;					}


		/// Sets the name of the bone for the track
		/// \param name bone name
        void                                    set_name                (const std::string &name)   {   _name_hash = MoreStrings::hash(name);	_name = name;	}

		/// Returns the name of the bone for the track
		/// \return bone name
        const std::string &                     name                    (void) const                {   return _name;						}
		
		/// Returns the hashed name of the object
		/// \return hashed bone name
        DTuint                                  name_hash               (void) const                {   return _name_hash;					}
		


		/// Updates and animation joint at a given time
		/// \param time time to update the joint to
		/// \param joint joint
		void                                    update_joint            (const DTfloat time, AnimationPoseJoint &joint) const;
		
	private:
		std::string                         _name;
		DTuint                              _name_hash;
		std::vector<AnimationKeyframe>		_keyframes;
            	
};

//==============================================================================
//==============================================================================

} // DT3

#endif


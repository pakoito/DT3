#ifndef DT3_KEYEDSPRITEANIMATIONPOSE
#define DT3_KEYEDSPRITEANIMATIONPOSE
//==============================================================================
///	
///	File: KeyedSpriteAnimationPose.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "BaseClass.hpp"
#include "Array.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Archive;
class KeyedSpriteAnimation;

//==============================================================================
//==============================================================================

class KeyedSpriteAnimationPose: public BaseClass {
    public:
        DEFINE_TYPE(KeyedSpriteAnimationPose,BaseClass)   
		DEFINE_CREATE_AND_CLONE
         
                                    KeyedSpriteAnimationPose    (void);	
                                    KeyedSpriteAnimationPose    (const KeyedSpriteAnimationPose &rhs);
        KeyedSpriteAnimationPose&   operator =                  (const KeyedSpriteAnimationPose &rhs);		
                                    ~KeyedSpriteAnimationPose   (void);
    
        void					archiveRead         (Archive *archive);
        void					archiveWrite		(Archive *archive);
		
	public:
		/// Calculates the pose for the object
		/// \param param description
		/// \return description
        void					update				(const DTfloat time, const KeyedSpriteAnimation *animation, std::shared_ptr<Callback2<const String&,const String&> > cb );
			
		/// Calculates the pose for the object
		/// \param param description
		/// \return description
		DTboolean				getJointState       (DTuint joint_name_hash, KeyedSpriteAnimationPoseJoint &joint) const;

	private:
        struct JointState {
            KeyedSpriteAnimationKeyframe    _keyframe;
            DTboolean                       _updated;
        };
    
		std::map<DTuint, JointState>        _joints;
};

//==============================================================================
//==============================================================================

} // DT3

#endif


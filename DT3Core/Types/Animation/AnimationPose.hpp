#ifndef DT3_ANIMATIONPOSE
#define DT3_ANIMATIONPOSE
//==============================================================================
///	
///	File: AnimationPose.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Animation/AnimationPoseJoint.hpp"
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Archive;
class AnimationResource;

//==============================================================================
/// Class
//==============================================================================

class AnimationPose: public BaseClass {
    public:
        DEFINE_TYPE(AnimationPose,BaseClass)   
		DEFINE_CREATE_AND_CLONE
         
								AnimationPose		(void);	
								AnimationPose		(const AnimationPose &rhs);
								AnimationPose		(AnimationPose &&rhs);
        AnimationPose &			operator =			(const AnimationPose &rhs);
        AnimationPose &			operator =			(AnimationPose &&rhs);
								~AnimationPose		(void);
    		
	public:
		/// Calculates the pose for the object
		/// \param time time which the pose will be calculated for
		/// \param animation animation to use for calculating the pose
        void					update				(const DTfloat time, const std::shared_ptr<AnimationResource> &animation);
	
		/// Returns the transform of the joint given the hash of its name
		/// \param joint_name_hash hash of the name of the joint
		/// \param transform returned transform
		/// \return success
		DTboolean				joint_transform     (DTuint joint_name_hash, Matrix4 &transform) const;
		
	private:
		std::map<DTuint, AnimationPoseJoint>	_joints;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, AnimationPose* &r);
Stream& operator >>(Stream &s, AnimationPose* &r);

Stream& operator <<(Stream &s, const std::shared_ptr<AnimationPose> &r);
Stream& operator >>(Stream &s, std::shared_ptr<AnimationPose> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif


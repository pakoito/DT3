#ifndef DT3_ANIMATIONPOSEJOINT
#define DT3_ANIMATIONPOSEJOINT
//==============================================================================
///	
///	File: AnimationPoseJoint.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Quaternion.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Archive;

//==============================================================================
/// Class
//==============================================================================

class AnimationPoseJoint {
    public:
        DEFINE_TYPE_SIMPLE_BASE(AnimationPoseJoint)   
	
         
								AnimationPoseJoint	(void);	
								AnimationPoseJoint	(const AnimationPoseJoint &rhs);
        AnimationPoseJoint &	operator =			(const AnimationPoseJoint &rhs);		
								~AnimationPoseJoint	(void);
    		
	public:

		/// Returns the hashed name of the object
		/// \return hashed name
        DTuint					name_hash           (void) const				{   return _name_hash;					}

		/// Sets the hashed name of the object
		/// \param name_hash hash value to set
        void					set_name_hash		(const DTuint name_hash)	{   _name_hash = name_hash;				}


		/// Sets cached index of the keyframe
		/// \param keyframe_cache cached index
		void					set_keyframe_cache	(DTuint keyframe_cache)		{	_keyframe_cache = keyframe_cache;	}
		
		/// Returns the cached index of the keyframe
		/// \return cached keyframe index
		DTuint					keyframe_cache      (void) const				{	return _keyframe_cache;				}


		/// Sets transform of the key frame
		/// \param transform transform
		void					set_transform		(const Matrix4 &transform);

		/// Sets transform of the key frame
		/// \param orientation orientation of joint
		/// \param translation orientation of joint
		void					set_transform		(const Quaternion &orientation, const Vector3 &translation);
		
		
		/// Returns orientation of the joint as a Quaternion
		/// \return transform
		const Quaternion &      orientation         (void) const				{	return _orientation;				}

		/// Returns translation of the key frame
		/// \return translation
		const Vector3 &         translation         (void) const				{	return _translation;				}


	public:
		DTuint					_name_hash;
		DTuint					_keyframe_cache;
		Quaternion				_orientation;
		Vector3					_translation;
};

//==============================================================================
//==============================================================================

} // DT3

#endif


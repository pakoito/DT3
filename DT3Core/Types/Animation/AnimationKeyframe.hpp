#ifndef DT3_ANIMATIONKEYFRAME
#define DT3_ANIMATIONKEYFRAME
//==============================================================================
///	
///	File: AnimationKeyframe.hpp
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
class Matrix4;

//==============================================================================
/// Class
//==============================================================================

class AnimationKeyframe {
    public:
        DEFINE_TYPE_SIMPLE_BASE(AnimationKeyframe)   
	
         
								AnimationKeyframe	(void);	
								AnimationKeyframe	(const AnimationKeyframe &rhs);
        AnimationKeyframe &		operator =			(const AnimationKeyframe &rhs);		
								~AnimationKeyframe	(void);
    
        void					archive             (const std::shared_ptr<Archive> &archive);
		
	public:

		/// Sets transform of the key frame
		/// \param transform transform
		void					set_transform		(const Matrix4 &transform);

		/// Sets transform of the key frame
		/// \param transform transform
		void					set_transform		(const Quaternion &orientation, const Vector3 &translation);
		
		
		/// Returns transform of the key frame
		/// \return transform
		const Quaternion &      orientation         (void) const				{	return _orientation;				}

		/// Returns transform of the key frame
		/// \return transform
		const Vector3 &         translation         (void) const				{	return _translation;				}

        DEFINE_ACCESSORS(time, set_time, DTfloat, _time)

	public:
		DTfloat					_time;
		Quaternion				_orientation;
		Vector3					_translation;
};

//==============================================================================
//==============================================================================

} // DT3

#endif


//==============================================================================
///	
///	File: AnimationKeyframe.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Animation/AnimationKeyframe.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

AnimationKeyframe::AnimationKeyframe (void)
	:	_time			(0.0F)
{

}

AnimationKeyframe::AnimationKeyframe (const AnimationKeyframe &rhs)
	:	_time			(rhs._time),
		_orientation	(rhs._orientation),
		_translation	(rhs._translation)
{

}
            
AnimationKeyframe& AnimationKeyframe::operator = (const AnimationKeyframe &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {		
		_time = rhs._time;
		_orientation = rhs._orientation;
		_translation = rhs._translation;
    }
    return (*this);
}	

AnimationKeyframe::~AnimationKeyframe (void)
{

}

//==============================================================================
//==============================================================================

void AnimationKeyframe::archive (const std::shared_ptr<Archive> &archive)
{    
	*archive << ARCHIVE_DATA_RAW(_time, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA_RAW(_orientation, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA_RAW(_translation, DATA_PERSISTENT);
}

//==============================================================================
//==============================================================================

void AnimationKeyframe::set_transform (const Matrix4 &transform)
{
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();
		
	_orientation = Quaternion(orientation);
	_translation = translation;
}

void AnimationKeyframe::set_transform (const Quaternion &orientation, const Vector3 &translation)
{
	_orientation = orientation;
	_translation = translation;
}

//==============================================================================
//==============================================================================

} // DT3

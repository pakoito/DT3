#ifndef DT3_SKELETONJOINT
#define DT3_SKELETONJOINT
//==============================================================================
///	
///	File: SkeletonJoint.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include <vector>

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

class SkeletonJoint {
    public:
        DEFINE_TYPE_SIMPLE_BASE(SkeletonJoint)   
	
										SkeletonJoint           (void);
										SkeletonJoint           (const SkeletonJoint &rhs);
										SkeletonJoint           (SkeletonJoint &&rhs);
        SkeletonJoint &					operator =              (const SkeletonJoint &rhs);
        SkeletonJoint &					operator =              (SkeletonJoint &&rhs);
										~SkeletonJoint          (void);
    
        void							archive                 (const std::shared_ptr<Archive> &archive);
		
	public:

		/// Sets the name of the object
		/// \param param description
		/// \return description
        void							set_name                (const std::string &name)   {   _name_hash = MoreStrings::hash(name);	_name = name;	}

		/// Returns the name of the object
		/// \param param description
		/// \return description
        const std::string &             name                    (void) const                {   return _name;					}
		
		/// Returns the name hash of the joint
		/// \param param description
		/// \return description
        DTuint							name_hash               (void) const                {   return _name_hash;				}


		/// Sets the index of the matrix that weights refer to
		/// \param offset offset of matrix
        void							set_joint_index         (const DTuint index)		{   _index = index;					}

		/// Returns the index of the matrix that weights refer to
		/// \return matrix offset
        DTuint							joint_index             (void) const				{   return _index;					}


		/// Sets the local transform
		/// \param transform transform
		void							set_local_transform     (const Matrix4 &transform)	{	_local_transform = transform;	_local_transform_inverse = _local_transform.inversed();	}
		
		/// Gets the local transform
		/// \param transform transform
		const Matrix4 &					local_transform         (void) const				{	return _local_transform;			}

		/// Gets the local transform inverse
		/// \param transform transform
		const Matrix4 &					local_transform_inverse (void) const                {	return _local_transform_inverse;	}


		/// Sets the local transform
		/// \param transform transform
		void							set_world_transform     (const Matrix4 &transform)	{	_world_transform = transform;	_world_transform_inverse = _world_transform.inversed();		}
		
		/// Gets the local transform
		/// \param transform transform
		const Matrix4 &					world_transform         (void) const				{	return _world_transform;			}

		/// Gets the local transform inverse
		/// \param transform transform
		const Matrix4 &					world_transform_inverse (void) const                {	return _world_transform_inverse;	}


		/// Sets the children of the joint
		/// \param children Array of children
		void                                set_children        (const std::vector<SkeletonJoint> &children)    {	_children = children;	}
		
		/// Returns the children of the joint
		/// \return children
		const std::vector<SkeletonJoint> &  children            (void) const                                    {	return _children;	}


	private:
		std::string                 _name;
		DTuint                      _name_hash;
		DTuint                      _index;
		
		Matrix4                     _local_transform;
		Matrix4                     _local_transform_inverse;

		Matrix4                     _world_transform;
		Matrix4                     _world_transform_inverse;

		std::vector<SkeletonJoint>	_children;
};

//==============================================================================
//==============================================================================

} // DT3

#endif


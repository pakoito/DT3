#ifndef DT3_SKELETON
#define DT3_SKELETON
//==============================================================================
///	
///	File: Skeleton.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Animation/SkeletonJoint.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================



//==============================================================================
/// Class
//==============================================================================

class Skeleton: public BaseClass {
    public:
        DEFINE_TYPE(Skeleton, BaseClass)
	
                                            Skeleton            (void);	
                                            Skeleton            (const Skeleton &rhs);
                                            Skeleton            (Skeleton &&rhs);
        Skeleton &                          operator =			(const Skeleton &rhs);
        Skeleton &                          operator =			(Skeleton &&rhs);
                                            ~Skeleton           (void);
    
    
		/// Sets the skeleton for the geometry
		/// \param skeleton skeleton
		void                                set_skeleton        (const std::vector<SkeletonJoint>& skeleton);

		/// Gets the skeleton for the geometry
		/// \return skeleton
		const std::vector<SkeletonJoint>&   skeleton            (void) const        {	return _skeleton;			}
        
		/// Gets the size of the skeleton for the geometry
		/// \return skeleton size
        DTuint                              skeleton_size       (void) const        {   return _num_joints;         }

    
	public:
		std::vector<SkeletonJoint>          _skeleton;
        DTuint                              _num_joints;

		void                                analyze_skeleton    (const std::vector<SkeletonJoint>& joints);
};

//==============================================================================
//==============================================================================

} // DT3

#endif


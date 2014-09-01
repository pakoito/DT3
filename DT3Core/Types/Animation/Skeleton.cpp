//==============================================================================
///	
///	File: Skeleton.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Animation/Skeleton.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Skeleton::Skeleton (void)
    :   _num_joints (0)
{

}

Skeleton::Skeleton (const Skeleton &rhs)
    :   _skeleton   (rhs._skeleton),
        _num_joints (rhs._num_joints)
{

}

Skeleton::Skeleton (Skeleton &&rhs)
    :   _skeleton   (std::move(rhs._skeleton)),
        _num_joints (rhs._num_joints)
{

}
    
Skeleton& Skeleton::operator = (const Skeleton &rhs)
{
    _skeleton = rhs._skeleton;
    _num_joints = rhs._num_joints;

    return (*this);
}	

Skeleton& Skeleton::operator = (Skeleton &&rhs)
{
    _skeleton = std::move(rhs._skeleton);
    _num_joints = rhs._num_joints;

    return (*this);
}	

Skeleton::~Skeleton (void)
{

}
		
//==============================================================================		
//==============================================================================

void Skeleton::set_skeleton (const std::vector<SkeletonJoint>& skeleton)
{	
    _skeleton = skeleton;   
    _num_joints = 0;    
    analyze_skeleton (_skeleton);
}

void Skeleton::analyze_skeleton (const std::vector<SkeletonJoint>& joints)
{
	// Calculate every sub joint
    for (auto &joint : joints) {
        ++_num_joints;
        
        analyze_skeleton (joint.children());
	}

}

//==============================================================================
//==============================================================================

} // DT3

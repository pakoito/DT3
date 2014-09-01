//==============================================================================
///	
///	File: Weights.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Weights.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Weights::Weights (	DTushort bone_1, DTfloat weight_1, 
					DTushort bone_2, DTfloat weight_2,
					DTushort bone_3, DTfloat weight_3, 
					DTushort bone_4, DTfloat weight_4)
	:	_bone_1		(bone_1),
		_bone_2		(bone_2),
		_bone_3		(bone_3),
		_bone_4		(bone_4),
		_weight_1	(weight_1),
		_weight_2	(weight_2),
		_weight_3	(weight_3),
		_weight_4	(weight_4)
{

}

Weights::Weights (const Weights &rhs)
    :	_bone_1(rhs._bone_1),
		_bone_2(rhs._bone_2),
		_bone_3(rhs._bone_3),
		_bone_4(rhs._bone_4),
		_weight_1(rhs._weight_1),
		_weight_2(rhs._weight_2),
		_weight_3(rhs._weight_3),
		_weight_4(rhs._weight_4)
{

}

Weights& Weights::operator = (const Weights &rhs)
{	
	_bone_1 = rhs._bone_1;
	_bone_2 = rhs._bone_2;
	_bone_3 = rhs._bone_3;
	_bone_4 = rhs._bone_4;
    
	_weight_1 = rhs._weight_1;
	_weight_2 = rhs._weight_2;
	_weight_3 = rhs._weight_3;
	_weight_4 = rhs._weight_4;
	   
    return (*this);
}	

//==============================================================================
//==============================================================================

void Weights::set (	DTushort bone_1, DTfloat weight_1, 
					DTushort bone_2, DTfloat weight_2,
					DTushort bone_3, DTfloat weight_3, 
					DTushort bone_4, DTfloat weight_4)
{
	_bone_1 = bone_1;
	_bone_2 = bone_2;
	_bone_3 = bone_3;
	_bone_4 = bone_4;

	_weight_1 = weight_1;
	_weight_2 = weight_2;
	_weight_3 = weight_3;
	_weight_4 = weight_4;
}

//==============================================================================
//==============================================================================

void Weights::normalize_weights	(void)
{
	DTfloat sum = _weight_1 + _weight_2 + _weight_3 + _weight_4;
    if (sum > 0.0F) {
        _weight_1 /= sum;
        _weight_2 /= sum;
        _weight_3 /= sum;
        _weight_4 /= sum;
    } else {
        _bone_1 = 0;
        _bone_2 = 0;
        _bone_3 = 0;
        _bone_4 = 0;

        _weight_1 = 1.0F;   // Default to weighting to first bone
        _weight_2 = 0.0F;
        _weight_3 = 0.0F;
        _weight_4 = 0.0F;
    }
    
    // Fix other weights
    if (_weight_2 == 0.0F)      _bone_2 = _bone_1;
    if (_weight_3 == 0.0F)      _bone_3 = _bone_1;
    if (_weight_4 == 0.0F)      _bone_4 = _bone_1;
}

//==============================================================================
//==============================================================================

} // DT3

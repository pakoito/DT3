#ifndef DT3_WEIGHTS
#define DT3_WEIGHTS
//==============================================================================
///	
///	File: Weights.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class Weights {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Weights)   
	
		inline					Weights				(void)  {}
								Weights				(   DTushort bone_1, DTfloat w1, 
														DTushort bone_2, DTfloat w2,
														DTushort bone_3, DTfloat w3, 
														DTushort bone_4, DTfloat w4	);
    
								Weights				(const Weights &rhs);
        Weights &				operator =			(const Weights &rhs);		
								~Weights			(void)  {}
                
    public:		

		DTboolean				operator ==			(const Weights& rhs) const   {	return	_weight_1 == rhs._weight_1 &&
                                                                                            _weight_2 == rhs._weight_2 &&
                                                                                            _weight_3 == rhs._weight_3 &&
                                                                                            _weight_4 == rhs._weight_4 &&
																							_bone_1 == rhs._bone_1 &&
                                                                                            _bone_2 == rhs._bone_2 &&
                                                                                            _bone_3 == rhs._bone_3 &&
                                                                                            _bone_4 == rhs._bone_4;     }

		DTboolean				operator !=			(const Weights& rhs) const   {	return	_weight_1 != rhs._weight_1 ||
                                                                                            _weight_2 != rhs._weight_2 ||
                                                                                            _weight_3 != rhs._weight_3 ||
                                                                                            _weight_4 != rhs._weight_4 ||
																							_bone_1 != rhs._bone_1 ||
                                                                                            _bone_2 != rhs._bone_2 ||
                                                                                            _bone_3 != rhs._bone_3 ||
                                                                                            _bone_4 != rhs._bone_4;     }

		/// Set the bones and weights
		/// \param bone_1 index of bone
		/// \param w1 weight of bone
		/// \param bone_2 index of bone
		/// \param w2 weight of bone
		/// \param bone_3 index of bone
		/// \param w3 weight of bone
		/// \param bone_4 index of bone
		/// \param w4 weight of bone
		void					set					(   DTushort bone_1, DTfloat w1,
														DTushort bone_2, DTfloat w2,
														DTushort bone_3, DTfloat w3, 
														DTushort bone_4, DTfloat w4	);
    
		/// Normalize the weights of the bones to that the weights sum to 1.0
		void					normalize_weights	(void);
    
    
        DEFINE_ACCESSORS(bone_1, set_bone_1, DTushort, _bone_1)
        DEFINE_ACCESSORS(bone_2, set_bone_2, DTushort, _bone_1)
        DEFINE_ACCESSORS(bone_3, set_bone_3, DTushort, _bone_1)
        DEFINE_ACCESSORS(bone_4, set_bone_4, DTushort, _bone_1)

        DEFINE_ACCESSORS(weight_1, set_weight_1, DTfloat, _weight_1)
        DEFINE_ACCESSORS(weight_2, set_weight_2, DTfloat, _weight_2)
        DEFINE_ACCESSORS(weight_3, set_weight_3, DTfloat, _weight_3)
        DEFINE_ACCESSORS(weight_4, set_weight_4, DTfloat, _weight_4)

	private:
		
		DTushort			_bone_1;
		DTushort			_bone_2;
		DTushort			_bone_3;
		DTushort			_bone_4;

		DTfloat				_weight_1;
		DTfloat				_weight_2;
		DTfloat				_weight_3;
		DTfloat				_weight_4;
};

//==============================================================================
// TODO: Fix these
//==============================================================================

inline Weights operator + (const Weights &a, const Weights &b)
{
    return a;
}

inline Weights operator * (const Weights &a, const DTfloat b)
{
    return a;
}

//==============================================================================
//==============================================================================

} // DT3

#endif


#ifndef DT3_WEIGHTSINDEX
#define DT3_WEIGHTSINDEX
//==============================================================================
///	
///	File: WeightsIndex.hpp
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

class Stream;

//==============================================================================
/// Class
//==============================================================================

class WeightsIndex {
    public:
        DEFINE_TYPE_SIMPLE_BASE(WeightsIndex)   
	   
                                WeightsIndex		(void);
								WeightsIndex		(const DTuint w0, const DTuint w1, const DTuint w2, const DTuint w3)  {	w[0] = w0; w[1] = w1; w[2] = w2; w[3] = w3;	    }
								WeightsIndex		(const WeightsIndex &rhs);
        WeightsIndex &			operator =			(const WeightsIndex &rhs);		
                                ~WeightsIndex		(void)  {}
                
    public:
    
		DTboolean               operator ==			(const WeightsIndex& rhs) const   {	return w[0] == rhs.w[0] && w[1] == rhs.w[1] && w[2] == rhs.w[2] && w[3] == rhs.w[3];	}
		DTboolean               operator !=			(const WeightsIndex& rhs) const   {	return w[0] != rhs.w[0] || w[1] != rhs.w[1] || w[2] != rhs.w[2] || w[3] != rhs.w[3];	}
				
    public:
		DTuint                 w[4];
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const WeightsIndex&t);
Stream& operator >>(Stream &s, WeightsIndex&t);

//==============================================================================
//==============================================================================

} // DT3

#endif


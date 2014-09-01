//==============================================================================
///	
///	File: WeightsIndex.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/WeightsIndex.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

WeightsIndex::WeightsIndex (void)
{
	w[0] = 0;
	w[1] = 0;
	w[2] = 0;
	w[3] = 0;
}

WeightsIndex::WeightsIndex (const WeightsIndex &rhs)
{
	w[0] = rhs.w[0];
	w[1] = rhs.w[1];
	w[2] = rhs.w[2];
	w[3] = rhs.w[3];
}
    
WeightsIndex& WeightsIndex::operator = (const WeightsIndex &rhs)
{	
	w[0] = rhs.w[0];
	w[1] = rhs.w[1];
	w[2] = rhs.w[2];
	w[3] = rhs.w[3];
    
    return (*this);
}	

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const WeightsIndex &t)
{
	s << t.w[0] << Stream::fs << t.w[1] << Stream::fs << t.w[2] << Stream::fs << t.w[3];
	return s;
}

Stream& operator >>(Stream &s, WeightsIndex &t)
{
	s >> t.w[0] >> t.w[1] >> t.w[2] >> t.w[3];
	return s;
}

//==============================================================================
//==============================================================================

} // DT3

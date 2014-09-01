#ifndef DT3_TRIANGLE
#define DT3_TRIANGLE
//==============================================================================
///	
///	File: Triangle.hpp
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

class Archive;

//==============================================================================
/// Class
//==============================================================================

class Triangle {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Triangle)   
	
		inline					Triangle			(void)  {}
								Triangle			(const DTushort v0, const DTushort v1, const DTushort v2);
								Triangle			(const Triangle &rhs);
        Triangle &				operator =			(const Triangle &rhs);		
								~Triangle			(void);
                
    public:
    
		DTboolean               operator ==			(const Triangle& rhs) const   {	return v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2];    }
		DTboolean               operator !=			(const Triangle& rhs) const   {	return v[0] != rhs.v[0] || v[1] != rhs.v[1] || v[2] != rhs.v[2];	}
				
    public:
		DTuint                  v[3];
};

//==============================================================================
//==============================================================================

} // DT3

#endif


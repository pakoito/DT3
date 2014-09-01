#ifndef DT3_SPHERE
#define DT3_SPHERE
//==============================================================================
///	
///	File: Sphere.hpp
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

class Sphere {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Sphere)

         
									Sphere					(void);	
		explicit					Sphere					(DTfloat _radius);
									Sphere					(const Sphere &rhs);
        Sphere &					operator =				(const Sphere &rhs);
									~Sphere					(void);
        
    public:
    
        DEFINE_ACCESSORS(radius, set_radius, DTfloat, _radius)
    
    private:
        DTfloat						_radius;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Sphere &v);
Stream& operator >>(Stream &s, Sphere &v);

//==============================================================================
//==============================================================================

} // DT3

#endif

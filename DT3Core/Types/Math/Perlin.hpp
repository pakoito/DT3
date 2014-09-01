#ifndef DT3_PERLIN
#define DT3_PERLIN
//==============================================================================
///	
///	File: Perlin.hpp
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

class Vector2;
class Vector3;

//==============================================================================
/// Class
//==============================================================================

class Perlin {
    private:		
							Perlin                  (void);	
							Perlin                  (const Perlin &rhs);
		Perlin &            operator =              (const Perlin &rhs);		
							~Perlin                 (void);
												
     
	public:
		/// Description
		/// \param param description
		/// \return description
        static DTfloat          noise_1D            (DTfloat x, DTfloat num_octaves = 1);
        	
		/// Description
		/// \param param description
		/// \return description
        static DTfloat          noise_2D            (DTfloat x, DTfloat y, DTfloat num_octaves = 1);
    
		/// Description
		/// \param param description
		/// \return description
        static DTfloat          noise_2D            (const Vector2 &p, DTfloat num_octaves = 1);
        	
		/// Description
		/// \param param description
		/// \return description
        static DTfloat          noise_3D            (DTfloat x, DTfloat y, DTfloat z, DTfloat num_octaves = 1);
    
		/// Description
		/// \param param description
		/// \return description
        static DTfloat          noise_3D            (const Vector3 &p, DTfloat num_octaves = 1);

	private:
        static DTfloat          fade                (DTfloat t);
        static DTfloat          lerp                (DTfloat t, DTfloat a, DTfloat b);
        static DTfloat          grad                (DTint hash, DTfloat x, DTfloat y, DTfloat z);

        static const DTubyte    p[512];

};

//==============================================================================
//==============================================================================

} // DT3

#endif

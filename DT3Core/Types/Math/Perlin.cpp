//==============================================================================
///	
///	File: Perlin.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Perlin.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Random constant data
//==============================================================================

const DTubyte Perlin::p[512] = { 
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,
    99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,
    11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,
    139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,
    245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,
    196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,
    183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,
    253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,
    238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,
    199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,
    67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,160,137,91,90,15,131,13,
    201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,
    247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,
    87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,
    111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,
    161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,
    100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,
    212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,
    154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,
    232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,
    241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,
    115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,
    78,66,215,61,156,180
};

//==============================================================================
//==============================================================================

DTfloat Perlin::fade(DTfloat t)
{ 
    return t * t * t * (t * (t * 6 - 15) + 10); 
}

DTfloat Perlin::lerp(DTfloat t, DTfloat a, DTfloat b)
{
    return a + t * (b - a);
}

DTfloat Perlin::grad(DTint hash, DTfloat x, DTfloat y, DTfloat z)
{
    DTint       h = hash & 15;
    DTfloat     u = h < 8 ? x : y, v = h < 4 ? y : h==12||h==14 ? x : z;
    
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

//==============================================================================
//==============================================================================

DTfloat Perlin::noise_1D (DTfloat x, DTfloat num_octaves)
{
    DTfloat r = 0.0F;
    
    DTfloat a = 1.0F;
    DTfloat f = 1.0F;
    
    for (DTuint o = 0; o < num_octaves; ++o) {

        DTfloat fx = std::floor(x*f);

        DTint   X = (DTint)fx & 255;        // find interval that contains the point
                
        x -= fx;                            // find relative point in interval
        
        DTfloat u = fade(x);                // compute fade curves for x
            
        DTint   A = p[X],                   // Hash coordinates of interval corners
                B = p[X+1];
        
        r +=    a * (
                    lerp(u, grad(p[A], x, 0.0F, 0.0F), grad(p[B], x-1, 0.0F, 0.0F))
                    );
        
        a *= 0.5F;
        f *= 2.0F;
    }
    
    return r;
}

//==============================================================================
//==============================================================================

DTfloat Perlin::noise_2D(DTfloat x, DTfloat y, DTfloat num_octaves)
{
    DTfloat r = 0.0F;
    
    DTfloat a = 1.0F;
    DTfloat f = 1.0F;
    
    for (DTuint o = 0; o < num_octaves; ++o) {

        DTfloat fx = std::floor(x*f);
        DTfloat fy = std::floor(y*f);

        DTint   X = (DTint)fx & 255,        // find unit square that contains the point
                Y = (DTint)fy & 255;
                
        x -= fx;                            // find relative xy of point in square
        y -= fy;                              
        
        DTfloat u = fade(x),                // compute fade curves for x,y
                v = fade(y);
        
        DTint   A = p[X],                   // Hash coordinates of square corners
                AA = p[A]+Y,
                B = p[X+1],
                BB = p[B]+Y;
        
        r +=    a * (   lerp(v,
                            lerp(u, grad(p[A], x, y, 0.0F), grad(p[B], x-1, y, 0.0F)),
                            lerp(u, grad(p[AA], x, y-1, 0.0F), grad(p[BB], x-1, y-1, 0.0F))
                            )
                    );
        
        a *= 0.5F;
        f *= 2.0F;
    }
    
    return r;
}

DTfloat Perlin::noise_2D (const Vector2 &pos, DTfloat num_octaves)
{
    return noise_2D(pos.x, pos.y, num_octaves);
}
    
//==============================================================================
//==============================================================================

DTfloat Perlin::noise_3D(DTfloat x, DTfloat y, DTfloat z, DTfloat num_octaves)
{
    DTfloat r = 0.0F;
    
    DTfloat a = 1.0F;
    DTfloat f = 1.0F;
    
    for (DTuint o = 0; o < num_octaves; ++o) {

        DTfloat fx = std::floor(x*f);
        DTfloat fy = std::floor(y*f);
        DTfloat fz = std::floor(z*f);

        DTint   X = (DTint)fx & 255,        // find unit cube that contains the point
                Y = (DTint)fy & 255,                
                Z = (DTint)fz & 255;
                
        x -= fx;                            // find relative xyz of point in cube
        y -= fy;                              
        z -= fz;
        
        DTfloat u = fade(x),                // compute fade curves for x,y,z
                v = fade(y),                
                w = fade(z);
            
        DTint   A = p[X]+Y,                 // Hash coordinates of cube corners
                AA = p[A]+Z, 
                AB = p[A+1]+Z, 
                B = p[X+1]+Y, 
                BA = p[B]+Z, 
                BB = p[B+1]+Z;
        
        r +=    a * (
                    lerp(w,lerp(v,lerp(u, grad(p[AA  ], x, y, z),
                    grad(p[BA  ], x-1, y, z)),
                    lerp(u, grad(p[AB  ], x, y-1, z),grad(p[BB  ], x-1, y-1, z))),
                    lerp(v, lerp(u, grad(p[AA+1], x, y, z-1 ),
                    grad(p[BA+1], x-1, y, z-1)),  
                    lerp(u, grad(p[AB+1], x, y-1, z-1), grad(p[BB+1], x-1, y-1, z-1))))
                );
        
        a *= 0.5F;
        f *= 2.0F;
    }
    
    return r;
}

DTfloat Perlin::noise_3D(const Vector3 &pos, DTfloat num_octaves)
{
    return noise_3D(pos.x, pos.y, pos.z, num_octaves);
}

//==============================================================================
//==============================================================================
		
		
		
		
} // DT3

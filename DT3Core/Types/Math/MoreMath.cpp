//==============================================================================
///	
///	File: MoreMath.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
// MT Random number generator from http://www.qbrundage.com/michaelb/pubs/essays/random_number_generation.html
//==============================================================================

#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

const DTfloat	PI = 3.14159265F;	
const DTfloat	PI_INV = 1.0F/PI;	
const DTfloat	TWO_PI = 2.0F * PI;	
const DTfloat	TWO_PI_INV = 1.0F/TWO_PI;	
const DTfloat	HALF_PI = 0.5F * PI;	
const DTfloat	E = 2.71828183F;	
const DTfloat	SQRT_2 = 1.41421356F;	
const DTfloat	SQRT_HALF = 0.707106781F;	
const DTfloat	DEG_TO_RAD = PI/180.0F;
const DTfloat	RAD_TO_DEG = 180.0F/PI;
const DTfloat   EPSILON = 0.000001F;

//==============================================================================
//==============================================================================

DTuint		MoreMath::_crc_table[CRC_TABLE_SIZE];
DTuint		MoreMath::_random_seed = 0;
DTfloat		MoreMath::_factorial[FACTORIAL_TABLE_SIZE];
DTfloat		MoreMath::_factorial_inv[FACTORIAL_TABLE_SIZE];

DTubyte		MoreMath::_4_to_8_bit[16];
DTubyte		MoreMath::_5_to_8_bit[32];
DTubyte     MoreMath::_6_to_8_bit[64];

// For Mersenne Twister
DTint       MoreMath::_mt_index;
DTuint      MoreMath::_mt_buffer[MT_LEN];

DTubyte     MoreMath::_entropy_pool[] = { 0 };
DTushort    MoreMath::_entropy_pool_pos = 0;

//==============================================================================
//==============================================================================

GLOBAL_INITIALIZATION(MoreMath::init_tables())

//==============================================================================
//==============================================================================

void MoreMath::init_tables(void)
{    
	// initialize CRC Table
	for (DTint i = 0; i < CRC_TABLE_SIZE; ++i) {
		DTuint crc = i;
		for (DTint j = 8; j > 0; --j) {
			if (crc & 1)    crc = (crc >> 1) ^ 0xEDB88320L;
			else            crc >>= 1;
		}
		_crc_table[i] = crc;
	}
	
	// initialize factorial table
	_factorial[0] = 0.0F;
	_factorial[1] = 1.0F;
	for (DTuint i = 2; i < FACTORIAL_TABLE_SIZE; ++i) {
		_factorial[i] = _factorial[i-1] * i;
		_factorial_inv[i] = 1.0F / _factorial[i];
	}
	
    // Conversion tables
    for (DTuint i = 0; i < ARRAY_SIZE(_4_to_8_bit); ++i) {
        _4_to_8_bit[i] = static_cast<DTubyte>(255 * i / (ARRAY_SIZE(_4_to_8_bit)-1));
    }
    
    for (DTuint i = 0; i < ARRAY_SIZE(_5_to_8_bit); ++i) {
        _5_to_8_bit[i] = static_cast<DTubyte>(255 * i / (ARRAY_SIZE(_5_to_8_bit)-1));
    }
    
    for (DTuint i = 0; i < ARRAY_SIZE(_6_to_8_bit); ++i) {
        _6_to_8_bit[i] = static_cast<DTubyte>(255 * i / (ARRAY_SIZE(_6_to_8_bit)-1));
    }
    
    // For Mersenne Twister
    _random_seed = (DTuint) ::time(0);  // Pick a seed
    
    for (DTint i = 0; i < MT_LEN; i++)
        _mt_buffer[i] = random_int();
    _mt_index = 0;
}	

//==============================================================================
//==============================================================================

void MoreMath::add_entropy (DTubyte *data, DTsize data_size)
{
    for (DTuint i = 0; i < data_size; ++i) {
        DTubyte roll = _entropy_pool[_entropy_pool_pos] % 8;    // Roll by number of bits in pool
        DTubyte bits = data[i];
        bits = (DTubyte) (bits << roll) | (DTubyte) (bits >> ( (DTubyte) 8 - roll));
    
        _entropy_pool[_entropy_pool_pos] ^= data[i];
        _entropy_pool_pos = (_entropy_pool_pos + 1) % sizeof(_entropy_pool);
    }
}

void MoreMath::entropy (DTubyte *data, DTsize data_size)
{
    ASSERT(data_size <= (DTsize) sizeof(_entropy_pool));

    for (DTuint i = 0; i < data_size; ++i) {
        data[i] ^= _entropy_pool[_entropy_pool_pos];
        _entropy_pool_pos = (_entropy_pool_pos + 1) % sizeof(_entropy_pool);
    }
}

//==============================================================================
// Not sure where I got this from...
//==============================================================================

DTfloat	MoreMath::random_float (void)
{
    IntOrFloat r;
    
    const DTuint jflone = 0x3f800000;
    const DTuint jflmsk = 0x007fffff;
    
    _random_seed = 1664525L*_random_seed + 1013904223L;	// Just magic numbers good for this purpose
    r.i = jflone | (jflmsk & _random_seed);    
	ASSERT( r.f >= 1.0F && r.f < 2.0F);
    return r.f-1.0F;
}

#define MT_IA           397
#define MT_IB           (MT_LEN - MT_IA)
#define UPPER_MASK      0x80000000
#define LOWER_MASK      0x7FFFFFFF
#define MATRIX_A        0x9908B0DF
#define TWIST(b,i,j)    ((b)[i] & UPPER_MASK) | ((b)[j] & LOWER_MASK)
#define MAGIC(s)        (((s)&1)*MATRIX_A)

DTfloat MoreMath::random_MT_float (void)
{
    IntOrFloat r;
    DTuint *b = _mt_buffer;
    DTint idx = _mt_index;
    DTuint s;
    DTint i;
	
    if (idx == MT_LEN*sizeof(DTuint))
    {
        idx = 0;
        i = 0;
        for (; i < MT_IB; i++) {
            s = TWIST(b, i, i+1);
            b[i] = b[i + MT_IA] ^ (s >> 1) ^ MAGIC(s);
        }
        for (; i < MT_LEN-1; i++) {
            s = TWIST(b, i, i+1);
            b[i] = b[i - MT_IB] ^ (s >> 1) ^ MAGIC(s);
        }
        
        s = TWIST(b, MT_LEN-1, 0);
        b[MT_LEN-1] = b[MT_IA-1] ^ (s >> 1) ^ MAGIC(s);
    }
    _mt_index = idx + sizeof(DTuint);
        
    const DTuint jflone = 0x3f800000;
    const DTuint jflmsk = 0x007fffff;
    
    _random_seed = r.i = jflone | (jflmsk & (*(DTuint *)((unsigned char *)b + idx) ^ _random_seed));
	ASSERT( r.f >= 1.0F && r.f < 2.0F);
    return r.f-1.0F;
}

DTuint	MoreMath::random_int (void)
{
    _random_seed = 1664525L*_random_seed + 1013904223L;	// Just magic numbers good for this purpose
    return _random_seed;
}

DTuint	MoreMath::random_MT_int (void)
{
    DTuint *b = _mt_buffer;
    DTint idx = _mt_index;
    DTuint s;
    DTint i;
	
    if (idx == MT_LEN*sizeof(DTuint))
    {
        idx = 0;
        i = 0;
        for (; i < MT_IB; i++) {
            s = TWIST(b, i, i+1);
            b[i] = b[i + MT_IA] ^ (s >> 1) ^ MAGIC(s);
        }
        for (; i < MT_LEN-1; i++) {
            s = TWIST(b, i, i+1);
            b[i] = b[i - MT_IB] ^ (s >> 1) ^ MAGIC(s);
        }
        
        s = TWIST(b, MT_LEN-1, 0);
        b[MT_LEN-1] = b[MT_IA-1] ^ (s >> 1) ^ MAGIC(s);
    }
    _mt_index = idx + sizeof(DTuint);
    
    _random_seed = *(DTuint *)((unsigned char *)b + idx) ^ _random_seed;
    return _random_seed;
}

//==============================================================================
//==============================================================================

void MoreMath::set_random_seed (void)
{
    entropy ((DTubyte*) &_random_seed, sizeof(_random_seed));
}

//==============================================================================
//==============================================================================

DTfloat MoreMath::factorial (DTint f)
{
	if (f < FACTORIAL_TABLE_SIZE) {
		return _factorial[f];
	} else {
		DTfloat result = 1.0F;
		for (DTint i = 2; i <= f; ++i) {
			result *= i;
		}
		return result;
	}
}

//==============================================================================
// From http://www.research.scea.com/gdc2003/spherical-harmonic-lighting.pdf
//==============================================================================

Vector3 MoreMath::random_vector (void)
{
    DTfloat ex = random_float();
    DTfloat ey = random_float();
    
    DTfloat theta = 2.0F * std::acos(std::sqrt(ex)); // Same as 2.0F * std::acos(std::sqrt(1.0-ex));
    DTfloat phi = 2.0F * PI * ey;
    
    DTfloat sin_theta = std::sin(theta);
    return Vector3( sin_theta * std::cos(phi),
                    sin_theta * std::sin(phi),
                    std::cos(theta) );
                    
}

//==============================================================================
//==============================================================================

void MoreMath::regression (std::vector<Vector2> &data, DTfloat &a, DTfloat &b, DTfloat &c, DTfloat &d)
{

    DTfloat s0=0.0F,s1=0.0F,s2=0.0F,s3=0.0F,s4=0.0F,s5=0.0F,s6=0.0F;
    DTfloat y0=0.0F,y1=0.0F,y2=0.0F,y3=0.0F;
    
    // Sample curve
    for (auto &i : data) {
        DTfloat x = i.x;
        DTfloat y = i.y;
    
        DTfloat x1 = x;
        DTfloat x2 = x1*x1;
        DTfloat x3 = x2*x1;
        DTfloat x4 = x2*x2;
        DTfloat x5 = x4*x1;
        DTfloat x6 = x4*x2;
        
        s0 += 1.0F;
        s1 += x1;
        s2 += x2;
        s3 += x3;
        s4 += x4;
        s5 += x5;
        s6 += x6;
        
        y0 += y;
        y1 += x1*y;
        y2 += x2*y;
        y3 += x3*y;
    }
    
    // Do regression
    Matrix4 X(  s0, s1, s2, s3,
                s1, s2, s3, s4,
                s2, s3, s4, s5,
                s3, s4, s5, s6  );
    Vector4 Y(  y0, y1, y2, y3  );
    
    Vector4 r = X.inversed() * Y;
            
    a = r.x;
    b = r.y;
    c = r.z;
    d = r.w;
}

//==============================================================================
//==============================================================================

DTuint	MoreMath::calc_crc32 (const void* data, DTsize length)
{
    DTubyte*    p;
    DTuint      temp1;
    DTuint      temp2;
    DTuint      crc = 0xFFFFFFFFL;
    
    p = (DTubyte*) data;
    
    while (length-- != 0) {
        temp1 = (crc >> 8) & 0x00FFFFFFL;
        temp2 = _crc_table[ ((DTint) crc ^ *p++) & 0xFF];
        crc = temp1 ^ temp2;
    }
    
    return (crc ^ 0xFFFFFFFFL);
}

//==============================================================================
//==============================================================================

Vector3 MoreMath::transform_4h(const Matrix4 &a, const Vector3 &b)
{
    DTfloat cw,cx,cy,cz;

    cx = a._m11 * b.x + a._m12 * b.y + a._m13 * b.z + a._m14; // Implied * 1.0
    cy = a._m21 * b.x + a._m22 * b.y + a._m23 * b.z + a._m24; // Implied * 1.0
    cz = a._m31 * b.x + a._m32 * b.y + a._m33 * b.z + a._m34; // Implied * 1.0
    cw = a._m41 * b.x + a._m42 * b.y + a._m43 * b.z + a._m44; // Implied * 1.0
	
    Vector3 c;
	c.x = cx / cw;
	c.y = cy / cw;
	c.z = cz / cw;
    
    return c;
}

//==============================================================================
//==============================================================================

} // DT3

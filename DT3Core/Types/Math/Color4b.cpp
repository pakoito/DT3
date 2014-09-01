//==============================================================================
///	
///	File: Color4b.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

const Color4b Color4b::black(0.0F,0.0F,0.0F,1.0F);
const Color4b Color4b::grey10 (0.10F,0.10F,0.10F,1.0F);
const Color4b Color4b::grey25 (0.25F,0.25F,0.25F,1.0F);
const Color4b Color4b::grey50 (0.5F,0.5F,0.5F,1.0F);
const Color4b Color4b::grey75 (0.75F,0.75F,0.75F,1.0F);
const Color4b Color4b::white (1.0F,1.0F,1.0F,1.0F);

const Color4b Color4b::red (1.0F,0.0F,0.0F,1.0F);
const Color4b Color4b::green (0.0F,1.0F,0.0F,1.0F);
const Color4b Color4b::blue (0.0F,0.0F,1.0F,1.0F);
const Color4b Color4b::cyan (0.0F,1.0F,1.0F,1.0F);
const Color4b Color4b::magenta (1.0F,0.0F,1.0F,1.0F);
const Color4b Color4b::yellow (1.0F,1.0F,0.0F,1.0F);

const Color4b Color4b::transparent_black (0.0F,0.0F,0.0F,0.0F);
const Color4b Color4b::transparent_white (1.0F,1.0F,1.0F,0.0F);

//==============================================================================
/// Constructors
//==============================================================================

Color4b::Color4b	(void)
    :	r(255),
		g(255),
        b(255),
        a(255)
{

}

Color4b::Color4b	(const Color4b &rhs)
    :	r(rhs.r),
		g(rhs.g),
        b(rhs.b),
        a(rhs.a)
{

}

Color4b::Color4b	(const DTfloat r_, const DTfloat g_, const DTfloat b_, const DTfloat a_)
{
    set(r_,g_,b_,a_);
}

Color4b::Color4b	(const DTubyte r_, const DTubyte g_, const DTubyte b_, const DTubyte a_)
{
    set(r_,g_,b_,a_);
}	

Color4b::Color4b	(const DTfloat rhs[])
{
    set(rhs);
}

Color4b::Color4b	(const DTubyte rhs[])
{
    set(rhs);
}

Color4b::Color4b    (const Color4f& rhs)
{
    r = rhs.r_as_byte();
    g = rhs.g_as_byte();
    b = rhs.b_as_byte();
    a = rhs.a_as_byte();
}

Color4b & Color4b::operator = (const Color4b& rhs)
{
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
    return (*this);
}	

//==============================================================================
//==============================================================================

Stream& operator << (Stream &s, const Color4b&c)
{
	s << c.r_as_float() << Stream::fs << c.g_as_float() << Stream::fs << c.b_as_float() << Stream::fs << c.a_as_float();
	return s;
}

Stream& operator >> (Stream &s, Color4b&c)
{
	DTfloat r = c.r_as_float(),
            g = c.g_as_float(),
            b = c.b_as_float(),
            a = c.a_as_float();
    
	s >> r >> g >> b >> a;
	
	c.set_r(r);
	c.set_g(g);
	c.set_b(b);
	c.set_a(a);
	
	return s;
}

//==============================================================================
//==============================================================================

void Color4b::set (const DTfloat r_, const DTfloat g_, const DTfloat b_, const DTfloat a_)
{ 
    r = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(r_) * static_cast<DTfloat>(DTUBYTE_MAX)));
    g = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(g_) * static_cast<DTfloat>(DTUBYTE_MAX)));
    b = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(b_) * static_cast<DTfloat>(DTUBYTE_MAX)));
    a = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(a_) * static_cast<DTfloat>(DTUBYTE_MAX)));
}

//==============================================================================
//==============================================================================

void Color4b::set (const DTubyte r_, const DTubyte g_, const DTubyte b_, const DTubyte a_)
{ 
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}

//==============================================================================
//==============================================================================

void Color4b::set (const DTfloat ar[])
{ 
    r = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(ar[0]) * static_cast<DTfloat>(DTUBYTE_MAX)));
    g = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(ar[1]) * static_cast<DTfloat>(DTUBYTE_MAX)));
    b = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(ar[2]) * static_cast<DTfloat>(DTUBYTE_MAX)));
    a = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(ar[3]) * static_cast<DTfloat>(DTUBYTE_MAX)));
}

//==============================================================================
//==============================================================================

void Color4b::set (const DTubyte ar[])
{ 
    r = ar[0];
    g = ar[1];
    b = ar[2];
    a = ar[3];
}

//==============================================================================
//==============================================================================

void Color4b::set_r (const DTfloat r_)
{
    r = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(r_) * static_cast<DTfloat>(DTUBYTE_MAX)));
}

void Color4b::set_g (const DTfloat g_)
{
    g = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(g_) * static_cast<DTfloat>(DTUBYTE_MAX)));
}

void Color4b::set_b (const DTfloat b_)
{
    b = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(b_) * static_cast<DTfloat>(DTUBYTE_MAX)));
}

void Color4b::set_a (const DTfloat a_)
{
    a = (DTubyte) (MoreMath::float_to_int(MoreMath::clamp_zero_one(a_) * static_cast<DTfloat>(DTUBYTE_MAX)));
}

//==============================================================================
//==============================================================================

DTfloat Color4b::r_as_float (void) 	const
{
    return MoreMath::int_to_float(r) / static_cast<DTfloat>(DTUBYTE_MAX);
}

DTfloat Color4b::g_as_float (void)	const
{
    return MoreMath::int_to_float(g) / static_cast<DTfloat>(DTUBYTE_MAX);
}

DTfloat Color4b::b_as_float (void)	const
{
    return MoreMath::int_to_float(b) / static_cast<DTfloat>(DTUBYTE_MAX);
}

DTfloat Color4b::a_as_float (void)	const
{
    return MoreMath::int_to_float(a) / static_cast<DTfloat>(DTUBYTE_MAX);
}

//==============================================================================
//==============================================================================

} // DT3

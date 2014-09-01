//==============================================================================
///	
///	File: Color4f.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

const Color4f Color4f::black(0.0F,0.0F,0.0F,1.0F);
const Color4f Color4f::grey10 (0.10F,0.10F,0.10F,1.0F);
const Color4f Color4f::grey25 (0.25F,0.25F,0.25F,1.0F);
const Color4f Color4f::grey50 (0.5F,0.5F,0.5F,1.0F);
const Color4f Color4f::grey75 (0.75F,0.75F,0.75F,1.0F);
const Color4f Color4f::white (1.0F,1.0F,1.0F,1.0F);

const Color4f Color4f::red (1.0F,0.0F,0.0F,1.0F);
const Color4f Color4f::green (0.0F,1.0F,0.0F,1.0F);
const Color4f Color4f::blue (0.0F,0.0F,1.0F,1.0F);
const Color4f Color4f::cyan (0.0F,1.0F,1.0F,1.0F);
const Color4f Color4f::magenta (1.0F,0.0F,1.0F,1.0F);
const Color4f Color4f::yellow (1.0F,1.0F,0.0F,1.0F);

const Color4f Color4f::transparent_black (0.0F,0.0F,0.0F,0.0F);
const Color4f Color4f::transparent_white (1.0F,1.0F,1.0F,0.0F);

//==============================================================================
/// Constructors
//==============================================================================

Color4f::Color4f	(void)
    :	r(1.0F),
		g(1.0F),
        b(1.0F),
        a(1.0F)
{

}

Color4f::Color4f	(const Color4f &rhs)
    :	r(rhs.r),
		g(rhs.g),
        b(rhs.b),
        a(rhs.a)
{

}

Color4f::Color4f	(const DTfloat r_, const DTfloat g_, const DTfloat b_, const DTfloat a_)
{
    set(r_,g_,b_,a_);
}

Color4f::Color4f	(const DTubyte r_, const DTubyte g_, const DTubyte b_, const DTubyte a_)
{
    set(r_,g_,b_,a_);
}	

Color4f::Color4f	(const DTfloat rhs[])
{
    set(rhs);
}

Color4f::Color4f	(const DTubyte rhs[])
{
    set(rhs);
}

Color4f::Color4f    (const Color4b& rhs)
{
    r = rhs.r_as_float();
    g = rhs.g_as_float();
    b = rhs.b_as_float();
    a = rhs.a_as_float();
}

Color4f & Color4f::operator = (const Color4f& rhs)
{
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
    return (*this);
}	

//==============================================================================
//==============================================================================

Stream& operator << (Stream &s, const Color4f&c)
{
	s << c.r_as_float() << Stream::fs << c.g_as_float() << Stream::fs << c.b_as_float() << Stream::fs << c.a_as_float();
	return s;
}

Stream& operator >> (Stream &s, Color4f&c)
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

void Color4f::set (const DTfloat r_, const DTfloat g_, const DTfloat b_, const DTfloat a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}

//==============================================================================
//==============================================================================

void Color4f::set (const DTubyte r_, const DTubyte g_, const DTubyte b_, const DTubyte a_)
{
    r = MoreMath::int_to_float(r_) / 255.0F;
    g = MoreMath::int_to_float(g_) / 255.0F;
    b = MoreMath::int_to_float(b_) / 255.0F;
    a = MoreMath::int_to_float(a_) / 255.0F;
}

//==============================================================================
//==============================================================================

void Color4f::set (const DTfloat ar[])
{ 
    r = ar[0];
    g = ar[1];
    b = ar[2];
    a = ar[3];
}

//==============================================================================
//==============================================================================

void Color4f::set (const DTubyte ar[])
{ 
    r = MoreMath::int_to_float(ar[0]) / 255.0F;
    g = MoreMath::int_to_float(ar[1]) / 255.0F;
    b = MoreMath::int_to_float(ar[2]) / 255.0F;
    a = MoreMath::int_to_float(ar[3]) / 255.0F;
}

//==============================================================================
//==============================================================================

void Color4f::set_r (const DTubyte r_)
{
    r = MoreMath::int_to_float(r_) / 255.0F;
}

void Color4f::set_g (const DTubyte g_)
{
	g = MoreMath::int_to_float(g_) / 255.0F;
}

void Color4f::set_b (const DTubyte b_)
{
    b = MoreMath::int_to_float(b_) / 255.0F;
}

void Color4f::set_a (const DTubyte a_)
{
    a = MoreMath::int_to_float(a_) / 255.0F;
}

//==============================================================================
//==============================================================================

DTubyte Color4f::r_as_byte (void) 	const
{
    return static_cast<DTubyte>(MoreMath::float_to_int(MoreMath::clamp_zero_one(r) * 255.0F));
}

DTubyte Color4f::g_as_byte (void)	const
{
    return static_cast<DTubyte>(MoreMath::float_to_int(MoreMath::clamp_zero_one(g) * 255.0F));
}

DTubyte Color4f::b_as_byte (void)	const
{
    return static_cast<DTubyte>(MoreMath::float_to_int(MoreMath::clamp_zero_one(b) * 255.0F));
}

DTubyte Color4f::a_as_byte (void)	const
{
    return static_cast<DTubyte>(MoreMath::float_to_int(MoreMath::clamp_zero_one(a) * 255.0F));
}

//==============================================================================
//==============================================================================

} // DT3

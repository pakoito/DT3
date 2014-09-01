//==============================================================================
///	
///	File: Matrix2.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Matrix2.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

Matrix2::Matrix2 (void)
    :   _m11 (1.0F), _m12 (0.0F),
        _m21 (0.0F), _m22 (1.0F)
{

}

Matrix2::Matrix2 (const Matrix2 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12;
	_m21 = rhs._m21; _m22 = rhs._m22;
}

Matrix2::Matrix2 (const Matrix3 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12;
	_m21 = rhs._m21; _m22 = rhs._m22;
}

Matrix2::Matrix2 (const Matrix4 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12;
	_m21 = rhs._m21; _m22 = rhs._m22;
}

Matrix2::Matrix2 (	const DTfloat m11,const DTfloat m12,
                    const DTfloat m21,const DTfloat m22	)
{	
	_m11 = m11; _m12 = m12; 
	_m21 = m21; _m22 = m22;
}

Matrix2::Matrix2 (const Vector2 &vx, const Vector2 &vy)
{	
	_m11 = vx.x;  _m12 = vy.x; 
	_m21 = vx.y;  _m22 = vy.y; 
}

Matrix2& Matrix2::operator = (const Matrix2 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12;
	_m21 = rhs._m21; _m22 = rhs._m22;
	return *this;                                       
}

//==============================================================================
//==============================================================================

Stream& operator << (Stream &s, const Matrix2&m)
{
	s	<< m._m11 << Stream::fs << m._m21 << Stream::fs
		<< m._m12 << Stream::fs << m._m22;

	return s;
}

Stream& operator >> (Stream &s, Matrix2&m)
{
	s	>> m._m11 >> m._m21 
		>> m._m12 >> m._m22;

	return s;
}

//==============================================================================
//==============================================================================

DTboolean Matrix2::operator == (const Matrix2& rhs) const
{
	return (    _m11 == rhs._m11	&& _m21 == rhs._m21		&& 
				_m12 == rhs._m12	&& _m22 == rhs._m22
    );
}


DTboolean Matrix2::operator != (const Matrix2& rhs) const
{
	return (    _m11 != rhs._m11	|| _m21 != rhs._m21     || 
				_m12 != rhs._m12	|| _m22 != rhs._m22 
    );
}

Matrix2& Matrix2::operator *= (const DTfloat rhs)
{
	_m11*=rhs; _m21*=rhs;
	_m12*=rhs; _m22*=rhs;

	return *this;
}

Matrix2& Matrix2::operator /= (const DTfloat rhs)
{
	_m11/=rhs; _m21/=rhs;
	_m12/=rhs; _m22/=rhs;

	return *this;
}

Matrix2& Matrix2::operator *= (const Matrix2 &rhs)
{
    Matrix2 t;
    
    t._m11  = _m11 * rhs._m11 + _m12 * rhs._m21;
    t._m21  = _m21 * rhs._m11 + _m22 * rhs._m21;
    
    t._m12  = _m11 * rhs._m12 + _m12 * rhs._m22;
    t._m22  = _m21 * rhs._m12 + _m22 * rhs._m22;
    
    *this = t;
	return *this;
}

Matrix2 operator * (const DTfloat a, const Matrix2 &b)
{
	return Matrix2(	b._m11 * a, b._m12 * a,
					b._m21 * a, b._m22 * a);
}

Matrix2 operator * (const Matrix2 &a, const DTfloat b)
{
	return Matrix2(	a._m11 * b, a._m12 * b,
					a._m21 * b, a._m22 * b);

}

Matrix2 operator * (const Matrix2 &a, const Matrix2 &b)
{
    return Matrix2(	a._m11 * b._m11  + a._m12 * b._m21, a._m11 * b._m12  + a._m12 * b._m22,
					a._m21 * b._m11  + a._m22 * b._m21, a._m21 * b._m12  + a._m22 * b._m22);
}

Vector2 operator * (const Matrix2 &a, const Vector2 &b)
{    
	return Vector2(	a._m11 * b.x + a._m12 * b.y,
					a._m21 * b.x + a._m22 * b.y);
}

Matrix2 operator / (const Matrix2 &a, const DTfloat b)
{
	Matrix2 c;

    c._m11 = a._m11 / b;
    c._m21 = a._m21 / b;
    c._m12 = a._m12 / b;
    c._m22 = a._m22 / b;

	return c;
}

Matrix2 operator + (const Matrix2 &a, const Matrix2 &b)
{
    return Matrix2(	a._m11 + b._m11, a._m12 + b._m12,
					a._m21 + b._m21, a._m22 + b._m22    );
}

void Matrix2::clear (void)
{
    _m11 = 0.0F;	_m12 = 0.0F;	
    _m21 = 0.0F;	_m22 = 0.0F;
}


DTfloat Matrix2::determinant (void) const
{
	return _m11 * _m22 - _m12 * _m21;
}


Matrix2 Matrix2::inversed (void) const
{
    DTfloat det = determinant();
	Matrix2 t;

    //UTIL_Assert(det != 0.0F);
    if (det == 0.0F) {
		t.clear();
        return t;
    }
    
	return Matrix2(	_m22/det, -_m21/det, -_m12/det, _m11/det);
}

Matrix2 Matrix2::transposed (void) const
{
	Matrix2 c;
    DTfloat temp;
    
    c._m11 = _m11;
    c._m22 = _m22;
    temp = _m21;		c._m21 = _m12;		c._m12 = temp;
    temp = _m12;		c._m12 = _m21;		c._m21 = temp;
    
	return c;
}

const Matrix2&	Matrix2::identity (void)
{
	static Matrix2 c(	1.0F, 0.0F,
						0.0F, 1.0F);
	return c;
}

const Matrix2& Matrix2::zero (void)
{
	static Matrix2 c(	0.0F, 0.0F,
						0.0F, 0.0F);
	return c;
}

//==============================================================================
//==============================================================================

Matrix2 Matrix2::orthoed (void) const
{
    Vector2 x = x_axis(), y;
    
    x.normalize();
    y = x.perped();
    
    Matrix2 r;
    r._m11 = x.x;
    r._m21 = x.y;
    
    r._m12 = y.x;
    r._m22 = y.y;
    return r;
}

//==============================================================================
//==============================================================================

Matrix2 Matrix2::set_rotation_z (const DTfloat a)
{	
    DTfloat cos_a = std::cos(a);
    DTfloat sin_a = std::sin(a);
    
    Matrix2 r;

    r._m11  = cos_a;
    r._m21  = -sin_a;
    
    r._m12  = sin_a;
    r._m22  = cos_a;
    
    return r;
}

//==============================================================================
//==============================================================================

void Matrix2::set_diagonal (const Vector2 &a)
{
    _m11 = a.x;
    _m21 = 0.0F;
    
    _m12 = 0.0F;
    _m22 = a.y;
}

//==============================================================================
//==============================================================================

Vector2 Matrix2::diagonal (void) const
{
    Vector2 c;
    c.x = _m11;
    c.y = _m22;
    return c;
}

//==============================================================================
//==============================================================================

} // DT3


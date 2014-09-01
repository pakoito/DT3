//==============================================================================
///	
///	File: Matrix4.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Matrix2.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

Matrix4::Matrix4 (void)
    :   _m11 (1.0F), _m12 (0.0F), _m13 (0.0F), _m14 (0.0F),
        _m21 (0.0F), _m22 (1.0F), _m23 (0.0F), _m24 (0.0F),
        _m31 (0.0F), _m32 (0.0F), _m33 (1.0F), _m34 (0.0F),
        _m41 (0.0F), _m42 (0.0F), _m43 (0.0F), _m44 (1.0F)
{

}

Matrix4::Matrix4 (const Matrix2 &rhs)
{	
    _m11 = rhs._m11;	_m12 = rhs._m12;    _m13 = 0.0F;    _m14 = 0.0F;	
    _m21 = rhs._m21;	_m22 = rhs._m22;    _m23 = 0.0F;    _m24 = 0.0F;
    _m31 = 0.0F;		_m32 = 0.0F;		_m33 = 1.0F;    _m34 = 0.0F;
    _m41 = 0.0F;		_m42 = 0.0F;		_m43 = 0.0F;	_m44 = 1.0F;
}

Matrix4::Matrix4 (const Matrix2 &o, const Vector2 &t)
{
    _m11 = o._m11;      _m12 = o._m12;      _m13 = 0.0F;    _m14 = t.x;	
    _m21 = o._m21;      _m22 = o._m22;      _m23 = 0.0F;    _m24 = t.y;
    _m31 = 0.0F;		_m32 = 0.0F;		_m33 = 1.0F;    _m34 = 0.0F;
    _m41 = 0.0F;		_m42 = 0.0F;		_m43 = 0.0F;	_m44 = 1.0F;
}

Matrix4::Matrix4 (const Matrix2 &o, const Vector2 &t, DTfloat s)
{
    _m11 = o._m11 * s;  _m12 = o._m12 * s;  _m13 = 0.0F;    _m14 = t.x;	
    _m21 = o._m21 * s;  _m22 = o._m22 * s;  _m23 = 0.0F;    _m24 = t.y;
    _m31 = 0.0F;		_m32 = 0.0F;		_m33 = 1.0F;    _m34 = 0.0F;
    _m41 = 0.0F;		_m42 = 0.0F;		_m43 = 0.0F;	_m44 = 1.0F;
}

Matrix4::Matrix4 (const Matrix2 &o, const Vector3 &t)
{
    _m11 = o._m11;      _m12 = o._m12;      _m13 = 0.0F;    _m14 = t.x;	
    _m21 = o._m21;      _m22 = o._m22;      _m23 = 0.0F;    _m24 = t.y;
    _m31 = 0.0F;		_m32 = 0.0F;		_m33 = 1.0F;    _m34 = t.z;
    _m41 = 0.0F;		_m42 = 0.0F;		_m43 = 0.0F;	_m44 = 1.0F;
}

Matrix4::Matrix4 (const Matrix2 &o, const Vector3 &t, DTfloat s)
{
    _m11 = o._m11 * s;  _m12 = o._m12 * s;  _m13 = 0.0F;    _m14 = t.x;	
    _m21 = o._m21 * s;  _m22 = o._m22 * s;  _m23 = 0.0F;    _m24 = t.y;
    _m31 = 0.0F;		_m32 = 0.0F;		_m33 = 1.0F;    _m34 = t.z;
    _m41 = 0.0F;		_m42 = 0.0F;		_m43 = 0.0F;	_m44 = 1.0F;
}


Matrix4::Matrix4 (const Matrix3 &rhs)
{
    _m11 = rhs._m11;	_m12 = rhs._m12;    _m13 = rhs._m13;    _m14 = 0.0F;	
    _m21 = rhs._m21;	_m22 = rhs._m22;    _m23 = rhs._m23;    _m24 = 0.0F;
    _m31 = rhs._m31;	_m32 = rhs._m32;    _m33 = rhs._m33;    _m34 = 0.0F;
    _m41 = 0.0F;		_m42 = 0.0F;		_m43 = 0.0F;		_m44 = 1.0F;
}

Matrix4::Matrix4 (const Matrix3 &o, const Vector3 &t)
{
    _m11 = o._m11;	_m12 = o._m12;  _m13 = o._m13;  _m14 = t.x;	
    _m21 = o._m21;	_m22 = o._m22;  _m23 = o._m23;  _m24 = t.y;
    _m31 = o._m31;	_m32 = o._m32;  _m33 = o._m33;  _m34 = t.z;
    _m41 = 0.0F;	_m42 = 0.0F;    _m43 = 0.0F;    _m44 = 1.0F;
}

Matrix4::Matrix4 (const Matrix3 &o, const Vector3 &t, DTfloat s)
{
    _m11 = o._m11 * s;	_m12 = o._m12 * s;  _m13 = o._m13 * s;  _m14 = t.x;	
    _m21 = o._m21 * s;	_m22 = o._m22 * s;  _m23 = o._m23 * s;  _m24 = t.y;
    _m31 = o._m31 * s;	_m32 = o._m32 * s;  _m33 = o._m33 * s;  _m34 = t.z;
    _m41 = 0.0F;        _m42 = 0.0F;        _m43 = 0.0F;        _m44 = 1.0F;
}

Matrix4::Matrix4 (const Matrix3 &o, const Vector3 &t, const Vector3 &s)
{
    _m11 = o._m11 * s.x;	_m12 = o._m12 * s.y;    _m13 = o._m13 * s.z;    _m14 = t.x;	
    _m21 = o._m21 * s.x;	_m22 = o._m22 * s.y;    _m23 = o._m23 * s.z;    _m24 = t.y;
    _m31 = o._m31 * s.x;	_m32 = o._m32 * s.y;    _m33 = o._m33 * s.z;    _m34 = t.z;
    _m41 = 0.0F;            _m42 = 0.0F;            _m43 = 0.0F;            _m44 = 1.0F;
}

Matrix4::Matrix4 (	const Matrix4 &rhs) {	
	_m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13; _m14 = rhs._m14;
	_m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23; _m24 = rhs._m24;
	_m31 = rhs._m31; _m32 = rhs._m32; _m33 = rhs._m33; _m34 = rhs._m34;
	_m41 = rhs._m41; _m42 = rhs._m42; _m43 = rhs._m43; _m44 = rhs._m44; 
}

Matrix4::Matrix4 (const Quaternion &o)
{
    Matrix4(Matrix3(o));
}

Matrix4::Matrix4 (const Quaternion &o, const Vector3 &t)
{
    Matrix4(Matrix3(o),t);
}

Matrix4::Matrix4 (const Quaternion &o, const Vector3 &t, DTfloat s)
{
    Matrix4(Matrix3(o),t,s);
}

Matrix4::Matrix4 (	const DTfloat m11,const DTfloat m12,const DTfloat m13,const DTfloat m14,
					const DTfloat m21,const DTfloat m22,const DTfloat m23,const DTfloat m24,
					const DTfloat m31,const DTfloat m32,const DTfloat m33,const DTfloat m34,
					const DTfloat m41,const DTfloat m42,const DTfloat m43,const DTfloat m44	)   
{	
	_m11 = m11; _m12 = m12; _m13 = m13; _m14 = m14;
	_m21 = m21; _m22 = m22; _m23 = m23; _m24 = m24;
	_m31 = m31; _m32 = m32; _m33 = m33; _m34 = m34;
	_m41 = m41; _m42 = m42; _m43 = m43; _m44 = m44;                
}

Matrix4::Matrix4 (const Vector3 &vx, const Vector3 &vy, const Vector3 &vz    )
{	
	_m11 = vx.x;  _m12 = vy.x;  _m13 = vz.x;  _m14 = 0.0F;
	_m21 = vx.y;  _m22 = vy.y;  _m23 = vz.y;  _m24 = 0.0F;
	_m31 = vx.z;  _m32 = vy.z;  _m33 = vz.z;  _m34 = 0.0F;
	_m41 = 0.0F;  _m42 = 0.0F;  _m43 = 0.0F;  _m44 = 1.0F;	
}

Matrix4::Matrix4 (const Vector3 &translation)
{
	_m11 = 1.0F;  _m12 = 0.0F;  _m13 = 0.0F;  _m14 = translation.x;
	_m21 = 0.0F;  _m22 = 1.0F;  _m23 = 0.0F;  _m24 = translation.y;
	_m31 = 0.0F;  _m32 = 0.0F;  _m33 = 1.0F;  _m34 = translation.z;
	_m41 = 0.0F;  _m42 = 0.0F;  _m43 = 0.0F;  _m44 = 1.0F;	
}


Matrix4& Matrix4::operator = (const Matrix4 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13; _m14 = rhs._m14;
	_m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23; _m24 = rhs._m24;
	_m31 = rhs._m31; _m32 = rhs._m32; _m33 = rhs._m33; _m34 = rhs._m34;
	_m41 = rhs._m41; _m42 = rhs._m42; _m43 = rhs._m43; _m44 = rhs._m44; 
	return *this;                                                   
}		

//==============================================================================
//==============================================================================

Stream& operator << (Stream &s, const Matrix4 &m)
{
	s	<< m._m11 << Stream::fs << m._m21 << Stream::fs << m._m31 << Stream::fs << m._m41 << Stream::fs
		<< m._m12 << Stream::fs << m._m22 << Stream::fs << m._m32 << Stream::fs << m._m42 << Stream::fs
		<< m._m13 << Stream::fs << m._m23 << Stream::fs << m._m33 << Stream::fs << m._m43 << Stream::fs
		<< m._m14 << Stream::fs << m._m24 << Stream::fs << m._m34 << Stream::fs << m._m44;
		
	return s;
}

Stream& operator >> (Stream &s, Matrix4 &m)
{
	s	>> m._m11 >> m._m21 >> m._m31 >> m._m41
		>> m._m12 >> m._m22 >> m._m32 >> m._m42
		>> m._m13 >> m._m23 >> m._m33 >> m._m43
		>> m._m14 >> m._m24 >> m._m34 >> m._m44;
		
	return s;
}

//==============================================================================
/// operators
//==============================================================================

DTboolean Matrix4::operator == (const Matrix4 &rhs) const
{
	return (    _m11 == rhs._m11	&& _m21 == rhs._m21     && _m31 == rhs._m31     && _m41 == rhs._m41	&&
                _m12 == rhs._m12	&& _m22 == rhs._m22     && _m32 == rhs._m32     && _m42 == rhs._m42	&&
                _m13 == rhs._m13	&& _m23 == rhs._m23     && _m33 == rhs._m33     && _m43 == rhs._m43	&&
                _m14 == rhs._m14	&& _m24 == rhs._m24     && _m34 == rhs._m34     && _m44 == rhs._m44
    );
}

//==============================================================================
//==============================================================================

DTboolean Matrix4::operator != (const Matrix4 &rhs) const
{
	return (    _m11 != rhs._m11	|| _m21 != rhs._m21     || _m31 != rhs._m31     || _m41 != rhs._m41	||
                _m12 != rhs._m12	|| _m22 != rhs._m22     || _m32 != rhs._m32     || _m42 != rhs._m42	||
                _m13 != rhs._m13	|| _m23 != rhs._m23     || _m33 != rhs._m33     || _m43 != rhs._m43	||
                _m14 != rhs._m14	|| _m24 != rhs._m24     || _m34 != rhs._m34     || _m44 != rhs._m44
    );
}

//==============================================================================
//==============================================================================

Matrix4& Matrix4::operator *= (const DTfloat rhs)
{
	_m11*=rhs; _m21*=rhs; _m31*=rhs; _m41*=rhs;
	_m12*=rhs; _m22*=rhs; _m32*=rhs; _m42*=rhs;
	_m13*=rhs; _m23*=rhs; _m33*=rhs; _m43*=rhs;
	_m14*=rhs; _m24*=rhs; _m34*=rhs; _m44*=rhs;
	
	return *this;
}

//==============================================================================
//==============================================================================

Matrix4& Matrix4::operator /= (const DTfloat rhs)
{
	_m11/=rhs; _m21/=rhs; _m31/=rhs; _m41/=rhs;
	_m12/=rhs; _m22/=rhs; _m32/=rhs; _m42/=rhs;
	_m13/=rhs; _m23/=rhs; _m33/=rhs; _m43/=rhs;
	_m14/=rhs; _m24/=rhs; _m34/=rhs; _m44/=rhs;

	return *this;
}

//==============================================================================
//==============================================================================

Matrix4& Matrix4::operator *= (const Matrix4 &rhs)
{
    Matrix4 t;
    
    t._m11 = _m11 * rhs._m11 + _m12 * rhs._m21 + _m13 * rhs._m31 + _m14 * rhs._m41;
    t._m21 = _m21 * rhs._m11 + _m22 * rhs._m21 + _m23 * rhs._m31 + _m24 * rhs._m41;
    t._m31 = _m31 * rhs._m11 + _m32 * rhs._m21 + _m33 * rhs._m31 + _m34 * rhs._m41;
    t._m41 = _m41 * rhs._m11 + _m42 * rhs._m21 + _m43 * rhs._m31 + _m44 * rhs._m41;
    t._m12 = _m11 * rhs._m12 + _m12 * rhs._m22 + _m13 * rhs._m32 + _m14 * rhs._m42;
    t._m22 = _m21 * rhs._m12 + _m22 * rhs._m22 + _m23 * rhs._m32 + _m24 * rhs._m42;
    t._m32 = _m31 * rhs._m12 + _m32 * rhs._m22 + _m33 * rhs._m32 + _m34 * rhs._m42;
    t._m42 = _m41 * rhs._m12 + _m42 * rhs._m22 + _m43 * rhs._m32 + _m44 * rhs._m42;
    t._m13 = _m13 * rhs._m33 + _m14 * rhs._m43 + _m11 * rhs._m13 + _m12 * rhs._m23;
    t._m23 = _m23 * rhs._m33 + _m24 * rhs._m43 + _m21 * rhs._m13 + _m22 * rhs._m23;
    t._m33 = _m33 * rhs._m33 + _m34 * rhs._m43 + _m31 * rhs._m13 + _m32 * rhs._m23;
    t._m43 = _m43 * rhs._m33 + _m44 * rhs._m43 + _m41 * rhs._m13 + _m42 * rhs._m23;
    t._m14 = _m11 * rhs._m14 + _m12 * rhs._m24 + _m13 * rhs._m34 + _m14 * rhs._m44;
    t._m24 = _m21 * rhs._m14 + _m22 * rhs._m24 + _m23 * rhs._m34 + _m24 * rhs._m44;
    t._m34 = _m31 * rhs._m14 + _m32 * rhs._m24 + _m33 * rhs._m34 + _m34 * rhs._m44;
    t._m44 = _m41 * rhs._m14 + _m42 * rhs._m24 + _m43 * rhs._m34 + _m44 * rhs._m44;
    
    *this = t;
	return *this;
}

//==============================================================================
//==============================================================================

Matrix4 operator * (const DTfloat a, const Matrix4 &b)
{	
	return Matrix4(	b._m11 * a, b._m12 * a, b._m13 * a, b._m14 * a,
					b._m21 * a, b._m22 * a, b._m23 * a, b._m24 * a,
					b._m31 * a, b._m32 * a, b._m33 * a, b._m34 * a,
					b._m41 * a, b._m42 * a, b._m43 * a, b._m44 * a);
}

//==============================================================================
//==============================================================================

Matrix4 operator * (const Matrix4 &a, const DTfloat b)
{
	return Matrix4(	a._m11 * b, a._m12 * b, a._m13 * b, a._m14 * b,
					a._m21 * b, a._m22 * b, a._m23 * b, a._m24 * b,
					a._m31 * b, a._m32 * b, a._m33 * b, a._m34 * b,
					a._m41 * b, a._m42 * b, a._m43 * b, a._m44 * b);
}

//==============================================================================
//==============================================================================

Matrix4 operator * (const Matrix4 &a, const Matrix4 &b)
{
	return Matrix4(	a._m11 * b._m11 + a._m12 * b._m21 + a._m13 * b._m31 + a._m14 * b._m41,
					a._m11 * b._m12 + a._m12 * b._m22 + a._m13 * b._m32 + a._m14 * b._m42,
					a._m13 * b._m33 + a._m14 * b._m43 + a._m11 * b._m13 + a._m12 * b._m23,
					a._m11 * b._m14 + a._m12 * b._m24 + a._m13 * b._m34 + a._m14 * b._m44,
					a._m21 * b._m11 + a._m22 * b._m21 + a._m23 * b._m31 + a._m24 * b._m41,
					a._m21 * b._m12 + a._m22 * b._m22 + a._m23 * b._m32 + a._m24 * b._m42,
					a._m23 * b._m33 + a._m24 * b._m43 + a._m21 * b._m13 + a._m22 * b._m23,
					a._m21 * b._m14 + a._m22 * b._m24 + a._m23 * b._m34 + a._m24 * b._m44,
					a._m31 * b._m11 + a._m32 * b._m21 + a._m33 * b._m31 + a._m34 * b._m41,
					a._m31 * b._m12 + a._m32 * b._m22 + a._m33 * b._m32 + a._m34 * b._m42,
					a._m33 * b._m33 + a._m34 * b._m43 + a._m31 * b._m13 + a._m32 * b._m23,
					a._m31 * b._m14 + a._m32 * b._m24 + a._m33 * b._m34 + a._m34 * b._m44,
					a._m41 * b._m11 + a._m42 * b._m21 + a._m43 * b._m31 + a._m44 * b._m41,
					a._m41 * b._m12 + a._m42 * b._m22 + a._m43 * b._m32 + a._m44 * b._m42,
					a._m43 * b._m33 + a._m44 * b._m43 + a._m41 * b._m13 + a._m42 * b._m23,
					a._m41 * b._m14 + a._m42 * b._m24 + a._m43 * b._m34 + a._m44 * b._m44);
}

//==============================================================================
//==============================================================================

Vector4 operator * (const Matrix4 &a, const Vector4 &b)
{
	return Vector4(	a._m11 * b.x + a._m12 * b.y + a._m13 * b.z + a._m14 * b.w,
					a._m21 * b.x + a._m22 * b.y + a._m23 * b.z + a._m24 * b.w,
					a._m31 * b.x + a._m32 * b.y + a._m33 * b.z + a._m34 * b.w,
					a._m41 * b.x + a._m42 * b.y + a._m43 * b.z + a._m44 * b.w);
}

Vector3 operator * (const Matrix4 &a, const Vector3 &b)
{
	return Vector3(	a._m11 * b.x + a._m12 * b.y + a._m13 * b.z + a._m14,
					a._m21 * b.x + a._m22 * b.y + a._m23 * b.z + a._m24,
					a._m31 * b.x + a._m32 * b.y + a._m33 * b.z + a._m34);
}

Vector2 operator * (const Matrix4 &a, const Vector2 &b)
{
	return Vector2(	a._m11 * b.x + a._m12 * b.y + a._m14,
					a._m21 * b.x + a._m22 * b.y + a._m24);
}

//==============================================================================
//==============================================================================

Matrix4 operator / (const Matrix4 &a, const DTfloat b)
{
	return Matrix4(	a._m11 / b, a._m12 / b, a._m13 / b, a._m14 / b,
					a._m21 / b, a._m22 / b, a._m23 / b, a._m24 / b,
					a._m31 / b, a._m32 / b, a._m33 / b, a._m34 / b,
					a._m41 / b, a._m42 / b, a._m43 / b, a._m44 / b	);
}

//==============================================================================
//==============================================================================

Matrix4 operator + (const Matrix4 &a, const Matrix4 &b)
{
    return Matrix4(	a._m11 + b._m11, a._m12 + b._m12, a._m13 + b._m13, a._m14 + b._m14,
					a._m21 + b._m21, a._m22 + b._m22, a._m23 + b._m23, a._m24 + b._m24,
					a._m31 + b._m31, a._m32 + b._m32, a._m33 + b._m33, a._m34 + b._m34,
					a._m41 + b._m41, a._m42 + b._m42, a._m43 + b._m43, a._m44 + b._m44 );
}

Matrix4 operator - (const Matrix4 &a, const Matrix4 &b)
{
    return Matrix4(	a._m11 - b._m11, a._m12 - b._m12, a._m13 - b._m13, a._m14 - b._m14,
					a._m21 - b._m21, a._m22 - b._m22, a._m23 - b._m23, a._m24 - b._m24,
					a._m31 - b._m31, a._m32 - b._m32, a._m33 - b._m33, a._m34 - b._m34,
					a._m41 - b._m41, a._m42 - b._m42, a._m43 - b._m43, a._m44 - b._m44 );
}

//==============================================================================
//==============================================================================

void Matrix4::clear (void)
{
    _m11 = 0.0F;	_m12 = 0.0F;	_m13 = 0.0F;	_m14 = 0.0F;
    _m21 = 0.0F;	_m22 = 0.0F;	_m23 = 0.0F;	_m24 = 0.0F;
    _m31 = 0.0F;	_m32 = 0.0F;	_m33 = 0.0F;	_m34 = 0.0F;
    _m41 = 0.0F;	_m42 = 0.0F;	_m43 = 0.0F;	_m44 = 0.0F;
}

//==============================================================================
//==============================================================================

DTfloat Matrix4::determinant (void) const
{
	return		+ _m11 * ( 	+ _m43 * _m24 * _m32 - _m43 * _m34 * _m22 + _m33 * _m44 * _m22    
                - _m33 * _m24 * _m42 - _m44 * _m32  * _m23 + _m34 * _m42  * _m23)
                + _m21 * (	+ _m43 * _m34 * _m12 - _m33 * _m44 * _m12 + _m44 * _m32  * _m13
                - _m34 * _m42  * _m13 - _m43 * _m14 * _m32 + _m33 * _m14 * _m42)
                + _m31 * ( 	+ _m24 * _m42  * _m13 - _m43 * _m24 * _m12 + _m43 * _m14 * _m22
                - _m44 * _m22  * _m13 - _m14 * _m42  * _m23 + _m44 * _m12  * _m23)
                + _m41 * ( 	+ _m33 * _m24 * _m12 - _m24 * _m32  * _m13 + _m34 * _m22  * _m13
                - _m34 * _m12  * _m23 - _m33 * _m14 * _m22 + _m14 * _m32  * _m23);

}

//==============================================================================
//==============================================================================

Matrix4 Matrix4::inversed (void) const
{
    DTfloat det = determinant();
	Matrix4 t;
    
    //UTIL_Assert(det != 0.0F);
    if (det == 0.0F) {
		t.clear();
        return t;
    }
	
	DTfloat det_inv = 1.0F / det;
        	
	return Matrix4(	(-_m43 * _m34 * _m22 + _m33 * _m44 * _m22 + _m43 * _m24 * _m32 - _m33 * _m24 * _m42 - _m44 * _m32 * _m23 + _m34 * _m42 * _m23) * det_inv,
					(_m43 * _m34 * _m12 - _m33 * _m44 * _m12 - _m43 * _m14 * _m32 + _m33 * _m14 * _m42 + _m44 * _m32 * _m13 - _m34 * _m42 * _m13) * det_inv,
					(-_m43 * _m24 * _m12 + _m43 * _m14 * _m22 - _m44 * _m22 * _m13 + _m24 * _m42 * _m13 + _m44 * _m12 * _m23 - _m14 * _m42 * _m23) * det_inv,
					(_m33 * _m24 * _m12 - _m33 * _m14 * _m22 + _m34 * _m22 * _m13 - _m24 * _m32 * _m13 - _m34 * _m12 * _m23 + _m14 * _m32 * _m23) * det_inv,
					(_m43 * _m34 * _m21 - _m33 * _m44 * _m21 - _m43 * _m24 * _m31 + _m33 * _m24 * _m41 + _m44 * _m31 * _m23 - _m34 * _m41 * _m23) * det_inv,
					(-_m11 * _m43 * _m34 + _m11 * _m33 * _m44 + _m43 * _m14 * _m31 - _m33 * _m14 * _m41 - _m44 * _m31 * _m13 + _m34 * _m41 * _m13) * det_inv,
					(_m11 * _m43 * _m24 - _m43 * _m14 * _m21 + _m44 * _m21 * _m13 - _m24 * _m41 * _m13 - _m11 * _m44 * _m23 + _m14 * _m41 * _m23) * det_inv,
					(-_m11 * _m33 * _m24 + _m33 * _m14 * _m21 - _m34 * _m21 * _m13 + _m24 * _m31 * _m13 + _m11 * _m34 * _m23 - _m14 * _m31 * _m23) * det_inv,
					(-_m44 * _m31 * _m22 + _m34 * _m41 * _m22 + _m44 * _m21 * _m32 - _m24 * _m41 * _m32 - _m34 * _m21 * _m42 + _m24 * _m31 * _m42) * det_inv,
					(_m44 * _m31 * _m12 - _m34 * _m41 * _m12 - _m11 * _m44 * _m32 + _m14 * _m41 * _m32 + _m11 * _m34 * _m42 - _m14 * _m31 * _m42) * det_inv,
					(-_m44 * _m21 * _m12 + _m24 * _m41 * _m12 + _m11 * _m44 * _m22 - _m14 * _m41 * _m22 - _m11 * _m24 * _m42 + _m14 * _m21 * _m42) * det_inv,
					(_m34 * _m21 * _m12 - _m24 * _m31 * _m12 - _m11 * _m34 * _m22 + _m14 * _m31 * _m22 + _m11 * _m24 * _m32 - _m14 * _m21 * _m32) * det_inv,
					(_m43 * _m31 * _m22 - _m33 * _m41 * _m22 - _m43 * _m21 * _m32 + _m33 * _m21 * _m42 + _m41 * _m32 * _m23 - _m31 * _m42 * _m23) * det_inv,
					(-_m43 * _m31 * _m12 + _m33 * _m41 * _m12 + _m11 * _m43 * _m32 - _m11 * _m33 * _m42 - _m41 * _m32 * _m13 + _m31 * _m42 * _m13) * det_inv,
					(_m43 * _m21 * _m12 - _m11 * _m43 * _m22 + _m41 * _m22 * _m13 - _m21 * _m42 * _m13 - _m41 * _m12 * _m23 + _m11 * _m42 * _m23) * det_inv,
					(-_m33 * _m21 * _m12 + _m11 * _m33 * _m22 - _m31 * _m22 * _m13 + _m21 * _m32 * _m13 + _m31 * _m12 * _m23 - _m11 * _m32 * _m23) * det_inv	);
}

//==============================================================================
//==============================================================================

Matrix4 Matrix4::transposed (void) const
{
	Matrix4 c;
    DTfloat temp;
    
    c._m11 = _m11;
    temp = _m21;		c._m21 = _m12;		c._m12 = temp;
    temp = _m31;		c._m31 = _m13;		c._m13 = temp;
    temp = _m41;		c._m41 = _m14;		c._m14 = temp;
    temp = _m12;		c._m12 = _m21;		c._m21 = temp;
    
    c._m22 = _m22;
    temp = _m32;		c._m32 = _m23;		c._m23 = temp;
    temp = _m42;		c._m42 = _m24;		c._m24 = temp;
    temp = _m13;		c._m13 = _m31;		c._m31 = temp;
    temp = _m23;		c._m23 = _m32;		c._m32 = temp;
    
    c._m33 = _m33;
    temp = _m43;		c._m43 = _m34;		c._m34 = temp;
    temp = _m14;		c._m14 = _m41;		c._m41 = temp;
    temp = _m24;		c._m24 = _m42;		c._m42 = temp;
    temp = _m34;		c._m34 = _m43;		c._m43 = temp;
    
    c._m44 = _m44;
	return c;
}

//==============================================================================
//==============================================================================

const Matrix4& Matrix4::identity (void)
{
	static Matrix4 c(	1.0F, 0.0F, 0.0F, 0.0F,
						0.0F, 1.0F, 0.0F, 0.0F,
						0.0F, 0.0F, 1.0F, 0.0F,
						0.0F, 0.0F, 0.0F, 1.0F);
	return c;
}

//==============================================================================
//==============================================================================

const Matrix4& Matrix4::zero (void)
{
	static Matrix4 c(	0.0F, 0.0F, 0.0F, 0.0F,
						0.0F, 0.0F, 0.0F, 0.0F,
						0.0F, 0.0F, 0.0F, 0.0F,
						0.0F, 0.0F, 0.0F, 0.0F);
	return c;
}

//==============================================================================
//==============================================================================

Matrix4 Matrix4::orthoed (void) const
{
    Vector3 x = x_axis(), y = y_axis(), z;
    
    x.normalize();
    y.normalize();
    
    z = Vector3::cross(x,y);
    z.normalize();

    y = Vector3::cross(z,x);	
    
    Matrix4 r;
    
    r._m11 = x.x;
    r._m21 = x.y;
    r._m31 = x.z;
    r._m41 = _m41;
    
    r._m12 = y.x;
    r._m22 = y.y;
    r._m32 = y.z;
    r._m42 = _m42;

    r._m13 = z.x;
    r._m23 = z.y;
    r._m33 = z.z;
    r._m43 = _m43;

    r._m14 = _m14;
    r._m24 = _m24;
    r._m34 = _m34;
    r._m44 = _m44;

    return r;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: Matrix3.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Matrix2.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

Matrix3::Matrix3 (void)
    :   _m11 (1.0F), _m12 (0.0F), _m13 (0.0F),
        _m21 (0.0F), _m22 (1.0F), _m23 (0.0F),
        _m31 (0.0F), _m32 (0.0F), _m33 (1.0F)
{

}

Matrix3::Matrix3 (const Matrix2 &rhs)
{	
	_m11 = rhs._m11;	_m12 = rhs._m12;	_m13 = 0.0F; 
	_m21 = rhs._m21;	_m22 = rhs._m22;	_m23 = 0.0F; 
	_m31 = 0.0F;		_m32 = 0.0F;		_m33 = 1.0F;  
}

Matrix3::Matrix3 (const Matrix3 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13; 
	_m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23; 
	_m31 = rhs._m31; _m32 = rhs._m32; _m33 = rhs._m33;  
}

Matrix3::Matrix3 (const Matrix4 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13; 
	_m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23; 
	_m31 = rhs._m31; _m32 = rhs._m32; _m33 = rhs._m33;  
}

Matrix3::Matrix3 (const Quaternion &rhs)
{
    // If q is guaranteed to be a unit quaternion, s will always
    // be 1.  In that case, this calculation can be optimized out.
    DTfloat norm = rhs.x*rhs.x + rhs.y*rhs.y + rhs.z*rhs.z + rhs.w*rhs.w;
    DTfloat s = (norm > 0.0F) ? 2.0F/norm : 0.0F;
    
    // Precalculate coordinate products
    DTfloat xs = rhs.x * s;
    DTfloat ys = rhs.y * s;
    DTfloat zs = rhs.z * s;
    
    DTfloat xx = rhs.x * xs;
    DTfloat yy = rhs.y * ys;
    DTfloat zz = rhs.z * zs;
    DTfloat xy = rhs.x * ys;
    DTfloat xz = rhs.x * zs;
    DTfloat yz = rhs.y * zs;
    DTfloat wx = rhs.w * xs;
    DTfloat wy = rhs.w * ys;
    DTfloat wz = rhs.w * zs;

    // Calculate 3x3 matrix from orthonormal basis
    // x axis
    _m11 = 1.0F - (yy + zz);
    _m12 = xy - wz;
    _m13 = xz + wy;
    
    // y axis
    _m21 = xy + wz;
    _m22 = 1.0F - (xx + zz);
    _m23 = yz - wx;
    
    // z axis
    _m31 = xz - wy;
    _m32 = yz + wx;
    _m33 = 1.0F - (xx + yy);
}

Matrix3::Matrix3 (	const DTfloat m11,const DTfloat m12,const DTfloat m13,
                    const DTfloat m21,const DTfloat m22,const DTfloat m23,
                    const DTfloat m31,const DTfloat m32,const DTfloat m33   )
{	
	_m11 = m11; _m12 = m12; _m13 = m13; 
	_m21 = m21; _m22 = m22; _m23 = m23; 
	_m31 = m31; _m32 = m32; _m33 = m33;                 
}

Matrix3::Matrix3 (const Vector3 &vx, const Vector3 &vy, const Vector3 &vz)
{	
	_m11 = vx.x;  _m12 = vy.x;  _m13 = vz.x;  
	_m21 = vx.y;  _m22 = vy.y;  _m23 = vz.y;  
	_m31 = vx.z;  _m32 = vy.z;  _m33 = vz.z;      
}

Matrix3& Matrix3::operator = (const Matrix3 &rhs)
{	
	_m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13; 
	_m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23; 
	_m31 = rhs._m31; _m32 = rhs._m32; _m33 = rhs._m33; 
	return *this;                                       
}

//==============================================================================
//==============================================================================

Stream& operator << (Stream &s, const Matrix3&m)
{
	s	<< m._m11 << Stream::fs << m._m21 << Stream::fs << m._m31 << Stream::fs
		<< m._m12 << Stream::fs << m._m22 << Stream::fs << m._m32 << Stream::fs
		<< m._m13 << Stream::fs << m._m23 << Stream::fs << m._m33;

	return s;
}

Stream& operator >> (Stream &s, Matrix3&m)
{
	s	>> m._m11 >> m._m21 >> m._m31;
	s	>> m._m12 >> m._m22 >> m._m32;
	s	>> m._m13 >> m._m23 >> m._m33;

	return s;
}

//==============================================================================
//==============================================================================

DTboolean Matrix3::operator == (const Matrix3& rhs) const
{
	return (    _m11 == rhs._m11	&& _m21 == rhs._m21     && _m31 == rhs._m31     && 
		_m12 == rhs._m12	&& _m22 == rhs._m22     && _m32 == rhs._m32     && 
		_m13 == rhs._m13	&& _m23 == rhs._m23     && _m33 == rhs._m33
    );
}


DTboolean Matrix3::operator != (const Matrix3& rhs) const
{
	return (    _m11 != rhs._m11	|| _m21 != rhs._m21     || _m31 != rhs._m31     || 
		_m12 != rhs._m12	|| _m22 != rhs._m22     || _m32 != rhs._m32     || 
		_m13 != rhs._m13	|| _m23 != rhs._m23     || _m33 != rhs._m33
    );
}

Matrix3& Matrix3::operator *= (const DTfloat rhs)
{
	_m11*=rhs; _m21*=rhs; _m31*=rhs;
	_m12*=rhs; _m22*=rhs; _m32*=rhs;
	_m13*=rhs; _m23*=rhs; _m33*=rhs;

	return *this;
}

Matrix3& Matrix3::operator /= (const DTfloat rhs)
{
	_m11/=rhs; _m21/=rhs; _m31/=rhs;
	_m12/=rhs; _m22/=rhs; _m32/=rhs;
	_m13/=rhs; _m23/=rhs; _m33/=rhs;

	return *this;
}

Matrix3& Matrix3::operator *= (const Matrix3 &rhs)
{
    Matrix3 t;
    
    t._m11  = _m11 * rhs._m11 + _m12 * rhs._m21 + _m13 * rhs._m31;
    t._m21  = _m21 * rhs._m11 + _m22 * rhs._m21 + _m23 * rhs._m31;
    t._m31  = _m31 * rhs._m11 + _m32 * rhs._m21 + _m33 * rhs._m31;
    
    t._m12  = _m11 * rhs._m12 + _m12 * rhs._m22 + _m13 * rhs._m32;
    t._m22  = _m21 * rhs._m12 + _m22 * rhs._m22 + _m23 * rhs._m32;
    t._m32  = _m31 * rhs._m12 + _m32 * rhs._m22 + _m33 * rhs._m32;
    
    t._m13  = _m13 * rhs._m33 + _m11 * rhs._m13 + _m12 * rhs._m23;
    t._m23  = _m23 * rhs._m33 + _m21 * rhs._m13 + _m22 * rhs._m23;
    t._m33  = _m33 * rhs._m33 + _m31 * rhs._m13 + _m32 * rhs._m23;
    
    *this = t;
	return *this;
}

Matrix3 operator * (const DTfloat a, const Matrix3 &b)
{
	return Matrix3(	b._m11 * a, b._m12 * a, b._m13 * a,
					b._m21 * a, b._m22 * a, b._m23 * a,
					b._m31 * a, b._m32 * a, b._m33 * a	);
}

Matrix3 operator * (const Matrix3 &a, const DTfloat b)
{
	return Matrix3(	a._m11 * b, a._m12 * b, a._m13 * b,
					a._m21 * b, a._m22 * b, a._m23 * b,
					a._m31 * b, a._m32 * b, a._m33 * b	);
}

Matrix3 operator * (const Matrix3 &a, const Matrix3 &b)
{    
    return Matrix3(	a._m11 * b._m11 + a._m12 * b._m21 + a._m13 * b._m31,
					a._m11 * b._m12 + a._m12 * b._m22 + a._m13 * b._m32,
					a._m13 * b._m33 + a._m11 * b._m13 + a._m12 * b._m23,
					a._m21 * b._m11 + a._m22 * b._m21 + a._m23 * b._m31,
					a._m21 * b._m12 + a._m22 * b._m22 + a._m23 * b._m32,
					a._m23 * b._m33 + a._m21 * b._m13 + a._m22 * b._m23,
					a._m31 * b._m11 + a._m32 * b._m21 + a._m33 * b._m31,
					a._m31 * b._m12 + a._m32 * b._m22 + a._m33 * b._m32,
					a._m33 * b._m33 + a._m31 * b._m13 + a._m32 * b._m23);
}

Vector3 operator * (const Matrix3 &a, const Vector3 &b)
{
	return Vector3(	a._m11 * b.x + a._m12 * b.y + a._m13 * b.z,
					a._m21 * b.x + a._m22 * b.y + a._m23 * b.z,
					a._m31 * b.x + a._m32 * b.y + a._m33 * b.z);
}

Vector2 operator * (const Matrix3 &a, const Vector2 &b)
{    
	return Vector2(	a._m11 * b.x + a._m12 * b.y + a._m13,
					a._m21 * b.x + a._m22 * b.y + a._m23);
}

Matrix3 operator / (const Matrix3 &a, const DTfloat b)
{
	return Matrix3(	a._m11 / b, a._m12 / b, a._m13 / b,
					a._m21 / b, a._m22 / b, a._m23 / b,
					a._m31 / b, a._m32 / b, a._m33 / b );
}

Matrix3 operator / (const Matrix3 &a, const Vector3 &b)
{
    return Matrix3(	a._m11 / b.x, a._m12 / b.y, a._m13 / b.z,
					a._m21 / b.x, a._m22 / b.y, a._m23 / b.z,
					a._m31 / b.x, a._m32 / b.y, a._m33 / b.z );
}

Matrix3 operator + (const Matrix3 &a, const Matrix3 &b)
{
    return Matrix3(	a._m11 + b._m11, a._m12 + b._m12, a._m13 + b._m13,
					a._m21 + b._m21, a._m22 + b._m22, a._m23 + b._m23,
					a._m31 + b._m31, a._m32 + b._m32, a._m33 + b._m33 );
}

Matrix3 operator - (const Matrix3 &a, const Matrix3 &b)
{
    return Matrix3(	a._m11 - b._m11, a._m12 - b._m12, a._m13 - b._m13,
					a._m21 - b._m21, a._m22 - b._m22, a._m23 - b._m23,
					a._m31 - b._m31, a._m32 - b._m32, a._m33 - b._m33 );
}


void Matrix3::clear (void)
{
    _m11 = 0.0F;	_m12 = 0.0F;	_m13 = 0.0F;	
    _m21 = 0.0F;	_m22 = 0.0F;	_m23 = 0.0F;	
    _m31 = 0.0F;	_m32 = 0.0F;	_m33 = 0.0F;	
}

DTfloat Matrix3::determinant (void) const
{
	return	+	_m11 * (_m33 * _m22 - _m32 * _m23)
			-	_m21 * (_m33 * _m12 - _m32 * _m13)
			+	_m31 * (_m23 * _m12 - _m22 * _m13);
}

Matrix3 Matrix3::inversed (void) const
{
    DTfloat det = determinant();
	Matrix3 t;
    
    //UTIL_Assert(det != 0.0F);
    if (det == 0.0F) {
		t.clear();
        return t;
    }
    	
	return Matrix3(	(-_m32 * _m23 + _m22 * _m33)/det, 
					(_m32 * _m13 - _m12 * _m33)/det, 
					(-_m22 * _m13 + _m12 * _m23)/det,
					(_m31 * _m23 - _m21 * _m33)/det,
					(-_m31 * _m13 + _m11 * _m33)/det,
					(_m21 * _m13 - _m11 * _m23)/det,
					(-_m31 * _m22 + _m21 * _m32)/det,
					(_m31 * _m12 - _m11 * _m32)/det,
					(-_m21 * _m12 + _m11 * _m22)/det );
					
					
}

Matrix3 Matrix3::transposed (void) const
{
	Matrix3 c;
    DTfloat temp;
    
    c._m11 = _m11;
    temp = _m21;		c._m21 = _m12;		c._m12 = temp;
    temp = _m31;		c._m31 = _m13;		c._m13 = temp;
    temp = _m12;		c._m12 = _m21;		c._m21 = temp;
    
    c._m22 = _m22;
    temp = _m32;		c._m32 = _m23;		c._m23 = temp;
    temp = _m13;		c._m13 = _m31;		c._m31 = temp;
    temp = _m23;		c._m23 = _m32;		c._m32 = temp;
    
    c._m33 = _m33;
	return c;
}

//==============================================================================
//==============================================================================

Matrix3 Matrix3::orthoed (void) const
{
    Vector3 x = x_axis(), y = y_axis(), z;
    
    x.normalize();
    y.normalize();
    
    z = Vector3::cross(x,y);
    z.normalize();

    y = Vector3::cross(z,x);	
    
    Matrix3 r;
    
    r._m11 = x.x;
    r._m21 = x.y;
    r._m31 = x.z;
    
    r._m12 = y.x;
    r._m22 = y.y;
    r._m32 = y.z;
    
    r._m13 = z.x;
    r._m23 = z.y;
    r._m33 = z.z;
    
    return r;
}

//==============================================================================
/// Rotation matix for rotating vectors from a to b
//==============================================================================

Matrix3 Matrix3::set_rotation (const Vector3 &a, const Vector3 &b)
{
    Vector3	axis;
    axis = Vector3::cross(a,b);
    
    DTfloat	dot;
    dot = Vector3::dot(a,b);
    DTfloat	u = 1.0F - dot;
    
    DTfloat	s = axis.abs();
    
    Matrix3 c;

    if (s < 0.0000001F && s > -0.0000001F/* && c > 0.0F*/) {
        return Matrix3::identity();
    }
    
    axis = axis / s;
    
    
    c._m11  = axis.x*axis.x*u+dot;
    c._m21  = axis.x*axis.y*u+axis.z*s;
    c._m31  = axis.x*axis.z*u-axis.y*s;
    
    c._m12  = axis.y*axis.x*u-axis.z*s;
    c._m22  = axis.y*axis.y*u+dot;
    c._m32  = axis.y*axis.z*u+axis.x*s;
    
    c._m13  = axis.z*axis.x*u+axis.y*s;
    c._m23  = axis.z*axis.y*u-axis.x*s;
    c._m33 = axis.z*axis.z*u+dot;
    
    return c;
}

//==============================================================================
//==============================================================================

Matrix3 Matrix3::set_rotation_around (const Vector3 &a, const DTfloat b)
{
    DTfloat cos_angle = std::cos(b);
    DTfloat one_minus_cos_angle = 1.0F - cos_angle;
    DTfloat sin_angle = std::sin(b);

    Matrix3 c;

    c._m11 = one_minus_cos_angle * a.x * a.x + cos_angle;
    c._m12 = one_minus_cos_angle * a.x * a.y - (a.z * sin_angle);
    c._m13 = one_minus_cos_angle * a.x * a.z + (a.y * sin_angle);
    c._m21 = one_minus_cos_angle * a.x * a.y + (a.z * sin_angle);
    c._m22 = one_minus_cos_angle * a.y * a.y + cos_angle;
    c._m23 = one_minus_cos_angle * a.y * a.z - (a.x * sin_angle); 
    c._m31 = one_minus_cos_angle * a.x * a.z - (a.y * sin_angle);
    c._m32 = one_minus_cos_angle * a.y * a.z + (a.x * sin_angle);
    c._m33 = one_minus_cos_angle * a.z * a.z + cos_angle;
    
    return c;
}

//==============================================================================
//==============================================================================

Matrix3 Matrix3::set_rotation_x (const DTfloat a)
{	
    DTfloat cos_a = std::cos(a);
    DTfloat sin_a = std::sin(a);
    
    Matrix3 b;

    b._m11  = 1.0F;
    b._m21  = 0.0F;
    b._m31  = 0.0F;
    
    b._m12  = 0.0F;
    b._m22  = cos_a;
    b._m32  = -sin_a;
    
    b._m13  = 0.0F;
    b._m23  = sin_a;
    b._m33 = cos_a;
    
    return b;
}

Matrix3 Matrix3::set_rotation_y (const DTfloat a)
{	
    DTfloat cos_a = std::cos(a);
    DTfloat sin_a = std::sin(a);
    
    Matrix3 b;

    b._m11  = cos_a;
    b._m21  = 0.0F;
    b._m31  = sin_a;
    
    b._m12  = 0.0F;
    b._m22  = 1.0F;
    b._m32  = 0.0F;
    
    b._m13  = -sin_a;
    b._m23  = 0.0F;
    b._m33 = cos_a;

    return b;
}

Matrix3 Matrix3::set_rotation_z (const DTfloat a)
{	
    DTfloat cos_a = std::cos(a);
    DTfloat sin_a = std::sin(a);
    
    Matrix3 b;

    b._m11  = cos_a;
    b._m21  = -sin_a;
    b._m31  = 0.0F;
    
    b._m12  = sin_a;
    b._m22  = cos_a;
    b._m32  = 0.0F;
    
    b._m13  = 0.0F;
    b._m23  = 0.0F;
    b._m33 = 1.0F;

    return b;
}

//==============================================================================
//==============================================================================

Matrix3 Matrix3::set_orientation (const Vector3 &back,  const Vector3 &up)
{
    Vector3 x, y, z;
    
    z = back.normalized();
    x = Vector3::cross(up,z);
	
	if (x == Vector3(0.0F,0.0F,0.0F)) {
		return Matrix3::identity(); 
	}
    
    x.normalize();	
    y = Vector3::cross(z,x);
    
    Matrix3 c;

    c._m11 = x.x;		c._m12 = y.x;		c._m13 = z.x;
    c._m21 = x.y;		c._m22 = y.y;		c._m23 = z.y;
    c._m31 = x.z;		c._m32 = y.z;		c._m33 = z.z;
    
    return c;
}

//==============================================================================
//==============================================================================

const Matrix3& Matrix3::identity (void)
{
	static Matrix3 c(	1.0F, 0.0F, 0.0F,
						0.0F, 1.0F, 0.0F,
						0.0F, 0.0F, 1.0F);
	return c;
}

const Matrix3& Matrix3::zero (void)
{
	static Matrix3 c(	0.0F, 0.0F, 0.0F,
						0.0F, 0.0F, 0.0F,
						0.0F, 0.0F, 0.0F);
	return c;
}

//==============================================================================
//==============================================================================

Matrix3 Matrix3::tilde (const Vector3 &a)
{
    Matrix3 r;
    
    r._m11 = 0.0F;
    r._m21 = a.z;
    r._m31 = -a.y;
    
    r._m12 = -a.z;
    r._m22 = 0.0F;
    r._m32 = a.x;
    
    r._m13 = a.y;
    r._m23 = -a.x;
    r._m33 = 0.0F;
    
    return r;
}

//==============================================================================
//==============================================================================

void Matrix3::set_diagonal (const Vector3 &a)
{
    _m11 = a.x;
    _m21 = 0.0F;
    _m31 = 0.0F;
    
    _m12 = 0.0F;
    _m22 = a.y;
    _m32 = 0.0F;
    
    _m13 = 0.0F;
    _m23 = 0.0F;
    _m33 = a.z;
}


//==============================================================================
//==============================================================================

Vector3 Matrix3::diagonal (void)
{
    Vector3 c;
    c.x = _m11;
    c.y = _m22;
    c.z = _m33;
    return c;
}

//==============================================================================
//==============================================================================

} // DT3


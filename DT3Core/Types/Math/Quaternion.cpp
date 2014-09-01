//==============================================================================
///	
///	File: Quaternion.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Quaternion.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

Quaternion::Quaternion (void)
    :   w   (1.0F),
        x   (0.0F),
        y   (0.0F),
        z   (0.0F)
{

}

Quaternion::Quaternion (const Matrix3 &rhs)
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    DTfloat fTrace = rhs._m[0][0] + rhs._m[1][1] + rhs._m[2][2];
    DTfloat fRoot;

    if ( fTrace > 0.0F )
    {
        // |w| > 1/2][ may as well choose w > 1/2
        fRoot = std::sqrt(fTrace + 1.0F);  // 2w
        w = 0.5F * fRoot;
        fRoot = 0.5F / fRoot;  // 1/(4w)
        x = (rhs._m[1][2]-rhs._m[2][1]) * fRoot;
        y = (rhs._m[2][0]-rhs._m[0][2]) * fRoot;
        z = (rhs._m[0][1]-rhs._m[1][0]) * fRoot;
    }
    else
    {
        // |w| <= 1/2
        DTint i = 0;
		static DTint ms_iNext[3] = { 1, 2, 0 };
	
        if ( rhs._m[1][1] > rhs._m[0][0] )
            i = 1;
        if ( rhs._m[2][2] > rhs._m[i][i] )
            i = 2;
	    
        DTint j = ms_iNext[i];
        DTint k = ms_iNext[j];

        fRoot = std::sqrt(rhs._m[i][i] - rhs._m[j][j] - rhs._m[k][k] + 1.0F);
        DTfloat* apfQuat[3] = { &x, &y, &z };
	
        *apfQuat[i] = 0.5F * fRoot;
        fRoot = 0.5F / fRoot;
	
        w = (rhs._m[j][k]-rhs._m[k][j]) * fRoot;
        *apfQuat[j] = (rhs._m[i][j]+rhs._m[j][i]) * fRoot;
        *apfQuat[k] = (rhs._m[i][k]+rhs._m[k][i]) * fRoot;
    }

}

Quaternion::Quaternion (const Matrix4 &rhs)
{
    Quaternion q(rhs.orientation());
    *this = q;
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Quaternion&q)
{
	s << q.w << Stream::fs << q.x << Stream::fs << q.y << Stream::fs << q.z;
	return s;
}

Stream& operator >>(Stream &s, Quaternion&q)
{
	DTfloat w,x,y,z;
	s >> w >> x >> y >> z;
	
	q.w = w;
	q.x = x;
	q.y = y;
	q.z = z;
	
	return s;
}

//==============================================================================
//==============================================================================

Quaternion Quaternion::identity (void)
{
	return Quaternion(1.0F,0.0F,0.0F,0.0F);
}

//==============================================================================
//==============================================================================

void Quaternion::clear (void)
{
    w = 0.0F;
    x = 0.0F;
    y = 0.0F;
    z = 0.0F;
}

//==============================================================================
//==============================================================================

Vector3 Quaternion::transform      (const Quaternion &a, const Vector3 &b)
{
    Vector3 r;
    Quaternion a_conj, temp;
    Quaternion b_quat(0.0F, b.x, b.y, b.z);
    
    a_conj = a.conjugated();
    temp = a * b_quat * a_conj;
    
    r.x = temp.x;
    r.y = temp.y;
    r.z = temp.z;

    return r;
}

Vector3 Quaternion::transform_inv  (const Quaternion &a, const Vector3 &b)
{
    Vector3 r;
    Quaternion a_conj, temp;
    Quaternion b_quat(0.0F, b.x, b.y, b.z);
    
    a_conj = a.conjugated();
    temp = a_conj * b_quat * a;

    r.x = temp.x;
    r.y = temp.y;
    r.z = temp.z;
    
    return r;
}

//==============================================================================
//==============================================================================

DTfloat Quaternion::dot (const Quaternion &a, const Quaternion &b)
{
    return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

//==============================================================================
//==============================================================================

DTfloat Quaternion::norm (void) const
{
    return w * w + x * x + y * y + z * z;
}

//==============================================================================
//==============================================================================

DTfloat Quaternion::length (void) const
{
    return std::sqrt(w*w + x*x + y*y + z*z);
}



//==============================================================================
//==============================================================================

Quaternion Quaternion::conjugated (void) const
{
    Quaternion r;

    r.w = w;
    r.x = -x;
    r.y = -y;
    r.z = -z;
    
    return r;
}


//==============================================================================
//==============================================================================

Quaternion Quaternion::normalized (void) const
{
    Quaternion r;
    
    DTfloat length = std::sqrt(w*w + x*x + y*y + z*z);

    if (length > 0.0F)
		r = (*this) / length;
    
    return r;
}

//==============================================================================
//==============================================================================

Quaternion Quaternion::inversed (void) const
{
    DTfloat n = norm();
    Quaternion r = conjugated();
    r = r / n;
    
    return r;
}

//==============================================================================
//==============================================================================

Quaternion Quaternion::logged (void) const
{
    Quaternion r;
    
    DTfloat   angle = std::acos(w);
    DTfloat   sin_angle = std::sin(angle);
   
    r.w = 0.0F;
    
    if (sin_angle > 0.0F) {
		DTfloat coeff = angle / sin_angle;
		r.x = (coeff * x); 
		r.y = (coeff * y); 
		r.z = (coeff * z); 
    } else {
    	r.x = (0.0F);
    	r.y = (0.0F);
		r.z = (0.0F);
    }
    
    return r;
}

//==============================================================================
//==============================================================================

Quaternion Quaternion::exped (void) const
{
    Quaternion r;
    
    DTfloat   angle = std::sqrt(x*x + y*y + z*z);
    DTfloat   sin_angle = std::sin(angle);

    r.w = std::cos(angle);

    if (angle > 0.0F) {
		DTfloat coeff = sin_angle / angle;
		r.x = (coeff * x);
		r.y = (coeff * y);
		r.z = (coeff * z);
    } else {
		r.x = (0.0F);
		r.y = (0.0F);
		r.z = (0.0F);
    }

    return r;
}

//==============================================================================
//==============================================================================

Quaternion Quaternion::lerp (const Quaternion &a, const Quaternion &b, const DTfloat t)
{
    Quaternion r;
    Quaternion temp;

    temp = a + ((b - a) * t);
    r = temp.normalized();
    
    return r;
}

//==============================================================================
//==============================================================================

Quaternion Quaternion::slerp_no_invert (const Quaternion &a, const Quaternion &b, const DTfloat t)
{
    Quaternion r;
    DTfloat dot = a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;

    if (dot > -0.97F && dot < 0.97F) {
		DTfloat angle = std::acos(dot);
		DTfloat sina, sinat, sinaomt;

		sina = std::sin(angle);
		sinat = std::sin(angle*t);
		sinaomt = std::sin(angle * (1.0F - t));

		r.w = (a.w * sinaomt + b.w * sinat) / sina;
		r.x = (a.x * sinaomt + b.x * sinat) / sina;
		r.y = (a.y * sinaomt + b.y * sinat) / sina;
		r.z = (a.z * sinaomt + b.z * sinat) / sina;

    } else {
		r = lerp(a,b,t);
    }
    
    return r;
}

Quaternion Quaternion::slerp (const Quaternion &a, const Quaternion &b, const DTfloat t)
{
    Quaternion r;
    Quaternion c;
    DTfloat dot = a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
	
    if (dot < 0.0F) {
		dot = -dot;
		c = -b;
    } else {
		c = b;
    }
	
    if (dot < 0.97F) {
		DTfloat angle = std::acos(dot);
		DTfloat sina, sinat, sinaomt;
		
		sina = std::sin(angle);
		sinat = std::sin(angle*t);
		sinaomt = std::sin(angle * (1.0F - t));
		
		r.w = (a.w * sinaomt + c.w * sinat) / sina;
		r.x = (a.x * sinaomt + c.x * sinat) / sina;
		r.y = (a.y * sinaomt + c.y * sinat) / sina;
		r.z = (a.z * sinaomt + c.z * sinat) / sina;
		
    } else {
		r = lerp(a,c,t);
    }
    
    return r;
}

Quaternion Quaternion::squad( const Quaternion &a, const Quaternion &b,
                        const Quaternion &a1, const Quaternion &b1, 
                        const DTfloat t )
{
    Quaternion r;
    Quaternion ra, rb;

    ra = slerp_no_invert (a, b, t);
    rb = slerp_no_invert (a1, b1, t);
    r = slerp_no_invert (ra, rb, 2.0F * t * (1.0F - t));
    return r;
}

Quaternion Quaternion::spline	(const Quaternion &a, const Quaternion &b, const Quaternion &c)
{
    Quaternion r;
    Quaternion b_inv, tmp0, tmp1;

    b_inv = b.conjugated();

    tmp0 = b_inv * a;
    tmp0 = tmp0.logged();

    tmp1 = b_inv * c;
    tmp1 = tmp1.logged();
    
    tmp0 = (tmp0 + tmp1) / -4.0F;

    tmp0 = tmp0.exped();
    r = tmp0 * b;
    
    return r;
}

//==============================================================================
//==============================================================================

Quaternion Quaternion::convert_axis_angle    (const Vector3 &a, const DTfloat angle)
{        
    Quaternion r;
    DTfloat s = std::sin(angle*0.5F);
    
    r.w = std::cos(angle / 2.0F);
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    
    return r;
}

//==============================================================================
/// Used in constraint code
//==============================================================================

Quaternion Quaternion::convert_axis	    (const Vector3 &a)
{
    Quaternion r;
    DTfloat absval = a.abs();
    
    if (absval == 0.0F) {
		r = Quaternion::identity();
		return r;
    }
    
    DTfloat absval_div_2 = absval / 2.0F;
    DTfloat s = std::sin (absval_div_2) / absval;
    
    r.w = std::cos (absval_div_2);
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    
    return r;
}

//==============================================================================
//==============================================================================

DTboolean Quaternion::is_zero (const Quaternion &a)
{
    return (a.w == 0.0 && a.x == 0.0 && a.y == 0.0 && a.z == 0.0);
}

//==============================================================================
//==============================================================================

} // DT3


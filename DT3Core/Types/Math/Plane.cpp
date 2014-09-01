//==============================================================================
///	
///	File: Plane.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Plane.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Math/Sphere.hpp"
#include "DT3Core/Types/Math/Box.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Plane::Plane (void)
    :	n(0.0F,1.0F,0.0F),
		d(0.0F)
{

}

Plane::Plane (const Vector3 &n_, const DTfloat &d_)
{
    n = n_;
    d = d_;
}

Plane::Plane (const Vector3 &n_, const Vector3 &p)
{
    n = n_;
    d = Vector3::dot(n, p);
}

Plane::Plane (const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
    set(a,b,c);
}

Plane::Plane (const DTfloat &a, const DTfloat &b, const DTfloat &c, const DTfloat &d_)
{
    n = Vector3(a,b,c);
    d = d_;
}

Plane::Plane (const Vector4 &a)
{
    n = Vector3(a.x, a.y, a.z);
    d = a.w;
}

Plane::Plane (const Plane &rhs)
    :	n(rhs.n),
        d(rhs.d)
{

}
            
Plane& Plane::operator = (const Plane &rhs)
{	
	n = rhs.n;
	d = rhs.d;
    return (*this);
}	

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const Plane&p)
{
	s << p.normal().x << Stream::fs << p.normal().y << Stream::fs << p.normal().z << Stream::fs << p.D();
	return s;
}

Stream& operator >>(Stream &s, Plane&p)
{
    Vector3 normal(0.0F,1.0F,0.0F);
    DTfloat d(0.0F);
    
	s >> normal.x >> normal.y >> normal.z >> d;
    
    p.set_normal(normal);
    p.set_d(d);
    
	return s;
}

//==============================================================================
//==============================================================================

DTboolean Plane::is_sphere_completely_in_front (const Vector3 &translation, const Sphere &s) const
{
    return distance_to_point(translation) > s.radius();
}

DTboolean Plane::is_sphere_completely_in_back (const Vector3 &translation, const Sphere &s) const
{
    return distance_to_point(translation) < -s.radius();
}

//==============================================================================
/// Calculates the distance to a sphere.
//==============================================================================

DTfloat Plane::distance_to_sphere (const Vector3 &translation, const Sphere &s) const
{
    DTfloat to_center = distance_to_point(translation);

    if (to_center > 0.0F) {
		if (to_center > s.radius())     return to_center - s.radius();
		else                            return 0.0F;
    } else {
		if (-to_center > s.radius())	return to_center + s.radius();
		else                            return 0.0F;
    }
    
    return 0.0F;
}

//==============================================================================
/// Calculate the distance to a box.
//==============================================================================

DTfloat Plane::distance_to_box (const Box &box) const
{
    Vector3 far_pos, far_neg;
    
    // find positive and negative points farthest from the plane
    if (n.x > 0.0F) {                                                                               // +x
        if (n.y > 0.0F) {                                                                           // +x,+y
            if (n.z > 0.0F)	{	far_pos = Vector3(box.plus_x(), box.plus_y(), box.plus_z());		// +x,+y,+z
                                far_neg = Vector3(box.minus_x(), box.minus_y(), box.minus_z());}
            else            {	far_pos = Vector3(box.plus_x(), box.plus_y(), box.minus_z());		// +x,+y,-z
                                far_neg = Vector3(box.minus_x(), box.minus_y(), box.plus_z());	}
        } else {																					// +x,-y
            if (n.z > 0.0F)	{	far_pos = Vector3(box.plus_x(), box.minus_y(), box.plus_z());		// +x,-y,+z
                                far_neg = Vector3(box.minus_x(), box.plus_y(), box.minus_z());	}
            else            {	far_pos = Vector3(box.plus_x(), box.minus_y(), box.minus_z());		// +x,-y,-z
                                far_neg = Vector3(box.minus_x(), box.plus_y(), box.plus_z());	}
        }
    } else {																						// -x
        if (n.y > 0.0F) {																			// -x,+y
            if (n.z > 0.0F)	{	far_pos = Vector3(box.minus_x(), box.plus_y(), box.plus_z());		// -x,+y,+z
                                far_neg = Vector3(box.plus_x(), box.minus_y(), box.minus_z());	}
            else            {	far_pos = Vector3(box.minus_x(), box.plus_y(), box.minus_z());		// -x,+y,-z
                                far_neg = Vector3(box.plus_x(), box.minus_y(), box.plus_z());	}
        } else {																						// -x,-y
            if (n.z > 0.0F)	{	far_pos = Vector3(box.minus_x(), box.minus_y(), box.plus_z());		// -x,-y,+z
                                far_neg = Vector3(box.plus_x(), box.plus_y(), box.minus_z());	}
            else            {	far_pos = Vector3(box.minus_x(), box.minus_y(), box.minus_z());		// -x,-y,-z
                                far_neg = Vector3(box.plus_x(), box.plus_y(), box.plus_z());	}
        }
    }
    
    DTfloat far_pos_dist = distance_to_point (far_pos);
    DTfloat far_neg_dist = distance_to_point (far_neg);
    
    if (far_pos_dist < 0.0F)		return far_pos_dist;
    else if (far_neg_dist > 0.0F)	return far_neg_dist;
    return 0.0F;
    
}


//==============================================================================
/// Normal and a point defines a plane
//==============================================================================

void Plane::set(const Vector3 &p, const Vector3 &n_)
{
    // Point on plane, point on n
    n = n_.normalized();
    d = -(n.x * p.x + n.y * p.y + n.z * p.z);
}

//==============================================================================
/// Three points on the plane define a plane
//==============================================================================

void Plane::set(const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
    Vector3 d1,d2;
    DTfloat mag;
    
    d1 = b - a;
    d2 = c - a;
    
    n = Vector3::cross(d1,d2);
    
    mag = n.abs();
    if (mag > 0.0F)
        n = n / mag;
	
    d = -(n.x * a.x + n.y * a.y + n.z * a.z);
}


//==============================================================================
/// Inverts the plane face.
//==============================================================================

Plane Plane::flipped() const
{
    Plane c;
    c.n = -n;
    c.d = -d;
    return c;
}

//==============================================================================
/// Tests if the planes are equivalent.
//==============================================================================

Vector3 Plane::reflect (const Vector3 &a) const
{
    DTfloat distance = distance_to_point(a);
    
    Vector3  offset;
    offset = n * (-2.0F * distance);
    offset = offset + a;
    
    return offset;
}

//==============================================================================
//==============================================================================

void Plane::normalize (void)
{
    DTfloat len_inv = 1.0F / n.abs();
    n *= len_inv;
    d *= len_inv;
}

//==============================================================================
//==============================================================================

} // DT3

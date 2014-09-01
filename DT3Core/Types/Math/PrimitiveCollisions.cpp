//==============================================================================
///	
///	File: PrimitiveCollisions.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/PrimitiveCollisions.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Plane.hpp"
#include "DT3Core/Types/Math/Box.hpp"
#include "DT3Core/Types/Math/Sphere.hpp"
#include <cstdlib>
#include <cmath>
#include <limits>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTboolean PrimitiveCollisions::line_intersect_line (    const Vector2 &from1, const Vector2 &to1,
                                                        const Vector2 &from2, const Vector2 &to2,
                                                        DTfloat &t1, DTfloat &t2)
{
    DTfloat to1y_minus_from1y = to1.y-from1.y;
    DTfloat to2y_minus_from2y = to2.y-from2.y;
    DTfloat to1x_minus_from1x = to1.x-from1.x;
    DTfloat to2x_minus_from2x = to2.x-from2.x;
	
	DTfloat den = (to2y_minus_from2y) * (to1x_minus_from1x) - (to2x_minus_from2x) * (to1y_minus_from1y);
	if (den < EPSILON && den > -EPSILON)	return false;
    
    DTfloat from1y_minus_from2y = from1.y-from2.y;
    DTfloat from1x_minus_from2x = from1.x-from2.x;

	DTfloat ua_num = (to2x_minus_from2x)*(from1y_minus_from2y) - (to2y_minus_from2y)*(from1x_minus_from2x);
	DTfloat ub_num = (to1x_minus_from1x)*(from1y_minus_from2y) - (to1y_minus_from1y)*(from1x_minus_from2x);
	
	t1 = ua_num / den;
	t2 = ub_num / den;

	return (t1 >= 0.0F && t1 <= 1.0F && t2 >= 0.0F && t2 <= 1.0F);
}
																			
//==============================================================================
//==============================================================================

DTboolean PrimitiveCollisions::ray_intersect_triangle   (   const Vector3 &from, const Vector3 &to,
                                                            const Vector3 &vert0, const Vector3 &vert1, const Vector3 &vert2,
                                                            DTfloat &t, Vector3 &n, Vector3 &p)
{
    Vector3 edge1, edge2, tvec, pvec, qvec, dir;
    DTfloat det,inv_det,u,v;

    // Calc dir
    dir = to - from;

    // find vectors for two edges sharing vert0
    edge1 = vert1 - vert0;
    edge2 = vert2 - vert0;

    // begin calculating determinant - also used to calculate U parameter
    pvec = Vector3::cross(dir, edge2);

    // if determinant is near zero, ray lies in plane of triangle
    det = Vector3::dot(edge1, pvec);

    if (det < EPSILON)
        return false;

    // calculate distance from vert0 to ray origin
    tvec = from - vert0;

    // calculate U parameter and test bounds
    u = Vector3::dot(tvec, pvec);
    if (u < 0.0F || u > det)
        return false;

    // prepare to test V parameter
	qvec = Vector3::cross(tvec, edge1);

    // calculate V parameter and test bounds
    v = Vector3::dot(dir, qvec);
    if (v < 0.0F || u + v > det)
        return false;

    // calculate t, scale parameters, ray intersects triangle
    DTfloat test_t;
    test_t = Vector3::dot(edge2, qvec);
    
    inv_det = 1.0F / det;
    test_t *= inv_det;

    if (test_t > t || test_t < 0.0F) {
        return false;
	
    } else {
        // Calculate collision parameters
        t = test_t;
        
        // Collision point
        Vector3::displace(vert0, edge1, u * inv_det, p);
        Vector3::displace(p, edge2, v * inv_det, p);
        
        // Collision normal
        n = Vector3::cross(edge1, edge2);

        return true;
    }
}

//==============================================================================
//==============================================================================

DTboolean     PrimitiveCollisions::ray_intersect_plane	(   const Vector3 &from, const Vector3 &direction,
															const Plane &p, DTfloat &t)
{
	DTfloat dir_dot_n = Vector3::dot(p.normal(), direction);

	if ( dir_dot_n < EPSILON && dir_dot_n > -EPSILON)
		return false;
		
	t = (p.D() -  Vector3::dot(from, p.normal())) / dir_dot_n;
	
	return true;
}

//==============================================================================
//==============================================================================

DTboolean     PrimitiveCollisions::ray_intersect_box (  const Vector3 &from, const Vector3 &direction,
                                                        const Box &b, DTfloat &t)
{
    DTfloat tmin = -std::numeric_limits<DTfloat>::infinity();
    DTfloat tmax = std::numeric_limits<DTfloat>::infinity();
    
    DTfloat t1,t2;
    
    // X axis
    if (direction.x > EPSILON || direction.x < -EPSILON) {
        DTfloat direction_inverse = 1.0F / direction.x;
        t1 = (b.minus_x() - from.x) * direction_inverse;
        t2 = (b.plus_x() - from.x) * direction_inverse;
        
        tmin = MoreMath::min(tmin,t1,t2);
        tmax = MoreMath::max(tmin,t1,t2);
        
        if (tmin > tmax)    return false;
        if (tmax < 0.0F)    return false;
    } else if (from.x < b.minus_x() || from.x > b.plus_x()) {
        return false;
    }   
    
    // Y axis
    if (direction.y > EPSILON || direction.y < -EPSILON) {
        DTfloat direction_inverse = 1.0F / direction.y;
        t1 = (b.minus_y() - from.y) * direction_inverse;
        t2 = (b.plus_y() - from.y) * direction_inverse;
        
        tmin = MoreMath::min(tmin,t1,t2);
        tmax = MoreMath::max(tmin,t1,t2);
        
        if (tmin > tmax)    return false;
        if (tmax < 0.0F)    return false;
    } else if (from.y < b.minus_y() || from.y > b.plus_y()) {
        return false;
    }
    
    // Z axis
    if (direction.z > EPSILON || direction.z < -EPSILON) {
        DTfloat direction_inverse = 1.0F / direction.z;
        t1 = (b.minus_z() - from.z) * direction_inverse;
        t2 = (b.plus_z() - from.z) * direction_inverse;
        
        tmin = MoreMath::min(tmin,t1,t2);
        tmax = MoreMath::max(tmin,t1,t2);
        
        if (tmin > tmax)    return false;
        if (tmax < 0.0F)    return false;
    } else if (from.z < b.minus_z() || from.z > b.plus_z()) {
        return false;
    }
    
    if (tmin <= tmax) {
        t = tmin;
        return true;
    }

    return false;
}

//==============================================================================
//==============================================================================

DTboolean     PrimitiveCollisions::ray_intersect_sphere(    const Vector3 &from, const Vector3 &direction,
                                                            const Vector3 &translation, const Sphere &s,
                                                            DTfloat &t0, DTfloat &t1)
{
	DTfloat fmpx = from.x - translation.x;
	DTfloat fmpy = from.y - translation.y;
	DTfloat fmpz = from.z - translation.z;

	DTfloat a = (direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z);
	DTfloat b = 2.0F * (direction.x * fmpx + direction.y * fmpy + direction.z * fmpz);
	DTfloat c = fmpx * fmpx + fmpy * fmpy + fmpz * fmpz - s.radius() * s.radius();
	
    //Solves at^2 + bt + c = 0. Page 623 of real time rendering.
	MoreMath::solve_quadratic (a, b, c, t0, t1);
				
	return true;
}

//==============================================================================
//==============================================================================

DTboolean     PrimitiveCollisions::extruded_sphere_intersect_triangle   (	const Vector3 &from, const Vector3 &to, const Sphere &s,
                                                                            const Vector3 &vert0, const Vector3 &vert1, const Vector3 &vert2,
                                                                            DTfloat &t, Vector3 &n, Vector3 &p)
{
    Vector3 temp, from_offset, to_offset;
    Plane plane;
    
    // set up the plane
    plane.set(vert0, vert1, vert2);
            
    // get distance
    DTfloat from_dist = plane.distance_to_point(from);
    DTfloat to_dist = plane.distance_to_point(to);
    
    // Make sure it could potentially be colliding
    if (to_dist > s.radius() || from_dist < 0.0F || from_dist <= to_dist)
		return false;

    // Offset the from and to vectors by the radius of the sphere
    Vector3::displace(from, plane.normal(), -s.radius(), from_offset);
    Vector3::displace(to, plane.normal(), -s.radius(), to_offset);
         
    // If we have made it this far, we know that the closest spot on the sphere to
    // the plane crosses the plane boundary.
    
    // Check the case where the ray intersects the triangle
    if (ray_intersect_triangle(from_offset, to_offset, vert0, vert1, vert2, t, n, p))
		return true;
        
    // Even if the ray doesn't intersect the triangle, the sphere might touch an edge
        
    const Vector3* verts[] = { &vert0, &vert1, &vert2 };  // references to verts
    DTfloat   r_squared = s.radius()*s.radius();
    
    // save the values for the vertices
    DTfloat   kss_save[3];
    DTfloat   kgs_save[3];
    DTfloat   kgg_save[3];
    
    // Some intermediate calcs that don't change in the loop
    Vector3 ks;
    ks = to - from;
    
    DTfloat kss;
    kss = Vector3::dot(ks,ks);
    
    // Mark that we haven't found a collision yet
    DTboolean found_collision = false;
    
    for (DTint i = 0; i < 3; ++i) {
		DTint j = (i+1) % 3;  // next index
			
		// Some intermediate calcs that do change in the loop
		Vector3 ke, kg;
		ke = *verts[j] - *verts[i];
		kg = *verts[i] - from;
		
		DTfloat kee,kes,kgs,keg,kgg;	
		kee = Vector3::dot(ke,ke);     keg = Vector3::dot(ke,kg);
		kes = Vector3::dot(ke,ks);     kgg = Vector3::dot(kg,kg);
		kgs = Vector3::dot(kg,ks);     
		
		// save a few values for vertex collision later
		kss_save[i] = kss;
		kgs_save[i] = kgs;
		kgg_save[i] = kgg;

		// Coefficients of quadratic
		DTfloat a,b,c;
		a = kee*kss - kes*kes;
		b = 2.0F * (keg*kes - kee*kgs);
		c = kee * (kgg - r_squared) - keg * keg;
		
		// Calculate collision time by solving quadratic
		DTfloat t0,t1;
		MoreMath::solve_quadratic (a, b, c, t0, t1);
				
		// sort the t's
		if (t1 < t0)    std::swap(t0,t1);
		
		// Pick a t
		DTfloat test_t;
		if (t0 >= 0.0F && t0 <= t)		test_t = t0;
		else if (t1 >= 0.0F && t1 <= t)		test_t = t1;
		else continue;  // t not in 0-t range
		
		
		// find distance along edge to collision
		Vector3 ct;
		DTfloat d;

		ct = ks * test_t;
		ct = ct + from - *verts[i];
		
		d = Vector3::dot(ct, ke);
		d = d / kee;
		
		// Check for d in 0-1 range
		if (d < 0.0F || d > 1.0F)
			continue;
			
		// We have found a collision with an edge
		t = test_t;
		
		// Calculate a normal
		Vector3 d_pos;
		d_pos = ke * d;
		n = ct - d_pos;
		
		// Calculate contact point
		Vector3::displace(*verts[i], ke, d, p);
		
		found_collision = true;
    }

    // If we've found an edge collision, we are done
    if (found_collision)
		return true;
    
    // Check the vertices
    for (DTint i = 0; i < 3; ++i) {
    	
		// Calculate collision time
		DTfloat t0,t1;
		MoreMath::solve_quadratic (kss_save[i], -2.0F * kgs_save[i], kgg_save[i] - r_squared, t0, t1);
				
		// sort the t's
		if (t1 < t0)    std::swap(t0,t1);
		
		// Pick a t
		if (t0 >= 0.0F && t0 <= t)	    t = t0;
		else if (t1 >= 0.0F && t1 <= t)     t = t1;
		else continue;  // t not in 0-t range
		
		// update normal
		Vector3 ct;
		Vector3::displace(from, ks, t, ct);
		n = ct - *verts[i];
		
		// Calc contact point - same as vertex
		p = *verts[i];

		found_collision = true;
    }
    
    
    // return the results
    return found_collision;
}

//==============================================================================
//==============================================================================

DTboolean PrimitiveCollisions::sphere_intersect_sphere (    const Vector3 &from1, const Vector3 &dir1, const Sphere &s1,
                                                            const Vector3 &from2, const Vector3 &dir2, const Sphere &s2,
                                                            DTfloat &t1, DTfloat &t2)
{
    Vector3 vab = dir1 - dir2;
    Vector3 l = from1 - from2;
    
    DTfloat a = Vector3::dot(vab,vab);
    DTfloat b = 2.0F * Vector3::dot(l, vab);
    
    DTfloat r = s1.radius() + s2.radius();
    DTfloat c = Vector3::dot(l,l) - r*r;
    
    DTfloat d = b*b-4.0F*a*c;
    if (d < 0.0F)
        return false;
    
    DTfloat q = -0.5F * (b + (b>=0.0F ? 1.0F : -1.0F) * std::sqrt(d));
    
    t1 = q/a;
    t2 = c/q;

    return true;
}

//==============================================================================
//==============================================================================

void		PrimitiveCollisions::distance_point_to_triangle (	const Vector3 &p,
                                                                const Vector3 &vert0, const Vector3 &vert1, const Vector3 &vert2,
                                                                Vector3 &pt, DTfloat &dist)
{
    // TODO: Can this be better? Seems a little large

    Vector3 edge_ab, edge_bc, edge_ca;
    Vector3 n,diff, diff_a, diff_b, diff_c,norm;
    DTfloat dot;
    DTboolean outside_ab, outside_bc, outside_ca;
    
    edge_ab = vert1 - vert0;
    edge_bc = vert2 - vert1;
    edge_ca = vert0 - vert2;
    
    n = Vector3::cross(edge_ca, edge_ab);
    
    // Test point outside edge_ab
    diff_a = p - vert0;
    diff = Vector3::cross(diff_a,edge_ab);
    dot = Vector3::dot(n,diff);
    outside_ab = (dot > 0.0F);

    // Test point outside edge_bc
    diff_b = p - vert1;
    diff = Vector3::cross(diff_b,edge_bc);
    dot = Vector3::dot(n,diff);
    outside_bc = (dot > 0.0F);

    // Test point outside edge_ca
    diff_c = p - vert2;
    diff = Vector3::cross(diff_c,edge_ca);
    dot = Vector3::dot(n,diff);
    outside_ca = (dot > 0.0F);
    
    // Check end points
    if (outside_ab && outside_bc) {
        pt = vert1;
        dist = diff_b.abs();
        return;
    }
    
    if (outside_bc && outside_ca) {
        pt = vert2;
        dist = diff_c.abs();
        return;
    }
    
    if (outside_ca && outside_ab) {
        pt = vert0;
        dist = diff_a.abs();
        return;
    }
    
    // Check edges
    if (outside_ab) {
        DTfloat length, t;
        length = edge_ab.abs();	    // Length of edge
        norm = edge_ab / length;	    // normalize
        t = Vector3::dot(norm,diff_a);		    // Component along edge
        
        // Clamp to corners
        if (t <= 0.0F)		pt = vert0;
        else if (t >= length)	pt = vert1;
        else			Vector3::displace(vert0, norm, t, pt);  // Position on edge
        
        diff = p - pt;
        dist = diff.abs();
        return;
    }
    
    if (outside_bc) {
        DTfloat length, t;
        length = edge_bc.abs();	    // Length of edge
        norm = edge_bc / length;	    // normalize
        t = Vector3::dot(norm,diff_b);		    // Component along edge
        
        // Clamp to corners
        if (t <= 0.0F)		pt = vert1;
        else if (t >= length)	pt = vert2;
        else			Vector3::displace(vert1, norm, t, pt);  // Position on edge
        
        diff = p - pt;
        dist = diff.abs();
        return;
    }
    
    if (outside_ca) {
        DTfloat length, t;
        length = edge_ca.abs();	    // Length of edge
        norm = edge_ca / length;	    // normalize
        t = Vector3::dot(norm,diff_c);		    // Component along edge
        
        // Clamp to corners
        if (t <= 0.0F)		pt = vert2;
        else if (t >= length)	pt = vert0;
        else			Vector3::displace(vert2, norm, t, pt);  // Position on edge
        
        diff = p - pt;
        dist = diff.abs(); 
        return;
    }
    
    
    // Point is in interior
    
    // Build plane equation
    Plane plane(vert0, vert1, vert2);
    
    // Distance to point
    dist = plane.distance_to_point(p);
    
    // add the normal offset
    Vector3::displace (p, plane.normal(), -dist, pt);
    
    dist = std::abs(dist);
}

//==============================================================================
//==============================================================================

void PrimitiveCollisions::dist_ray_ray  (   const Vector3 &from1, const Vector3 &direction1,
                                            const Vector3 &from2, const Vector3 &direction2,
                                            DTfloat &t1, DTfloat &t2)
{
    Vector3 u = direction1;
    Vector3 v = direction2;
    Vector3 w = from1 - from2;
    DTfloat a = Vector3::dot(u,u);      // always >= 0
    DTfloat b = Vector3::dot(u,v);
    DTfloat c = Vector3::dot(v,v);      // always >= 0
    DTfloat d = Vector3::dot(u,w);
    DTfloat e = Vector3::dot(v,w);
    DTfloat D = a*c - b*b;              // always >= 0

    // compute the line parameters of the two closest points
    if (D < EPSILON) {              // the lines are almost parallel
        t1 = 0.0F;
        t2 = (b>c ? d/b : e/c);     // use the largest denominator
    } else {
        t1 = (b*e - c*d) / D;
        t2 = (a*e - b*d) / D;
    }
}

void PrimitiveCollisions::dist_ray_point (const Vector3 &from, const Vector3 &direction, const Vector3 &pt, DTfloat &t)
{
    Vector3 dp = pt - from;
    DTfloat len = direction.abs2();
    
    t = Vector3::dot( dp, direction ) / len;
}

//==============================================================================
//==============================================================================

void PrimitiveCollisions::bressenham_line (DTint x0, DTint y0, DTint x1, DTint y1, std::vector<XY> &visited)
{
    // Algorithm presented at http://en.wikipedia.org/wiki/Bresenham's_line_algorithm

    DTboolean steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    DTint deltax = x1 - x0;
    DTint deltay = std::abs(y1 - y0);
    DTint error = deltax / 2;
    DTint ystep;
    DTint y = y0;
     
    if (y0 < y1)   ystep = 1;
    else           ystep = -1;

    for (DTint x = x0; x <= x1; ++x) {
        if (steep)      {   XY xy; xy.x = y; xy.y = x; visited.push_back(xy);    }
        else            {   XY xy; xy.x = x; xy.y = y; visited.push_back(xy);    }

        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
}

//==============================================================================
// From: http://playtechs.blogspot.ca/2007/03/raytracing-on-grid.html
//==============================================================================

void PrimitiveCollisions::raytrace_line (DTfloat x0, DTfloat y0, DTfloat x1, DTfloat y1, std::vector<XY> &visited)
{
    DTdouble dx = fabs(x1 - x0);
    DTdouble dy = fabs(y1 - y0);

    DTint x = DTint(floor(x0));
    DTint y = DTint(floor(y0));

    DTint n = 1;
    DTint x_inc, y_inc;
    DTdouble error;

    if (dx == 0) {
        x_inc = 0;
        error = std::numeric_limits<DTdouble>::infinity();
    } else if (x1 > x0) {
        x_inc = 1;
        n += DTint(floor(x1)) - x;
        error = (floor(x0) + 1 - x0) * dy;
    } else {
        x_inc = -1;
        n += x - DTint(floor(x1));
        error = (x0 - floor(x0)) * dy;
    }

    if (dy == 0) {
        y_inc = 0;
        error -= std::numeric_limits<DTdouble>::infinity();
    } else if (y1 > y0) {
        y_inc = 1;
        n += DTint(floor(y1)) - y;
        error -= (floor(y0) + 1 - y0) * dx;
    } else {
        y_inc = -1;
        n += y - DTint(floor(y1));
        error -= (y0 - floor(y0)) * dx;
    }

    for (; n > 0; --n) {
        XY xy;
        xy.x = x;
        xy.y = y;
        visited.push_back(xy);

        if (error > 0) {
            y += y_inc;
            error -= dx;
        } else {
            x += x_inc;
            error += dy;
        }
    }
}

//==============================================================================
//==============================================================================

} // DT3

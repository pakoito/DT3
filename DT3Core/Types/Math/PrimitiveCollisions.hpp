#ifndef DT3_PRIMITIVECOLLISIONS
#define DT3_PRIMITIVECOLLISIONS
//==============================================================================
///	
///	File: PrimitiveCollisions.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Vector2;
class Vector3;
class Plane;
class Box;
class Sphere;

//==============================================================================
/// Class
//==============================================================================

class PrimitiveCollisions {
	
    private:
         
									PrimitiveCollisions					(void);
									PrimitiveCollisions					(const PrimitiveCollisions &rhs);
		PrimitiveCollisions &		operator =							(const PrimitiveCollisions &rhs);	
        virtual						~PrimitiveCollisions				(void);

	public:

		/// Find intersection of 2 lines in 2D
		/// \param from1 Line 1 start
		/// \param to1 Line 1 end
		/// \param from2 Line 2 start
		/// \param to2 Line 2 end
		/// \param t1 Line 1 parametric intersection value
		/// \param t2 Line 2 parametric intersection value
		/// \return Has intersection
		static DTboolean			line_intersect_line                 (   const Vector2 &from1, const Vector2 &to1,
																			const Vector2 &from2, const Vector2 &to2,
																			DTfloat &t1, DTfloat &t2);

		/// Find intersection of ray and triangle
		/// \param from Ray start
		/// \param direction Ray direction
		/// \param vert0 Triangle vertex 0
		/// \param vert1 Triangle vertex 1
		/// \param vert2 Triangle vertex 2
		/// \param t Parametric intersection value
		/// \param n Intersection normal
		/// \param p Intersection point
		/// \return Has intersection
		static DTboolean			ray_intersect_triangle              (   const Vector3 &from, const Vector3 &direction,
																			const Vector3 &vert0, const Vector3 &vert1, const Vector3 &vert2,
																			DTfloat &t, Vector3 &n, Vector3 &p);
		/// Find intersection of ray and plane
		/// \param from Ray start
		/// \param direction Ray direction
		/// \param p Plane
		/// \param t Parametric intersection value
		/// \return Has intersection
		static DTboolean			ray_intersect_plane                 (   const Vector3 &from, const Vector3 &direction,
																			const Plane &p, DTfloat &t);

		/// Find intersection of ray and box
		/// \param from Ray start
		/// \param direction Ray direction
		/// \param b Box
		/// \param t Parametric intersection value
		/// \return Has intersection
        static DTboolean            ray_intersect_box                   (   const Vector3 &from, const Vector3 &direction,
                                                                            const Box &b, DTfloat &t);


		/// Find intersection of ray and sphere
		/// \param from Ray start
		/// \param direction Ray direction
		/// \param translation Position of sphere
		/// \param s Sphere
		/// \param t0 Parametric intersection value
		/// \param t1 Parametric intersection value
		/// \return Has intersection
		static DTboolean			ray_intersect_sphere                (   const Vector3 &from, const Vector3 &direction,
																			const Vector3 &translation, const Sphere &s,
																			DTfloat &t0, DTfloat &t1);
																											
		/// Find swept intersection of sphere and triangle
		/// \param from Sphere start pos
		/// \param to Sphere end pos
		/// \param s Sphere
		/// \param vert0 Triangle vertex 0
		/// \param vert1 Triangle vertex 1
		/// \param vert2 Triangle vertex 2
		/// \param t Parametric intersection value
		/// \param n Intersection normal
		/// \param p Intersection point
		/// \return Has intersection
		static DTboolean			extruded_sphere_intersect_triangle  (   const Vector3 &from, const Vector3 &to, const Sphere &s,
																			const Vector3 &vert0, const Vector3 &vert1, const Vector3 &vert2,
																			DTfloat &t, Vector3 &n, Vector3 &p);
		
		/// intersection of two swept spheres
		/// \param from1 Line 1 start
		/// \param direction1 Line 1 direction
		/// \param s1 Sphere 1
		/// \param from2 Line 2 start
		/// \param direction2 Line 2 direction
		/// \param s1 Sphere 2
		/// \param t1 Line 1 parametric intersection value
		/// \param t2 Line 2 parametric intersection value
		/// \return Has intersection
        static DTboolean            sphere_intersect_sphere             (   const Vector3 &from1, const Vector3 &dir1, const Sphere &s1,
                                                                            const Vector3 &from2, const Vector3 &dir2, const Sphere &s2,
                                                                            DTfloat &t1, DTfloat &t2);

		/// Distance from point to triangle
		/// \param p point
		/// \param vert0 Triangle vertex 0
		/// \param vert1 Triangle vertex 1
		/// \param vert2 Triangle vertex 2
		/// \param pt Closest point
		/// \param dist Closest distance
		static void					distance_point_to_triangle          (	const Vector3 &p,
                                                                            const Vector3 &vert0, const Vector3 &vert1, const Vector3 &vert2,
																			Vector3 &pt, DTfloat &dist);

		/// Find two closest points between two rays
		/// \param from1 Line 1 start
		/// \param direction1 Line 1 direction
		/// \param from2 Line 2 start
		/// \param direction2 Line 2 direction
		/// \param t1 Line 1 parametric intersection value
		/// \param t2 Line 2 parametric intersection value
        static void                 dist_ray_ray                        (   const Vector3 &from1, const Vector3 &direction1,
                                                                            const Vector3 &from2, const Vector3 &direction2,
                                                                            DTfloat &t1, DTfloat &t2);


		/// Find distance from ray to point
		/// \param from Ray start
		/// \param direction Ray direction
		/// \param p1 closest point on ray
		/// \param t Line 1 parametric intersection value
        static void                 dist_ray_point                      (   const Vector3 &from, const Vector3 &direction,
                                                                            const Vector3 &pt, DTfloat &t);


        struct XY {
            DTint x;
            DTint y;
        };  
    
		/// Bressenham Line algorithm
		/// \param x0 start point x
		/// \param y0 start point y
		/// \param x1 start point x
		/// \param y1 start point y
		/// \param visited list of points that get visited
        static void                 bressenham_line                     (DTint x0, DTint y0, DTint x1, DTint y1, std::vector<XY> &visited);

		/// Raytracing Line algorithm. Works better than Bressenham if points are floats.
		/// \param x0 start point x
		/// \param y0 start point y
		/// \param x1 start point x
		/// \param y1 start point y
		/// \param visited list of points that get visited
        static void                 raytrace_line                       (DTfloat x0, DTfloat y0, DTfloat x1, DTfloat y1, std::vector<XY> &visited);
};
							
//==============================================================================
//==============================================================================

} // DT3

#endif






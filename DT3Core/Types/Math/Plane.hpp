#ifndef DT3_PLANE
#define DT3_PLANE
//==============================================================================
///	
///	File: Plane.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Vector4;
class Sphere;
class Box;

//==============================================================================
/// Class
//==============================================================================

class Plane {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Plane)

									Plane                           (void);
									Plane                           (const Vector3 &n, const DTfloat &d);
									Plane                           (const Vector3 &n, const Vector3 &p);
									Plane                           (const Vector3 &a, const Vector3 &b, const Vector3 &c);
									Plane                           (const Vector4 &a);
									Plane                           (const DTfloat &a, const DTfloat &b, const DTfloat &c, const DTfloat &d);
									Plane                           (const Plane &rhs);
                                                
        
        Plane &						operator =                      (const Plane &rhs);
        
        
        //virtual					~Plane                          (void);
                
    public:
    
		inline DTboolean			operator ==                     (const Plane& rhs) const				{	return	n == rhs.n && d == rhs.d;   }
		inline DTboolean			operator !=                     (const Plane& rhs) const				{	return	n != rhs.n || d != rhs.d;   }


        DEFINE_ACCESSORS(D, set_d, DTfloat, d)
        DEFINE_ACCESSORS(normal, set_normal, Vector3, n)

		/// Distance from plane to a point
		/// \param p point
		/// \return Distance
        inline DTfloat				distance_to_point               (const Vector3 &p) const                {	return n.x * p.x + n.y * p.y + n.z * p.z + d;	}

		/// Distance from plane to a sphere
		/// \param s sphere
		/// \return Distance
        DTfloat						distance_to_sphere              (const Vector3 &translation, const Sphere &s) const;
		
		/// Check if a sphere is completely in front of the plane
		/// \param s sphere
		/// \return is in front
        DTboolean                   is_sphere_completely_in_front   (const Vector3 &translation, const Sphere &s) const;

		/// Check if a sphere is completely behind the plane
		/// \param s sphere
		/// \return is behind
        DTboolean                   is_sphere_completely_in_back    (const Vector3 &translation, const Sphere &s) const;

		/// Distance from plane to a box
		/// \param box Box
		/// \return Distance
        DTfloat						distance_to_box                 (const Box &box) const;
    
		/// Sets the plane equation using a point and normal
		/// \param p point
		/// \param n_ normal
        void						set                             (const Vector3 &p, const Vector3 &n_);

		/// Sets the plane equation using three points
		/// \param a point 0
		/// \param b point 1
		/// \param c point 2
        void						set                             (const Vector3 &a, const Vector3 &b, const Vector3 &c);
    
		/// Returns a plane that is flipped in the opposite direction
		/// \return Flipped Plane
        Plane						flipped                         (void) const;

		/// Reflect a vector around the plane
		/// \param a vector
		/// \return Reflected vector
		Vector3						reflect                         (const Vector3 &a) const;
        
		/// Normalizes the plane so that the normal is unit length without changine its position
        void                        normalize                       (void);
        
    private:
        Vector3						n;
        DTfloat						d;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Plane&v);
Stream& operator >>(Stream &s, Plane&v);

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Plane> {
	static Plane				default_value(void)	{	return Plane(0.0F,1.0F,0.0F,0.0F);			}
	static const DTcharacter*	name(void)          {	return "Plane";}
	static const DTcharacter*	name_caps(void)     {	return "Plane";}
    enum { isFundamental = 0 };
};

}

//==============================================================================
//==============================================================================

} // DT3

#endif


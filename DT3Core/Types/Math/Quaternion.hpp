#ifndef DT3_QUATERNION
#define DT3_QUATERNION
//==============================================================================
///	
///	File: Quaternion.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class Vector3;
class Matrix4;
class Matrix3;

//==============================================================================
/// Class
//==============================================================================

class Quaternion {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Quaternion)
        
         
                                Quaternion          (void);
                                Quaternion          (const Quaternion &rhs)         {	w = rhs.w; x = rhs.x; y = rhs.y; z = rhs.z;                 }
		explicit				Quaternion          (const Matrix3 &rhs);
		explicit				Quaternion          (const Matrix4 &rhs);
                                Quaternion          (   const DTfloat w_,
                                                        const DTfloat x_, 
                                                        const DTfloat y_, 
                                                        const DTfloat z_)			{	w = w_; x = x_; y = y_; z = z_;                             }
                                                
        Quaternion &            operator =          (const Quaternion &rhs)         {	w = rhs.w; x = rhs.x; y = rhs.y; z = rhs.z; return *this;   }
        
                                ~Quaternion         (void)                          {}
    
	public:
    
		DTboolean               operator ==				(const Quaternion& rhs) const   {	return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;	}
		DTboolean               operator !=				(const Quaternion& rhs) const   {	return w != rhs.w || x != rhs.x || y != rhs.y || z != rhs.z;	}


		/// Set each of the components to zero
		void					clear					(void);

		/// Returns an identity quaternion
		/// \return identity quaternion
		static Quaternion		identity				(void);

		/// Transform a vector by Quaternion
		/// \param a Quaternion
		/// \param b Vector to transform
		/// \return Transformed Vector
		static Vector3          transform               (const Quaternion &a, const Vector3 &b);

		/// Transform a vector by the inverse of a Quaternion
		/// \param a Quaternion
		/// \param b Vector to transform
		/// \return Transformed Vector
		static Vector3          transform_inv           (const Quaternion &a, const Vector3 &b);

		/// Dot product of two quaternions
		/// \param a Quaternion 1
		/// \param b Quaternion 2
		/// \return Dot product
		static DTfloat          dot                     (const Quaternion &a, const Quaternion &b);

		/// Returns the norm of the quaternion
		/// \return norm
        DTfloat                 norm                    (void) const;

		/// Returns the length of the quaternion
		/// \return length
		DTfloat                 length                  (void) const;

		/// Sets the log of the quaternion
		void                    log                     (void)  {   *this = logged();       }

		/// Returns the log of the quaternion
		/// \return log
        Quaternion              logged                  (void) const;

		/// Sets the exp of the quaternion
        void                    exp                     (void)  {   *this = exped();        }

		/// Returns the exp of the quaternion
		/// \return exp
        Quaternion              exped                   (void) const;

		/// Sets the conjugate of the quaternion
        void                    conjugate               (void)  {   *this = conjugated();   }

		/// Returns the conjugate of the quaternion
		/// \return conjugate
        Quaternion              conjugated              (void) const;

		/// Normalize the quaternion
        void                    normalize               (void)  {   *this = normalized();   }

		/// Returns the Normalized quaternion
		/// \return Normalized quaternion
        Quaternion              normalized              (void) const;

		/// Invert the Quaternion
        void                    inverse                 (void)  {   *this = inversed();     }

		/// Returns the Inverted quaternion
		/// \return Inverted quaternion
        Quaternion              inversed                (void) const;


		/// Linear interpolation of quaternions
		/// \param a Quaternion 1
		/// \param b Quaternion 2
		/// \param t Parametric value
		/// \return Interpolated Quaternion
		static Quaternion       lerp                    (const Quaternion &a, const Quaternion &b, const DTfloat t);

		/// Spherical interpolation of quaternions
		/// \param a Quaternion 1
		/// \param b Quaternion 2
		/// \param t Parametric value
		/// \return Interpolated Quaternion
		static Quaternion       slerp                   (const Quaternion &a, const Quaternion &b, const DTfloat t);

		/// Spherical interpolation of quaternions
		/// \param a Quaternion 1
		/// \param b Quaternion 2
		/// \param t Parametric value
		/// \return Interpolated Quaternion
		static Quaternion       slerp_no_invert         (const Quaternion &a, const Quaternion &b, const DTfloat t);

		/// Spherical cubic interpolation of quaternions
		/// \param a Quaternion 1
		/// \param b Quaternion 2
		/// \param a1 Quaternion 1 interpolator
		/// \param b1 Quaternion 2 interpolator
		/// \param t Parametric value
		/// \return Interpolated Quaternion
		static Quaternion       squad                   (   const Quaternion &a, const Quaternion &b,
                                                            const Quaternion &a1, const Quaternion &b1,
                                                            const DTfloat t );

		/// Calculates a spline from quaternions
		/// \return Calculated quaternion
		static Quaternion       spline                  (const Quaternion &a, const Quaternion &b, const Quaternion &c);

		/// Convert Axis-Angle to quaternion
		/// \param a Axis
		/// \param angle Angle
		/// \return Quaternion
		static Quaternion       convert_axis_angle      (const Vector3 &a, const DTfloat angle);

		/// Convert Axis to quaternion
		/// \param a Axis
		/// \return Quaternion
		static Quaternion       convert_axis            (const Vector3 &a);
        

		/// Checks for a zero quaternion
		/// \return is zero
		static DTboolean        is_zero                 (const Quaternion &a);
			
    public:
        DTfloat       w,x,y,z;

};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Quaternion&q);
Stream& operator >>(Stream &s, Quaternion&q);

//==============================================================================
//==============================================================================

inline Quaternion operator + (const Quaternion &a, const Quaternion &b)
{
    return Quaternion(a.w + b.w, a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Quaternion operator - (const Quaternion &a, const Quaternion &b)
{
    return Quaternion(a.w - b.w, a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Quaternion operator - (const Quaternion &a)
{
    return Quaternion(-a.w, -a.x, -a.y, -a.z);
}

inline Quaternion operator * (const DTfloat a, const Quaternion &b)
{
    return Quaternion(b.w * a, b.x * a, b.y * a, b.z * a);
}

inline Quaternion operator * (const Quaternion &a, const DTfloat b)
{
    return Quaternion(a.w * b, a.x * b, a.y * b, a.z * b);
}

inline Quaternion operator * (const Quaternion &a, const Quaternion &b)
{
    return Quaternion(  a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
                        a.y*b.z - a.z*b.y + a.w*b.x + a.x*b.w, 
                        a.z*b.x - a.x*b.z + a.w*b.y + a.y*b.w,
                        a.x*b.y - a.y*b.x + a.w*b.z + a.z*b.w   );
}

inline Quaternion operator / (const Quaternion &a, const DTfloat b)
{
    return Quaternion(a.w / b, a.x / b, a.y / b, a.z / b);
}

//==============================================================================
//==============================================================================
		
} // DT3
#endif

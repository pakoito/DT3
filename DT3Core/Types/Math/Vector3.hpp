#ifndef DT3_VECTOR3
#define DT3_VECTOR3
//==============================================================================
///	
///	File: Vector.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class Vector2;
class Vector4;

//==============================================================================
/// Class
//==============================================================================

class Vector3 {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Vector3)
         
        inline				Vector3					(void)  :   x(0.0F), y(0.0F), z(0.0F)    {}
		
		explicit			Vector3					(const Vector2 &rhs, DTfloat z_ = 0.0F);
							Vector3					(const Vector3 &rhs)        {   x = rhs.x;
																					y = rhs.y;
																					z = rhs.z;
                                                                                }
							Vector3					(const Vector4 &rhs);

		explicit inline     Vector3					(  const DTfloat x_,
														const DTfloat y_,
														const DTfloat z_)		{   x = x_;  
																					y = y_;  
																					z = z_;
                                                                                }
		explicit inline     Vector3					(const DTfloat v)           {   x = v;  
																					y = v;			
                                                                                    z = v;
                                                                                }

        Vector3 &			operator =              (const Vector3 &rhs)		{   x = rhs.x; 	
																					y = rhs.y; 	
																					z = rhs.z;
																					return *this;	}
        inline              ~Vector3				(void)                      {}
    
    public:                    

		inline DTboolean	operator ==				(const Vector3& rhs) const	{	return x == rhs.x && y == rhs.y && z == rhs.z;		}
		inline DTboolean	operator !=				(const Vector3& rhs) const	{	return x != rhs.x || y != rhs.y || z != rhs.z;		}

		Vector3 &			operator *=				(const DTfloat rhs)			{	x*=rhs;		y*=rhs;		z*=rhs;		return *this;	}
		Vector3 &			operator /=				(const DTfloat rhs)			{	ASSERT(rhs != 0.0F);    x/=rhs;		y/=rhs;		z/=rhs;		return *this;	}
		Vector3 &			operator +=				(const Vector3& rhs)		{	x+=rhs.x;	y+=rhs.y;	z+=rhs.z;	return *this;	}
		Vector3 &			operator -=				(const Vector3& rhs)		{	x-=rhs.x;	y-=rhs.y;	z-=rhs.z;	return *this;	}
		
		/// Set each of the components to zero
		void				clear					(void)						{	x = y = z = 0.0F;                                       }

		/// Returns the magnitude of the vector
		/// \return magnitude of the vector
		DTfloat				abs						(void) const				{	return std::sqrt(x*x + y*y + z*z);          }

		/// Returns the magnitude^2 of the vector
		/// \return magnitude^2 of the vector
		DTfloat				abs2					(void) const				{	return x*x + y*y + z*z;                     }

		/// Returns a normalized copy of the vector
		/// \return normalized vector
		Vector3				normalized				(void) const				{	DTfloat absv = abs();	ASSERT(absv != 0.0F);   return Vector3(x/absv,y/absv,z/absv);       }

		/// Normalizes the vector in place
		void                normalize				(void)						{	DTfloat absv = abs();	ASSERT(absv != 0.0F);   (*this) = Vector3(x/absv,y/absv,z/absv);    }

		/// Dot product of two vectors
		/// \param a Vector 1
		/// \param b Vector 2
		/// \return Dot product
		static DTfloat		dot						(const Vector3& a, const Vector3& b)	{	return a.x*b.x + a.y*b.y + a.z*b.z;	}

		/// Cross product of two vectors
		/// \param a Vector 1
		/// \param b Vector 2
		/// \return Cross product
		static Vector3		cross					(const Vector3& a, const Vector3& b);
	
        /// Displaces a position along a normal by and amount of the 3rd param
		/// \param a position
		/// \param b normal
		/// \param c amount
        static void         displace                (const Vector3 &a, const Vector3 &n, DTfloat d, Vector3 &r);

        /// Reflects a vector around a normal with a certain restitution
        static void         reflect                 (const Vector3 &a, const Vector3 &n, const DTfloat restitution, Vector3 &r);

    public:
        union {
            DTfloat x;
            DTfloat u;
        };

        union {
            DTfloat y;
            DTfloat v;
        };

        union {
            DTfloat z;
            DTfloat w;
        };

};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Vector3&v);
Stream& operator >>(Stream &s, Vector3&v);

//==============================================================================
//==============================================================================

inline Vector3 operator + (const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector3 operator - (const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector3 operator - (const Vector3 &a)
{
    return Vector3(-a.x, -a.y, -a.z);
}

inline Vector3 operator * (const DTfloat a, const Vector3 &b)
{
    return Vector3(b.x * a, b.y * a, b.z * a);
}

inline Vector3 operator * (const Vector3 &a, const DTfloat b)
{
    return Vector3(a.x * b, a.y * b, a.z * b);
}

inline Vector3 operator * (const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vector3 operator / (const Vector3 &a, const DTfloat b)
{
    return Vector3(a.x / b, a.y / b, a.z / b);
}

inline Vector3 operator / (const DTfloat b, const Vector3 &a)
{
    return Vector3(b / a.x, b / a.y, b / a.z);
}

inline Vector3 operator / (const Vector3 &a, const Vector3 &b)
{
    return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
}

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Vector3> {
	static Vector3				default_value(void)	{	return Vector3(0.0F,0.0F,0.0F);		}
	static const DTcharacter*	name(void)          {	return "Vector3";}
	static const DTcharacter*	name_caps(void)     {	return "Vector3";}
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================
		
} // DT3
#endif

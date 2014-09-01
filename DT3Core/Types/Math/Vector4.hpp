#ifndef DT3_VECTOR4
#define DT3_VECTOR4
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
class Vector3;

//==============================================================================
/// Class
//==============================================================================

class Vector4 {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Vector4)
         
        inline				Vector4					(void)  :   x(0.0F), y(0.0F), z(0.0F), w(0.0F)    {}
		
		explicit			Vector4					(const Vector2 &rhs, DTfloat z_ = 0.0F, DTfloat w_ = 0.0F);
		explicit			Vector4					(const Vector3 &rhs, DTfloat w_ = 0.0F);
							Vector4					(const Vector4 &rhs);

		inline				Vector4					(   const DTfloat x_,
                                                        const DTfloat y_,
														const DTfloat z_,
                                                        const DTfloat w_)		{   x = x_;  
																					y = y_;  
																					z = z_;
                                                                                    w = w_;         }
		inline				Vector4					(const DTfloat v)           {   x = v;  
																					y = v;			
                                                                                    z = v;
                                                                                    w = v;          }

        Vector4 &			operator =              (const Vector4 &rhs)		{   x = rhs.x; 	
																					y = rhs.y; 	
																					z = rhs.z;  
																					w = rhs.w;  
																					return *this;	}	
        inline              ~Vector4				(void)                      {}
    
    
    public:                    
		inline DTboolean	operator ==				(const Vector4& rhs) const	{	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;		}
		inline DTboolean	operator !=				(const Vector4& rhs) const	{	return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;		}

		Vector4 &			operator *=				(const DTfloat rhs)			{	x*=rhs;		y*=rhs;		z*=rhs;		w*=rhs;		return *this;	}
		Vector4 &			operator /=				(const DTfloat rhs)			{	x/=rhs;		y/=rhs;		z/=rhs;		w/=rhs;		return *this;	}
		Vector4 &			operator +=				(const Vector4& rhs)		{	x+=rhs.x;	y+=rhs.y;	z+=rhs.z;	w+=rhs.w;	return *this;	}
		Vector4 &			operator -=				(const Vector4& rhs)		{	x-=rhs.x;	y-=rhs.y;	z-=rhs.z;   w-=rhs.w;   return *this;	}
		
		/// Set each of the components to zero
		void				clear					(void)						{	x = y = z = w = 0.0F;								}

		/// Returns the magnitude of the vector
		/// \return magnitude of the vector
		DTfloat				abs						(void) const				{	return std::sqrt(x*x + y*y + z*z + w*w);            }

		/// Returns the magnitude^2 of the vector
		/// \return magnitude^2 of the vector
		DTfloat				abs2					(void) const				{	return x*x + y*y + z*z + w*w;						}

		/// Returns a normalized copy of the vector
		/// \return normalized vector
		Vector4				normalized				(void) const				{	ASSERT(x != 0.0 || y != 0.0F || z != 0.0F || w != 0.0F);	DTfloat absv = abs();	return Vector4(x/absv,y/absv,z/absv,w/absv);	}

		/// Normalizes the vector in place
		void                normalize				(void)						{	ASSERT(x != 0.0 || y != 0.0F || z != 0.0F || w != 0.0F);	DTfloat absv = abs();	(*this) = Vector4(x/absv,y/absv,z/absv,w/absv); }
		
    public:
        union {
            struct {
                DTfloat       x,y,z,w;
            };
            DTfloat v[4];
        };
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Vector4&v);
Stream& operator >>(Stream &s, Vector4&v);

//==============================================================================
//==============================================================================

inline Vector4 operator + (const Vector4 &a, const Vector4 &b)
{
    return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vector4 operator - (const Vector4 &a, const Vector4 &b)
{
    return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline Vector4 operator - (const Vector4 &a)
{
    return Vector4(-a.x, -a.y, -a.z, -a.w);
}

inline Vector4 operator * (const DTfloat a, const Vector4 &b)
{
    return Vector4(b.x * a, b.y * a, b.z * a, b.w * a);
}

inline Vector4 operator * (const Vector4 &a, const DTfloat b)
{
    return Vector4(a.x * b, a.y * b, a.z * b, a.w * b);
}

inline Vector4 operator * (const Vector4 &a, const Vector4 &b)
{
    return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline Vector4 operator / (const Vector4 &a, const DTfloat b)
{
    return Vector4(a.x / b, a.y / b, a.z / b, a.w / b);
}

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Vector4> {
	static Vector4				default_value(void)	{	return Vector4(0.0F,0.0F,0.0F,0.0F);}
	static const DTcharacter*	name(void)          {	return "Vector4";}
	static const DTcharacter*	name_caps(void)     {	return "Vector4";}
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================
		
} // DT3
#endif

#ifndef DT3_VECTOR2
#define DT3_VECTOR2
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
class Vector3;
class Vector4;

//==============================================================================
/// Class
//==============================================================================

class Vector2 {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Vector2)
         
        inline				Vector2					(void)  :   x(0.0F), y(0.0F)    {}
		
							Vector2					(const Vector2 &rhs)		{   x = rhs.x;
																					y = rhs.y;  }
		explicit			Vector2					(const Vector3 &rhs);
		explicit			Vector2					(const Vector4 &rhs);

		inline explicit     Vector2					(   const DTfloat x_,
														const DTfloat y_)		{   x = x_;  
																					y = y_;     }
		inline explicit     Vector2					(const DTfloat v)           {   x = v;  
																					y = v;      }
        Vector2 &			operator =              (const Vector2 &rhs)		{   x = rhs.x; 	
																					y = rhs.y;
																					return *this;	}
        inline				~Vector2				(void)                      {}
           
    public:                    
		/// Description
		/// \param param description
		/// \return description
		inline DTboolean	operator ==				(const Vector2& rhs) const  {   return x == rhs.x && y == rhs.y;		}
		
		/// Description
		/// \param param description
		/// \return description
		inline DTboolean	operator !=				(const Vector2& rhs) const  {	return x != rhs.x || y != rhs.y;		}


		/// Description
		/// \param param description
		/// \return description
		Vector2 &			operator *=				(const DTfloat rhs)			{	x*=rhs;		y*=rhs;     return *this;	}

		/// Description
		/// \param param description
		/// \return description
		Vector2 &			operator /=				(const DTfloat rhs)			{	x/=rhs;		y/=rhs;     return *this;	}

		/// Description
		/// \param param description
		/// \return description
		inline Vector2 &    operator +=				(const Vector2& rhs)		{	x+=rhs.x;	y+=rhs.y;   return *this;	}

		/// Description
		/// \param param description
		/// \return description
		inline Vector2 &    operator -=				(const Vector2& rhs)		{	x-=rhs.x;	y-=rhs.y;   return *this;	}
		
		/// Set each of the components to zero
		void				clear					(void)						{	x = y = 0.0F;							}

		/// Returns the magnitude of the vector
		/// \return magnitude of the vector
		inline DTfloat      abs						(void) const				{   return std::sqrt(x*x + y*y);            }

		/// Returns the magnitude^2 of the vector
		/// \return magnitude^2 of the vector
		inline DTfloat      abs2					(void) const				{   return x*x + y*y;						}

		/// Returns a normalized copy of the vector
		/// \return normalized vector
		Vector2				normalized				(void) const				{	DTfloat absv = abs();   ASSERT(absv != 0.0F);   return Vector2(x/absv,y/absv);      }
		
		/// Normalizes the vector in place
		void                normalize				(void)						{	DTfloat absv = abs();   ASSERT(absv != 0.0F);   (*this) = Vector2(x/absv,y/absv);   }
			
		/// Return perped copy of the vector
		/// \return perped copy of the vector
		inline Vector2      perped					(void) const							{   return Vector2(-y,x);			}

		///  Perp the vector in place
		inline void         perp					(void)									{   (*this) = Vector2(-y,x);        }
		
    
		/// Dot product of two vectors
		/// \param a Vector 1
		/// \param b Vector 2
		/// \return Dot product
		static DTfloat		dot						(const Vector2& a, const Vector2& b)	{	return a.x*b.x + a.y*b.y;		}

		/// perp dot (like 2D cross product) of two vectors
		/// \param a Vector 1
		/// \param b Vector 2
		/// \return Perp Dot product
		static DTfloat		perp_dot                (const Vector2& a, const Vector2& b)    {	return a.x * b.y - a.y * b.x;	}

    public:
        union {
            DTfloat x;
            DTfloat u;
        };

        union {
            DTfloat y;
            DTfloat v;
        };
    
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Vector2&v);
Stream& operator >>(Stream &s, Vector2&v);

//==============================================================================
//==============================================================================

inline Vector2 operator + (const Vector2 &a, const Vector2 &b)
{
    return Vector2(a.x + b.x, a.y + b.y);
}

inline Vector2 operator - (const Vector2 &a, const Vector2 &b)
{
    return Vector2(a.x - b.x, a.y - b.y);
}

inline Vector2 operator - (const Vector2 &a)
{
    return Vector2(-a.x, -a.y);
}

inline Vector2 operator * (const DTfloat a, const Vector2 &b)
{
    return Vector2(b.x * a, b.y * a);
}

inline Vector2 operator * (const Vector2 &a, const DTfloat b)
{
    return Vector2(a.x * b, a.y * b);
}

inline Vector2 operator * (const Vector2 &a, const Vector2 &b)
{
    return Vector2(a.x * b.x, a.y * b.y);
}

inline Vector2 operator / (const Vector2 &a, const DTfloat b)
{
    return Vector2(a.x / b, a.y / b);
}

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Vector2> {
	static Vector2				default_value(void)	{	return Vector2(0.0F,0.0F);			}
	static const DTcharacter*	name(void)          {	return "Vector2";}
	static const DTcharacter*	name_caps(void)     {	return "Vector2";}
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================
		
} // DT3
#endif

#ifndef DT3_MATRIX2
#define DT3_MATRIX2
//==============================================================================
///	
///	File: Matrix2.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class Vector2;
class Matrix3;
class Matrix4;
class Quaternion;

//==============================================================================
/// Class
//==============================================================================

class Matrix2 {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Matrix2)
    
         
                                Matrix2                     (void);
                                Matrix2                     (const Matrix2 &rhs);
		explicit                Matrix2                     (const Matrix3 &rhs);
		explicit                Matrix2                     (const Matrix4 &rhs);
		explicit                Matrix2                     (const Quaternion &rhs);
                                Matrix2                     (	const DTfloat m11,const DTfloat m12,
                                                                const DTfloat m21,const DTfloat m22);
                                Matrix2                     (const Vector2 &vx, const Vector2 &vy);
                                                                
        Matrix2 &               operator =                  (const Matrix2 &rhs);
		
        inline                  ~Matrix2                    (void) {}
        
    public:        

		DTboolean               operator ==                 (const Matrix2& rhs) const;
		DTboolean               operator !=                 (const Matrix2& rhs) const;

		Matrix2 &               operator *=                 (const DTfloat rhs);
		Matrix2 &               operator /=                 (const DTfloat rhs);
		Matrix2 &               operator *=                 (const Matrix2 &rhs);


		/// Description
		/// \param param description
		/// \return description
		void                    clear                       (void);

		/// Description
		/// \param param description
		/// \return description
		DTfloat                 determinant                 (void) const;

		/// Description
		/// \param param description
		/// \return description
		Matrix2                 inversed                    (void) const;

		/// Description
		/// \param param description
		/// \return description
		void                    inverse                     (void)          {	*this = inversed();     }

		/// Description
		/// \param param description
		/// \return description
		Matrix2                 transposed                  (void) const;

		/// Description
		/// \param param description
		/// \return description
		void                    transpose                   (void)          {	*this = transposed();   }

		/// Description
		/// \param param description
		/// \return description
        Vector2                 x_axis                      (void) const    {	return Vector2(_m11, _m21);		}

		/// Description
		/// \param param description
		/// \return description
        Vector2                 y_axis                      (void) const    {	return Vector2(_m12, _m22);		}
		
		/// Description
		/// \param param description
		/// \return description
        Matrix2                 orthoed                     (void) const;

		/// Description
		/// \param param description
		/// \return description
        void                    ortho                       (void)          {   *this = orthoed();      }

		/// Description
		/// \param param description
		/// \return description
        static Matrix2          set_rotation_z              (const DTfloat a);

		/// Description
		/// \param param description
		/// \return description
        void                    set_diagonal                (const Vector2 &a);

		/// Description
		/// \param param description
		/// \return description
        Vector2                 diagonal                    (void) const;

		/// Description
		/// \param param description
		/// \return description
		static const Matrix2&	identity                    (void);

		/// Description
		/// \param param description
		/// \return description
		static const Matrix2&	zero                        (void);

    public:
		union {
			struct {
				DTfloat			_m11, _m21;
				DTfloat			_m12, _m22;
			};
			DTfloat _m[2][2];
			DTfloat _mi[4];
		};
    
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator << (Stream &s, const Matrix2 &m);
Stream& operator >> (Stream &s, Matrix2 &m);

//==============================================================================
//==============================================================================

Matrix2 operator * (const DTfloat a, const Matrix2 &b);
Matrix2 operator * (const Matrix2 &a, const DTfloat b);
Matrix2 operator * (const Matrix2 &a, const Matrix2 &b);
Vector2 operator * (const Matrix2 &a, const Vector2 &b);
Matrix2 operator / (const Matrix2 &a, const DTfloat b);
Matrix2 operator + (const Matrix2 &a, const Matrix2 &b);

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Matrix2> {
	static Matrix2				default_value(void)	{	return Matrix2::identity();			}
	static const DTcharacter*	name(void)          {	return "Matrix2";}
	static const DTcharacter*	name_caps(void)     {	return "Matrix2";}
    enum { isFundamental = 0 };
};

} // TypeTraitsDefaults

//==============================================================================
//==============================================================================


} // DT3

#endif

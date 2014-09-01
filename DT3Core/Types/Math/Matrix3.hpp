#ifndef DT3_MATRIX3
#define DT3_MATRIX3
//==============================================================================
///	
///	File: Matrix3.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class Vector2;
class Vector3;
class Matrix2;
class Matrix4;
class Quaternion;

//==============================================================================
/// Class
//==============================================================================

class Matrix3 {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Matrix3)
    
         
                                Matrix3                     (void);
		explicit                Matrix3                     (const Matrix2 &rhs);
                                Matrix3                     (const Matrix3 &rhs);
		explicit                Matrix3                     (const Matrix4 &rhs);
		explicit                Matrix3                     (const Quaternion &rhs);
                                Matrix3                     (   const DTfloat m11,const DTfloat m12,const DTfloat m13,
                                                                const DTfloat m21,const DTfloat m22,const DTfloat m23,
                                                                const DTfloat m31,const DTfloat m32,const DTfloat m33);
		explicit                Matrix3                     (const Vector3 &vx, const Vector3 &vy, const Vector3 &vz);
                                                                
        Matrix3 &               operator =                  (const Matrix3 &rhs);
		
        inline                  ~Matrix3                    (void) {}
        
    public:        

		DTboolean               operator ==                 (const Matrix3& rhs) const;
		DTboolean               operator !=                 (const Matrix3& rhs) const;

		Matrix3 &               operator *=                 (const DTfloat rhs);
		Matrix3 &               operator /=                 (const DTfloat rhs);
		Matrix3 &               operator *=                 (const Matrix3 &rhs);

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
		Matrix3                 inversed                    (void) const;

		/// Description
		/// \param param description
		/// \return description
		void                    inverse                     (void)          {	*this = inversed();     }

		/// Description
		/// \param param description
		/// \return description
		Matrix3                 transposed                  (void) const;

		/// Description
		/// \param param description
		/// \return description
		void                    transpose                   (void)          {	*this = transposed();   }

		/// Description
		/// \param param description
		/// \return description
        Vector3                 x_axis                      (void) const    {	return Vector3(_m11, _m21, _m31);		}

		/// Description
		/// \param param description
		/// \return description
        Vector3                 y_axis                      (void) const    {	return Vector3(_m12, _m22, _m32);		}

		/// Description
		/// \param param description
		/// \return description
        Vector3                 z_axis                      (void) const    {	return Vector3(_m13, _m23, _m33);		}
		
		/// Description
		/// \param param description
		/// \return description
        Matrix3                 orthoed                     (void) const;

		/// Description
		/// \param param description
		/// \return description
        void                    ortho                       (void)          {   *this = orthoed();      }

		/// Description
		/// \param param description
		/// \return description
        static Matrix3          set_rotation                (const Vector3 &a, const Vector3 &b);

		/// Description
		/// \param param description
		/// \return description
        static Matrix3          set_rotation_around         (const Vector3 &a, const DTfloat b);

		/// Description
		/// \param param description
		/// \return description
        static Matrix3          set_rotation_x              (const DTfloat a);

		/// Description
		/// \param param description
		/// \return description
        static Matrix3          set_rotation_y              (const DTfloat a);

		/// Description
		/// \param param description
		/// \return description
        static Matrix3          set_rotation_z              (const DTfloat a);

		/// Description
		/// \param param description
		/// \return description
        static Matrix3          set_orientation             (const Vector3 &back,  const Vector3 &up);

		/// Description
		/// \param param description
		/// \return description
        static Matrix3          tilde                       (const Vector3 &a);

		/// Description
		/// \param param description
		/// \return description
        void                    set_diagonal                (const Vector3 &a);

		/// Description
		/// \param param description
		/// \return description
        Vector3                 diagonal                    (void);

		/// Description
		/// \param param description
		/// \return description
		static const Matrix3&	identity                    (void);

		/// Description
		/// \param param description
		/// \return description
		static const Matrix3&	zero                        (void);

    public:
		union {
			struct {
				DTfloat			_m11, _m21, _m31;
				DTfloat			_m12, _m22, _m32;
				DTfloat			_m13, _m23, _m33;
			};
			DTfloat _m[3][3];
			DTfloat _mi[9];
		};
    
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Matrix3 &m);
Stream& operator >>(Stream &s, Matrix3 &m);

//==============================================================================
//==============================================================================
 
Matrix3 operator * (const DTfloat a, const Matrix3 &b);
Matrix3 operator * (const Matrix3 &a, const DTfloat b);
Matrix3 operator * (const Matrix3 &a, const Matrix3 &b);
Vector3 operator * (const Matrix3 &a, const Vector3 &b);
Vector2 operator * (const Matrix3 &a, const Vector2 &b);
Matrix3 operator / (const Matrix3 &a, const DTfloat b);
Matrix3 operator / (const Matrix3 &a, const Vector3 &b);
Matrix3 operator + (const Matrix3 &a, const Matrix3 &b);
Matrix3 operator - (const Matrix3 &a, const Matrix3 &b);

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Matrix3> {
	static Matrix3				default_value(void)	{	return Matrix3::identity();			}
	static const DTcharacter*	name(void)          {	return "Matrix3";}
	static const DTcharacter*	name_caps(void)     {	return "Matrix3";}
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================


} // DT3

#endif

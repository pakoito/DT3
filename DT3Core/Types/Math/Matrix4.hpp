#ifndef DT3_MATRIX4
#define DT3_MATRIX4
//==============================================================================
///	
///	File: Matrix4.hpp
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
#include "DT3Core/Types/Math/Matrix3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class Vector2;
class Vector3;
class Vector4;
class Matrix2;
class Matrix3;
class Quaternion;

//==============================================================================
/// Class
//==============================================================================

class Matrix4 {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Matrix4)

         
                                Matrix4					(void);
							
		explicit                Matrix4					(const Matrix2 &rhs);
        explicit                Matrix4					(const Matrix2 &o, const Vector2 &t);
        explicit                Matrix4					(const Matrix2 &o, const Vector2 &t, DTfloat s);
        explicit                Matrix4					(const Matrix2 &o, const Vector3 &t);
        explicit                Matrix4					(const Matrix2 &o, const Vector3 &t, DTfloat s);

		explicit                Matrix4					(const Matrix3 &rhs);
		explicit                Matrix4					(const Matrix3 &o, const Vector3 &t);
		explicit                Matrix4					(const Matrix3 &o, const Vector3 &t, DTfloat s);
        explicit                Matrix4					(const Matrix3 &o, const Vector3 &t, const Vector3 &s);
        
                                Matrix4					(const Matrix4 &rhs);
                            
		explicit                Matrix4					(const Quaternion &rhs);
		explicit                Matrix4					(const Quaternion &o, const Vector3 &t);
		explicit                Matrix4					(const Quaternion &o, const Vector3 &t, DTfloat s);

		explicit                Matrix4					(   const DTfloat m11,const DTfloat m12,const DTfloat m13,const DTfloat m14,
                                                            const DTfloat m21,const DTfloat m22,const DTfloat m23,const DTfloat m24,
                                                            const DTfloat m31,const DTfloat m32,const DTfloat m33,const DTfloat m34,
                                                            const DTfloat m41,const DTfloat m42,const DTfloat m43,const DTfloat m44);
		explicit                Matrix4					(const Vector3 &vx, const Vector3 &vy, const Vector3 &vz);
		explicit                Matrix4					(const Vector3 &translation);
							 
        Matrix4 &               operator =				(const Matrix4 &rhs);
    
        inline                  ~Matrix4                (void) {}
            
    public:        

		DTboolean               operator ==				(const Matrix4& rhs) const;
		DTboolean               operator !=				(const Matrix4& rhs) const;

		Matrix4 &               operator *=				(const DTfloat rhs);
		Matrix4 &               operator /=				(const DTfloat rhs);
		Matrix4 &               operator *=				(const Matrix4 &rhs);

		/// Clear Matrix to all zero
		void                    clear					(void);

		/// Calculate Determinant for matrix
		/// \return Determinant
		DTfloat                 determinant				(void) const;

		/// Return the inverse of the matrix
		/// \return inversed Matrix
		Matrix4                 inversed				(void) const;

		/// Calculate the inverse of the matrix
		void                    inverse					(void)              {	*this = inversed();     }

		/// Return the transpose of the matrix
		/// \return transposed Matrix
		Matrix4                 transposed				(void) const;

		/// Calculate the transpose of the matrix
		void                    transpose				(void)              {	*this = transposed();   }

		/// X Axis from the transform
		/// \return X Axis
        Vector3                 x_axis					(void) const		{	return Vector3(_m11, _m21, _m31);		}

		/// Y Axis from the transform
		/// \return Y Axis
        Vector3                 y_axis					(void) const		{	return Vector3(_m12, _m22, _m32);		}

		/// Z Axis from the transform
		/// \return Z Axis
        Vector3                 z_axis					(void) const		{	return Vector3(_m13, _m23, _m33);		}

		/// Return the orthogonalized matrix
		/// \return Orthogonalized Matrix
		Matrix4                 orthoed                 (void) const;

		/// Calculate the ortho of the matrix
		void                    ortho                   (void)              {   *this = orthoed();      }

		/// Description
		/// \param param description
		/// \return description
        Matrix3                 orientation             (void) const        {   return Matrix3(*this)/scale();       }
        
		/// Description
		/// \param param description
		/// \return description
        void                    set_orientation         (const Matrix3 &o)  {   _m11 = o._m11;  _m21 = o._m21;  _m31 = o._m31;
                                                                                _m12 = o._m12;  _m22 = o._m22;  _m32 = o._m32;
                                                                                _m13 = o._m13;  _m23 = o._m23;  _m33 = o._m33;  }

		/// Description
		/// \param param description
		/// \return description
        Vector3                 scale                   (void) const        {   return Vector3(x_axis().abs(),y_axis().abs(),z_axis().abs());       }
        
		/// Description
		/// \param param description
		/// \return description
        void                    set_scale               (const Vector3 &s)  {   Vector3 os = scale();
                                                                                _m11 = _m11 * s.x/os.x;  _m21 = _m21 * s.x/os.x;  _m31 = _m31 * s.x/os.x;
                                                                                _m12 = _m12 * s.y/os.y;  _m22 = _m22 * s.y/os.y;  _m32 = _m32 * s.y/os.y;
                                                                                _m13 = _m13 * s.z/os.z;  _m23 = _m23 * s.z/os.z;  _m33 = _m33 * s.z/os.z;  }

		/// Description
		/// \param param description
		/// \return description
        Vector3                 translation             (void) const        {   return Vector3(_m14, _m24, _m34); }

		/// Description
		/// \param param description
		/// \return description
        void                    set_translation         (const Vector3 &t)  {   _m14 = t.x;
                                                                                _m24 = t.y;
                                                                                _m34 = t.z;     }

		/// Description
		/// \param param description
		/// \return description
		static const Matrix4 &	identity                (void);

		/// Description
		/// \param param description
		/// \return description
		static const Matrix4 &	zero                    (void);

    public:
		union {
			struct {
				DTfloat			_m11, _m21, _m31, _m41;		// xx xy xz 0
				DTfloat			_m12, _m22, _m32, _m42;		// yx yy yz 0
				DTfloat			_m13, _m23, _m33, _m43;		// zx zy zz 0
				DTfloat			_m14, _m24, _m34, _m44;		// tx ty tz 1
			};
			DTfloat _m[4][4];
			DTfloat _mi[16];
		};
    
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Matrix4 &m);
Stream& operator >>(Stream &s, Matrix4 &m);

//==============================================================================
//==============================================================================

Matrix4 operator * (const DTfloat a, const Matrix4 &b);
Matrix4 operator * (const Matrix4 &a, const DTfloat b);
Matrix4 operator * (const Matrix4 &a, const Matrix4 &b);

Vector4 operator * (const Matrix4 &a, const Vector4 &b);
Vector3 operator * (const Matrix4 &a, const Vector3 &b);
Vector2 operator * (const Matrix4 &a, const Vector2 &b);

Matrix4 operator / (const Matrix4 &a, const DTfloat b);

Matrix4 operator + (const Matrix4 &a, const Matrix4 &b);
Matrix4 operator - (const Matrix4 &a, const Matrix4 &b);

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Matrix4> {
	static Matrix4				default_value(void)	{	return Matrix4::identity();			}
	static const DTcharacter*	name(void)          {	return "Matrix4";}
	static const DTcharacter*	name_caps(void)     {	return "Matrix4";}
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================


} // DT3

#endif

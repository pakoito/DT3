#ifndef DT3_MOREMATH
#define DT3_MOREMATH
//==============================================================================
///	
///	File: MoreMath.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include <vector>
#include <cmath>
#include <limits>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Constants
//==============================================================================

extern const DTfloat PI;
extern const DTfloat PI_INV;
extern const DTfloat TWO_PI;
extern const DTfloat TWO_PI_INV;
extern const DTfloat HALF_PI;
extern const DTfloat E;
extern const DTfloat SQRT_2;
extern const DTfloat SQRT_HALF;
extern const DTfloat DEG_TO_RAD;
extern const DTfloat RAD_TO_DEG;
extern const DTfloat EPSILON;

//==============================================================================
/// Forward declarations
//==============================================================================

class Vector2;
class Vector3;

//==============================================================================
/// Class
//==============================================================================

class MoreMath {
	private:
								MoreMath                        (void);	
								MoreMath                        (const MoreMath &rhs);
		MoreMath &				operator =                      (const MoreMath &rhs);
								~MoreMath                       (void);
	
	public:
		/// Initializes internal tables. This function is called automatically and you don't have to call it.
		static void				init_tables                     (void);


        //
        // Entropy Pool
        //
    
        /// Add some random bytes to pool
        /// \param data pointer to bytes
        /// \param data_size number of bytes to add
        static void             add_entropy                     (DTubyte *data, DTsize data_size);

        /// Get some random bytes from pool
        /// \param data pointer to bytes
        /// \param data_size number of bytes to retrieve (Max 17)
        static void             entropy                         (DTubyte *data, DTsize data_size);

        //
        // Random Number generators
        //

		/// Generate a random float
		/// \return random number
		static DTfloat			random_float                    (void);

		/// Generate a random float using a Mersenne Twister
		/// \return random number
		static DTfloat			random_MT_float                 (void);

		/// Generate a random int
		/// \return random number
		static DTuint			random_int                      (void);

		/// Generate a random int
		/// \return random number
		static DTuint			random_MT_int                   (void);
    
		/// Sets the random seed
        /// \param random_seed Random seed
		static void				set_random_seed                 (DTuint random_seed)	{	_random_seed = random_seed;		}

		/// Sets the random seed from entropy pool
        /// \param random_seed Random seed
		static void				set_random_seed                 (void);

		/// Returns the random seed
		/// \return random seed
        static DTuint           random_seed                     (void)                  {   return _random_seed;            }


        //
        // Math functions
        //

		/// Calculate a factorial. Note it it uses floats so it's probably really only useful for smallish inputs
        /// \param f number
		/// \return Factorial of number
		static DTfloat			factorial						(DTint f);

		/// Solves a quadratic equation at^2 + bt + c = 0. See http://en.wikipedia.org/wiki/Quadratic_equation
		/// \param a a coefficient of equation
		/// \param b b coefficient of equation
		/// \param c c coefficient of equation
		/// \param t0 result 1
		/// \param t2 result 2
        template<typename T>
		static void             solve_quadratic     (const T a, const T b, const T c, T &t0, T &t1)
        {
            T sign = (b >= 0.0F ? 1.0F : -1.0F);
            T q = -0.5F * ( b + sign * std::sqrt(b*b - 4.0F*a*c) );
            
            t0 = (T) (q / a);
            t1 = (T) (c / q);
        }
    
		/// Test is two numbers are approximately equal
		/// \param a number
		/// \param b number
		/// \param thresh threshold
		/// \return is approximately equal
        template<typename T>
		static inline DTboolean approximately_equal (T a, T b, T thresh)
		{
			T delta = a - b;
			return ((delta * delta) <= (thresh * thresh));
		}


		/// Tries to quickly convert a float to an int
		/// \param f float value
		/// \return int value
		static inline DTint		float_to_int        (DTfloat f)
		{
		#if DT3_CPU	== DT3_INTEL
			return (DTint) f;
		#else
			IntOrFloat	n;
			IntOrFloat	bias;
			
			bias.i = (23 + 127) << 23;
			n.f = f;
			
			n.f += bias.f;
			n.i -= bias.i;
			
			return n.i;
		#endif
		}

		/// Tries to quickly convert a int to a float
		/// \param i int value
		/// \return float value
		static inline DTfloat   int_to_float        (DTint i)
		{
		#if DT3_CPU	== DT3_INTEL
			return static_cast<DTfloat>(i);
		#else
			IntOrFloat	n;
			IntOrFloat	bias;
			
			bias.i = (23 + 127) << 23;
			n.i = i;
			
			n.i += bias.i;
			n.f -= bias.f;
			
			return n.f;	
		#endif
		}

		/// Clamps a number from 0 to 1
		/// \param v number
		/// \return clamped number
        template<typename T>
		static inline T         clamp_zero_one      (T v)
		{
			if (v < 0.0F)		return 0.0F;
			else if (v > 1.0F)	return 1.0F;
			
			return v;
		}

		/// Clamps a number between a range
		/// \param v number
		/// \param low low range
		/// \param high high range
		/// \return clamped number
        template<typename T>
		static inline DTfloat   clamp               (T v, T low, T high)
		{
			if (v < low)		return low;
			else if (v > high)	return high;
			
			return v;
		}
        
		/// Clamps and changes range of input
		/// \param v number
		/// \param in_min low input range
		/// \param in_max high input range
		/// \param out_min low output range
		/// \param out_max high output range
		/// \return clamped number
        template<typename T>
		static inline T         change_range_clamp  (T v, T in_min, T in_max, T out_min, T out_max)
		{
            T t = (v - in_min) / (in_max - in_min);
            t = clampZeroOne(t);
            return t * (out_max - out_min) + out_min; 
		}

		/// Clamps and changes range of input
		/// \param v number
		/// \param in_min low input range
		/// \param in_max high input range
		/// \param out_min low output range
		/// \param out_max high output range
		/// \return clamped number
        template<typename T>
		static inline T         change_range_clamp_min  (T v, T in_min, T in_max, T out_min, T out_max)
		{
            T t = (v - in_min) / (in_max - in_min);
            if (t < 0.0F)
                t = 0.0F;
            return t * (out_max - out_min) + out_min; 
		}

		/// Clamps and changes range of input
		/// \param v number
		/// \param in_min low input range
		/// \param in_max high input range
		/// \param out_min low output range
		/// \param out_max high output range
		/// \return clamped number
        template<typename T>
		static inline T         change_range  (T v, T in_min, T in_max, T out_min, T out_max)
		{
            T t = (v - in_min) / (in_max - in_min);
            return t * (out_max - out_min) + out_min;
		}

		/// Version of MOD that wraps -ve numbers in a more useful way.
		/// \param v value
		/// \param m mod value
		/// \return result
        template<typename T>
        static inline T         wrap_mod            (T v, T m)
        {
            T r = v % m;
            return r < 0 ? r+m : r;
        }

		/// Generates a uniform random vector
		/// \return Random vector
        static Vector3          random_vector       (void);

		/// Hermite Spline interpolation
		/// \param point0 point 0
		/// \param tangent0 tangent 0
		/// \param point1 point 1
		/// \param tangent1 tangent 1
		/// \param t parametric value
		/// \return Interpolated value
        template<typename T>
        static inline T         spline              (const T &point0, const T &tangent0, const T &point1, const T &tangent1, DTfloat t)
        {   
            DTfloat t2 = t * t;
            DTfloat t3 = t * t2;
        
            // Interpolate via Hermite spline
            // See Realtime Rendering, 2nd Ed., Page 492
            T p1,p2,p3,p4;
            p1 = point0 * (2.0F * t3 - 3.0F * t2 + 1.0F);
            p2 = tangent0 * (t3 - 2.0F * t2 + t);

            p3 = tangent1 * (t3 - t2);
            p4 = point1 * (-2.0F * t3 + 3.0F * t2);
            
            return p1 + p2 + p3 + p4;
        }

		/// Checks is a value is a power of 2
		/// \param v value
		/// \return return wether is power of 2 or not
		static inline DTboolean	is_power_2      (DTuint v)
		{
			return (!(v & (v-1)));
		}

		/// Gets next power of 2
		/// \param v value
		/// \return next power of 2
		static inline DTuint	next_power_2    (DTuint v)
		{
			--v;
			v = (v >> 1) | v;
			v = (v >> 2) | v;
			v = (v >> 4) | v;
			v = (v >> 8) | v;
			v = (v >> 16) | v;
			++v;
			
			return v;
		}
    
		/// Builds an approximating 3rd-order polynomial for the data set (i.e.  y = a + b*x + c*x^2 + d*x^3)
		/// \param data dataset to fit the polynomial to
		/// \param a coefficient
		/// \param b coefficient
		/// \param c coefficient
		/// \param d coefficient
        static void regression (std::vector<Vector2> &data, DTfloat &a, DTfloat &b, DTfloat &c, DTfloat &d);
    
    
		/// Minimum of two numbers
		/// \param a number
		/// \param b number
		/// \return result
        template <class T>
        static T min (const T &a, const T &b)
        {
            return a < b ? a : b;
        }

		/// Minimum of three numbers
		/// \param a number
		/// \param b number
		/// \param c number
		/// \return result
        template <class T>
        static T min (const T &a, const T &b, const T &c)
        {
            return min(min(a,b),c);
        }
    
		/// Minimum of four numbers
		/// \param a number
		/// \param b number
		/// \param c number
		/// \param d number
		/// \return result
        template <class T>
        static T min (const T &a, const T &b, const T &c, const T &d)
        {
            return min(min(a,b),min(c,d));
        }


		/// Maximum of two numbers
		/// \param a number
		/// \param b number
		/// \return result
        template <class T>
        static T max (const T &a, const T &b)
        {
            return a > b ? a : b;
        }

		/// Maximum of three numbers
		/// \param a number
		/// \param b number
		/// \param c number
		/// \return result
        template <class T>
        static T max (const T &a, const T &b, const T &c)
        {
            return max(max(a,b),c);
        }

		/// Maximum of four numbers
		/// \param a number
		/// \param b number
		/// \param c number
		/// \param d number
		/// \return result
        template <class T>
        static T max (const T &a, const T &b, const T &c, const T &d)
        {
            return max(max(a,b),max(c,d));
        }


        //
        // Hash functions
        //

		/// Calculate a hash of some data
		/// \param data pointer to raw bytes
		/// \param length length of raw bytes
		/// \return 32 bit hash
		static DTuint			calc_crc32      (const void* data, DTsize length);

    
        //
        // Conversion Routines
        //

		/// Converts a 4 bit number to 8 bit keeping the correct proportions
		/// \param v 4 bit value
		/// \return 8 bit value out
        static inline DTubyte convert_4_to_8_bit    (DTubyte v)
        {
            return _4_to_8_bit[v];
        }
		
		/// Converts a 5 bit number to 8 bit keeping the correct proportions
		/// \param v 5 bit value
		/// \return 8 bit value out
        static inline DTubyte convert_5_to_8_bit    (DTubyte v)
        {
            return _5_to_8_bit[v];
        }
		
		/// Converts a 6 bit number to 8 bit keeping the correct proportions
		/// \param v 6 bit value
		/// \return 8 bit value out
        static inline DTubyte convert_6_to_8_bit    (DTubyte v)
        {
            return _6_to_8_bit[v];
        }
    
		/// Transforms a vector and divides by W
		/// \param v 8 bit value
		/// \param v 8 bit value
		/// \param v 8 bit value
        static Vector3 transform_4h (const Matrix4 &a,  const Vector3 &b);

	private:
    
		typedef union {
			DTint		i;
			DTfloat		f;
		} IntOrFloat;
    
        typedef union {
            DTubyte     b[2];
            DTushort    s;
        } ShortOrBytes;

        static const DTint CRC_TABLE_SIZE = 256;
        static const DTint FACTORIAL_TABLE_SIZE = 16;
        static const DTint MT_LEN = 624;

		static DTuint		_crc_table[CRC_TABLE_SIZE];
		static DTuint		_random_seed;
		
        static DTubyte      _4_to_8_bit[16];
        static DTubyte      _5_to_8_bit[32];
        static DTubyte      _6_to_8_bit[64];
    
		static DTfloat		_factorial[FACTORIAL_TABLE_SIZE];
		static DTfloat		_factorial_inv[FACTORIAL_TABLE_SIZE];

		static DTint        _mt_index;
		static DTuint       _mt_buffer[MT_LEN];
    
        static DTubyte      _entropy_pool[17];  // 17 is prime
        static DTushort     _entropy_pool_pos;
};

//==============================================================================
//==============================================================================


} // DT3

#endif

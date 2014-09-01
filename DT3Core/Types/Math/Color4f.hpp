#ifndef DT3_COLOR4F
#define DT3_COLOR4F
//==============================================================================
///	
///	File: Color4f.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class Color4b;

//==============================================================================
/// Class
//==============================================================================

class Color4f {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Color4f)
        
                                                Color4f					(void);
                                                Color4f					(const Color4f &rhs);
                                                Color4f					(const DTfloat r_, const DTfloat g_, const DTfloat b_, const DTfloat a_ = 1.0F);
                                                Color4f					(const DTubyte r_, const DTubyte g_, const DTubyte b_, const DTubyte a_ = DTUBYTE_MAX);
        explicit                                Color4f					(const DTfloat rhs[]);			
		explicit								Color4f					(const DTubyte rhs[]);
        explicit                                Color4f                 (const Color4b& rhs);
        Color4f &								operator =              (const Color4f& rhs);
                                                ~Color4f				(void)  {}
                        
	public:
    
		DTboolean								operator ==				(const Color4f& rhs) const	{	return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;	}
		DTboolean								operator !=				(const Color4f& rhs) const	{	return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a;	}

        //
        // Color4f setting routines
        //
    
		/// Sets the color components as floats
		/// \param r_ red
		/// \param g_ green
		/// \param b_ blue
		/// \param a_ alpha
        void                                    set                     (const DTfloat r_, const DTfloat g_, const DTfloat b_, const DTfloat a_ = 1.0F);

		/// Sets the color components as bytes
		/// \param r_ red
		/// \param g_ green
		/// \param b_ blue
		/// \param a_ alpha
        void                                    set                     (const DTubyte r_, const DTubyte g_, const DTubyte b_, const DTubyte a_ = DTUBYTE_MAX);

		/// Sets the color components as float array
		/// \param af color components as array
        void                                    set                     (const DTfloat af[]);

		/// Sets the color components as byte array
		/// \param af color components as array
        void                                    set                     (const DTubyte af[]);


        //
        // Color4f component setting routines
        //

		/// Sets color components as floats
        void                                    set_r                   (const DTfloat r_)		{	r = r_;     }
        void                                    set_g                   (const DTfloat g_)		{	g = g_;     }
        void                                    set_b                   (const DTfloat b_)		{	b = b_;     }
        void                                    set_a                   (const DTfloat a_)		{	a = a_;     }
    
		/// Sets color components as bytes
        void                                    set_r                   (const DTubyte r_);
        void                                    set_g                   (const DTubyte g_);
        void                                    set_b                   (const DTubyte b_);
        void                                    set_a                   (const DTubyte a_);


		/// Returns color components as floats
		/// \return color components
        inline DTfloat							r_as_float              (void) 	const           {	return r;	}
        inline DTfloat							g_as_float              (void)	const           {	return g;	}
        inline DTfloat							b_as_float              (void)	const           {	return b;	}
        inline DTfloat							a_as_float              (void)	const           {	return a;	}
    
		/// Returns color components as bytes
		/// \return color components
        DTubyte                                 r_as_byte               (void) 	const;
        DTubyte                                 g_as_byte               (void)	const;
        DTubyte                                 b_as_byte               (void)	const;
        DTubyte                                 a_as_byte               (void)	const;
    
    
    
    
		/// Returns wether the color is black
		/// \return is black
		DTboolean								is_black                (void) const            {	return r == 0.0F && g == 0.0F && b == 0.0F; }

		/// Returns wether the color is white
		/// \return is white
		DTboolean								is_white                (void) const            {	return r == 1.0F && g == 1.0F && b == 1.0F;	}
                        
   
        /// Some standard colors
        static const Color4f                    black;
        static const Color4f                    grey10;
        static const Color4f                    grey25;
        static const Color4f                    grey50;
        static const Color4f                    grey75;
        static const Color4f                    white;

        static const Color4f                    red;
        static const Color4f                    green;
        static const Color4f                    blue;
        static const Color4f                    cyan;
        static const Color4f                    magenta;
        static const Color4f                    yellow;

        static const Color4f                    transparent_black;
        static const Color4f                    transparent_white;


	public:
        DTfloat r;
        DTfloat g;
        DTfloat b;
        DTfloat a;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator << (Stream &s, const Color4f&c);
Stream& operator >> (Stream &s, Color4f&c);

//==============================================================================
//==============================================================================

inline Color4f operator + (const Color4f &a, const Color4f &b)
{
    return Color4f(	a.r_as_float() + b.r_as_float(),
                    a.g_as_float() + b.g_as_float(),
                    a.b_as_float() + b.b_as_float(),
                    a.a_as_float() + b.a_as_float()	);
}

inline Color4f operator * (const Color4f &a, const Color4f &b)
{
	return Color4f(	a.r_as_float() * b.r_as_float(),
                    a.g_as_float() * b.g_as_float(),
                    a.b_as_float() * b.b_as_float(),
                    a.a_as_float() * b.a_as_float()	);
}

inline Color4f operator * (const Color4f &a, DTfloat b)
{
	return Color4f(	a.r_as_float() * b,
					a.g_as_float() * b,
					a.b_as_float() * b,
					a.a_as_float() * b	);
}

inline Color4f operator * (DTfloat a, const Color4f &b)
{
	return Color4f(	a * b.r_as_float(),
					a * b.g_as_float(),
					a * b.b_as_float(),
					a * b.a_as_float()	);
}

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Color4f> {
	static Color4f				default_value(void)	{	return Color4f(0.0F,0.0F,0.0F,0.0F);    }
	static const DTcharacter*	name(void)          {	return "Color4f";}
	static const DTcharacter*	name_caps(void)     {	return "Color4f";}
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================

} // DT3
#endif

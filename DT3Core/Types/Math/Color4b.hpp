#ifndef DT3_COLOR4B
#define DT3_COLOR4B
//==============================================================================
///	
///	File: Color4b.hpp
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
class Color4f;

//==============================================================================
/// Class
//==============================================================================

class Color4b {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Color4b)
        
                                                Color4b					(void);
                                                Color4b					(const Color4b &rhs);
                                                Color4b					(const DTfloat r_, const DTfloat g_, const DTfloat b_, const DTfloat a_ = 1.0F);
                                                Color4b					(const DTubyte r_, const DTubyte g_, const DTubyte b_, const DTubyte a_ = DTUBYTE_MAX);
        explicit                                Color4b					(const DTfloat rhs[]);			
		explicit								Color4b					(const DTubyte rhs[]);			
        explicit                                Color4b                 (const Color4f& rhs);
        Color4b &								operator =              (const Color4b& rhs);
                                                ~Color4b				(void)  {}
                        
	public:
    
		DTboolean								operator ==				(const Color4b& rhs) const	{	return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;	}
		DTboolean								operator !=				(const Color4b& rhs) const	{	return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a;	}

        //
        // Color4b setting routines
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
        // Color4b component setting routines
        //

		/// Sets color components as floats
        void                                    set_r                   (const DTfloat r_);
        void                                    set_g                   (const DTfloat g_);
        void                                    set_b                   (const DTfloat b_);
        void                                    set_a                   (const DTfloat a_);
    
		/// Sets color components as bytes
        void                                    set_r                   (const DTubyte r_)		{	r = r_;				}
        void                                    set_g                   (const DTubyte g_)		{	g = g_;				}
        void                                    set_b                   (const DTubyte b_)		{	b = b_;				}
        void                                    set_a                   (const DTubyte a_)		{	a = a_;				}


		/// Returns color components as floats
		/// \return color components
        DTfloat                                 r_as_float              (void) 	const;
        DTfloat                                 g_as_float              (void)	const;
        DTfloat                                 b_as_float              (void)	const;
        DTfloat                                 a_as_float              (void)	const;
    
		/// Returns color components as bytes
		/// \return color components
        inline DTubyte							r_as_byte               (void) 	const           {	return r;			}
        inline DTubyte							g_as_byte               (void)	const           {	return g;			}
        inline DTubyte							b_as_byte               (void)	const           {	return b;			}
        inline DTubyte							a_as_byte               (void)	const           {	return a;			}
    
    
    
    
		/// Returns wether the color is black
		/// \return is black
		DTboolean								is_black                (void) const				{	return r == 0 && g == 0 && b == 0;		}

		/// Returns wether the color is white
		/// \return is white
		DTboolean								is_white                (void) const				{	return r == 255 && g == 255 && b == 255;	}
                        
   
        /// Some standard colors
        static const Color4b                    black;
        static const Color4b                    grey10;
        static const Color4b                    grey25;
        static const Color4b                    grey50;
        static const Color4b                    grey75;
        static const Color4b                    white;

        static const Color4b                    red;
        static const Color4b                    green;
        static const Color4b                    blue;
        static const Color4b                    cyan;
        static const Color4b                    magenta;
        static const Color4b                    yellow;

        static const Color4b                    transparent_black;
        static const Color4b                    transparent_white;


	public:
		union {
			struct {
				DTubyte r;
				DTubyte g;
				DTubyte b;
				DTubyte a;
			};
			DTuint rgba;
		};
};		

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Color4b&c);
Stream& operator >>(Stream &s, Color4b&c);

//==============================================================================
//==============================================================================

inline Color4b operator + (const Color4b &a, const Color4b &b)
{
    return Color4b(	a.r_as_float() + b.r_as_float(),
                    a.g_as_float() + b.g_as_float(),
                    a.b_as_float() + b.b_as_float(),
                    a.a_as_float() + b.a_as_float()	);
}

inline Color4b operator * (const Color4b &a, const Color4b &b)
{
	if (a.rgba == 0xFFFFFFFF)
		return b;
	else if (b.rgba == 0xFFFFFFFF)
		return a;
	else
		return Color4b(	a.r_as_float() * b.r_as_float(),
                        a.g_as_float() * b.g_as_float(),
                        a.b_as_float() * b.b_as_float(),
                        a.a_as_float() * b.a_as_float()	);
}

inline Color4b operator * (const Color4b &a, DTfloat b)
{
	return Color4b(	a.r_as_float() * b,
					a.g_as_float() * b,
					a.b_as_float() * b,
					a.a_as_float() * b	);
}

inline Color4b operator * (DTfloat a, const Color4b &b)
{
	return Color4b(	a * b.r_as_float(),
					a * b.g_as_float(),
					a * b.b_as_float(),
					a * b.a_as_float()	);
}

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Color4b> {
	static Color4b				default_value(void)	{	return Color4b(0.0F,0.0F,0.0F,0.0F);    }
	static const DTcharacter*	name(void)          {	return "Color4b";}
	static const DTcharacter*	name_caps(void)     {	return "Color4b";}
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================

} // DT3
#endif

#ifndef DT3_FILTERS
#define DT3_FILTERS
//==============================================================================
///	
///	File: Filters.hpp
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

//==============================================================================
/// Class
//==============================================================================

class Filters {
    public:		
		DEFINE_TYPE_SIMPLE_BASE(Filters)
         
								Filters     (void);
								Filters     (const Filters &rhs);
        Filters &               operator =  (const Filters &rhs);
								~Filters    (void);
		
	public:
	
		enum FilterType {
			FILTER_BUTTERWORTH_LOW_PASS = 0,
			FILTER_CRITICALLY_DAMPED_LOW_PASS = 1,
			FILTER_BESSEL_LOW_PASS = 2
		};
	
		/// Initialize the filter
		/// \param type filter type
		/// \param n number of filter passes
		/// \param f0 3db frequency
		/// \param fs sampling frequency
		/// \param default_val initialization value
		void            initialize			(	FilterType type,
                                                DTuint n,
                                                DTfloat f0,
                                                DTfloat fs,
                                                DTfloat default_val);
		
		/// Apply the filter
		/// \param in value
		/// \return filtered value
		DTfloat         filter              (const DTfloat &in);
		
	private:
        friend Stream& operator <<(Stream &s, const Filters &f);
        friend Stream& operator >>(Stream &s, Filters &f);
    
		DTfloat _a0,_a1,_a2,_b1,_b2;
		DTfloat _x[3];
		DTfloat _y[3];
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Filters &f);
Stream& operator >>(Stream &s, Filters &f);

//==============================================================================
// Info classes for Traits
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<Filters> {
	static Filters				default_value(void)	{	return Filters();   }
	static const DTcharacter*	name(void)          {	return "Filters";   }
	static const DTcharacter*	name_caps(void)     {	return "Filters";   }
    enum { isFundamental = 0 };
};

template <> struct Info<Filters::FilterType> {
	static Filters::FilterType  default_value(void)	{	return Filters::FILTER_BUTTERWORTH_LOW_PASS;   }
	static const DTcharacter*	name(void)          {	return "FilterType";   }
	static const DTcharacter*	name_caps(void)     {	return "FilterType";   }
    enum { isFundamental = 0 };
};

} // TypeTraitsInfo

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_DISPLAYMODE
#define DT3_DISPLAYMODE
//==============================================================================
///	
///	File: DisplayMode.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class DisplayMode: public BaseClass {
    public:		
        DEFINE_TYPE(DisplayMode,BaseClass)
		DEFINE_CREATE_AND_CLONE
         
								DisplayMode			(void);	
								DisplayMode         (const DisplayMode &rhs);
        DisplayMode &			operator =          (const DisplayMode &rhs);		
								~DisplayMode        (void);

	public:
    
		DTboolean				operator ==			(const DisplayMode& rhs) const	{	return (_width == rhs._width) && (_height == rhs._height);  }

		/// Comparison operators for properly sorting resolutions
		DTboolean               operator <			(const DisplayMode& rhs) const	{	return _width * _height * _refresh_rate < rhs._width * rhs._height * rhs._refresh_rate; }
		DTboolean               operator >          (const DisplayMode& rhs) const	{	return !operator < (rhs);   }

        DEFINE_ACCESSORS(width, set_width, DTint, _width)
        DEFINE_ACCESSORS(height, set_height, DTint, _height)
        DEFINE_ACCESSORS(refresh_rate, set_refresh_rate, DTdouble, _refresh_rate)
        
	private:
		DTint		_width;
		DTint		_height;
		DTdouble    _refresh_rate;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_GPSCOORD
#define DT3_GPSCOORD
//==============================================================================
///	
///	File: GPSCoord.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

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

class GPSCoord {
    public:		
        DEFINE_TYPE_SIMPLE_BASE(GPSCoord)
         
								GPSCoord			(void);	
								GPSCoord			(const GPSCoord &rhs);
        GPSCoord &              operator =          (const GPSCoord &rhs);		
								~GPSCoord			(void);
		
	public:
        DEFINE_ACCESSORS (latitude, set_latitude, DTdouble, _latitude);
        DEFINE_ACCESSORS (longitude, set_longitude, DTdouble, _longitude);
        DEFINE_ACCESSORS (elevation, set_elevation, DTdouble, _elevation);

		/// Calculate the distance in kilometers between GPS coords
		/// \param a GPS coordinate a
		/// \param b GPS coordinate b
		/// \return distance in km
        static DTdouble         distance            (const GPSCoord &a, const GPSCoord &b);
        
	private:
		friend Stream& operator <<(Stream &s, const GPSCoord &v);
		friend Stream& operator >>(Stream &s, GPSCoord &v);

        DTdouble                _latitude;
        DTdouble                _longitude;
        DTdouble                _elevation;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const GPSCoord&v);
Stream& operator >>(Stream &s, GPSCoord&v);

//==============================================================================
//==============================================================================

} // DT3

#endif

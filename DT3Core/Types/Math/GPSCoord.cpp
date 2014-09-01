//==============================================================================
///	
///	File: GPSCoord.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/GPSCoord.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GPSCoord::GPSCoord (void)
    :   _latitude   (0.0),
        _longitude  (0.0),
        _elevation  (0.0)
{

}

GPSCoord::GPSCoord (const GPSCoord &rhs)
	:	_latitude   (rhs._latitude),
        _longitude  (rhs._longitude),
        _elevation  (rhs._elevation)
{
	
}
		
GPSCoord& GPSCoord::operator = (const GPSCoord &rhs)
{	
    _latitude = rhs._latitude;
    _longitude = rhs._longitude;
    _elevation = rhs._elevation;

    return (*this);
}	

GPSCoord::~GPSCoord (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const GPSCoord &v)
{
    s << v._latitude;
    s << v._longitude;
    s << v._elevation;
	return s;
}

Stream& operator >>(Stream &s, GPSCoord &v)
{
    s >> v._latitude;
    s >> v._longitude;
    s >> v._elevation;
	return s;
}

//==============================================================================
// http://stackoverflow.com/questions/365826/calculate-distance-between-2-gps-coordinates
//==============================================================================

DTdouble GPSCoord::distance (const GPSCoord &a, const GPSCoord &b)
{
    const DTdouble r = 6371.0; // km
    DTdouble d_lat = (b._latitude - a._latitude) * DEG_TO_RAD;
    DTdouble d_lon = (b._longitude - a._longitude) * DEG_TO_RAD;
    DTdouble lat_1 = a._latitude * DEG_TO_RAD;
    DTdouble lat_2 = b._latitude * DEG_TO_RAD;

    DTdouble aa =   std::sin(d_lat * 0.5) * std::sin(d_lat * 0.5) +
                    std::sin(d_lon * 0.5) * std::sin(d_lon * 0.5) * std::cos(lat_1) * std::cos(lat_2);
    
    DTdouble c = 2.0 * std::atan2(std::sqrt(aa), std::sqrt(1.0-aa));
    
    return r * c;
}

//==============================================================================
//==============================================================================

} // DT3

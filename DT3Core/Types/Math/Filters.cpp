//==============================================================================
///	
///	File: Filters.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Filters.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Filters::Filters (void)
{
    _a0 = 0.0F;
    _a1 = 0.0F;
    _a2 = 0.0F;
    _b1 = 0.0F;
    _b2 = 0.0F;
    _x[0] = 0.0F;
    _x[1] = 0.0F;
    _x[2] = 0.0F;
    _y[0] = 0.0F;
    _y[1] = 0.0F;
    _y[2] = 0.0F;
}

Filters::Filters (const Filters &rhs)
{
    _a0 = rhs._a0;
    _a1 = rhs._a1;
    _a2 = rhs._a2;
    _b1 = rhs._b1;
    _b2 = rhs._b2;
    _x[0] = rhs._x[0];
    _x[1] = rhs._x[1];
    _x[2] = rhs._x[2];
    _y[0] = rhs._y[0];
    _y[1] = rhs._y[1];
    _y[2] = rhs._y[2];
}
		
Filters& Filters::operator = (const Filters &rhs)
{	
    _a0 = rhs._a0;
    _a1 = rhs._a1;
    _a2 = rhs._a2;
    _b1 = rhs._b1;
    _b2 = rhs._b2;
    _x[0] = rhs._x[0];
    _x[1] = rhs._x[1];
    _x[2] = rhs._x[2];
    _y[0] = rhs._y[0];
    _y[1] = rhs._y[1];
    _y[2] = rhs._y[2];

    return (*this);
}	

Filters::~Filters (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator << (Stream &s, const Filters &f)
{
    s   << f._a0 << Stream::fs
        << f._a1 << Stream::fs
        << f._a2 << Stream::fs
        << f._b1 << Stream::fs
        << f._b2 << Stream::fs
        << f._x[0] << Stream::fs
        << f._x[1] << Stream::fs
        << f._x[2] << Stream::fs
        << f._y[0] << Stream::fs
        << f._y[1] << Stream::fs
        << f._y[2];

	return s;
}

Stream& operator >> (Stream &s, Filters &f)
{
    s   >> f._a0 >> f._a1 >> f._a2
        >> f._b1 >> f._b2
        >> f._x[0] >> f._x[1] >> f._x[2]
        >> f._y[0] >> f._y[1] >> f._y[2];

	return s;
}

//==============================================================================
//==============================================================================

void Filters::initialize (	FilterType type,
                            DTuint n,
                            DTfloat f0,
                            DTfloat fs,
                            DTfloat default_val)
{
    // See: http://unicorn.us.com/trading/allpolefilters.html#part2

    DTfloat c,g,p;

    switch (type) {
        case FILTER_BUTTERWORTH_LOW_PASS:
            c = std::pow(std::pow(2.0F,1.0F/n) - 1.0F, -1.0F/4.0F);
            g = 1.0F;
            p = std::sqrt(2.0F);
            break;

        case FILTER_CRITICALLY_DAMPED_LOW_PASS:
            c = std::pow(std::pow(2.0F,1.0F/(2.0F*n)) - 1.0F, -1.0F/2.0F);
            g = 1.0F;
            p = 2.0F;
            break;

        case FILTER_BESSEL_LOW_PASS:
            c = std::pow(std::pow(std::pow(2.0F,1.0F/n) - 0.75F, 1.0F/2.0F) - 1.0F/2.0F,-1.0F/2.0F) / std::sqrt(3.0F);
            g = 3.0F;
            p = 3.0F;
            break;
    };
    
    
    DTfloat fcorr = c * f0 / fs;
        
    DTfloat w0 = std::tan(PI * fcorr);
    
    DTfloat K1 = p * w0;
    DTfloat K2 = g * w0 * w0;
    DTfloat A0 = K2 / (1.0F + K1 + K2);
    DTfloat A1 = 2.0F * A0;
    DTfloat A2 = A0;
    
    DTfloat B1 = 2.0F * A0 * (1.0F/K2-1.0F);
    DTfloat B2 = 1.0F - (A0 + A1 + A2 + B1);
    
    _a0 = A0;
    _a1 = A1;
    _a2 = A2;
    _b1 = B1;
    _b2 = B2;
    
    // clear history
    _x[0] = _x[1] = _x[2] = default_val;
    _y[0] = _y[1] = _y[2] = default_val;
}
	
//==============================================================================
//==============================================================================

DTfloat Filters::filter (const DTfloat &in)
{
    _x[2] = _x[1];
    _x[1] = _x[0];
    _x[0] = in;

    _y[2] = _y[1];
    _y[1] = _y[0];
    _y[0] = _a0 * _x[0] + 
            _a1 * _x[1] + 
            _a2 * _x[2] + 
            _b1 * _y[1] + 
            _b2 * _y[2];
            
    return _y[0];
}

//==============================================================================
//==============================================================================
		
} // DT3

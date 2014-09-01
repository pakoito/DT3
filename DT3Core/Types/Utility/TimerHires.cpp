//==============================================================================
///	
///	File: TimerHires.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/TimerHires.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TimerHires::TimerHires (void)
    :   _last_time  (0.0F)
{
    delta_time ();
    reset_abs_time ();
}

TimerHires::TimerHires (const TimerHires &rhs)
	:	_last_time	(rhs._last_time),
		_first_time	(rhs._first_time)
{

}
		
TimerHires& TimerHires::operator = (const TimerHires &rhs)
{	
	_last_time = rhs._last_time;
	_first_time = rhs._first_time;
    return (*this);
}	

TimerHires::~TimerHires (void)
{

}

//==============================================================================
//==============================================================================

DTdouble TimerHires::delta_time (void)
{
    DTdouble real_time = HAL::program_running_time();
    DTdouble delta_time = real_time - _last_time;
    
    _last_time = real_time;
            
    return delta_time < 0.0 ? 0.0 : delta_time;
}

//==============================================================================
//==============================================================================

void TimerHires::reset_abs_time (void)
{
    _first_time = HAL::program_running_time();
}

//==============================================================================
//==============================================================================

DTdouble TimerHires::abs_time (void) const
{
    return HAL::program_running_time() - _first_time;
}

//==============================================================================
//==============================================================================

} // DT3

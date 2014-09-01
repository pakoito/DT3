//==============================================================================
///	
///	File: TimerLores.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/TimerLores.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTdouble TimerLores::_app_running_time = 0.0;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TimerLores::TimerLores (void)
    :   _last_time  (0.0F)
{
    delta_time ();
    reset_abs_time ();
}

TimerLores::TimerLores (const TimerLores &rhs)
	:	_last_time	(rhs._last_time),
		_first_time	(rhs._first_time)
{

}
		
TimerLores& TimerLores::operator = (const TimerLores &rhs)
{	
	_last_time = rhs._last_time;
	_first_time = rhs._first_time;
    return (*this);
}	

TimerLores::~TimerLores (void)
{

}

//==============================================================================
//==============================================================================

void TimerLores::update_program_running_time(void)
{
    _app_running_time = HAL::program_running_time();
}

//==============================================================================
//==============================================================================

void TimerLores::reset_abs_time (void)
{
    _first_time = _app_running_time;
}

//==============================================================================
//==============================================================================

} // DT3

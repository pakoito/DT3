//==============================================================================
///	
///	File: Time.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Time.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include <time.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTuint64 Time::seconds (void)
{
	time_t rawtime;
	time ( &rawtime );
	return rawtime;
}

//==============================================================================
//==============================================================================

std::string Time::seconds_to_clock (DTuint s)
{
	DTint minutes = s / 60;
	DTint seconds = s % 60;
	
	if (minutes > 0) {
		if (seconds >= 0 && seconds <= 9)
			return MoreStrings::cast_to_string(minutes) + ":0" + MoreStrings::cast_to_string(seconds);
		else
			return MoreStrings::cast_to_string(minutes) + ":" + MoreStrings::cast_to_string(seconds);
	} else {
		return MoreStrings::cast_to_string(seconds);
	}
				
}

//==============================================================================
//==============================================================================

std::string Time::time_string	(void)
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime(&rawtime);
    
	return MoreStrings::find_and_replace(std::string(asctime(timeinfo)), "\n", "");
}

std::string Time::time_short_string	(void)
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	DTcharacter timestr[1024];
	strftime ( timestr, ARRAY_SIZE(timestr), "%b %d %I:%M %p", timeinfo);
	
	return std::string( timestr );
}

//==============================================================================
//==============================================================================

} // DT3

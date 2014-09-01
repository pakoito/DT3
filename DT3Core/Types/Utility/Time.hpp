#ifndef DT3_TIME
#define DT3_TIME
//==============================================================================
///	
///	File: Time.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Time {
    private:
									Time                (void);	
									Time                (const Time &rhs);
        Time &						operator =          (const Time &rhs);
		virtual						~Time               (void);

	public:
		/// Returns the number of seconds on clock
		static DTuint64				seconds             (void);

		/// Returns the number of seconds like a clock
		static std::string          seconds_to_clock    (DTuint seconds);
	
		/// Returns the current time as a string
		/// \return time as a string
		static std::string          time_string         (void);

		/// Returns the current time as a string
		/// \return time as a string
		static std::string          time_short_string   (void);

};

//==============================================================================
//==============================================================================


} // DT3

#endif

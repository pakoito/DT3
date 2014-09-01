#ifndef DT3_TIMERLORES
#define DT3_TIMERLORES
//==============================================================================
///	
///	File: TimerLores.hpp
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
/// Class
//==============================================================================

class TimerLores {
    public:		
        DEFINE_TYPE_SIMPLE_BASE(TimerLores)

								TimerLores                  (void);
								TimerLores                  (const TimerLores &rhs);
        TimerLores &			operator =                  (const TimerLores &rhs);
								~TimerLores                 (void);

	public:
		/// Get the change in time since last called
		/// \return dt
        inline DTdouble			delta_time                  (void)
        {
            DTdouble delta_time = _app_running_time - _last_time;
            _last_time = _app_running_time;
            return delta_time;
        }
    
		/// Reset the absolute timer
        void					reset_abs_time              (void);

		/// Get the absolute timer
		/// \return time
        DTdouble                abs_time                    (void) const
        {
            return _app_running_time - _first_time;
        }
                
		/// Update the program running time. This gets called from the mail loop
        static void				update_program_running_time (void);
    
    private:
		static DTdouble         _app_running_time;
        DTdouble				_last_time;
        DTdouble				_first_time;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

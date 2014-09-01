#ifndef DT3_TIMERHIRES
#define DT3_TIMERHIRES
//==============================================================================
///	
///	File: TimerHires.hpp
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

class TimerHires {
    public:		
        DEFINE_TYPE_SIMPLE_BASE(TimerHires)

								TimerHires				(void);	
								TimerHires				(const TimerHires &rhs);
        TimerHires &			operator =              (const TimerHires &rhs);		
								~TimerHires             (void);

	public:
		/// Get the change in time since last called
		/// \return dt
        DTdouble                delta_time              (void);
        
		/// Reset the absolute timer
        void					reset_abs_time          (void);

		/// Get the absolute timer
		/// \return time
        DTdouble                abs_time                (void) const;
                    
	protected:
		void					set_last_time           (DTdouble last_time)    {	_last_time = last_time;	}
		DTdouble                last_time               (void)					{	return _last_time;		}
			    
    private:		
        DTdouble                _last_time;
        DTdouble                _first_time;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_MONKEYTEST
#define DT3_MONKEYTEST
//==============================================================================
///	
///	File: MonkeyTest.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/TimerHires.hpp"
#include "DT3Core/Types/Utility/TouchEvent.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class MonkeyTest {
    public:
        DEFINE_TYPE_SIMPLE_BASE(MonkeyTest)

							MonkeyTest				(void);
    private:
							MonkeyTest				(const MonkeyTest &rhs);
        MonkeyTest &		operator =				(const MonkeyTest &rhs);
    public:
        virtual				~MonkeyTest             (void);
                
	public:
		/// Run the monkey test
        void                run                     (void);
    
    private:
        void                new_event               (void);
    
        // Event kinds    
        enum {
            STATE_START_TAP,
            STATE_TAPPING,
            STATE_STOP_TAP,

            STATE_START_DRAG,
            STATE_DRAGGING,
            STATE_STOP_DRAG,
            
            STATE_START_IDLE,
            STATE_IDLING,
            STATE_STOP_IDLE
        }   _state;
    
    
        DTfloat             _time;
        DTfloat             _time_remaining;
    
        Vector2             _position_1;
        Vector2             _position_2;
    
        TimerHires          _timer;
        TouchEvent          _event;
    
};

//==============================================================================
//==============================================================================

} // DT3

#endif

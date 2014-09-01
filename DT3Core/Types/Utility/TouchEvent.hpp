#ifndef DT3_TOUCHEVENT
#define DT3_TOUCHEVENT
//==============================================================================
///	
///	File: TouchEvent.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Utility/TimerHires.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class TouchEvent {
    public:		
        DEFINE_TYPE_SIMPLE_BASE(TouchEvent)
         
								TouchEvent              (void);
								TouchEvent              (const TouchEvent &rhs);
        TouchEvent &            operator =              (const TouchEvent &rhs);
								~TouchEvent             (void);
		
	public:
    
		/// Clear the touch state
        void					clear                   (void);

		/// Convenience finction to re-range input to 0 - 1
		/// \param pos position from touch event
		/// \return normalized position
        static Vector2          normalize_input         (Vector2 pos);

		/// Convenience finction to re-range input to 0 - 1
		/// \param pos position from touch event
		/// \return normalized velocity
        static Vector2          normalize_input_vel     (Vector2 pos);

		/// Convenience finction to re-range input to -1 - 1
		/// \param pos position from touch event
		/// \return normalized position
        static Vector2          normalize_input_ndc     (Vector2 pos);

		/// Sets all of the released touches to none
        void                    cleanup_releases        (void);


	public:
        static const DTsize MAX_NUM_TOUCHES = 10;
    
        enum State {
            STATE_PRESSED,
            STATE_RELEASED,
            STATE_DOWN,
            STATE_HOVER,
            STATE_NONE
        };
        
        struct Touch {
            Touch   (void)
                :   state       (STATE_NONE),
                    pos         (0.0F,0.0F),
                    previous_pos(0.0F,0.0F),
                    first_pos   (0.0F,0.0F),
                    delta       (0.0F,0.0F),
                    velocity    (0.0F,0.0F),
                    dt          (0.0F)
            {}
            
            State       state;
            
            Vector2     pos;
            Vector2     previous_pos;
            Vector2     first_pos;
            
            Vector2     delta;
            Vector2     velocity;
            
            TimerHires  timer;
            DTfloat     dt;
        };
        
        Touch           touches[MAX_NUM_TOUCHES];
};

//==============================================================================
//==============================================================================

} // DT3

#endif

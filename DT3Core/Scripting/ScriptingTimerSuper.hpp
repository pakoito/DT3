#ifndef DT3_SCRIPTINGTIMERSUPER
#define DT3_SCRIPTINGTIMERSUPER
//==============================================================================
///	
///	File: ScriptingTimerSuper.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Timer is active when input is active. Resets when inactive.
//==============================================================================

class ScriptingTimerSuper: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingTimerSuper,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTimerSuper     (void);	
									ScriptingTimerSuper     (const ScriptingTimerSuper &rhs);
        ScriptingTimerSuper &		operator =				(const ScriptingTimerSuper &rhs);	
        virtual                     ~ScriptingTimerSuper	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize				(void);
				
	private:
        enum {
            STATE_CHANGE_BEHAVIOUR_COUNT_UP = 0,
            STATE_CHANGE_BEHAVIOUR_COUNT_UP_FROM_LOWER = 1,
            STATE_CHANGE_BEHAVIOUR_COUNT_DOWN = 2,
            STATE_CHANGE_BEHAVIOUR_COUNT_DOWN_FROM_UPPER = 3,
            STATE_CHANGE_BEHAVIOUR_COUNT_STOP = 4,
            STATE_CHANGE_BEHAVIOUR_COUNT_STOP_RESET_LOWER = 5,
            STATE_CHANGE_BEHAVIOUR_COUNT_STOP_RESET_UPPER = 6
        };
    
        Plug<DTint>                     _when_activated;
        Plug<DTint>                     _when_deactivated;
    
        enum {
            STATE_RANGE_CONTINUE = 0,
            STATE_RANGE_STOP = 1,
            STATE_RANGE_REVERSE = 2,
            STATE_RANGE_WRAP = 3
        };

        Plug<DTint>                     _when_reach_upper;
        Plug<DTint>                     _when_reach_lower;

		Plug<DTfloat>                   _upper_range;
		Plug<DTfloat>                   _lower_range;

		Plug<DTfloat>                   _counting_up_speed;
		Plug<DTfloat>                   _counting_down_speed;
		Plug<DTfloat>                   _current_speed;
    
		Plug<DTboolean>                 _last_active;
		Plug<DTboolean>                 _active;

        // State
		Plug<DTfloat>                   _time;
		Plug<DTfloat>                   _t;
    
        Event                           _upper_reached;
        Event                           _lower_reached;
		Plug<DTboolean>                 _counting_down;
		Plug<DTboolean>                 _counting_up;
    
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif

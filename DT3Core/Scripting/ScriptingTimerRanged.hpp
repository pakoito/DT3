#ifndef DT3_SCRIPTINGTIMERRANGED
#define DT3_SCRIPTINGTIMERRANGED
//==============================================================================
///	
///	File: ScriptingTimerRanged.hpp
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
/// Ranged Timer.
//==============================================================================

class ScriptingTimerRanged: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingTimerRanged,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTimerRanged	(void);	
									ScriptingTimerRanged	(const ScriptingTimerRanged &rhs);
        ScriptingTimerRanged &		operator =				(const ScriptingTimerRanged &rhs);	
        virtual                     ~ScriptingTimerRanged	(void);
        
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
		Plug<DTfloat>				_lower_bound;
		Plug<DTfloat>				_upper_bound;
	
		Plug<DTfloat>				_speed;		
		Plug<DTboolean>				_count_up;
		Plug<DTboolean>				_count_down;
		Plug<DTfloat>				_time;		
		Plug<DTfloat>				_t;		
		Plug<DTboolean>				_counting;		
};

//==============================================================================
//==============================================================================

} // DT3

#endif

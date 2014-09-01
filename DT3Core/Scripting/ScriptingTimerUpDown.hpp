#ifndef DT3_SCRIPTINGTIMERUPDOWN
#define DT3_SCRIPTINGTIMERUPDOWN
//==============================================================================
///	
///	File: ScriptingTimerUpDown.hpp
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
/// Timer counds up and down between ranges.
//==============================================================================

class ScriptingTimerUpDown: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingTimerUpDown,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTimerUpDown	(void);	
									ScriptingTimerUpDown	(const ScriptingTimerUpDown &rhs);
        ScriptingTimerUpDown &		operator =				(const ScriptingTimerUpDown &rhs);	
        virtual                     ~ScriptingTimerUpDown	(void);
    
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
		DTfloat						_lower_range;
		DTfloat						_upper_range;
	
		Plug<DTfloat>				_speed;		
		Plug<DTboolean>				_active;

		Plug<DTfloat>				_time;		
		Plug<DTfloat>				_t;		
		Plug<DTboolean>				_counting_up;
		Plug<DTboolean>				_counting_down;
		
		DTboolean                   _is_counting_up;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

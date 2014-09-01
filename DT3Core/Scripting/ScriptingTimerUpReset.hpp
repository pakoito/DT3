#ifndef DT3_SCRIPTINGTIMERUPRESET
#define DT3_SCRIPTINGTIMERUPRESET
//==============================================================================
///	
///	File: ScriptingTimerUpReset.hpp
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
/// Timer resets when limit is hit.
//==============================================================================

class ScriptingTimerUpReset: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingTimerUpReset,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTimerUpReset	(void);	
									ScriptingTimerUpReset	(const ScriptingTimerUpReset &rhs);
        ScriptingTimerUpReset &		operator =				(const ScriptingTimerUpReset &rhs);	
        virtual                     ~ScriptingTimerUpReset	(void);
    
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
		DTfloat						_upper_range;
	
		Plug<DTfloat>				_speed;
		Plug<DTboolean>				_active;

		Plug<DTfloat>				_time;
		Plug<DTfloat>				_t;
        
        DTboolean                   _reset_when_inactive;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

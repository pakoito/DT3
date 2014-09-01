#ifndef DT3_SCRIPTINGDELAYOFF
#define DT3_SCRIPTINGDELAYOFF
//==============================================================================
///	
///	File: ScriptingDelayOff.hpp
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
/// Delays a boolean transition from 1 to 0.
//==============================================================================

class ScriptingDelayOff: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingDelayOff,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingDelayOff		(void);	
									ScriptingDelayOff		(const ScriptingDelayOff &rhs);
        ScriptingDelayOff &			operator =				(const ScriptingDelayOff &rhs);	
        virtual                     ~ScriptingDelayOff		(void);
    
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
		DTfloat						_time;
		DTfloat						_delay;
	
		Plug<DTboolean>				_in;
		Plug<DTboolean>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

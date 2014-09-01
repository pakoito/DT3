#ifndef DT3_SCRIPTINGPULSE
#define DT3_SCRIPTINGPULSE
//==============================================================================
///	
///	File: ScriptingPulse.hpp
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
/// Outputs a pulse while input is 1.
//==============================================================================

class ScriptingPulse: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingPulse,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingPulse			(void);	
									ScriptingPulse			(const ScriptingPulse &rhs);
        ScriptingPulse &			operator =				(const ScriptingPulse &rhs);	
        virtual                     ~ScriptingPulse			(void);
    
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
        
        /// Event
        void                        eventIn                 (PlugNode *sender);
				
	private:		
		DTfloat						_time;
		DTfloat						_pulse_time;
	
		Plug<DTboolean>				_in;
		Plug<DTboolean>				_out;
        
        Event                       _in_e;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

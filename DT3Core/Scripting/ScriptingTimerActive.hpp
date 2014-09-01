#ifndef DT3_SCRIPTINGTIMERACTIVE
#define DT3_SCRIPTINGTIMERACTIVE
//==============================================================================
///	
///	File: ScriptingTimerActive.hpp
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

class ScriptingTimerActive: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingTimerActive,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTimerActive	(void);	
									ScriptingTimerActive	(const ScriptingTimerActive &rhs);
        ScriptingTimerActive &		operator =				(const ScriptingTimerActive &rhs);	
        virtual                     ~ScriptingTimerActive	(void);
    
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
		Plug<DTfloat>                   _speed;		
		Plug<DTboolean>                 _count;
		Plug<DTfloat>                   _time;		
};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_ScriptingBoolToEvent
#define DT3_ScriptingBoolToEvent
//==============================================================================
///	
///	File: ScriptingBoolToEvent.hpp
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
/// Converting bool changes to an event.
//==============================================================================

class ScriptingBoolToEvent: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingBoolToEvent,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingBoolToEvent        (void);	
									ScriptingBoolToEvent        (const ScriptingBoolToEvent &rhs);
        ScriptingBoolToEvent &      operator =                  (const ScriptingBoolToEvent &rhs);	
        virtual                     ~ScriptingBoolToEvent       (void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world           (void);

	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                        (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize                  (void);
				
	private:		
        Plug<DTboolean>				_in;
        DTboolean                   _last_in;
		Event                       _out_positive;
		Event                       _out_negative;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

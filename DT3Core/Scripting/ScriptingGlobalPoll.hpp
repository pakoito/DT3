#ifndef DT3_SCRIPTINGGLOBALPOLL
#define DT3_SCRIPTINGGLOBALPOLL
//==============================================================================
///	
///	File: ScriptingGlobalPoll.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Can continuously read a global value.
//==============================================================================

class ScriptingGlobalPoll: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingGlobalPoll,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingGlobalPoll     (void);	
									ScriptingGlobalPoll     (const ScriptingGlobalPoll &rhs);
        ScriptingGlobalPoll &		operator =				(const ScriptingGlobalPoll &rhs);	
        virtual                     ~ScriptingGlobalPoll	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:
    
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Play Event
        void                        read                    (PlugNode *sender);

        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

	private:			                
		Plug<std::string>           _global;
		Plug<std::string>			_value;

        Plug<DTboolean>             _read;
        DTboolean                   _substitute;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

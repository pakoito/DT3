#ifndef DT3_SCRIPTINGISTRANSITIONING
#define DT3_SCRIPTINGISTRANSITIONING
//==============================================================================
///	
///	File: ScriptingIsTransitioning.hpp
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
/// Queries the transitioning flag for the world.
//==============================================================================

class ScriptingIsTransitioning: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingIsTransitioning,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingIsTransitioning    (void);	
									ScriptingIsTransitioning    (const ScriptingIsTransitioning &rhs);
        ScriptingIsTransitioning &	operator =                  (const ScriptingIsTransitioning &rhs);	
        virtual                     ~ScriptingIsTransitioning	(void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world           (void);

	public:
    
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                        (const DTfloat dt);

	private:			                    
        Plug<DTboolean>				_is_transitioning;
        DTboolean                   _last_is_transitioning;
        
        Event                       _begin_transition_e;
        Event                       _end_transition_e;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

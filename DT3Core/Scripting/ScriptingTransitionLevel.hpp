#ifndef DT3_SCRIPTINGTRANSITIONLEVEL
#define DT3_SCRIPTINGTRANSITIONLEVEL
//==============================================================================
///	
///	File: ScriptingTransitionLevel.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

class ScriptingTransitionLevel: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingTransitionLevel,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTransitionLevel    (void);	
									ScriptingTransitionLevel    (const ScriptingTransitionLevel &rhs);
        ScriptingTransitionLevel &	operator =                  (const ScriptingTransitionLevel &rhs);	
        virtual                     ~ScriptingTransitionLevel	(void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
    
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
		/// Play Event
        void                        transition                  (PlugNode *sender);

	private:			                    
                               
		Plug<FilePath>				_pathname;
		Plug<std::string>           _transition;
		Plug<std::string>			_progress;
		Plug<DTfloat>				_duration;

		Plug<DTboolean>				_same_session;
        
        Event                       _transition_e;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_SCRIPTINGANALYTICS
#define DT3_SCRIPTINGANALYTICS
//==============================================================================
///	
///	File: ScriptingAnalytics.hpp
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
/// Record an analytics event.
//==============================================================================

class ScriptingAnalytics: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingAnalytics,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingAnalytics          (void);	
									ScriptingAnalytics          (const ScriptingAnalytics &rhs);
        ScriptingAnalytics &        operator =                  (const ScriptingAnalytics &rhs);	
        virtual                     ~ScriptingAnalytics         (void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
    
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
		/// Play Event
        void                        register_event              (PlugNode *sender);

	private:			                    
                               
		Plug<std::string>           _context;
		Plug<std::string>           _event_id;
		Plug<std::string>			_data;

        Event                       _register_e;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_SCRIPTINGOPENURL
#define DT3_SCRIPTINGOPENURL
//==============================================================================
///	
///	File: ScriptingOpenURL.hpp
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
/// Opens a URL in a web browser.
//==============================================================================

class ScriptingOpenURL: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingOpenURL,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingOpenURL        (void);	
									ScriptingOpenURL        (const ScriptingOpenURL &rhs);
        ScriptingOpenURL &          operator =				(const ScriptingOpenURL &rhs);	
        virtual                     ~ScriptingOpenURL       (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
    
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Go Event
        void                        go                      (PlugNode *sender);

	private:			        
        Event                       _go;
        Plug<std::string>           _url;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

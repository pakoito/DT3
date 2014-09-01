#ifndef DT3_SCRIPTINGLOADCONFIG
#define DT3_SCRIPTINGLOADCONFIG
//==============================================================================
///	
///	File: ScriptingLoadConfig.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class ScriptingLoadConfig: public ScriptingBase {
		/// Queries the transitioning flag for the world.

    public:
        DEFINE_TYPE(ScriptingLoadConfig,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingLoadConfig         (void);	
									ScriptingLoadConfig         (const ScriptingLoadConfig &rhs);
        ScriptingLoadConfig &       operator =                  (const ScriptingLoadConfig &rhs);	
        virtual                     ~ScriptingLoadConfig        (void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
    
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
                
		/// Load Event
        void                        event_load                  (PlugNode *sender);

	private:			                    
        Plug<FilePath>				_config_path;        
        Event                       _load;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

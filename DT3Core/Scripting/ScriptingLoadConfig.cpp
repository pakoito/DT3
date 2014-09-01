//==============================================================================
///	
///	File: ScriptingLoadConfig.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingLoadConfig.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/Configure.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingLoadConfig,"Engine",NULL)
IMPLEMENT_PLUG_NODE(ScriptingLoadConfig)

IMPLEMENT_PLUG_INFO_INDEX(_config_path)		
IMPLEMENT_EVENT_INFO_INDEX(_load)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingLoadConfig)
        
	PLUG_INIT(_config_path,"Config_Path")
		.set_output(true);
                
	EVENT_INIT(_load,"Load")
        .set_input(true)
        .set_event(&ScriptingLoadConfig::event_load);
   
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingLoadConfig::ScriptingLoadConfig (void)
    :   _config_path        (PLUG_INFO_INDEX(_config_path)),
        _load               (EVENT_INFO_INDEX(_load))
{  

}
		
ScriptingLoadConfig::ScriptingLoadConfig (const ScriptingLoadConfig &rhs)
    :   ScriptingBase       (rhs),
		_config_path        (rhs._config_path),
        _load               (rhs._load)
{   

}

ScriptingLoadConfig & ScriptingLoadConfig::operator = (const ScriptingLoadConfig &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_config_path = rhs._config_path;
	}
    return (*this);
}
			
ScriptingLoadConfig::~ScriptingLoadConfig (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingLoadConfig::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingLoadConfig::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_config_path, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingLoadConfig::event_load (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    Configure::import_config(_config_path);
}

//==============================================================================
//==============================================================================

} // DT3


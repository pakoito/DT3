//==============================================================================
///	
///	File: ScriptingOpenURL.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingOpenURL.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Network/URL.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingOpenURL,"Internet",NULL)
IMPLEMENT_PLUG_NODE(ScriptingOpenURL)

IMPLEMENT_PLUG_INFO_INDEX(_url)		
IMPLEMENT_EVENT_INFO_INDEX(_go)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingOpenURL)

	PLUG_INIT(_url,"URL")
		.set_input(true);
        
	EVENT_INIT(_go,"Go")
        .set_input(true)
        .set_event(&ScriptingOpenURL::go);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingOpenURL::ScriptingOpenURL (void)
    :   _url            (PLUG_INFO_INDEX(_url), "http://www.smellslikedonkey.com"),
        _go             (EVENT_INFO_INDEX(_go))
{  

}
		
ScriptingOpenURL::ScriptingOpenURL (const ScriptingOpenURL &rhs)
    :   ScriptingBase	(rhs),
        _url            (rhs._url),
        _go             (rhs._go)
{   

}

ScriptingOpenURL & ScriptingOpenURL::operator = (const ScriptingOpenURL &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _url = rhs._url;
	}
    return (*this);
}
			
ScriptingOpenURL::~ScriptingOpenURL (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingOpenURL::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingOpenURL::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_url, DATA_PERSISTENT | DATA_SETTABLE);
		        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingOpenURL::go (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    HAL::launch_browser(URL(_url));
}

//==============================================================================
//==============================================================================

} // DT3


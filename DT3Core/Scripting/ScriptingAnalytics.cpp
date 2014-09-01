//==============================================================================
///	
///	File: ScriptingAnalytics.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingAnalytics.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/Analytics.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingAnalytics,"Engine",NULL)
IMPLEMENT_PLUG_NODE(ScriptingAnalytics)

IMPLEMENT_PLUG_INFO_INDEX(_context)
IMPLEMENT_PLUG_INFO_INDEX(_event_id)		
IMPLEMENT_PLUG_INFO_INDEX(_data)

IMPLEMENT_EVENT_INFO_INDEX(_register_e)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingAnalytics)

	PLUG_INIT(_context,"Context")
		.set_input(true);

	PLUG_INIT(_event_id,"Event_ID")
		.set_input(true);

	PLUG_INIT(_data,"Data")
		.set_input(true);
                
	EVENT_INIT(_register_e,"Register_Event")
        .set_input(true)
        .set_event(&ScriptingAnalytics::register_event);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingAnalytics::ScriptingAnalytics (void)
    :   _context            (PLUG_INFO_INDEX(_context), "SomeContext"),
        _event_id           (PLUG_INFO_INDEX(_event_id), "SomeEvent"),
		_data               (PLUG_INFO_INDEX(_data), ""),
        _register_e         (EVENT_INFO_INDEX(_register_e))
{  

}
		
ScriptingAnalytics::ScriptingAnalytics (const ScriptingAnalytics &rhs)
    :   ScriptingBase	(rhs),
        _context        (rhs._context),
        _event_id       (rhs._event_id),
        _data           (rhs._data),
        _register_e     (rhs._register_e)
{   

}

ScriptingAnalytics & ScriptingAnalytics::operator = (const ScriptingAnalytics &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _context = rhs._context;
        _event_id = rhs._event_id;
        _data = rhs._data;
	}
    return (*this);
}
			
ScriptingAnalytics::~ScriptingAnalytics (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingAnalytics::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingAnalytics::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_context, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_event_id, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_data, DATA_PERSISTENT | DATA_SETTABLE);
		        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingAnalytics::register_event (PlugNode *sender)
{
    Analytics::record_event(_context, _event_id, _data);
}

//==============================================================================
//==============================================================================

} // DT3


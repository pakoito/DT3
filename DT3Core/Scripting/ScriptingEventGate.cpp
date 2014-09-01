//==============================================================================
///	
///	File: ScriptingEventGate.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingEventGate.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingEventGate,"Events",NULL)
IMPLEMENT_PLUG_NODE(ScriptingEventGate)

IMPLEMENT_PLUG_INFO_INDEX(_enable)

IMPLEMENT_EVENT_INFO_INDEX(_event_in)
IMPLEMENT_EVENT_INFO_INDEX(_event_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingEventGate)

	PLUG_INIT(_enable,"enable")
		.set_input(true);

	EVENT_INIT(_event_in,"Event_In")
        .set_input(true)
        .set_event(&ScriptingEventGate::event_in);

	EVENT_INIT(_event_out,"Event_Out")
        .set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingEventGate::ScriptingEventGate (void)
    :   _enable			(PLUG_INFO_INDEX(_enable), true),
        _event_in       (EVENT_INFO_INDEX(_event_in)),
        _event_out      (EVENT_INFO_INDEX(_event_out))
{  

}
		
ScriptingEventGate::ScriptingEventGate (const ScriptingEventGate &rhs)
    :   ScriptingBase	(rhs),
		_enable			(rhs._enable),
		_event_in       (rhs._event_in),
		_event_out      (rhs._event_out)
{   

}

ScriptingEventGate & ScriptingEventGate::operator = (const ScriptingEventGate &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_enable = rhs._enable;
	}
    return (*this);
}
			
ScriptingEventGate::~ScriptingEventGate (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingEventGate::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingEventGate::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_enable, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingEventGate::event_in (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    if (_enable)
        _event_out.send(sender);
}

//==============================================================================
//==============================================================================

} // DT3


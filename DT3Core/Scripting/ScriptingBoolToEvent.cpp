//==============================================================================
///	
///	File: ScriptingBoolToEvent.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBoolToEvent.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingBoolToEvent,"Conversion",NULL)
IMPLEMENT_PLUG_NODE(ScriptingBoolToEvent)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_EVENT_INFO_INDEX(_out_positive)
IMPLEMENT_EVENT_INFO_INDEX(_out_negative)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingBoolToEvent)

	PLUG_INIT(_in,"In")
		.set_input(true);

	EVENT_INIT(_out_positive,"Out_Positive")
		.set_output(true);
        
	EVENT_INIT(_out_negative,"Out_Negative")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingBoolToEvent::ScriptingBoolToEvent (void)
    :   _in             (PLUG_INFO_INDEX(_in), false),
        _last_in        (false),
		_out_positive   (EVENT_INFO_INDEX(_out_positive)),
		_out_negative   (EVENT_INFO_INDEX(_out_negative))
{  

}
		
ScriptingBoolToEvent::ScriptingBoolToEvent (const ScriptingBoolToEvent &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
        _last_in        (rhs._last_in),
		_out_positive   (rhs._out_positive),
		_out_negative   (rhs._out_negative)
{   

}

ScriptingBoolToEvent & ScriptingBoolToEvent::operator = (const ScriptingBoolToEvent &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
        _last_in = rhs._last_in;
	}
    return (*this);
}
			
ScriptingBoolToEvent::~ScriptingBoolToEvent (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingBoolToEvent::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingBoolToEvent::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_last_in, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingBoolToEvent::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (!_last_in && _in) {   
        _out_positive.send(this);    // this is the originator of the event
        _last_in = _in;
        return;
    }
    
    if (_last_in && !_in) { 
        _out_negative.send(this);    // this is the originator of the event
        _last_in = _in;
        return;
    }
    
}

//==============================================================================
//==============================================================================

void ScriptingBoolToEvent::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingBoolToEvent::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


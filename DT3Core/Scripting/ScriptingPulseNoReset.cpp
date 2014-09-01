//==============================================================================
///	
///	File: ScriptingPulseNoReset.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingPulseNoReset.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingPulseNoReset,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingPulseNoReset)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_EVENT_INFO_INDEX(_in_e)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingPulseNoReset)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
    EVENT_INIT(_in_e, "In_Event")
        .set_input(true)
        .set_event(&ScriptingPulseNoReset::eventIn);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingPulseNoReset::ScriptingPulseNoReset (void)
    :   _time		(0.0F),
		_in			(PLUG_INFO_INDEX(_in), false),
		_pulse_time	(1.0F),
        _event_in   (false),
		_out		(PLUG_INFO_INDEX(_out), false),
		_in_e		(EVENT_INFO_INDEX(_in_e))
{  

}
		
ScriptingPulseNoReset::ScriptingPulseNoReset (const ScriptingPulseNoReset &rhs)
    :   ScriptingBase	(rhs),
		_time			(rhs._time),
		_in				(rhs._in),
		_pulse_time		(rhs._pulse_time),
        _event_in       (rhs._event_in),
		_out			(rhs._out),
		_in_e           (EVENT_INFO_INDEX(_in_e))
{   

}

ScriptingPulseNoReset & ScriptingPulseNoReset::operator = (const ScriptingPulseNoReset &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_time = rhs._time;
		_in = rhs._in;
		_pulse_time = rhs._pulse_time;
        _event_in = rhs._event_in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingPulseNoReset::~ScriptingPulseNoReset (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingPulseNoReset::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingPulseNoReset::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_time, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_pulse_time, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingPulseNoReset::eventIn (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    if (_time <= 0.0F) {
        _event_in = true;
    }
}

//==============================================================================
//==============================================================================

void ScriptingPulseNoReset::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    // Check if staring
    if (_time > 0.0F || _in || _event_in) {
        _time += dt;
        _event_in = false;
    }
    
    // Check for reset
    if (_time > _pulse_time && !_in) {
        _time = 0.0F;
    }
    
    // If in is true and timer is expired
    if (_time > 0.0F && _time <= _pulse_time) {
        _out = true;
    } else {
        _out = false;
    }
}

//==============================================================================
//==============================================================================

void ScriptingPulseNoReset::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingPulseNoReset::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


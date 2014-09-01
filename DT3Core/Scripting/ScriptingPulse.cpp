//==============================================================================
///	
///	File: ScriptingPulse.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingPulse.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingPulse,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingPulse)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_EVENT_INFO_INDEX(_in_e)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingPulse)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
    EVENT_INIT(_in_e, "In_Event")
        .set_input(true)
        .set_event(&ScriptingPulse::eventIn);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingPulse::ScriptingPulse (void)
    :   _time		(0.0F),
		_in			(PLUG_INFO_INDEX(_in), false),
		_pulse_time	(1.0F),
		_out		(PLUG_INFO_INDEX(_out), false),
		_in_e		(EVENT_INFO_INDEX(_in_e))
{  

}
		
ScriptingPulse::ScriptingPulse (const ScriptingPulse &rhs)
    :   ScriptingBase	(rhs),
		_time			(rhs._time),
		_in				(rhs._in),
		_pulse_time		(rhs._pulse_time),
		_out			(rhs._out),
		_in_e           (EVENT_INFO_INDEX(_in_e))
{   

}

ScriptingPulse & ScriptingPulse::operator = (const ScriptingPulse &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_time = rhs._time;
		_in = rhs._in;
		_pulse_time = rhs._pulse_time;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingPulse::~ScriptingPulse (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingPulse::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingPulse::archive (const std::shared_ptr<Archive> &archive)
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

void ScriptingPulse::eventIn (PlugNode *sender)
{
    _in = true;
    _time = 0.0F;
}

//==============================================================================
//==============================================================================

void ScriptingPulse::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    _time += dt;
    
    // reset the timer if transitioned to true
    if (!(_in)) {
        _time = 0.0F;
    }
    
    // If in is true and timer is expired
    if (_in && _time > 0.0F && _time <= _pulse_time) {
        _out = true;
    } else {
        _out = false;
    }

}

//==============================================================================
//==============================================================================

void ScriptingPulse::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingPulse::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


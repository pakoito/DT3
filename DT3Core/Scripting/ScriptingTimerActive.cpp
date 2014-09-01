//==============================================================================
///	
///	File: ScriptingTimerActive.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTimerActive.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTimerActive,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTimerActive)

IMPLEMENT_PLUG_INFO_INDEX(_speed)		
IMPLEMENT_PLUG_INFO_INDEX(_count)
IMPLEMENT_PLUG_INFO_INDEX(_time)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTimerActive)

	PLUG_INIT(_speed,"Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_count,"Count")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_time,"Time")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTimerActive::ScriptingTimerActive (void)
    :   _speed		(PLUG_INFO_INDEX(_speed), 1.0F),
		_count      (PLUG_INFO_INDEX(_count), false),
		_time		(PLUG_INFO_INDEX(_time), 0.0F)
{  

}
		
ScriptingTimerActive::ScriptingTimerActive (const ScriptingTimerActive &rhs)
    :   ScriptingBase	(rhs),
		_speed			(rhs._speed),
		_count          (rhs._count),
		_time			(rhs._time)
{   

}

ScriptingTimerActive & ScriptingTimerActive::operator = (const ScriptingTimerActive &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_speed = rhs._speed;
		_count = rhs._count;
		_time = rhs._time;
	}
    return (*this);
}
			
ScriptingTimerActive::~ScriptingTimerActive (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTimerActive::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTimerActive::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_speed, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_count, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTimerActive::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat time = _time.value_without_compute();
    
    if (_count) {
        time += (dt) * (_speed);
    } else {
        time = 0.0F;
    }
    
    _time = time;
}

//==============================================================================
//==============================================================================

void ScriptingTimerActive::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingTimerActive::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


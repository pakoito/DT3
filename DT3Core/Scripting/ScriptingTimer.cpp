//==============================================================================
///	
///	File: ScriptingTimer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTimer.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTimer,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTimer)

IMPLEMENT_PLUG_INFO_INDEX(_speed)		
IMPLEMENT_PLUG_INFO_INDEX(_count_up)
IMPLEMENT_PLUG_INFO_INDEX(_count_down)
IMPLEMENT_PLUG_INFO_INDEX(_time)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTimer)

	PLUG_INIT(_speed,"Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_count_up,"Count_Up")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_count_down,"Count_Down")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_time,"Time")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTimer::ScriptingTimer (void)
    :   _speed          (PLUG_INFO_INDEX(_speed), 1.0F),
		_count_up       (PLUG_INFO_INDEX(_count_up), false),
		_count_down     (PLUG_INFO_INDEX(_count_down), false),
		_time           (PLUG_INFO_INDEX(_time), 0.0F)
{  

}
		
ScriptingTimer::ScriptingTimer (const ScriptingTimer &rhs)
    :   ScriptingBase	(rhs),
		_speed			(rhs._speed),
		_count_up		(rhs._count_up),
		_count_down		(rhs._count_down),
		_time			(rhs._time)
{   

}

ScriptingTimer & ScriptingTimer::operator = (const ScriptingTimer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_speed = rhs._speed;
		_count_up = rhs._count_up;
		_count_down = rhs._count_down;
		_time = rhs._time;
	}
    return (*this);
}
			
ScriptingTimer::~ScriptingTimer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTimer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTimer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_speed, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_count_up, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_count_down, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTimer::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat time = _time.value_without_compute();
    
    if ((_count_up) && !(_count_down)) {
        time += (dt) * (_speed);
    } else if (!(_count_up) && (_count_down)) {
        time -= (dt) * (_speed);		
    }
    
    _time = time;
}

//==============================================================================
//==============================================================================

void ScriptingTimer::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingTimer::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


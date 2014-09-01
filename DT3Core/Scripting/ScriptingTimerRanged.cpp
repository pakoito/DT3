//==============================================================================
///	
///	File: ScriptingTimerRanged.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTimerRanged.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTimerRanged,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTimerRanged)

IMPLEMENT_PLUG_INFO_INDEX(_lower_bound)
IMPLEMENT_PLUG_INFO_INDEX(_upper_bound)
IMPLEMENT_PLUG_INFO_INDEX(_speed)		
IMPLEMENT_PLUG_INFO_INDEX(_count_up)
IMPLEMENT_PLUG_INFO_INDEX(_count_down)
IMPLEMENT_PLUG_INFO_INDEX(_time)		
IMPLEMENT_PLUG_INFO_INDEX(_t)		
IMPLEMENT_PLUG_INFO_INDEX(_counting)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTimerRanged)

	PLUG_INIT(_lower_bound,"Lower_Range")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_counting))
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));
		
	PLUG_INIT(_upper_bound,"Upper_Range")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_counting))
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));
		
	PLUG_INIT(_speed,"Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_counting))
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_count_up,"Count_Up")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_counting))
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_count_down,"Count_Down")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_counting))
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_time,"Time")
		.set_input(true)
		.set_output(true);
		
	PLUG_INIT(_t,"t")
		.set_output(true);
		
	PLUG_INIT(_counting,"Counting")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTimerRanged::ScriptingTimerRanged (void)
    :   _lower_bound	(PLUG_INFO_INDEX(_lower_bound), 0.0F),
		_upper_bound	(PLUG_INFO_INDEX(_upper_bound), 10.0F),
		_speed			(PLUG_INFO_INDEX(_speed), 1.0F),
		_count_up		(PLUG_INFO_INDEX(_count_up), false),
		_count_down		(PLUG_INFO_INDEX(_count_down), false),
		_time			(PLUG_INFO_INDEX(_time), 0.0F),
		_t				(PLUG_INFO_INDEX(_t), 0.0F),
		_counting		(PLUG_INFO_INDEX(_counting), false)
{  

}
		
ScriptingTimerRanged::ScriptingTimerRanged (const ScriptingTimerRanged &rhs)
    :   ScriptingBase	(rhs),
		_lower_bound	(rhs._lower_bound),
		_upper_bound	(rhs._upper_bound),
		_speed			(rhs._speed),
		_count_up		(rhs._count_up),
		_count_down		(rhs._count_down),
		_time			(rhs._time),
		_t				(rhs._t),
		_counting		(rhs._counting)
{   

}

ScriptingTimerRanged & ScriptingTimerRanged::operator = (const ScriptingTimerRanged &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_lower_bound = rhs._lower_bound;
		_upper_bound = rhs._upper_bound;
		
		_speed = rhs._speed;
		_count_up = rhs._count_up;
		_count_down = rhs._count_down;
		_time = rhs._time;
		_t = rhs._t;
	}
    return (*this);
}
			
ScriptingTimerRanged::~ScriptingTimerRanged (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTimerRanged::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTimerRanged::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
    *archive << ARCHIVE_PLUG(_lower_bound, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_upper_bound, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_speed, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_count_up, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_count_down, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT);
    *archive << ARCHIVE_PLUG(_counting, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTimerRanged::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat time = _time.value_without_compute();
                                                        
    if ((_count_up) && !(_count_down)) {
        time += (dt) * (_speed);
    } if (!(_count_up) && (_count_down)) {
        time -= (dt) * (_speed);		
    } else {
        *_speed;
    }
    
    // Clamp time
    if (time > _upper_bound)
        time = _upper_bound;
    if (time < _lower_bound)
        time = _lower_bound;
    
    _t = (time - _lower_bound) / (_upper_bound - _lower_bound);
    _time = time;
    
    _t.set_clean();
    _time.set_clean();
    
    // set counting flag
    if (time < _upper_bound && time > _lower_bound && ((_count_up) || (_count_down))) {
        _counting = true;
    } else {
        _counting = false;
    }

}

//==============================================================================
//==============================================================================

void ScriptingTimerRanged::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingTimerRanged::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


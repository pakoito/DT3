//==============================================================================
///	
///	File: ScriptingTimerUpReset.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTimerUpReset.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTimerUpReset,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTimerUpReset)

IMPLEMENT_PLUG_INFO_INDEX(_speed)		
IMPLEMENT_PLUG_INFO_INDEX(_time)		
IMPLEMENT_PLUG_INFO_INDEX(_t)	
IMPLEMENT_PLUG_INFO_INDEX(_active)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTimerUpReset)

	PLUG_INIT(_speed,"Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_active,"Active")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));


	PLUG_INIT(_time,"Time")
		.set_output(true);
		
	PLUG_INIT(_t,"t")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTimerUpReset::ScriptingTimerUpReset (void)
    :	_upper_range            (10.0F),
		_speed                  (PLUG_INFO_INDEX(_speed), 1.0F),
		_active                 (PLUG_INFO_INDEX(_active), true),
		_time                   (PLUG_INFO_INDEX(_time), 0.0F),
		_t                      (PLUG_INFO_INDEX(_t), 0.0F),
        _reset_when_inactive    (false)
{  

}
		
ScriptingTimerUpReset::ScriptingTimerUpReset (const ScriptingTimerUpReset &rhs)
    :   ScriptingBase           (rhs),
		_upper_range            (rhs._upper_range),
		_speed                  (rhs._speed),
		_active                 (rhs._active),
		_time                   (rhs._time),
		_t                      (rhs._t),
        _reset_when_inactive    (rhs._reset_when_inactive)
{   

}

ScriptingTimerUpReset & ScriptingTimerUpReset::operator = (const ScriptingTimerUpReset &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_upper_range = rhs._upper_range;
		
		_speed = rhs._speed;
		_active = rhs._active;

		_time = rhs._time;
		_t = rhs._t;
        
        _reset_when_inactive = rhs._reset_when_inactive;
	}
    return (*this);
}
			
ScriptingTimerUpReset::~ScriptingTimerUpReset (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTimerUpReset::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTimerUpReset::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_upper_range, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_speed, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_active, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA(_reset_when_inactive, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTimerUpReset::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat time = _time.value_without_compute();	// Since we are evaluating _time, reading it the other
															// way would keep causing this function to be called
    if (_active) {
        time += (dt) * (_speed);
        
        // Clamp time
        while (time > _upper_range) {
            time -= _upper_range;
        }
        while (time < 0.0F) {
            time += _upper_range;
        }
        
        _t = time / _upper_range;
        _time = time;
    } else if (_reset_when_inactive) {
        _t = 0.0F;
        _time = 0.0F;
    }
}

//==============================================================================
//==============================================================================

void ScriptingTimerUpReset::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingTimerUpReset::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


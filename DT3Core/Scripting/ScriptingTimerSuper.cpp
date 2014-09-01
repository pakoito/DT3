//==============================================================================
///	
///	File: ScriptingTimerSuper.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTimerSuper.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTimerSuper,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTimerSuper)

IMPLEMENT_PLUG_INFO_INDEX(_when_activated)		
IMPLEMENT_PLUG_INFO_INDEX(_when_deactivated)		

IMPLEMENT_PLUG_INFO_INDEX(_when_reach_upper)		
IMPLEMENT_PLUG_INFO_INDEX(_when_reach_lower)		

IMPLEMENT_PLUG_INFO_INDEX(_upper_range)		
IMPLEMENT_PLUG_INFO_INDEX(_lower_range)		

IMPLEMENT_PLUG_INFO_INDEX(_counting_up_speed)
IMPLEMENT_PLUG_INFO_INDEX(_counting_down_speed)
IMPLEMENT_PLUG_INFO_INDEX(_current_speed)

IMPLEMENT_PLUG_INFO_INDEX(_last_active)
IMPLEMENT_PLUG_INFO_INDEX(_active)

IMPLEMENT_PLUG_INFO_INDEX(_time)		
IMPLEMENT_PLUG_INFO_INDEX(_t)

IMPLEMENT_EVENT_INFO_INDEX(_upper_reached)
IMPLEMENT_EVENT_INFO_INDEX(_lower_reached)

IMPLEMENT_PLUG_INFO_INDEX(_counting_down)
IMPLEMENT_PLUG_INFO_INDEX(_counting_up)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTimerSuper)

	PLUG_INIT(_when_activated,"When_Activated")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_when_deactivated,"When_Deactivated")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_when_reach_upper,"When_Reach_Upper")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_when_reach_lower,"When_Reach_Lower")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_upper_range,"Upper_Range")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_lower_range,"Lower_Range")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_counting_up_speed,"Counting_Up_Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_counting_down_speed,"Counting_Down_Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_current_speed,"Current_Speed")
		.set_input(true)
		.set_output(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_active,"Active")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_last_active,"Last_Active")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time))
        .affects(PLUG_INFO_INDEX(_t));

	PLUG_INIT(_time,"Time")
		.set_output(true);

	PLUG_INIT(_t,"t")
		.set_output(true);
    
    EVENT_INIT(_upper_reached,"Upper_Reached")
        .set_output(true);
    
    EVENT_INIT(_lower_reached,"Lower_Reached")
        .set_output(true);

	PLUG_INIT(_counting_down,"Counting_Down")
		.set_output(true);

	PLUG_INIT(_counting_up,"Counting_Up")
		.set_output(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTimerSuper::ScriptingTimerSuper (void)
    :   _when_activated		(PLUG_INFO_INDEX(_when_activated), STATE_CHANGE_BEHAVIOUR_COUNT_UP),
        _when_deactivated	(PLUG_INFO_INDEX(_when_deactivated), STATE_CHANGE_BEHAVIOUR_COUNT_STOP),
        _when_reach_upper   (PLUG_INFO_INDEX(_when_reach_upper), STATE_RANGE_STOP),
        _when_reach_lower	(PLUG_INFO_INDEX(_when_reach_lower), STATE_RANGE_STOP),
        _upper_range        (PLUG_INFO_INDEX(_upper_range), 1.0F),
        _lower_range        (PLUG_INFO_INDEX(_lower_range), 0.0F),
        _counting_up_speed  (PLUG_INFO_INDEX(_counting_up_speed), 1.0F),
        _counting_down_speed(PLUG_INFO_INDEX(_counting_down_speed), 1.0F),
        _current_speed      (PLUG_INFO_INDEX(_current_speed), 0.0F),
        _active             (PLUG_INFO_INDEX(_active), false),
        _last_active        (PLUG_INFO_INDEX(_last_active), false),
		_time               (PLUG_INFO_INDEX(_time), 0.0F),
		_t                  (PLUG_INFO_INDEX(_t), 0.0F),
        _counting_down      (PLUG_INFO_INDEX(_counting_down), false),
        _counting_up        (PLUG_INFO_INDEX(_counting_up), false),
        _upper_reached      (EVENT_INFO_INDEX(_upper_reached)),
        _lower_reached      (EVENT_INFO_INDEX(_lower_reached))
{  

}
		
ScriptingTimerSuper::ScriptingTimerSuper (const ScriptingTimerSuper &rhs)
    :   ScriptingBase       (rhs),
		_when_activated     (rhs._when_activated),
        _when_deactivated	(rhs._when_deactivated),
        _when_reach_upper   (rhs._when_reach_upper),
        _when_reach_lower	(rhs._when_reach_lower),
        _upper_range        (rhs._upper_range),
        _lower_range        (rhs._lower_range),
        _counting_up_speed  (rhs._counting_up_speed),
        _counting_down_speed(rhs._counting_down_speed),
        _current_speed      (rhs._current_speed),
        _active             (rhs._active),
        _last_active        (rhs._last_active),
		_time               (rhs._time),
		_t                  (rhs._t),
        _counting_down      (rhs._counting_down),
        _counting_up        (rhs._counting_up),
        _upper_reached      (rhs._upper_reached),
        _lower_reached      (rhs._lower_reached)
{   

}

ScriptingTimerSuper & ScriptingTimerSuper::operator = (const ScriptingTimerSuper &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_when_activated     = rhs._when_activated;
        _when_deactivated	= rhs._when_deactivated;
        _when_reach_upper   = rhs._when_reach_upper;
        _when_reach_lower	= rhs._when_reach_lower;
        _upper_range        = rhs._upper_range;
        _lower_range        = rhs._lower_range;
        _counting_up_speed  = rhs._counting_up_speed;
        _counting_down_speed= rhs._counting_down_speed;
        _current_speed      = rhs._current_speed;
        _active             = rhs._active;
        _last_active        = rhs._last_active;
		_time               = rhs._time;
		_t                  = rhs._t;
        _counting_down      = rhs._counting_down;
        _counting_up        = rhs._counting_up;
	}
    return (*this);
}
			
ScriptingTimerSuper::~ScriptingTimerSuper (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTimerSuper::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTimerSuper::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

    archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_when_activated, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Count Up")
        .add_enum("Count Up From Lower")
        .add_enum("Count Down")
        .add_enum("Count Down From Upper")
        .add_enum("Stop")
        .add_enum("Stop and Reset To Lower")
        .add_enum("Stop and Reset To Upper");

	*archive << ARCHIVE_PLUG(_when_deactivated, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Count Up")
        .add_enum("Count Up From Lower")
        .add_enum("Count Down")
        .add_enum("Count Down From Upper")
        .add_enum("Stop")
        .add_enum("Stop and Reset To Lower")
        .add_enum("Stop and Reset To Upper");

	*archive << ARCHIVE_PLUG(_when_reach_upper, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Continue")
        .add_enum("Stop")
        .add_enum("Reverse")
        .add_enum("Wrap");
    
	*archive << ARCHIVE_PLUG(_when_reach_lower, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Continue")
        .add_enum("Stop")
        .add_enum("Reverse")
        .add_enum("Wrap");

	*archive << ARCHIVE_PLUG(_upper_range, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_lower_range, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_counting_up_speed, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_counting_down_speed, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_current_speed, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_active, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_last_active, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT | DATA_SETTABLE);
	
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTimerSuper::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat time = _time.value_without_compute();
    
    if (_active != _last_active) {
    
        DTint action = _active ? _when_activated : _when_deactivated;
    
        switch(action) {
        
            case STATE_CHANGE_BEHAVIOUR_COUNT_UP:
                _current_speed = _counting_up_speed;
                break;
                
            case STATE_CHANGE_BEHAVIOUR_COUNT_UP_FROM_LOWER:
                time = _lower_range;
                _current_speed = _counting_up_speed;
                break;
                
            case STATE_CHANGE_BEHAVIOUR_COUNT_DOWN:
                _current_speed = -_counting_down_speed;
                break;
                
            case STATE_CHANGE_BEHAVIOUR_COUNT_DOWN_FROM_UPPER:
                time = _upper_range;
                _current_speed = -_counting_down_speed;
                break;
                
            case STATE_CHANGE_BEHAVIOUR_COUNT_STOP:
                _current_speed = 0.0F;
                break;
                
            case STATE_CHANGE_BEHAVIOUR_COUNT_STOP_RESET_LOWER:
                time = _lower_range;
                _current_speed = 0.0F;
                break;
            case STATE_CHANGE_BEHAVIOUR_COUNT_STOP_RESET_UPPER:
                time = _upper_range;
                _current_speed = 0.0F;
                break;
        };
            
        _last_active = _active;
    }
    
    time += _current_speed * dt;
    
    if (_current_speed >= 0.0F && time >= _upper_range) {
        
        _upper_reached.send(this);

        switch (_when_reach_upper) {
            case STATE_RANGE_CONTINUE:
                // Do nothing
                break;
            case STATE_RANGE_STOP:
                _current_speed = 0.0F;
                time = _upper_range;
                break;
            case STATE_RANGE_REVERSE:
                time = _upper_range - (time - _upper_range);
                _current_speed = _counting_down_speed;
                break;
            case STATE_RANGE_WRAP:
                time = time - (_upper_range - _lower_range);
                break;
        };

    } 

    if (_current_speed <= 0.0F && time <= _lower_range) {
    
        _lower_reached.send(this);
        
        switch (_when_reach_lower) {
            case STATE_RANGE_CONTINUE:
                // Do nothing
                break;
            case STATE_RANGE_STOP:
                _current_speed = 0.0F;
                time = _lower_range;
                break;
            case STATE_RANGE_REVERSE:
                time = _lower_range + (_lower_range - time);
                _current_speed = _counting_up_speed;
                break;
            case STATE_RANGE_WRAP:
                time = time + (_upper_range - _lower_range);
                break;
        };

    }
    
    _counting_down = (_current_speed < 0.0F);
    _counting_up = (_current_speed > 0.0F);
    
    _t = (_time - _lower_range) / (_upper_range - _lower_range);
    _time = time;
}

//==============================================================================
//==============================================================================

void ScriptingTimerSuper::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingTimerSuper::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


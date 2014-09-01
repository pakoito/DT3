//==============================================================================
///	
///	File: ScriptingTimerUpDown.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTimerUpDown.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTimerUpDown,"Timers",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTimerUpDown)

IMPLEMENT_PLUG_INFO_INDEX(_speed)		
IMPLEMENT_PLUG_INFO_INDEX(_time)		
IMPLEMENT_PLUG_INFO_INDEX(_t)	
IMPLEMENT_PLUG_INFO_INDEX(_active	)

IMPLEMENT_PLUG_INFO_INDEX(_counting_up)
IMPLEMENT_PLUG_INFO_INDEX(_counting_down)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTimerUpDown)

	PLUG_INIT(_speed,"Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_counting_up))
		.affects(PLUG_INFO_INDEX(_counting_down))
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));

	PLUG_INIT(_active,"Active")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_counting_up))
		.affects(PLUG_INFO_INDEX(_counting_down))
		.affects(PLUG_INFO_INDEX(_t))
		.affects(PLUG_INFO_INDEX(_time));


	PLUG_INIT(_time,"Time")
		.set_output(true);
		
	PLUG_INIT(_t,"t")
		.set_output(true);
		
	PLUG_INIT(_counting_up,"Counting_Up")
		.set_output(true);

	PLUG_INIT(_counting_down,"Counting_Down")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTimerUpDown::ScriptingTimerUpDown (void)
    :   _lower_range	(0.0F),
		_upper_range	(10.0F),
		_speed			(PLUG_INFO_INDEX(_speed), 1.0F),
		_active			(PLUG_INFO_INDEX(_active), true),
		_time			(PLUG_INFO_INDEX(_time), 0.0F),
		_t				(PLUG_INFO_INDEX(_t), 0.0F),
		_counting_up	(PLUG_INFO_INDEX(_counting_up), false),
		_counting_down	(PLUG_INFO_INDEX(_counting_down), false),
		_is_counting_up	(true)
{  

}
		
ScriptingTimerUpDown::ScriptingTimerUpDown (const ScriptingTimerUpDown &rhs)
    :   ScriptingBase	(rhs),
		_lower_range	(rhs._lower_range),
		_upper_range	(rhs._upper_range),
		_speed			(rhs._speed),
		_active			(rhs._active),
		_time			(rhs._time),
		_t				(rhs._t),
		_counting_up	(rhs._counting_up),
		_counting_down	(rhs._counting_down),
		_is_counting_up	(rhs._is_counting_up)
{   

}

ScriptingTimerUpDown & ScriptingTimerUpDown::operator = (const ScriptingTimerUpDown &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_lower_range = rhs._lower_range;
		_upper_range = rhs._upper_range;
		
		_speed = rhs._speed;
		_active = rhs._active;

		_time = rhs._time;
		_t = rhs._t;

		_counting_up = rhs._counting_up;
		_counting_down = rhs._counting_down;
		
		_is_counting_up = rhs._is_counting_up;
	}
    return (*this);
}
			
ScriptingTimerUpDown::~ScriptingTimerUpDown (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTimerUpDown::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTimerUpDown::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_lower_range, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_upper_range, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_speed, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_active, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_counting_up, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_counting_down, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_is_counting_up, DATA_PERSISTENT);
				
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTimerUpDown::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat time = _time.value_without_compute();	// Since we are evaluating _time, reading it the other
                                                        // way would keep causing this function to be called
    
    if (_active) {
        if (_is_counting_up)	time += (dt) * (_speed);
        else					time -= (dt) * (_speed);
        
        // Clamp time
        if (time >= _upper_range) {
            time = _upper_range;
            _is_counting_up = false;
        }
        if (time <= _lower_range) {
            time = _lower_range;
            _is_counting_up = true;
        }
        
        _t = (time - _lower_range) / (_upper_range - _lower_range);
        _time = time;
        
        
        _counting_up = _is_counting_up;
        _counting_down = !_is_counting_up;
    } else {
        _counting_up = false;
        _counting_down = false;
    }

}

//==============================================================================
//==============================================================================

void ScriptingTimerUpDown::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingTimerUpDown::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


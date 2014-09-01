//==============================================================================
///	
///	File: ScriptingAccelerometer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingAccelerometer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Devices/DeviceInput.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingAccelerometer,"Input",NULL)
IMPLEMENT_PLUG_NODE(ScriptingAccelerometer)

IMPLEMENT_PLUG_INFO_INDEX(_enable)
IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_PLUG_INFO_INDEX(_out_low_freq)
IMPLEMENT_PLUG_INFO_INDEX(_out_high_freq)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingAccelerometer)
		
	PLUG_INIT(_enable,"Enable")
		.set_input(true);

	PLUG_INIT(_out,"Out")
		.set_output(true);

	PLUG_INIT(_out_low_freq,"Low_Pass")
		.set_output(true);

	PLUG_INIT(_out_high_freq,"High_Pass")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingAccelerometer::ScriptingAccelerometer (void)
    :	_enable                 (PLUG_INFO_INDEX(_enable), true),
		_out                    (PLUG_INFO_INDEX(_out), Vector3(0.0F,-1.0F,0.0F)),
		_out_low_freq           (PLUG_INFO_INDEX(_out_low_freq), Vector3(0.0F,-1.0F,0.0F)),
		_out_high_freq          (PLUG_INFO_INDEX(_out_high_freq), Vector3(0.0F,0.0F,0.0F)),
        _warmup_time            (3.0F),
        _current_warmup_time    (0.0F)
{  

}
		
ScriptingAccelerometer::ScriptingAccelerometer (const ScriptingAccelerometer &rhs)
    :   ScriptingBase           (rhs),
        _enable                 (rhs._enable),
		_out                    (rhs._out),
		_out_low_freq           (rhs._out_low_freq),
		_out_high_freq          (rhs._out_high_freq),
        _warmup_time            (rhs._warmup_time),
        _current_warmup_time    (rhs._current_warmup_time)
{   

}

ScriptingAccelerometer & ScriptingAccelerometer::operator = (const ScriptingAccelerometer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _enable = rhs._enable;
        
		_out = rhs._out;
		_out_low_freq = rhs._out_low_freq;
		_out_high_freq = rhs._out_high_freq;
        
        _warmup_time = rhs._warmup_time;
        _current_warmup_time = rhs._current_warmup_time;
	}
    return (*this);
}
			
ScriptingAccelerometer::~ScriptingAccelerometer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingAccelerometer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingAccelerometer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_PLUG(_enable, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out_low_freq, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out_high_freq, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_warmup_time, DATA_PERSISTENT | DATA_SETTABLE);
	   
    archive->pop_domain ();
}

void ScriptingAccelerometer::archive_done   (const std::shared_ptr<Archive> &archive)
{		
	ScriptingBase::archive_done (archive);
    
    if (archive->is_writing())
        return;

    _current_warmup_time = 0.0F;
}

//==============================================================================
//==============================================================================

#if DT3_OS == DT3_MACOSX || DT3_OS == DT3_WINDOWS

void ScriptingAccelerometer::key (DTuint modifiers, DTushort key)
{
	PROFILER(SCRIPTING);

    if (key == DeviceInput::INPUT_KP4)   accelerate (Vector3(-1.0F,0.0F,0.0F));
    if (key == DeviceInput::INPUT_KP6)   accelerate (Vector3(1.0F,0.0F,0.0F));
    if (key == DeviceInput::INPUT_KP8)   accelerate (Vector3(0.0F,1.0F,0.0F));
    if (key == DeviceInput::INPUT_KP2)   accelerate (Vector3(0.0F,-1.0F,0.0F));
}

#endif

//==============================================================================
//==============================================================================

void ScriptingAccelerometer::accelerate (const Vector3 &a)
{
	PROFILER(SCRIPTING);
    
    Vector3 out, out_low_freq, out_high_freq;
    
    if (_enable) {
        const DTfloat FILTER_FACTOR = 0.1F;

        out = a;
        out_low_freq = (a * FILTER_FACTOR) + (_out_low_freq * (1.0F - FILTER_FACTOR));
        out_high_freq = a - _out_low_freq;
    }
    
    // Ease into values
    if (_current_warmup_time < _warmup_time) {
                
        DTfloat t = _current_warmup_time / _warmup_time;
        
        _out = t * _out + (1.0F - t) * Vector3(0.0F,-1.0F,0.0F);
        _out_low_freq = t * _out_low_freq + (1.0F - t) * Vector3(0.0F,-1.0F,0.0F);
        _out_high_freq = t * _out_high_freq + (1.0F - t) * Vector3(0.0F,0.0F,0.0F);
        
    } else {
        _out = out;
        _out_low_freq = out_low_freq;
        _out_high_freq = out_high_freq;
    }
    
}

void ScriptingAccelerometer::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    _current_warmup_time += dt;
}

//==============================================================================
//==============================================================================

void ScriptingAccelerometer::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    SystemCallbacks::acceleration_cb().add(make_callback(this, &type::accelerate));
    world->register_for_tick(this, make_callback(this, &type::tick));
#if DT3_OS == DT3_MACOSX || DT3_OS == DT3_WINDOWS
    SystemCallbacks::key_down_event_cb().add(make_callback(this, &type::key));
#endif
}

void ScriptingAccelerometer::remove_from_world (void)
{
    SystemCallbacks::acceleration_cb().remove(make_callback(this, &type::accelerate));
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

#if DT3_OS == DT3_MACOSX || DT3_OS == DT3_WINDOWS
    SystemCallbacks::key_down_event_cb().remove(make_callback(this, &type::key));
#endif

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


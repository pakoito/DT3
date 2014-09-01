//==============================================================================
///	
///	File: ScriptingGyro.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingGyro.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingGyro,"Input",NULL)
IMPLEMENT_PLUG_NODE(ScriptingGyro)

IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_PLUG_INFO_INDEX(_out_low_freq)
IMPLEMENT_PLUG_INFO_INDEX(_out_high_freq)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingGyro)
		
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

ScriptingGyro::ScriptingGyro (void)
    :	_out                    (PLUG_INFO_INDEX(_out), Vector3(0.0F,-1.0F,0.0F)),
		_out_low_freq           (PLUG_INFO_INDEX(_out_low_freq), Vector3(0.0F,-1.0F,0.0F)),
		_out_high_freq          (PLUG_INFO_INDEX(_out_high_freq), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingGyro::ScriptingGyro (const ScriptingGyro &rhs)
    :   ScriptingBase           (rhs),
		_out                    (rhs._out),
		_out_low_freq           (rhs._out_low_freq),
		_out_high_freq          (rhs._out_high_freq)
{   

}

ScriptingGyro & ScriptingGyro::operator = (const ScriptingGyro &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_out = rhs._out;
		_out_low_freq = rhs._out_low_freq;
		_out_high_freq = rhs._out_high_freq;
	}
    return (*this);
}
			
ScriptingGyro::~ScriptingGyro (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingGyro::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingGyro::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out_low_freq, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out_high_freq, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingGyro::accelerate (const Vector3 &a)
{
	PROFILER(SCRIPTING);
    
    const DTfloat FILTER_FACTOR = 0.1F;

    _out = a;
	_out_low_freq = (a * FILTER_FACTOR) + (_out_low_freq * (1.0F - FILTER_FACTOR));
    _out_high_freq = a - _out_low_freq;
}

//==============================================================================
//==============================================================================

void ScriptingGyro::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    SystemCallbacks::gyro_cb().add(make_callback(this, &type::accelerate));
}

void ScriptingGyro::remove_from_world (void)
{
    SystemCallbacks::gyro_cb().remove(make_callback(this, &type::accelerate));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


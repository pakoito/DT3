//==============================================================================
///	
///	File: ScriptingVector3Smooth.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Smooth.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Smooth,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Smooth)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_PLUG_INFO_INDEX(_reset)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Smooth)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_reset,"reset")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Smooth::ScriptingVector3Smooth (void)
    :   _in				(PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F)),
		_reset			(PLUG_INFO_INDEX(_reset), false),
		_local_reset	(true),
		_history_index	(0),
		_sum			(0.0F,0.0F,0.0F)
{  

}
		
ScriptingVector3Smooth::ScriptingVector3Smooth (const ScriptingVector3Smooth &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_out			(rhs._out),
		_reset			(rhs._reset),
		_local_reset	(rhs._local_reset),
		_history        (rhs._history),
		_history_index	(rhs._history_index),
		_sum			(rhs._sum)
{   
	
}

ScriptingVector3Smooth & ScriptingVector3Smooth::operator = (const ScriptingVector3Smooth &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_out = rhs._out;
		_reset = rhs._reset;
		_local_reset = rhs._local_reset;
		_history = rhs._history;
		_history_index = rhs._history_index;
		_sum = rhs._sum;
    }
    return (*this);
}
			
ScriptingVector3Smooth::~ScriptingVector3Smooth (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Smooth::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Smooth::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_reset, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA_ACCESSORS("History_Size", ScriptingVector3Smooth::getHistorySize, ScriptingVector3Smooth::setHistorySize, DATA_PERSISTENT | DATA_SETTABLE);
											     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Smooth::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (_history.size() == 0) {
        _out = _in;
        
    } else {
        // If resetting
        if (_reset || _local_reset) {
            _sum = Vector3(0.0F,0.0F,0.0F);
            for (DTuint i = 0; i < _history.size(); ++i) {
                _sum += _in;
                _history[i] = _in;
            }
                
            _out = _sum / static_cast<DTfloat>(_history.size());
        } else {
            // Bubble down
            _sum = _sum - _history[_history_index];
            _history[_history_index] = _in;
            _sum = _sum + _history[_history_index];

            _history_index = (_history_index + 1) % _history.size();
            
            _out = _sum / static_cast<DTfloat>(_history.size());
        }
    }

    _local_reset = false;
}

//==============================================================================
//==============================================================================

void ScriptingVector3Smooth::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingVector3Smooth::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


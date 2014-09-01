//==============================================================================
///	
///	File: ScriptingCounter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingCounter.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingCounter,"Counters",NULL)
IMPLEMENT_PLUG_NODE(ScriptingCounter)

IMPLEMENT_PLUG_INFO_INDEX(_min)
IMPLEMENT_PLUG_INFO_INDEX(_max)
IMPLEMENT_PLUG_INFO_INDEX(_wrap)
IMPLEMENT_PLUG_INFO_INDEX(_out)

IMPLEMENT_EVENT_INFO_INDEX(_incr_e)
IMPLEMENT_EVENT_INFO_INDEX(_decr_e)
IMPLEMENT_EVENT_INFO_INDEX(_reset_e)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingCounter)

	PLUG_INIT(_min,"min")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_max,"max")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_wrap,"wrap")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
        
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
        
	EVENT_INIT(_incr_e,"Incr")
        .set_input(true)
        .set_event(&ScriptingCounter::event_incr);

	EVENT_INIT(_decr_e,"Decr")
        .set_input(true)
        .set_event(&ScriptingCounter::event_decr);

	EVENT_INIT(_reset_e,"Reset")
        .set_input(true)
        .set_event(&ScriptingCounter::event_reset);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingCounter::ScriptingCounter (void)
    :   _min			(PLUG_INFO_INDEX(_min), 0),
		_max			(PLUG_INFO_INDEX(_max), 3),
		_wrap			(PLUG_INFO_INDEX(_wrap), 1),
		_out            (PLUG_INFO_INDEX(_out), 0),
        _incr_e         (EVENT_INFO_INDEX(_incr_e)),
        _decr_e         (EVENT_INFO_INDEX(_decr_e)),
        _reset_e        (EVENT_INFO_INDEX(_reset_e))
{  

}
		
ScriptingCounter::ScriptingCounter (const ScriptingCounter &rhs)
    :   ScriptingBase	(rhs),
		_min			(rhs._min),
		_max			(rhs._max),
		_wrap			(rhs._wrap),
		_out            (rhs._out),
        _incr_e         (rhs._incr_e),
        _decr_e         (rhs._decr_e),
        _reset_e        (rhs._reset_e)
{   

}

ScriptingCounter & ScriptingCounter::operator = (const ScriptingCounter &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_min = rhs._min;
		_max = rhs._max;
		_wrap = rhs._wrap;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingCounter::~ScriptingCounter (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingCounter::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingCounter::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_min, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_max, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_wrap, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_EVENT(_incr_e, DATA_SETTABLE);
	*archive << ARCHIVE_EVENT(_decr_e, DATA_SETTABLE);
	*archive << ARCHIVE_EVENT(_reset_e, DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingCounter::event_incr (PlugNode *sender)
{
	PROFILER(SCRIPTING);
    _out = _out + 1;
    
    if (_out > _max) {
        if (_wrap)  _out = _min;
        else        _out = _max;
    }
    
}

void ScriptingCounter::event_decr (PlugNode *sender)
{
	PROFILER(SCRIPTING);
    _out = _out - 1;
    
    if (_out < _min) {
        if (_wrap)  _out = _max;
        else        _out = _min;
    }
}

void ScriptingCounter::event_reset (PlugNode *sender)
{   
    PROFILER(SCRIPTING);
    _out = 0;
}

//==============================================================================
//==============================================================================

DTboolean ScriptingCounter::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		
		_out.set_clean();		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


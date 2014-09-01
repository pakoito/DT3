//==============================================================================
///	
///	File: ScriptingFLIPFLOP.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingFLIPFLOP.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingFLIPFLOP,"Logic",NULL)
IMPLEMENT_PLUG_NODE(ScriptingFLIPFLOP)

IMPLEMENT_PLUG_INFO_INDEX(_set)
IMPLEMENT_PLUG_INFO_INDEX(_reset)
IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_PLUG_INFO_INDEX(_out_inv)

IMPLEMENT_EVENT_INFO_INDEX(_set_e)
IMPLEMENT_EVENT_INFO_INDEX(_reset_e)
IMPLEMENT_EVENT_INFO_INDEX(_toggle_e)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingFLIPFLOP)

	PLUG_INIT(_set,"set")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_reset,"reset")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_out,"Out")
		.set_output(true);
		
	PLUG_INIT(_out_inv,"Out_Inv")
		.set_output(true);
        
	EVENT_INIT(_set_e,"Set_Event")
        .set_input(true)
        .set_event(&ScriptingFLIPFLOP::event_set);

	EVENT_INIT(_reset_e,"Reset_Event")
        .set_input(true)
        .set_event(&ScriptingFLIPFLOP::event_reset);


	EVENT_INIT(_toggle_e,"Toggle_Event")
        .set_input(true)
        .set_event(&ScriptingFLIPFLOP::event_toggle);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingFLIPFLOP::ScriptingFLIPFLOP (void)
    :   _set			(PLUG_INFO_INDEX(_set), false),
		_reset			(PLUG_INFO_INDEX(_reset), false),
		_out			(PLUG_INFO_INDEX(_out), false),
		_out_inv		(PLUG_INFO_INDEX(_out_inv), false),
        _set_e          (EVENT_INFO_INDEX(_set_e)),
        _reset_e        (EVENT_INFO_INDEX(_reset_e)),
        _toggle_e       (EVENT_INFO_INDEX(_toggle_e))
{  

}
		
ScriptingFLIPFLOP::ScriptingFLIPFLOP (const ScriptingFLIPFLOP &rhs)
    :   ScriptingBase	(rhs),
		_set			(rhs._set),
		_reset			(rhs._reset),
		_out			(rhs._out),
		_out_inv		(rhs._out_inv),
        _set_e          (rhs._set_e),
        _reset_e        (rhs._reset_e),
        _toggle_e       (rhs._toggle_e)
{   

}

ScriptingFLIPFLOP & ScriptingFLIPFLOP::operator = (const ScriptingFLIPFLOP &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_set = rhs._set;
		_reset = rhs._reset;
		_out = rhs._out;
		_out_inv = rhs._out_inv;
	}
    return (*this);
}
			
ScriptingFLIPFLOP::~ScriptingFLIPFLOP (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingFLIPFLOP::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingFLIPFLOP::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_set, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_reset, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_inv, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_EVENT(_set_e, DATA_SETTABLE);
	*archive << ARCHIVE_EVENT(_reset_e, DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingFLIPFLOP::event_set (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    _out = (!_reset);
    _out_inv = !(_out.value_without_compute());
}

void ScriptingFLIPFLOP::event_reset (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    _out = false;
    _out_inv = true;
}

void ScriptingFLIPFLOP::event_toggle (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    _out = !_out;
    _out_inv = !_out_inv;
}

//==============================================================================
//==============================================================================

DTboolean ScriptingFLIPFLOP::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out || plug == &_out_inv) {
		
		_out = (_out.value_without_compute() || _set) && (!_reset);
		_out_inv = !(_out.value_without_compute());
		_out.set_clean();
		_out_inv.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ScriptingKeyDown.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyDown.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Devices/DeviceInput.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingKeyDown,"Input",NULL)
IMPLEMENT_PLUG_NODE(ScriptingKeyDown)

IMPLEMENT_PLUG_INFO_INDEX(_keys)
IMPLEMENT_PLUG_INFO_INDEX(_left_shift)
IMPLEMENT_PLUG_INFO_INDEX(_right_shift)
IMPLEMENT_PLUG_INFO_INDEX(_left_ctrl)
IMPLEMENT_PLUG_INFO_INDEX(_right_ctrl)
IMPLEMENT_PLUG_INFO_INDEX(_left_alt)
IMPLEMENT_PLUG_INFO_INDEX(_right_alt)
IMPLEMENT_PLUG_INFO_INDEX(_left_meta)
IMPLEMENT_PLUG_INFO_INDEX(_right_meta)
IMPLEMENT_PLUG_INFO_INDEX(_caps_meta)
IMPLEMENT_PLUG_INFO_INDEX(_out)
	
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingKeyDown)

	PLUG_INIT(_keys,"Keys")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_left_shift))
		.affects(PLUG_INFO_INDEX(_right_shift))
		.affects(PLUG_INFO_INDEX(_left_ctrl))
		.affects(PLUG_INFO_INDEX(_right_ctrl))
		.affects(PLUG_INFO_INDEX(_left_alt))
		.affects(PLUG_INFO_INDEX(_right_alt))
		.affects(PLUG_INFO_INDEX(_left_meta))
		.affects(PLUG_INFO_INDEX(_right_meta))
		.affects(PLUG_INFO_INDEX(_caps_meta))
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);

	PLUG_INIT(_left_shift,"Left_Shift")
		.set_output(true);

	PLUG_INIT(_right_shift,"Right_Shift")
		.set_output(true);

	PLUG_INIT(_left_ctrl,"Left_Ctrl")
		.set_output(true);

	PLUG_INIT(_right_ctrl,"Right_Ctrl")
		.set_output(true);

	PLUG_INIT(_left_alt,"Left_Alt")
		.set_output(true);

	PLUG_INIT(_right_alt,"Right_Alt")
		.set_output(true);

	PLUG_INIT(_left_meta,"Left_Meta")
		.set_output(true);

	PLUG_INIT(_right_meta,"Right_Meta")
		.set_output(true);

	PLUG_INIT(_caps_meta,"Caps_Lock")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingKeyDown::ScriptingKeyDown (void)
    :	_keys			(PLUG_INFO_INDEX(_keys)),
		_left_shift		(PLUG_INFO_INDEX(_left_shift), false),
		_right_shift	(PLUG_INFO_INDEX(_right_shift), false),
		_left_ctrl		(PLUG_INFO_INDEX(_left_ctrl), false),
		_right_ctrl		(PLUG_INFO_INDEX(_right_ctrl), false),
		_left_alt		(PLUG_INFO_INDEX(_left_alt), false),
		_right_alt		(PLUG_INFO_INDEX(_right_alt), false),
		_left_meta		(PLUG_INFO_INDEX(_left_meta), false),
		_right_meta		(PLUG_INFO_INDEX(_right_meta), false),
		_caps_meta		(PLUG_INFO_INDEX(_caps_meta), false),
		_out			(PLUG_INFO_INDEX(_out), false)
{  

}
		
ScriptingKeyDown::ScriptingKeyDown (const ScriptingKeyDown &rhs)
    :   ScriptingBase	(rhs),
		_keys			(rhs._keys),
		_left_shift		(rhs._left_shift),
		_right_shift	(rhs._right_shift),
		_left_ctrl		(rhs._left_ctrl),
		_right_ctrl		(rhs._right_ctrl),
		_left_alt		(rhs._left_alt),
		_right_alt		(rhs._right_alt),
		_left_meta		(rhs._left_meta),
		_right_meta		(rhs._right_meta),
		_caps_meta		(rhs._caps_meta),
		_out			(rhs._out)
{   

}

ScriptingKeyDown & ScriptingKeyDown::operator = (const ScriptingKeyDown &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_left_shift = rhs._left_shift;
		_right_shift = rhs._right_shift;
		_left_ctrl = rhs._left_ctrl;
		_right_ctrl = rhs._right_ctrl;
		_left_alt = rhs._left_alt;
		_right_alt = rhs._right_alt;
		_left_meta = rhs._left_meta;
		_right_meta = rhs._right_meta;
		_caps_meta = rhs._caps_meta;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingKeyDown::~ScriptingKeyDown (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingKeyDown::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingKeyDown::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_keys, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingKeyDown::key (DTuint modifiers, DTushort key)
{
	PROFILER(SCRIPTING);

    _out = (_keys->find( static_cast<DTcharacter>(key) ) != std::string::npos);
    
    _left_shift =   modifiers & DeviceInput::MOD_LSHIFT;
    _right_shift =  modifiers & DeviceInput::MOD_RSHIFT;
    _left_ctrl =    modifiers & DeviceInput::MOD_LCTRL;
    _right_ctrl =   modifiers & DeviceInput::MOD_RCTRL;
    _left_alt =     modifiers & DeviceInput::MOD_LALT;
    _right_alt =    modifiers & DeviceInput::MOD_RALT;
    _left_meta =    modifiers & DeviceInput::MOD_LMETA;
    _right_meta =   modifiers & DeviceInput::MOD_RMETA;
    _caps_meta =    modifiers & DeviceInput::MOD_CAPS;
}

//==============================================================================
//==============================================================================

void ScriptingKeyDown::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    SystemCallbacks::key_down_event_cb().add(make_callback(this, &ScriptingKeyDown::key));
}

void ScriptingKeyDown::remove_from_world (void)
{
    SystemCallbacks::key_down_event_cb().remove(make_callback(this, &ScriptingKeyDown::key));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


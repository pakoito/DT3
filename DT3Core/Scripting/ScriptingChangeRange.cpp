//==============================================================================
///	
///	File: ScriptingChangeRange.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingChangeRange.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingChangeRange,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingChangeRange)

IMPLEMENT_PLUG_INFO_INDEX(_in_low);
IMPLEMENT_PLUG_INFO_INDEX(_in_high);
IMPLEMENT_PLUG_INFO_INDEX(_out_low);
IMPLEMENT_PLUG_INFO_INDEX(_out_high);
IMPLEMENT_PLUG_INFO_INDEX(_in);
IMPLEMENT_PLUG_INFO_INDEX(_out);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingChangeRange)

	PLUG_INIT(_in_low,"In_Low")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_high,"In_High")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_out_low,"Out_Low")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out_high,"Out_High")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingChangeRange::ScriptingChangeRange (void)
    :   _in_low			(PLUG_INFO_INDEX(_in_low), 0.0F),
		_in_high		(PLUG_INFO_INDEX(_in_high), 0.0F),
		_out_low		(PLUG_INFO_INDEX(_out_low), 0.0F),
		_out_high		(PLUG_INFO_INDEX(_out_high), 0.0F),
		_in				(PLUG_INFO_INDEX(_in), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), 0.0F)
{  

}
		
ScriptingChangeRange::ScriptingChangeRange (const ScriptingChangeRange &rhs)
    :   ScriptingBase	(rhs),
		_in_low			(rhs._in_low),
		_in_high		(rhs._in_high),
		_out_low		(rhs._out_low),
		_out_high		(rhs._out_high),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingChangeRange & ScriptingChangeRange::operator = (const ScriptingChangeRange &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_low = rhs._in_low;
		_in_high = rhs._in_high;
		_out_low = rhs._out_low;
		_out_high = rhs._out_high;
		_in = rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingChangeRange::~ScriptingChangeRange (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingChangeRange::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingChangeRange::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in_low, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_high, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_low, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_high, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingChangeRange::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {

		if (_in < _in_low)
			_out = _out_low;
		else if (_in > _in_high)
			_out = _out_high;
		else {
			DTfloat t = (_in - _in_low) / (_in_high - _in_low);
			_out = _out_low + t * (_out_high - _out_low);
		}
	
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


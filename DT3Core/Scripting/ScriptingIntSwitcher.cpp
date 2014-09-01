//==============================================================================
///	
///	File: ScriptingIntSwitcher.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingIntSwitcher.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingIntSwitcher,"Switches",NULL)
IMPLEMENT_PLUG_NODE(ScriptingIntSwitcher)

IMPLEMENT_PLUG_INFO_INDEX(_in1)
IMPLEMENT_PLUG_INFO_INDEX(_in_int1)
IMPLEMENT_PLUG_INFO_INDEX(_in2)
IMPLEMENT_PLUG_INFO_INDEX(_in_int2)
IMPLEMENT_PLUG_INFO_INDEX(_in3)
IMPLEMENT_PLUG_INFO_INDEX(_in_int3)
IMPLEMENT_PLUG_INFO_INDEX(_in4)
IMPLEMENT_PLUG_INFO_INDEX(_in_int4)
IMPLEMENT_PLUG_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingIntSwitcher)

	PLUG_INIT(_in1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_int1,"Int_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in2,"In_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_int2,"Int_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in3,"In_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_int3,"Int_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in4,"In_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_int4,"Int_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingIntSwitcher::ScriptingIntSwitcher (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), false),
		_in_int1		(PLUG_INFO_INDEX(_in_int1), 0),
		_in2			(PLUG_INFO_INDEX(_in2), false),
		_in_int2		(PLUG_INFO_INDEX(_in_int2), 0),
		_in3			(PLUG_INFO_INDEX(_in3), false),
		_in_int3		(PLUG_INFO_INDEX(_in_int3), 0),
		_in4			(PLUG_INFO_INDEX(_in4), false),
		_in_int4		(PLUG_INFO_INDEX(_in_int4), 0),
        _buffer_output  (false),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingIntSwitcher::ScriptingIntSwitcher (const ScriptingIntSwitcher &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in_int1		(rhs._in_int1),
		_in2			(rhs._in2),
		_in_int2		(rhs._in_int2),
		_in3			(rhs._in3),
		_in_int3		(rhs._in_int3),
		_in4			(rhs._in4),
		_in_int4		(rhs._in_int4),
        _buffer_output  (rhs._buffer_output),
		_out			(rhs._out)
{   

}

ScriptingIntSwitcher & ScriptingIntSwitcher::operator = (const ScriptingIntSwitcher &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in_int1 = rhs._in_int1;
		_in2 = rhs._in2;
		_in_int2 = rhs._in_int2;
		_in3 = rhs._in3;
		_in_int3 = rhs._in_int3;
		_in4 = rhs._in4;
		_in_int4 = rhs._in_int4;
        _buffer_output = rhs._buffer_output;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingIntSwitcher::~ScriptingIntSwitcher (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingIntSwitcher::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingIntSwitcher::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_int1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_int2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_int3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_int4, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_buffer_output, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingIntSwitcher::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {

		if (_in4)		
			_out = _in_int4;
		else if (_in3)	
			_out = _in_int3;
		else if (_in2)	
			_out = _in_int2;
		else if (_in1)	
			_out = _in_int1;
		else if (!_buffer_output)
			_out = 0;

		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


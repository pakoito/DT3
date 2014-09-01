//==============================================================================
///	
///	File: ScriptingBoolSwitcher.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBoolSwitcher.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingBoolSwitcher,"Switches",NULL)
IMPLEMENT_PLUG_NODE(ScriptingBoolSwitcher)

IMPLEMENT_PLUG_INFO_INDEX(_in1)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool1)
IMPLEMENT_PLUG_INFO_INDEX(_in2)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool2)
IMPLEMENT_PLUG_INFO_INDEX(_in3)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool3)
IMPLEMENT_PLUG_INFO_INDEX(_in4)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool4)
IMPLEMENT_PLUG_INFO_INDEX(_in5)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool5)
IMPLEMENT_PLUG_INFO_INDEX(_in6)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool6)
IMPLEMENT_PLUG_INFO_INDEX(_in7)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool7)
IMPLEMENT_PLUG_INFO_INDEX(_in8)
IMPLEMENT_PLUG_INFO_INDEX(_in_bool8)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingBoolSwitcher)

	PLUG_INIT(_in1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool1,"Bool_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in2,"In_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool2,"Bool_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in3,"In_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool3,"Bool_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in4,"In_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool4,"Bool_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in5,"In_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool5,"Bool_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in6,"In_6")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool6,"Bool_6")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in7,"In_7")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool7,"Bool_7")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in8,"In_8")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_bool8,"Bool_8")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingBoolSwitcher::ScriptingBoolSwitcher (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), false),
		_in_bool1		(PLUG_INFO_INDEX(_in_bool1), false),
		_in2			(PLUG_INFO_INDEX(_in2), false),
		_in_bool2		(PLUG_INFO_INDEX(_in_bool2), false),
		_in3			(PLUG_INFO_INDEX(_in3), false),
		_in_bool3		(PLUG_INFO_INDEX(_in_bool3), false),
		_in4			(PLUG_INFO_INDEX(_in4), false),
		_in_bool4		(PLUG_INFO_INDEX(_in_bool4), false),
		_in5			(PLUG_INFO_INDEX(_in5), false),
		_in_bool5		(PLUG_INFO_INDEX(_in_bool5), false),
		_in6			(PLUG_INFO_INDEX(_in6), false),
		_in_bool6		(PLUG_INFO_INDEX(_in_bool6), false),
		_in7			(PLUG_INFO_INDEX(_in7), false),
		_in_bool7		(PLUG_INFO_INDEX(_in_bool7), false),
		_in8			(PLUG_INFO_INDEX(_in8), false),
		_in_bool8		(PLUG_INFO_INDEX(_in_bool8), false),
        _buffer_output  (false),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingBoolSwitcher::ScriptingBoolSwitcher (const ScriptingBoolSwitcher &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in_bool1		(rhs._in_bool1),
		_in2			(rhs._in2),
		_in_bool2		(rhs._in_bool2),
		_in3			(rhs._in3),
		_in_bool3		(rhs._in_bool3),
		_in4			(rhs._in4),
		_in_bool4		(rhs._in_bool4),
		_in5			(rhs._in5),
		_in_bool5		(rhs._in_bool5),
		_in6			(rhs._in6),
		_in_bool6		(rhs._in_bool6),
		_in7			(rhs._in7),
		_in_bool7		(rhs._in_bool7),
		_in8			(rhs._in8),
		_in_bool8		(rhs._in_bool8),
        _buffer_output  (rhs._buffer_output),
		_out			(rhs._out)
{   

}

ScriptingBoolSwitcher & ScriptingBoolSwitcher::operator = (const ScriptingBoolSwitcher &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in_bool1 = rhs._in_bool1;
		_in2 = rhs._in2;
		_in_bool2 = rhs._in_bool2;
		_in3 = rhs._in3;
		_in_bool3 = rhs._in_bool3;
		_in4 = rhs._in4;
		_in_bool4 = rhs._in_bool4;
		_in5 = rhs._in5;
		_in_bool5 = rhs._in_bool5;
		_in6 = rhs._in6;
		_in_bool6 = rhs._in_bool6;
		_in7 = rhs._in7;
		_in_bool7 = rhs._in_bool7;
		_in8 = rhs._in8;
		_in_bool8 = rhs._in_bool8;
        _buffer_output = rhs._buffer_output;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingBoolSwitcher::~ScriptingBoolSwitcher (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingBoolSwitcher::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingBoolSwitcher::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in5, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool5, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in6, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool6, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in7, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool7, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in8, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_bool8, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_buffer_output, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingBoolSwitcher::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
	
	
		if (_in8)		
			_out = _in_bool8;
		else if (_in7)	
			_out = _in_bool7;
		else if (_in6)	
			_out = _in_bool6;
		else if (_in5)	
			_out = _in_bool5;
		else if (_in4)	
			_out = _in_bool4;
		else if (_in3)	
			_out = _in_bool3;
		else if (_in2)	
			_out = _in_bool2;
		else if (_in1)	
			_out = _in_bool1;
		else if (!_buffer_output) 
			_out = false;

		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


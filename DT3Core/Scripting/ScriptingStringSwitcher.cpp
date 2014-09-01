//==============================================================================
///	
///	File: ScriptingStringSwitcher.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingStringSwitcher.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingStringSwitcher,"Switches",NULL)
IMPLEMENT_PLUG_NODE(ScriptingStringSwitcher)

IMPLEMENT_PLUG_INFO_INDEX(_in1)
IMPLEMENT_PLUG_INFO_INDEX(_in_string1)
IMPLEMENT_PLUG_INFO_INDEX(_in2)
IMPLEMENT_PLUG_INFO_INDEX(_in_string2)
IMPLEMENT_PLUG_INFO_INDEX(_in3)
IMPLEMENT_PLUG_INFO_INDEX(_in_string3)
IMPLEMENT_PLUG_INFO_INDEX(_in4)
IMPLEMENT_PLUG_INFO_INDEX(_in_string4)
IMPLEMENT_PLUG_INFO_INDEX(_in5)
IMPLEMENT_PLUG_INFO_INDEX(_in_string5)
IMPLEMENT_PLUG_INFO_INDEX(_in6)
IMPLEMENT_PLUG_INFO_INDEX(_in_string6)
IMPLEMENT_PLUG_INFO_INDEX(_in7)
IMPLEMENT_PLUG_INFO_INDEX(_in_string7)
IMPLEMENT_PLUG_INFO_INDEX(_in8)
IMPLEMENT_PLUG_INFO_INDEX(_in_string8)
IMPLEMENT_PLUG_INFO_INDEX(_in9)
IMPLEMENT_PLUG_INFO_INDEX(_in_string9)
IMPLEMENT_PLUG_INFO_INDEX(_in10)
IMPLEMENT_PLUG_INFO_INDEX(_in_string10)
IMPLEMENT_PLUG_INFO_INDEX(_in11)
IMPLEMENT_PLUG_INFO_INDEX(_in_string11)
IMPLEMENT_PLUG_INFO_INDEX(_in12)
IMPLEMENT_PLUG_INFO_INDEX(_in_string12)
IMPLEMENT_PLUG_INFO_INDEX(_in13)
IMPLEMENT_PLUG_INFO_INDEX(_in_string13)
IMPLEMENT_PLUG_INFO_INDEX(_in14)
IMPLEMENT_PLUG_INFO_INDEX(_in_string14)
IMPLEMENT_PLUG_INFO_INDEX(_in15)
IMPLEMENT_PLUG_INFO_INDEX(_in_string15)
IMPLEMENT_PLUG_INFO_INDEX(_in16)
IMPLEMENT_PLUG_INFO_INDEX(_in_string16)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingStringSwitcher)

	PLUG_INIT(_in1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string1,"String_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in2,"In_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string2,"String_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in3,"In_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string3,"String_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in4,"In_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string4,"String_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in5,"In_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string5,"String_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in6,"In_6")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string6,"String_6")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in7,"In_7")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string7,"String_7")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in8,"In_8")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string8,"String_8")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in9,"In_9")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string9,"String_9")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in10,"In_10")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string10,"String_10")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in11,"In_11")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string11,"String_11")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in12,"In_12")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string12,"String_12")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in13,"In_13")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string13,"String_13")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in14,"In_14")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string14,"String_14")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in15,"In_15")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string15,"String_15")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_in16,"In_16")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_string16,"String_16")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingStringSwitcher::ScriptingStringSwitcher (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), false),
		_in_string1		(PLUG_INFO_INDEX(_in_string1)),
		_in2			(PLUG_INFO_INDEX(_in2), false),
		_in_string2		(PLUG_INFO_INDEX(_in_string2)),
		_in3			(PLUG_INFO_INDEX(_in3), false),
		_in_string3		(PLUG_INFO_INDEX(_in_string3)),
		_in4			(PLUG_INFO_INDEX(_in4), false),
		_in_string4		(PLUG_INFO_INDEX(_in_string4)),
		_in5			(PLUG_INFO_INDEX(_in5), false),
		_in_string5		(PLUG_INFO_INDEX(_in_string5)),
		_in6			(PLUG_INFO_INDEX(_in6), false),
		_in_string6		(PLUG_INFO_INDEX(_in_string6)),
		_in7			(PLUG_INFO_INDEX(_in7), false),
		_in_string7		(PLUG_INFO_INDEX(_in_string7)),
		_in8			(PLUG_INFO_INDEX(_in8), false),
		_in_string8		(PLUG_INFO_INDEX(_in_string8)),
		_in9			(PLUG_INFO_INDEX(_in9), false),
		_in_string9		(PLUG_INFO_INDEX(_in_string9)),
		_in10			(PLUG_INFO_INDEX(_in10), false),
		_in_string10	(PLUG_INFO_INDEX(_in_string10)),
		_in11			(PLUG_INFO_INDEX(_in11), false),
		_in_string11	(PLUG_INFO_INDEX(_in_string11)),
		_in12			(PLUG_INFO_INDEX(_in12), false),
		_in_string12	(PLUG_INFO_INDEX(_in_string12)),
		_in13			(PLUG_INFO_INDEX(_in13), false),
		_in_string13	(PLUG_INFO_INDEX(_in_string13)),
		_in14			(PLUG_INFO_INDEX(_in14), false),
		_in_string14	(PLUG_INFO_INDEX(_in_string14)),
		_in15			(PLUG_INFO_INDEX(_in15), false),
		_in_string15	(PLUG_INFO_INDEX(_in_string15)),
		_in16			(PLUG_INFO_INDEX(_in16), false),
		_in_string16	(PLUG_INFO_INDEX(_in_string16)),
        _buffer_output  (false),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingStringSwitcher::ScriptingStringSwitcher (const ScriptingStringSwitcher &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in_string1		(rhs._in_string1),
		_in2			(rhs._in2),
		_in_string2		(rhs._in_string2),
		_in3			(rhs._in3),
		_in_string3		(rhs._in_string3),
		_in4			(rhs._in4),
		_in_string4		(rhs._in_string4),
		_in5			(rhs._in5),
		_in_string5		(rhs._in_string5),
		_in6			(rhs._in6),
		_in_string6		(rhs._in_string6),
		_in7			(rhs._in7),
		_in_string7		(rhs._in_string7),
		_in8			(rhs._in8),
		_in_string8		(rhs._in_string8),
		_in9			(rhs._in9),
		_in_string9		(rhs._in_string9),
		_in10			(rhs._in10),
		_in_string10	(rhs._in_string10),
		_in11			(rhs._in11),
		_in_string11	(rhs._in_string11),
		_in12			(rhs._in12),
		_in_string12	(rhs._in_string12),
		_in13			(rhs._in13),
		_in_string13	(rhs._in_string13),
		_in14			(rhs._in14),
		_in_string14	(rhs._in_string14),
		_in15			(rhs._in15),
		_in_string15	(rhs._in_string15),
		_in16			(rhs._in16),
		_in_string16	(rhs._in_string16),
        _buffer_output  (rhs._buffer_output),
		_out			(rhs._out)
{   

}

ScriptingStringSwitcher & ScriptingStringSwitcher::operator = (const ScriptingStringSwitcher &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in_string1 = rhs._in_string1;
		_in2 = rhs._in2;
		_in_string2 = rhs._in_string2;
		_in3 = rhs._in3;
		_in_string3 = rhs._in_string3;
		_in4 = rhs._in4;
		_in_string4 = rhs._in_string4;
		_in5 = rhs._in5;
		_in_string5 = rhs._in_string5;
		_in6 = rhs._in6;
		_in_string6 = rhs._in_string6;
		_in7 = rhs._in7;
		_in_string7 = rhs._in_string7;
		_in8 = rhs._in8;
		_in_string8 = rhs._in_string8;
		_in9 = rhs._in9;
		_in_string9 = rhs._in_string9;
		_in10 = rhs._in10;
		_in_string10 = rhs._in_string10;
		_in11 = rhs._in11;
		_in_string11 = rhs._in_string11;
		_in12 = rhs._in12;
		_in_string12 = rhs._in_string12;
		_in13 = rhs._in13;
		_in_string13 = rhs._in_string13;
		_in14 = rhs._in14;
		_in_string14 = rhs._in_string14;
		_in15 = rhs._in15;
		_in_string15 = rhs._in_string15;
		_in16 = rhs._in16;
		_in_string16 = rhs._in_string16;
        _buffer_output = rhs._buffer_output;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingStringSwitcher::~ScriptingStringSwitcher (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingStringSwitcher::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingStringSwitcher::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_string1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_string2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_string3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_string4, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in5, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string5, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in6, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string6, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in7, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string7, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in8, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string8, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in9, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string9, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in10, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string10, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in11, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string11, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in12, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string12, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in13, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string13, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in14, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string14, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in15, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string15, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in16, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_string16, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_buffer_output, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingStringSwitcher::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {

		if (_in16)		_out = _in_string16;
		else if (_in15)	_out = _in_string15;
		else if (_in14)	_out = _in_string14;
		else if (_in13)	_out = _in_string13;
		else if (_in12)	_out = _in_string12;
		else if (_in11)	_out = _in_string11;
		else if (_in10)	_out = _in_string10;
		else if (_in9)	_out = _in_string9;
		else if (_in8)	_out = _in_string8;
		else if (_in7)	_out = _in_string7;
		else if (_in6)	_out = _in_string6;
		else if (_in5)	_out = _in_string5;
		else if (_in4)	_out = _in_string4;
		else if (_in3)	_out = _in_string3;
		else if (_in2)	_out = _in_string2;
		else if (_in1)	_out = _in_string1;
		else  if (!_buffer_output)  _out = "";

		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


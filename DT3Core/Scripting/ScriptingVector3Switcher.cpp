//==============================================================================
///	
///	File: ScriptingVector3Switcher.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Switcher.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Switcher,"Switches",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Switcher)

IMPLEMENT_PLUG_INFO_INDEX(_in1)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector1)
IMPLEMENT_PLUG_INFO_INDEX(_in2)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector2)
IMPLEMENT_PLUG_INFO_INDEX(_in3)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector3)
IMPLEMENT_PLUG_INFO_INDEX(_in4)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector4)
IMPLEMENT_PLUG_INFO_INDEX(_in5)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector5)
IMPLEMENT_PLUG_INFO_INDEX(_in6)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector6)
IMPLEMENT_PLUG_INFO_INDEX(_in7)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector7)
IMPLEMENT_PLUG_INFO_INDEX(_in8)
IMPLEMENT_PLUG_INFO_INDEX(_in_vector8)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Switcher)

	PLUG_INIT(_in1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector1,"Vector_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in2,"In_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector2,"Vector_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in3,"In_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector3,"Vector_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in4,"In_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector4,"Vector_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in5,"In_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector5,"Vector_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in6,"In_6")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector6,"Vector_6")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in7,"In_7")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector7,"Vector_7")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in8,"In_8")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_vector8,"Vector_8")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Switcher::ScriptingVector3Switcher (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), false),
		_in_vector1		(PLUG_INFO_INDEX(_in_vector1), Vector3(0.0F,0.0F,0.0F)),
		_in2			(PLUG_INFO_INDEX(_in2), false),
		_in_vector2		(PLUG_INFO_INDEX(_in_vector2), Vector3(0.0F,0.0F,0.0F)),
		_in3			(PLUG_INFO_INDEX(_in3), false),
		_in_vector3		(PLUG_INFO_INDEX(_in_vector3), Vector3(0.0F,0.0F,0.0F)),
		_in4			(PLUG_INFO_INDEX(_in4), false),
		_in_vector4		(PLUG_INFO_INDEX(_in_vector4), Vector3(0.0F,0.0F,0.0F)),
		_in5			(PLUG_INFO_INDEX(_in5), false),
		_in_vector5		(PLUG_INFO_INDEX(_in_vector5), Vector3(0.0F,0.0F,0.0F)),
		_in6			(PLUG_INFO_INDEX(_in6), false),
		_in_vector6		(PLUG_INFO_INDEX(_in_vector6), Vector3(0.0F,0.0F,0.0F)),
		_in7			(PLUG_INFO_INDEX(_in7), false),
		_in_vector7		(PLUG_INFO_INDEX(_in_vector7), Vector3(0.0F,0.0F,0.0F)),
		_in8			(PLUG_INFO_INDEX(_in8), false),
		_in_vector8		(PLUG_INFO_INDEX(_in_vector8), Vector3(0.0F,0.0F,0.0F)),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F)),
        _buffer_output  (false)
{  

}
		
ScriptingVector3Switcher::ScriptingVector3Switcher (const ScriptingVector3Switcher &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in_vector1		(rhs._in_vector1),
		_in2			(rhs._in2),
		_in_vector2		(rhs._in_vector2),
		_in3			(rhs._in3),
		_in_vector3		(rhs._in_vector3),
		_in4			(rhs._in4),
		_in_vector4		(rhs._in_vector4),
		_in5			(rhs._in5),
		_in_vector5		(rhs._in_vector5),
		_in6			(rhs._in6),
		_in_vector6		(rhs._in_vector6),
		_in7			(rhs._in7),
		_in_vector7		(rhs._in_vector7),
		_in8			(rhs._in8),
		_in_vector8		(rhs._in_vector8),
		_out			(rhs._out),
        _buffer_output  (rhs._buffer_output)
{   

}

ScriptingVector3Switcher & ScriptingVector3Switcher::operator = (const ScriptingVector3Switcher &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in_vector1 = rhs._in_vector1;
		_in2 = rhs._in2;
		_in_vector2 = rhs._in_vector2;
		_in3 = rhs._in3;
		_in_vector3 = rhs._in_vector3;
		_in4 = rhs._in4;
		_in_vector4 = rhs._in_vector4;
		_in5 = rhs._in5;
		_in_vector5 = rhs._in_vector5;
		_in6 = rhs._in6;
		_in_vector6 = rhs._in_vector6;
		_in7 = rhs._in7;
		_in_vector7 = rhs._in_vector7;
		_in8 = rhs._in8;
		_in_vector8 = rhs._in_vector8;
		_out = rhs._out;
        
        _buffer_output = rhs._buffer_output;
	}
    return (*this);
}
			
ScriptingVector3Switcher::~ScriptingVector3Switcher (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Switcher::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Switcher::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_vector1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_vector2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_vector3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_vector4, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in5, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_vector5, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in6, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_vector6, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in7, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_vector7, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in8, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_in_vector8, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_buffer_output, DATA_PERSISTENT | DATA_SETTABLE);
    
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector3Switcher::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
	
		if (_in8)		
			_out = _in_vector8;
		else if (_in7)	
			_out = _in_vector7;
		else if (_in6)	
			_out = _in_vector6;
		else if (_in5)	
			_out = _in_vector5;
		else if (_in4)	
			_out = _in_vector4;
		else if (_in3)	
			_out = _in_vector3;
		else if (_in2)	
			_out = _in_vector2;
		else if (_in1)	
			_out = _in_vector1;
		else if (!_buffer_output)
			_out = Vector3(0.0F,0.0F,0.0F);

		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


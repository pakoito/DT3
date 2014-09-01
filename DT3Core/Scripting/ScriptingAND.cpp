//==============================================================================
///	
///	File: ScriptingAND.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingAND.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingAND,"Logic",NULL)
IMPLEMENT_PLUG_NODE(ScriptingAND)

IMPLEMENT_PLUG_INFO_INDEX(_in1);
IMPLEMENT_PLUG_INFO_INDEX(_in2);
IMPLEMENT_PLUG_INFO_INDEX(_in3);
IMPLEMENT_PLUG_INFO_INDEX(_in4);
IMPLEMENT_PLUG_INFO_INDEX(_in5);
IMPLEMENT_PLUG_INFO_INDEX(_in6);
IMPLEMENT_PLUG_INFO_INDEX(_in7);
IMPLEMENT_PLUG_INFO_INDEX(_in8);
IMPLEMENT_PLUG_INFO_INDEX(_out);
IMPLEMENT_PLUG_INFO_INDEX(_out_inv);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingAND)

	PLUG_INIT(_in1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_in2,"In_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_in3,"In_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_in4,"In_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_in5,"In_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_in6,"In_6")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_in7,"In_7")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_in8,"In_8")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out))
		.affects(PLUG_INFO_INDEX(_out_inv));

	PLUG_INIT(_out,"Out")
		.set_output(true);
		
	PLUG_INIT(_out_inv,"Out_Inv")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingAND::ScriptingAND (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), true),
		_in2			(PLUG_INFO_INDEX(_in2), true),
		_in3			(PLUG_INFO_INDEX(_in3), true),
		_in4			(PLUG_INFO_INDEX(_in4), true),
		_in5			(PLUG_INFO_INDEX(_in5), true),
		_in6			(PLUG_INFO_INDEX(_in6), true),
		_in7			(PLUG_INFO_INDEX(_in7), true),
		_in8			(PLUG_INFO_INDEX(_in8), true),
		_out			(PLUG_INFO_INDEX(_out), true),
		_out_inv		(PLUG_INFO_INDEX(_out_inv), false)
{  

}
		
ScriptingAND::ScriptingAND (const ScriptingAND &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in2			(rhs._in2),
		_in3			(rhs._in3),
		_in4			(rhs._in4),
		_in5			(rhs._in5),
		_in6			(rhs._in6),
		_in7			(rhs._in7),
		_in8			(rhs._in8),
		_out			(rhs._out),
		_out_inv		(rhs._out_inv)
{   

}

ScriptingAND & ScriptingAND::operator = (const ScriptingAND &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in2 = rhs._in2;
		_in3 = rhs._in3;
		_in4 = rhs._in4;
		_in5 = rhs._in5;
		_in6 = rhs._in6;
		_in7 = rhs._in7;
		_in8 = rhs._in8;
		_out = rhs._out;
		_out_inv = rhs._out_inv;
	}
    return (*this);
}
			
ScriptingAND::~ScriptingAND (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingAND::initialize (void)
{
	ScriptingBase::initialize();

}

//==============================================================================
//==============================================================================

void ScriptingAND::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	    
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in5, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in6, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in7, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in8, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_inv, DATA_PERSISTENT);
																			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingAND::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out || plug == &_out_inv) {

		_out = (_in1) && (_in2) && (_in3) && (_in4) && (_in5) && (_in6) && (_in7) && (_in8);
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


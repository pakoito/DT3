//==============================================================================
///	
///	File: ScriptingStringConcat.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingStringConcat.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingStringConcat,"Strings",NULL)
IMPLEMENT_PLUG_NODE(ScriptingStringConcat)

IMPLEMENT_PLUG_INFO_INDEX(_in1);
IMPLEMENT_PLUG_INFO_INDEX(_in2);
IMPLEMENT_PLUG_INFO_INDEX(_insert_space);
IMPLEMENT_PLUG_INFO_INDEX(_out);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingStringConcat)

	PLUG_INIT(_in1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_in2,"In_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_insert_space,"Insert_Space")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingStringConcat::ScriptingStringConcat (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), ""),
		_in2			(PLUG_INFO_INDEX(_in2), ""),
        _insert_space   (PLUG_INFO_INDEX(_insert_space), true),
		_out			(PLUG_INFO_INDEX(_out), "")
{  

}
		
ScriptingStringConcat::ScriptingStringConcat (const ScriptingStringConcat &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in2			(rhs._in2),
        _insert_space   (rhs._insert_space),
		_out			(rhs._out)
{   

}

ScriptingStringConcat & ScriptingStringConcat::operator = (const ScriptingStringConcat &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in2 = rhs._in2;
        _insert_space = rhs._insert_space;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingStringConcat::~ScriptingStringConcat (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingStringConcat::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingStringConcat::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_insert_space, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingStringConcat::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
        
        if (_insert_space) {
            _out = (*_in1) + " " + (*_in2);
        } else {
            _out = (*_in1) + (*_in2);
        }
    
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


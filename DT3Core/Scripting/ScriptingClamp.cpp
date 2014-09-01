//==============================================================================
///	
///	File: ScriptingClamp.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingClamp.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingClamp,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingClamp)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_min)
IMPLEMENT_PLUG_INFO_INDEX(_max)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingClamp)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_min,"Min")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_max,"Max")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingClamp::ScriptingClamp (void)
    :   _in				(PLUG_INFO_INDEX(_in), 0.0F),
		_min			(PLUG_INFO_INDEX(_min), 0.0F),
		_max			(PLUG_INFO_INDEX(_max), 1.0F),
		_out			(PLUG_INFO_INDEX(_out), true)
{  

}
		
ScriptingClamp::ScriptingClamp (const ScriptingClamp &rhs)
    :   ScriptingBase	(rhs),
		_in				(PLUG_INFO_INDEX(_in), rhs._in),
		_min			(PLUG_INFO_INDEX(_min), rhs._min),
		_max			(PLUG_INFO_INDEX(_max), rhs._max),
		_out			(PLUG_INFO_INDEX(_out), rhs._out)
{   

}

ScriptingClamp & ScriptingClamp::operator = (const ScriptingClamp &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_min = rhs._min;
		_max = rhs._max;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingClamp::~ScriptingClamp (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingClamp::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingClamp::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	    
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_min, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_max, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														    					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingClamp::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		DTfloat val = _in;
		
		if (val < _min)			val = _min;
		else if (val > _max)	val = _max;
		
		_out = val;
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


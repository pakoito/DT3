//==============================================================================
///	
///	File: ScriptingInRange.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingInRange.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingInRange,"Comparison",NULL)
IMPLEMENT_PLUG_NODE(ScriptingInRange)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_lower)
IMPLEMENT_PLUG_INFO_INDEX(_upper)
IMPLEMENT_PLUG_INFO_INDEX(_out)
	
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingInRange)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_lower,"Lower")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_upper,"Upper")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingInRange::ScriptingInRange (void)
    :   _in				(PLUG_INFO_INDEX(_in), 0.0F),
		_lower			(PLUG_INFO_INDEX(_lower), 0.0F),
		_upper			(PLUG_INFO_INDEX(_upper), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), false)
{  

}
		
ScriptingInRange::ScriptingInRange (const ScriptingInRange &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_lower			(rhs._lower),
		_upper			(rhs._upper),
		_out			(rhs._out)
{   

}

ScriptingInRange & ScriptingInRange::operator = (const ScriptingInRange &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_lower = rhs._lower;
		_upper = rhs._upper;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingInRange::~ScriptingInRange (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingInRange::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingInRange::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_lower, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_upper, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingInRange::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		_out = ((_in) >= (_lower)) && ((_in) <= (_upper));
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


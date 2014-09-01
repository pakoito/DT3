//==============================================================================
///	
///	File: ScriptingATAN2.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingATAN2.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingATAN2,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingATAN2)

IMPLEMENT_PLUG_INFO_INDEX(_in_y)
IMPLEMENT_PLUG_INFO_INDEX(_in_x)
IMPLEMENT_PLUG_INFO_INDEX(_out)
	
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingATAN2)

	PLUG_INIT(_in_y,"In_Y")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_x,"In_X")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingATAN2::ScriptingATAN2 (void)
    :   _in_y		(PLUG_INFO_INDEX(_in_y), 0.0F),
        _in_x		(PLUG_INFO_INDEX(_in_x), 0.0F),
		_out		(PLUG_INFO_INDEX(_out), 1.0F)
{  

}
		
ScriptingATAN2::ScriptingATAN2 (const ScriptingATAN2 &rhs)
    :   ScriptingBase	(rhs),
		_in_y			(rhs._in_y),
		_in_x			(rhs._in_x),
		_out			(rhs._out)
{   

}

ScriptingATAN2 & ScriptingATAN2::operator = (const ScriptingATAN2 &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_y = rhs._in_y;
		_in_x = rhs._in_x;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingATAN2::~ScriptingATAN2 (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingATAN2::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingATAN2::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_in_y, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_x, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingATAN2::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		_out = std::atan2(_in_y,_in_x);
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


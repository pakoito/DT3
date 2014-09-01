//==============================================================================
///	
///	File: ScriptingKeyframesRoot.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframesRoot.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingKeyframesRoot,"Animation",NULL)
IMPLEMENT_PLUG_NODE(ScriptingKeyframesRoot)

IMPLEMENT_PLUG_INFO_INDEX(_time_in)
IMPLEMENT_PLUG_INFO_INDEX(_time_out)
	
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingKeyframesRoot)

	PLUG_INIT(_time_in,"Time_In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_time_out));

	PLUG_INIT(_time_out,"Time_Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingKeyframesRoot::ScriptingKeyframesRoot (void)
    :   _time_in	(PLUG_INFO_INDEX(_time_in), 0.0F),
		_time_out	(PLUG_INFO_INDEX(_time_out), 0.0F)
{  

}
		
ScriptingKeyframesRoot::ScriptingKeyframesRoot (const ScriptingKeyframesRoot &rhs)
    :   ScriptingBase	(rhs),
		_time_in		(rhs._time_in),
		_time_out		(rhs._time_out)
{   

}

ScriptingKeyframesRoot & ScriptingKeyframesRoot::operator = (const ScriptingKeyframesRoot &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_time_in = rhs._time_in;
		_time_out = rhs._time_out;
	}
    return (*this);
}
			
ScriptingKeyframesRoot::~ScriptingKeyframesRoot (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingKeyframesRoot::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesRoot::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_time_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time_out, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingKeyframesRoot::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_time_out) {
		_time_out = _time_in;
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


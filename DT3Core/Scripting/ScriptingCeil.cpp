//==============================================================================
///	
///	File: ScriptingCeil.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingCeil.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingCeil,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingCeil)

IMPLEMENT_PLUG_INFO_INDEX(_in);
IMPLEMENT_PLUG_INFO_INDEX(_out);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingCeil)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingCeil::ScriptingCeil (void)
    :   _in			(PLUG_INFO_INDEX(_in), 0.0F),
		_out		(PLUG_INFO_INDEX(_out), 0.0F)
{  

}
		
ScriptingCeil::ScriptingCeil (const ScriptingCeil &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingCeil & ScriptingCeil::operator = (const ScriptingCeil &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingCeil::~ScriptingCeil (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingCeil::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingCeil::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingCeil::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		_out = std::ceil(_in);
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


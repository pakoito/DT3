//==============================================================================
///	
///	File: ScriptingVector3Passthrough.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Passthrough.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Passthrough,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Passthrough)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_enable)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Passthrough)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_enable,"Enable")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Passthrough::ScriptingVector3Passthrough (void)
    :   _in			(PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_enable		(PLUG_INFO_INDEX(_enable), true),
		_out		(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingVector3Passthrough::ScriptingVector3Passthrough (const ScriptingVector3Passthrough &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_enable			(rhs._enable),
		_out			(rhs._out)
{   

}

ScriptingVector3Passthrough & ScriptingVector3Passthrough::operator = (const ScriptingVector3Passthrough &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_enable = rhs._enable;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingVector3Passthrough::~ScriptingVector3Passthrough (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Passthrough::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Passthrough::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_enable, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT | DATA_SETTABLE);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector3Passthrough::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {

		if (_enable) {
			_out = _in;
		}
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


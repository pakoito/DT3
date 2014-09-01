//==============================================================================
///	
///	File: ScriptingComponentsToVector3.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingComponentsToVector3.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingComponentsToVector3,"Conversion",NULL)
IMPLEMENT_PLUG_NODE(ScriptingComponentsToVector3)

IMPLEMENT_PLUG_INFO_INDEX(_in_x)
IMPLEMENT_PLUG_INFO_INDEX(_in_y)
IMPLEMENT_PLUG_INFO_INDEX(_in_z)
IMPLEMENT_PLUG_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingComponentsToVector3)

	PLUG_INIT(_in_x,"In_X")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_y,"In_Y")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_z,"In_Z")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingComponentsToVector3::ScriptingComponentsToVector3 (void)
    :   _in_x			(PLUG_INFO_INDEX(_in_x), 0.0F),
		_in_y			(PLUG_INFO_INDEX(_in_y), 0.0F),
		_in_z			(PLUG_INFO_INDEX(_in_z), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingComponentsToVector3::ScriptingComponentsToVector3 (const ScriptingComponentsToVector3 &rhs)
    :   ScriptingBase	(rhs),
		_in_x			(rhs._in_x),
		_in_y			(rhs._in_y),
		_in_z			(rhs._in_z),
		_out			(rhs._out)
{   

}

ScriptingComponentsToVector3 & ScriptingComponentsToVector3::operator = (const ScriptingComponentsToVector3 &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_x = rhs._in_x;
		_in_y = rhs._in_y;
		_in_z = rhs._in_z;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingComponentsToVector3::~ScriptingComponentsToVector3 (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingComponentsToVector3::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingComponentsToVector3::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        					
	*archive << ARCHIVE_PLUG(_in_x, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_y, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_z, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
								
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingComponentsToVector3::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		_out = Vector3(_in_x, _in_y, _in_z);
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


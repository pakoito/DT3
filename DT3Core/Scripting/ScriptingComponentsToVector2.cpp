//==============================================================================
///	
///	File: ScriptingComponentsToVector2.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingComponentsToVector2.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingComponentsToVector2,"Conversion",NULL)
IMPLEMENT_PLUG_NODE(ScriptingComponentsToVector2)

IMPLEMENT_PLUG_INFO_INDEX(_in_x)
IMPLEMENT_PLUG_INFO_INDEX(_in_y)
IMPLEMENT_PLUG_INFO_INDEX(_out)
	
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingComponentsToVector2)

	PLUG_INIT(_in_x,"In_X")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_y,"In_Y")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingComponentsToVector2::ScriptingComponentsToVector2 (void)
    :   _in_x			(PLUG_INFO_INDEX(_in_x), 0.0F),
		_in_y			(PLUG_INFO_INDEX(_in_y), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), Vector2(0.0F,0.0F))
{  

}
		
ScriptingComponentsToVector2::ScriptingComponentsToVector2 (const ScriptingComponentsToVector2 &rhs)
    :   ScriptingBase	(rhs),
		_in_x			(rhs._in_x),
		_in_y			(rhs._in_y),
		_out			(rhs._out)
{   

}

ScriptingComponentsToVector2 & ScriptingComponentsToVector2::operator = (const ScriptingComponentsToVector2 &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_x = rhs._in_x;
		_in_y = rhs._in_y;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingComponentsToVector2::~ScriptingComponentsToVector2 (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingComponentsToVector2::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingComponentsToVector2::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        					
	*archive << ARCHIVE_PLUG(_in_x, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_y, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
								
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingComponentsToVector2::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		_out = Vector2(_in_x, _in_y);
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


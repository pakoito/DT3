//==============================================================================
///	
///	File: ScriptingComponentsToColor.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingComponentsToColor.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingComponentsToColor,"Conversion",NULL)
IMPLEMENT_PLUG_NODE(ScriptingComponentsToColor)

IMPLEMENT_PLUG_INFO_INDEX(_in_r)
IMPLEMENT_PLUG_INFO_INDEX(_in_g)
IMPLEMENT_PLUG_INFO_INDEX(_in_b)
IMPLEMENT_PLUG_INFO_INDEX(_in_a)
IMPLEMENT_PLUG_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingComponentsToColor)

	PLUG_INIT(_in_r,"In_R")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_g,"In_G")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_b,"In_B")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_in_a,"In_A")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingComponentsToColor::ScriptingComponentsToColor (void)
    :   _in_r			(PLUG_INFO_INDEX(_in_r), 0.0F),
		_in_g			(PLUG_INFO_INDEX(_in_g), 0.0F),
		_in_b			(PLUG_INFO_INDEX(_in_b), 0.0F),
		_in_a			(PLUG_INFO_INDEX(_in_a), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), Color4f(0.0F,0.0F,0.0F,0.0F))
{  

}
		
ScriptingComponentsToColor::ScriptingComponentsToColor (const ScriptingComponentsToColor &rhs)
    :   ScriptingBase	(rhs),
		_in_r			(rhs._in_r),
		_in_g			(rhs._in_g),
		_in_b			(rhs._in_b),
		_in_a			(rhs._in_a),
		_out			(rhs._out)
{   

}

ScriptingComponentsToColor & ScriptingComponentsToColor::operator = (const ScriptingComponentsToColor &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_r = rhs._in_r;
		_in_g = rhs._in_g;
		_in_b = rhs._in_b;
		_in_a = rhs._in_a;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingComponentsToColor::~ScriptingComponentsToColor (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingComponentsToColor::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingComponentsToColor::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        					
	*archive << ARCHIVE_PLUG(_in_r, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_g, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_b, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_a, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
								
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingComponentsToColor::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		_out = Color4f(_in_r, _in_g, _in_b, _in_a);
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


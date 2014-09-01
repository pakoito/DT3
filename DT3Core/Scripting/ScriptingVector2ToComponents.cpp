//==============================================================================
///	
///	File: ScriptingVector2ToComponents.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector2ToComponents.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector2ToComponents,"Conversion",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector2ToComponents)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out_x)
IMPLEMENT_PLUG_INFO_INDEX(_out_y)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector2ToComponents)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_x))
		.affects(PLUG_INFO_INDEX(_out_y));

	PLUG_INIT(_out_x,"Out_X")
		.set_output(true);
		
	PLUG_INIT(_out_y,"Out_Y")
		.set_output(true);
		
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector2ToComponents::ScriptingVector2ToComponents (void)
    :   _in				(PLUG_INFO_INDEX(_in), Vector2(0.0F,0.0F)),
		_out_x			(PLUG_INFO_INDEX(_out_x), 0.0F),
		_out_y			(PLUG_INFO_INDEX(_out_y), 0.0F)
{  

}
		
ScriptingVector2ToComponents::ScriptingVector2ToComponents (const ScriptingVector2ToComponents &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_out_x			(rhs._out_x),
		_out_y			(rhs._out_y)
{   

}

ScriptingVector2ToComponents & ScriptingVector2ToComponents::operator = (const ScriptingVector2ToComponents &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_out_x = rhs._out_x;
		_out_y = rhs._out_y;
	}
    return (*this);
}
			
ScriptingVector2ToComponents::~ScriptingVector2ToComponents (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector2ToComponents::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector2ToComponents::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_x, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_y, DATA_PERSISTENT);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector2ToComponents::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_x || plug == &_out_y) {
		_out_x = _in->x;
		_out_y = _in->y;
		_out_x.set_clean();
		_out_y.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


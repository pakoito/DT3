//==============================================================================
///	
///	File: ScriptingColorToComponents.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingColorToComponents.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingColorToComponents,"Conversion",NULL)
IMPLEMENT_PLUG_NODE(ScriptingColorToComponents)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out_r)
IMPLEMENT_PLUG_INFO_INDEX(_out_g)
IMPLEMENT_PLUG_INFO_INDEX(_out_b)
IMPLEMENT_PLUG_INFO_INDEX(_out_a)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingColorToComponents)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_r))
		.affects(PLUG_INFO_INDEX(_out_g))
		.affects(PLUG_INFO_INDEX(_out_b))
		.affects(PLUG_INFO_INDEX(_out_a));

	PLUG_INIT(_out_r,"Out_R")
		.set_output(true);
		
	PLUG_INIT(_out_g,"Out_G")
		.set_output(true);
		
	PLUG_INIT(_out_b,"Out_B")
		.set_output(true);

	PLUG_INIT(_out_a,"Out_A")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingColorToComponents::ScriptingColorToComponents (void)
    :   _in				(PLUG_INFO_INDEX(_in), Color4f(0.0F,0.0F,0.0F,0.0F)),
		_out_r			(PLUG_INFO_INDEX(_out_r), 0.0F),
		_out_g			(PLUG_INFO_INDEX(_out_g), 0.0F),
		_out_b			(PLUG_INFO_INDEX(_out_b), 0.0F),
		_out_a			(PLUG_INFO_INDEX(_out_a), 0.0F)
{  

}
		
ScriptingColorToComponents::ScriptingColorToComponents (const ScriptingColorToComponents &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_out_r			(rhs._out_r),
		_out_g			(rhs._out_g),
		_out_b			(rhs._out_b),
		_out_a			(rhs._out_a)
{   

}

ScriptingColorToComponents & ScriptingColorToComponents::operator = (const ScriptingColorToComponents &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_out_r = rhs._out_r;
		_out_g = rhs._out_g;
		_out_b = rhs._out_b;
		_out_a = rhs._out_a;
	}
    return (*this);
}
			
ScriptingColorToComponents::~ScriptingColorToComponents (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingColorToComponents::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingColorToComponents::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_r, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_g, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_b, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_a, DATA_PERSISTENT);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingColorToComponents::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_r || plug == &_out_g || plug == &_out_b || plug == &_out_a) {
		_out_r = _in->r_as_float();
		_out_g = _in->g_as_float();
		_out_b = _in->b_as_float();
		_out_a = _in->a_as_float();
		_out_r.set_clean();
		_out_g.set_clean();
		_out_b.set_clean();
		_out_a.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ScriptingColorInterpolator.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingColorInterpolator.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingColorInterpolator,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingColorInterpolator)

IMPLEMENT_PLUG_INFO_INDEX(_in_1);
IMPLEMENT_PLUG_INFO_INDEX(_in_2);
IMPLEMENT_PLUG_INFO_INDEX(_t);
IMPLEMENT_PLUG_INFO_INDEX(_out);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingColorInterpolator)

	PLUG_INIT(_in_1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_2,"In_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_t,"t")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingColorInterpolator::ScriptingColorInterpolator (void)
    :   _in_1			(PLUG_INFO_INDEX(_in_1), Color4f(1.0F,1.0F,1.0F,1.0F)),
		_in_2			(PLUG_INFO_INDEX(_in_2), Color4f(1.0F,1.0F,1.0F,1.0F)),
		_t				(PLUG_INFO_INDEX(_t), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), Color4f(1.0F,1.0F,1.0F,1.0F))
{  

}
		
ScriptingColorInterpolator::ScriptingColorInterpolator (const ScriptingColorInterpolator &rhs)
    :   ScriptingBase	(rhs),
		_in_1			(rhs._in_1),
		_in_2			(rhs._in_2),
		_t				(rhs._t),
		_out			(rhs._out)
{   

}

ScriptingColorInterpolator & ScriptingColorInterpolator::operator = (const ScriptingColorInterpolator &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_1 = rhs._in_1;
		_in_2 = rhs._in_2;
		_t = rhs._t;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingColorInterpolator::~ScriptingColorInterpolator (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingColorInterpolator::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingColorInterpolator::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in_1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingColorInterpolator::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		
		Color4f in_1 = _in_1;
		Color4f in_2 = _in_2;
		Color4f out;

		out.set_r(in_1.r_as_float() + (in_2.r_as_float() - in_1.r_as_float()) * _t);
		out.set_g(in_1.g_as_float() + (in_2.g_as_float() - in_1.g_as_float()) * _t);
		out.set_b(in_1.b_as_float() + (in_2.b_as_float() - in_1.b_as_float()) * _t);
		out.set_a(in_1.a_as_float() + (in_2.a_as_float() - in_1.a_as_float()) * _t);
		_out = out;
		
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


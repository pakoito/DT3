//==============================================================================
///	
///	File: ScriptingSinSuperposition.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSinSuperposition.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSinSuperposition,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSinSuperposition)

IMPLEMENT_PLUG_INFO_INDEX(_in);
IMPLEMENT_PLUG_INFO_INDEX(_mul1);
IMPLEMENT_PLUG_INFO_INDEX(_mul2);
IMPLEMENT_PLUG_INFO_INDEX(_mul3);
IMPLEMENT_PLUG_INFO_INDEX(_mul4);
IMPLEMENT_PLUG_INFO_INDEX(_mul5);
IMPLEMENT_PLUG_INFO_INDEX(_out);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSinSuperposition)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_mul1,"Mul_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_mul2,"Mul_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_mul3,"Mul_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_mul4,"Mul_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_mul5,"Mul_5")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSinSuperposition::ScriptingSinSuperposition (void)
    :   _in             (PLUG_INFO_INDEX(_in), 0.0F),
        _mul1			(PLUG_INFO_INDEX(_mul1), 0.0F),
		_mul2			(PLUG_INFO_INDEX(_mul2), 0.0F),
		_mul3			(PLUG_INFO_INDEX(_mul3), 0.0F),
		_mul4			(PLUG_INFO_INDEX(_mul4), 0.0F),
		_mul5			(PLUG_INFO_INDEX(_mul5), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), 0.0F)
{  

}
		
ScriptingSinSuperposition::ScriptingSinSuperposition (const ScriptingSinSuperposition &rhs)
    :   ScriptingBase	(rhs),
		_in             (rhs._in),
		_mul1			(rhs._mul1),
		_mul2			(rhs._mul2),
		_mul3			(rhs._mul3),
		_mul4			(rhs._mul4),
		_mul5			(rhs._mul5),
		_out			(rhs._out)
{   

}

ScriptingSinSuperposition & ScriptingSinSuperposition::operator = (const ScriptingSinSuperposition &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_mul1 = rhs._mul1;
		_mul2 = rhs._mul2;
		_mul3 = rhs._mul3;
		_mul4 = rhs._mul4;
		_mul5 = rhs._mul5;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingSinSuperposition::~ScriptingSinSuperposition (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSinSuperposition::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSinSuperposition::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_mul1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_mul2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_mul3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_mul4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_mul5, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSinSuperposition::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		_out =  std::sin(_mul1 * _in) + 
                std::sin(_mul2 * _in) + 
                std::sin(_mul3 * _in) + 
                std::sin(_mul4 * _in) + 
                std::sin(_mul5 * _in);
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


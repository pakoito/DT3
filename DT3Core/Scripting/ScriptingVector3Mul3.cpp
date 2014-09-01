//==============================================================================
///	
///	File: ScriptingVector3Mul3.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Mul3.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Mul3,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Mul3)

IMPLEMENT_PLUG_INFO_INDEX(_in1)
IMPLEMENT_PLUG_INFO_INDEX(_in_mul_x)
IMPLEMENT_PLUG_INFO_INDEX(_in_mul_y)
IMPLEMENT_PLUG_INFO_INDEX(_in_mul_z)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Mul3)

	PLUG_INIT(_in1,"In_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_in_mul_x,"In_Mul_X")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_mul_y,"In_Mul_Y")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_mul_z,"In_Mul_Z")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Mul3::ScriptingVector3Mul3 (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), Vector3(0.0F,0.0F,0.0F)),
		_in_mul_x		(PLUG_INFO_INDEX(_in_mul_x), 1.0F),
		_in_mul_y		(PLUG_INFO_INDEX(_in_mul_y), 1.0F),
		_in_mul_z		(PLUG_INFO_INDEX(_in_mul_z), 1.0F),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingVector3Mul3::ScriptingVector3Mul3 (const ScriptingVector3Mul3 &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in_mul_x		(rhs._in_mul_x),
		_in_mul_y		(rhs._in_mul_y),
		_in_mul_z		(rhs._in_mul_z),
		_out			(rhs._out)
{   

}

ScriptingVector3Mul3 & ScriptingVector3Mul3::operator = (const ScriptingVector3Mul3 &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in_mul_x = rhs._in_mul_x;
		_in_mul_y = rhs._in_mul_y;
		_in_mul_z = rhs._in_mul_z;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingVector3Mul3::~ScriptingVector3Mul3 (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Mul3::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Mul3::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_mul_x, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_mul_y, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_mul_z, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector3Mul3::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
        Vector3 in1 = _in1;
    
		_out = Vector3(in1.x * _in_mul_x, in1.y * _in_mul_y, in1.z * _in_mul_z);
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


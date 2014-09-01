//==============================================================================
///	
///	File: ScriptingVector3Clip.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Clip.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Clip,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Clip)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_in_plane)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Clip)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_in_plane,"In_Plane")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Clip::ScriptingVector3Clip (void)
    :   _in				(PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_in_plane		(PLUG_INFO_INDEX(_in_plane), Plane(1.0F,0.0F,0.0F,0.0F)),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingVector3Clip::ScriptingVector3Clip (const ScriptingVector3Clip &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_in_plane		(rhs._in_plane),
		_out			(rhs._out)
{   

}

ScriptingVector3Clip & ScriptingVector3Clip::operator = (const ScriptingVector3Clip &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_in_plane = rhs._in_plane;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingVector3Clip::~ScriptingVector3Clip (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Clip::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Clip::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_plane, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector3Clip::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
	
		// Solve plane equation for x
		DTfloat xpos = (-_in_plane->D() - _in_plane->normal().y * _in->y - _in_plane->normal().z * _in->z) / _in_plane->normal().x;
	
		_out = Vector3(xpos, _in->y, _in->z);
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


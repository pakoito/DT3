//==============================================================================
///	
///	File: ScriptingVector3Transform.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Transform.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Transform,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Transform)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_translation)
IMPLEMENT_PLUG_INFO_INDEX(_orientation)
IMPLEMENT_PLUG_INFO_INDEX(_scale)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Transform)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_translation,"Translation")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_orientation,"Orientation")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_scale,"Scale")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Transform::ScriptingVector3Transform (void)
    :   _in             (PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_translation	(PLUG_INFO_INDEX(_translation), Vector3(0.0F,0.0F,0.0F)),
		_orientation	(PLUG_INFO_INDEX(_orientation), Matrix3   (1.0F,  0.0F,   0.0F,
                                                             0.0F,  1.0F,   0.0F,
                                                             0.0F,  0.0F,   1.0F)),
		_scale			(PLUG_INFO_INDEX(_scale), 1.0F),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingVector3Transform::ScriptingVector3Transform (const ScriptingVector3Transform &rhs)
    :   ScriptingBase	(rhs),
		_in             (rhs._in),
		_translation	(rhs._translation),
		_orientation	(rhs._orientation),
		_scale			(rhs._scale),
		_out			(rhs._out)
{   

}

ScriptingVector3Transform & ScriptingVector3Transform::operator = (const ScriptingVector3Transform &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_translation = rhs._translation;
		_orientation = rhs._orientation;
		_scale = rhs._scale;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingVector3Transform::~ScriptingVector3Transform (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Transform::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Transform::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_translation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_orientation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_scale, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector3Transform::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
        Matrix4 transform(_orientation, _translation, _scale);
    
		_out = transform * _in;
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


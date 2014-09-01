//==============================================================================
///	
///	File: ScriptingOrientTowards.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingOrientTowards.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingOrientTowards,"MatrixMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingOrientTowards)

IMPLEMENT_PLUG_INFO_INDEX(_direction)
IMPLEMENT_PLUG_INFO_INDEX(_up)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingOrientTowards)

	PLUG_INIT(_direction,"Direction")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_up,"Up")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingOrientTowards::ScriptingOrientTowards (void)
    :   _direction      (PLUG_INFO_INDEX(_direction), Vector3(0.0F,0.0F,-1.0F)),
		_up             (PLUG_INFO_INDEX(_up), Vector3(0.0F,1.0F,0.0F)),
		_out			(PLUG_INFO_INDEX(_out), Matrix3::identity())
{  

}
		
ScriptingOrientTowards::ScriptingOrientTowards (const ScriptingOrientTowards &rhs)
    :   ScriptingBase	(rhs),
		_direction      (rhs._direction),
		_up             (rhs._up),
		_out			(rhs._out)
{   

}

ScriptingOrientTowards & ScriptingOrientTowards::operator = (const ScriptingOrientTowards &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_direction = rhs._direction;
		_up = rhs._up;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingOrientTowards::~ScriptingOrientTowards (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingOrientTowards::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingOrientTowards::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_direction, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_up, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingOrientTowards::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
        Matrix3 orientation;
        orientation = Matrix3::set_orientation (-_direction,  _up);
    
        _out = orientation;
		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


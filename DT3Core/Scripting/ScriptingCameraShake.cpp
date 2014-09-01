//==============================================================================
///	
///	File: ScriptingCameraShake.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingCameraShake.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingCameraShake,"Camera",NULL)
IMPLEMENT_PLUG_NODE(ScriptingCameraShake)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_magnitude)
IMPLEMENT_PLUG_INFO_INDEX(_on)
IMPLEMENT_PLUG_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingCameraShake)
		
	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_magnitude,"Magnitude")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_on,"On")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingCameraShake::ScriptingCameraShake (void)
    :   _in				(PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_magnitude		(PLUG_INFO_INDEX(_magnitude), 0.5F),
		_on				(PLUG_INFO_INDEX(_on), false),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingCameraShake::ScriptingCameraShake (const ScriptingCameraShake &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_magnitude		(rhs._magnitude),
		_on				(rhs._on),
		_out			(rhs._out)
{   

}

ScriptingCameraShake & ScriptingCameraShake::operator = (const ScriptingCameraShake &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_magnitude = rhs._magnitude;
		_on = rhs._on;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingCameraShake::~ScriptingCameraShake (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingCameraShake::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingCameraShake::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_magnitude, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_on, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingCameraShake::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (_on) {
        _out.set_dirty();
    }
}

//==============================================================================
//==============================================================================

DTboolean ScriptingCameraShake::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
    
        if (_on) {
            DTfloat theta = 2.0F * std::acos(std::sqrt(1.0F - MoreMath::random_float()));
            DTfloat phi = TWO_PI * MoreMath::random_float();
        
            DTfloat sin_theta = std::sin(theta);
            
            _out = (_in) + Vector3(sin_theta * std::cos(phi), sin_theta * std::sin(phi), std::cos(theta)) * (_magnitude);
        } else {
            _out = _in;
        }
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

void ScriptingCameraShake::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingCameraShake::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


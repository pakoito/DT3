//==============================================================================
///	
///	File: ScriptingParticleAccelerator.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleAccelerator.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleAccelerator,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleAccelerator)

IMPLEMENT_PLUG_INFO_INDEX(_acceleration)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleAccelerator)

	PLUG_INIT(_acceleration,"Acceleration")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleAccelerator::ScriptingParticleAccelerator (void)
    :   _acceleration	(PLUG_INFO_INDEX(_acceleration), Vector3(0.0F,0.0F,0.0F)),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleAccelerator::ScriptingParticleAccelerator (const ScriptingParticleAccelerator &rhs)
    :   ScriptingBase	(rhs),
		_acceleration	(rhs._acceleration),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticleAccelerator & ScriptingParticleAccelerator::operator = (const ScriptingParticleAccelerator &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_acceleration = rhs._acceleration;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleAccelerator::~ScriptingParticleAccelerator (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleAccelerator::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleAccelerator::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_acceleration, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingParticleAccelerator::tick (const DTfloat dt)
{
	PROFILER(PARTICLES);

    // Make sure there are input particles
    std::shared_ptr<Particles> particles = _in;
    if (!particles || particles->translations_stream().size() <= 0) {
        _out.set_clean();
        return;
    }
        
    // Build the velocities stream
    if (particles->velocity_stream().size() <= 0) {
        particles->build_velocity_stream();
    }
    
    // Do processing
    std::vector<Vector3> &velocities = particles->velocity_stream();

    for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
        velocities[i] += _acceleration * dt;
    }

    _out = particles;

}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleAccelerator::compute (const PlugBase *plug)
{
	PROFILER(PARTICLES);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		
		_out = _in;
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

void ScriptingParticleAccelerator::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingParticleAccelerator::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


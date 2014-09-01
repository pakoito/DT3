//==============================================================================
///	
///	File: ScriptingParticleVelocityWake.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleVelocityWake.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleVelocityWake,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleVelocityWake)

IMPLEMENT_PLUG_INFO_INDEX(_up)
IMPLEMENT_PLUG_INFO_INDEX(_in_velocity)
IMPLEMENT_PLUG_INFO_INDEX(_scale)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleVelocityWake)

	PLUG_INIT(_up,"Up")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_velocity,"In_Velocity")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_scale,"Scale")
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

ScriptingParticleVelocityWake::ScriptingParticleVelocityWake (void)
    :   _up				(PLUG_INFO_INDEX(_up), Vector3(0.0F,1.0F,0.0F)),
		_in_velocity	(PLUG_INFO_INDEX(_in_velocity), Vector3(0.0F,0.0F,0.0F)),
		_scale			(PLUG_INFO_INDEX(_scale), 1.0F),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleVelocityWake::ScriptingParticleVelocityWake (const ScriptingParticleVelocityWake &rhs)
    :   ScriptingBase	(rhs),
		_up				(rhs._up),
		_in_velocity	(rhs._in_velocity),
		_scale			(rhs._scale),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticleVelocityWake & ScriptingParticleVelocityWake::operator = (const ScriptingParticleVelocityWake &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_up = rhs._up;
		_in_velocity = rhs._in_velocity;
		_scale = rhs._scale;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleVelocityWake::~ScriptingParticleVelocityWake (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityWake::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityWake::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_up, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_velocity, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_scale, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleVelocityWake::compute (const PlugBase *plug)
{
	PROFILER(PARTICLES);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		
		// Make sure there are input particles
		std::shared_ptr<Particles> particles = _in;
		if (!particles || particles->translations_stream().size() <= 0) {
			_out.set_clean();
            return true;
		}
			
		// Build the velocities stream
		if (particles->velocity_stream().size() <= 0) {
			particles->build_velocity_stream();
		}
		
		// Do processing
        std::vector<Vector3> &velocities = particles->velocity_stream();
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			if (lifetimes[i] == 0.0F) {
				Vector3 dir;
				if (i%2 == 0) {
					dir = Vector3::cross(_in_velocity,_up);
				} else {
					dir = Vector3::cross(_up,_in_velocity);
				}
			
				velocities[i] += dir * _scale;
			}
		}

		_out = particles;
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


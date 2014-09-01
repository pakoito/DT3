//==============================================================================
///	
///	File: ScriptingParticleVelocityAdder.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleVelocityAdder.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleVelocityAdder,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleVelocityAdder)

IMPLEMENT_PLUG_INFO_INDEX(_velocity)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleVelocityAdder)

	PLUG_INIT(_velocity,"Velocity")
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

ScriptingParticleVelocityAdder::ScriptingParticleVelocityAdder (void)
    :   _continuous		(false),
		_velocity		(PLUG_INFO_INDEX(_velocity), Vector3(0.0F,0.0F,0.0F)),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleVelocityAdder::ScriptingParticleVelocityAdder (const ScriptingParticleVelocityAdder &rhs)
    :   ScriptingBase	(rhs),
		_continuous		(rhs._continuous),
		_velocity		(rhs._velocity),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticleVelocityAdder & ScriptingParticleVelocityAdder::operator = (const ScriptingParticleVelocityAdder &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_continuous = rhs._continuous;
		_velocity = rhs._velocity;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleVelocityAdder::~ScriptingParticleVelocityAdder (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityAdder::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityAdder::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_continuous, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_velocity, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleVelocityAdder::compute (const PlugBase *plug)
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
			if (_continuous || lifetimes[i] == 0.0F) {
				velocities[i] += _velocity;
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

#ifdef DT3_EDITOR

void ScriptingParticleVelocityAdder::dump_code(const std::string &object_name, Stream &s)
{
	PROFILER(PARTICLES);

    s << object_name << "->set_continuous(" << _continuous << ");\n";
    s << object_name << "->set_velocity(Vector3(" << _velocity->x << "," << _velocity->y << "," << _velocity->z << "));\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


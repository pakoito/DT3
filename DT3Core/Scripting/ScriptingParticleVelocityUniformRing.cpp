//==============================================================================
///	
///	File: ScriptingParticleVelocityUniformRing.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleVelocityUniformRing.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleVelocityUniformRing,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleVelocityUniformRing)

IMPLEMENT_PLUG_INFO_INDEX(_normal)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleVelocityUniformRing)

	PLUG_INIT(_normal,"Normal")
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

ScriptingParticleVelocityUniformRing::ScriptingParticleVelocityUniformRing (void)
    :   _normal			(PLUG_INFO_INDEX(_normal), Vector3(0.0F,1.0F,0.0F)),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out)),
		_velocity		(1.0F)
{  

}
		
ScriptingParticleVelocityUniformRing::ScriptingParticleVelocityUniformRing (const ScriptingParticleVelocityUniformRing &rhs)
    :   ScriptingBase	(rhs),
		_normal			(rhs._normal),
		_in				(rhs._in),
		_out			(rhs._out),
		_velocity		(rhs._velocity)
{   

}

ScriptingParticleVelocityUniformRing & ScriptingParticleVelocityUniformRing::operator = (const ScriptingParticleVelocityUniformRing &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_normal = rhs._normal;
		_velocity = rhs._velocity;
	}
    return (*this);
}
			
ScriptingParticleVelocityUniformRing::~ScriptingParticleVelocityUniformRing (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityUniformRing::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityUniformRing::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_normal, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_velocity, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleVelocityUniformRing::compute (const PlugBase *plug)
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
				Matrix3 rot;
				
				Vector3 v(0.0F,1.0F,0.0F);
				v = Vector3::cross(v,_normal);
				v.normalize();
				
				rot = Matrix3::set_rotation_around(_normal, i * 2.0F * PI / (velocities.size()-1));	// Minus 1 because there's always one item free in the particles
				v = rot * v;
			
				velocities[i] += v * _velocity;
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


//==============================================================================
///	
///	File: ScriptingParticleDirectionalVelocityRandomizer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleDirectionalVelocityRandomizer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleDirectionalVelocityRandomizer,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleDirectionalVelocityRandomizer)

IMPLEMENT_PLUG_INFO_INDEX(_rand_scale_perp)
IMPLEMENT_PLUG_INFO_INDEX(_rand_scale_tangent)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleDirectionalVelocityRandomizer)

	PLUG_INIT(_rand_scale_perp,"Rand_Scale_Perp")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_rand_scale_tangent,"Rand_Scale_Tangent")
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

ScriptingParticleDirectionalVelocityRandomizer::ScriptingParticleDirectionalVelocityRandomizer (void)
    :   _continuous			(false),
		_rand_scale_perp	(PLUG_INFO_INDEX(_rand_scale_perp), 1.0F),
		_rand_scale_tangent	(PLUG_INFO_INDEX(_rand_scale_tangent), 1.0F),
		_in					(PLUG_INFO_INDEX(_in)),
		_out				(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleDirectionalVelocityRandomizer::ScriptingParticleDirectionalVelocityRandomizer (const ScriptingParticleDirectionalVelocityRandomizer &rhs)
    :   ScriptingBase		(rhs),
		_continuous			(rhs._continuous),
		_rand_scale_perp	(rhs._rand_scale_perp),
		_rand_scale_tangent	(rhs._rand_scale_tangent),
		_in					(rhs._in),
		_out				(rhs._out)
{   

}

ScriptingParticleDirectionalVelocityRandomizer & ScriptingParticleDirectionalVelocityRandomizer::operator = (const ScriptingParticleDirectionalVelocityRandomizer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_continuous = rhs._continuous;
		_rand_scale_perp = rhs._rand_scale_perp;
		_rand_scale_tangent = rhs._rand_scale_tangent;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleDirectionalVelocityRandomizer::~ScriptingParticleDirectionalVelocityRandomizer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleDirectionalVelocityRandomizer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleDirectionalVelocityRandomizer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_continuous, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_rand_scale_perp, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_rand_scale_tangent, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleDirectionalVelocityRandomizer::compute (const PlugBase *plug)
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
				Vector3 dir = velocities[i].normalized();
				
				Vector3 tangent_component = dir * Vector3::dot(dir, velocities[i]);
				Vector3 perp_component = velocities[i] - tangent_component;
			
				velocities[i] =	_rand_scale_perp * MoreMath::random_float() * perp_component +
                                _rand_scale_tangent * MoreMath::random_float() * tangent_component;
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


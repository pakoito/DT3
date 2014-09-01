//==============================================================================
///	
///	File: ScriptingParticleCollisionPlane.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleCollisionPlane.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/Math/Plane.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleCollisionPlane,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleCollisionPlane)

IMPLEMENT_PLUG_INFO_INDEX(_normal)
IMPLEMENT_PLUG_INFO_INDEX(_d)
IMPLEMENT_PLUG_INFO_INDEX(_restitution)
IMPLEMENT_PLUG_INFO_INDEX(_friction_damping)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleCollisionPlane)

	PLUG_INIT(_normal,"Normal")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_d,"D")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_restitution,"Restitution")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_friction_damping,"Friction_Damping")
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

ScriptingParticleCollisionPlane::ScriptingParticleCollisionPlane (void)
    :   _normal				(PLUG_INFO_INDEX(_normal), Vector3(0.0F,1.0F,0.0F)),
		_d					(PLUG_INFO_INDEX(_d), 0.0F),
		_restitution		(PLUG_INFO_INDEX(_restitution), 0.1F),
		_friction_damping	(PLUG_INFO_INDEX(_friction_damping), 0.9F),
		_in					(PLUG_INFO_INDEX(_in)),
		_out				(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleCollisionPlane::ScriptingParticleCollisionPlane (const ScriptingParticleCollisionPlane &rhs)
    :   ScriptingBase		(rhs),
		_normal				(rhs._normal),
		_d					(rhs._d),
		_restitution		(rhs._restitution),
		_friction_damping	(rhs._friction_damping),
		_in					(rhs._in),
		_out				(rhs._out)
{   

}

ScriptingParticleCollisionPlane & ScriptingParticleCollisionPlane::operator = (const ScriptingParticleCollisionPlane &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_normal = rhs._normal;
		_d = rhs._d;
		_restitution = rhs._restitution;
		_friction_damping = rhs._friction_damping;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleCollisionPlane::~ScriptingParticleCollisionPlane (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleCollisionPlane::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleCollisionPlane::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_normal, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_d, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_restitution, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_friction_damping, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleCollisionPlane::compute (const PlugBase *plug)
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
        std::vector<Vector3> &translations = particles->translations_stream();
        std::vector<Vector3> &velocities = particles->velocity_stream();
		
		Vector3 normal = _normal;
		Plane p(normal.x, normal.y, normal.z, _d);

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			Vector3 &pt = translations[i];
			Vector3 &vel = velocities[i];
			
			// If there's a penetration
			DTfloat dist = p.distance_to_point(pt);
			if (dist <= 0.0F) {
				DTfloat vel_norm_mag = Vector3::dot(_normal, vel);
				
				if (vel_norm_mag <= 0.0F) {
					// Remove penetration
					pt = pt - _normal * dist;
					
					// Apply friction
					Vector3 vel_norm = _normal * vel_norm_mag;
					Vector3 vel_perp = vel - vel_norm;
					vel_perp *= _friction_damping;
					vel = vel_perp + vel_norm;
					
					// Do reflection
					Vector3::reflect(vel, _normal, _restitution, vel);
				}
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


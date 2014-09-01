//==============================================================================
///	
///	File: ScriptingParticleVelocityRing.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleVelocityRing.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleVelocityRing,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleVelocityRing)

IMPLEMENT_PLUG_INFO_INDEX(_normal)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleVelocityRing)

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

ScriptingParticleVelocityRing::ScriptingParticleVelocityRing (void)
    :   _normal			(PLUG_INFO_INDEX(_normal), Vector3(0.0F,1.0F,0.0F)),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out)),
		_velocity		(1.0F),
		_variation		(0.0F)
{  

}
		
ScriptingParticleVelocityRing::ScriptingParticleVelocityRing (const ScriptingParticleVelocityRing &rhs)
    :   ScriptingBase	(rhs),
		_normal			(rhs._normal),
		_in				(rhs._in),
		_out			(rhs._out),
		_velocity		(rhs._velocity),
		_variation		(rhs._variation)
{   

}

ScriptingParticleVelocityRing & ScriptingParticleVelocityRing::operator = (const ScriptingParticleVelocityRing &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_normal = rhs._normal;
		_velocity = rhs._velocity;
		_variation = rhs._variation;
	}
    return (*this);
}
			
ScriptingParticleVelocityRing::~ScriptingParticleVelocityRing (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityRing::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityRing::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_normal, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_velocity, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_variation, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleVelocityRing::compute (const PlugBase *plug)
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
        
        Vector3 n = _normal->normalized();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			if (lifetimes[i] == 0.0F) {                
                Vector3 v = MoreMath::random_vector();
                v = v - n * Vector3::dot(n, v);
                
                if (v.abs2() > 0.0F)
                    v.normalize();
							
				velocities[i] += v * (_velocity + MoreMath::random_float() * _variation);
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

void ScriptingParticleVelocityRing::dump_code(const std::string &object_name, Stream &s)
{
	PROFILER(PARTICLES);

    s << object_name << "->set_normal(Vector3(" << _normal->x << "," << _normal->y << "," << _normal->z << "));\n";
    s << object_name << "->set_velocity(" << _velocity << ");\n";
    s << object_name << "->set_velocity_variation(" << _variation << ");\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


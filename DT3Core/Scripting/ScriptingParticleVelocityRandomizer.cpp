//==============================================================================
///	
///	File: ScriptingParticleVelocityRandomizer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleVelocityRandomizer.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleVelocityRandomizer,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleVelocityRandomizer)

IMPLEMENT_PLUG_INFO_INDEX(_rand_scale_x)
IMPLEMENT_PLUG_INFO_INDEX(_rand_scale_y)
IMPLEMENT_PLUG_INFO_INDEX(_rand_scale_z)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleVelocityRandomizer)

	PLUG_INIT(_rand_scale_x,"Rand_Scale_X")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_rand_scale_y,"Rand_Scale_Y")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_rand_scale_z,"Rand_Scale_Z")
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

ScriptingParticleVelocityRandomizer::ScriptingParticleVelocityRandomizer (void)
    :   _continuous		(false),
		_rand_scale_x	(PLUG_INFO_INDEX(_rand_scale_x), 1.0F),
		_rand_scale_y	(PLUG_INFO_INDEX(_rand_scale_y), 1.0F),
		_rand_scale_z	(PLUG_INFO_INDEX(_rand_scale_z), 1.0F),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleVelocityRandomizer::ScriptingParticleVelocityRandomizer (const ScriptingParticleVelocityRandomizer &rhs)
    :   ScriptingBase	(rhs),
		_continuous		(rhs._continuous),
		_rand_scale_x	(rhs._rand_scale_x),
		_rand_scale_y	(rhs._rand_scale_y),
		_rand_scale_z	(rhs._rand_scale_z),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticleVelocityRandomizer & ScriptingParticleVelocityRandomizer::operator = (const ScriptingParticleVelocityRandomizer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_continuous = rhs._continuous;
		_rand_scale_x = rhs._rand_scale_x;
		_rand_scale_y = rhs._rand_scale_y;
		_rand_scale_z = rhs._rand_scale_z;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleVelocityRandomizer::~ScriptingParticleVelocityRandomizer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityRandomizer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleVelocityRandomizer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_continuous, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_rand_scale_x, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_rand_scale_y, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_rand_scale_z, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleVelocityRandomizer::compute (const PlugBase *plug)
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
            
                Vector3 v = MoreMath::random_vector();
				velocities[i].x += v.x * _rand_scale_x;
				velocities[i].y += v.y * _rand_scale_y;
				velocities[i].z += v.z * _rand_scale_z;
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

void ScriptingParticleVelocityRandomizer::dump_code(const std::string &object_name, Stream &s)
{
	PROFILER(PARTICLES);

    s << object_name << "->set_continuous(" << _continuous << ");\n";
    s << object_name << "->set_rand_scale_x(" << _rand_scale_x << ");\n";
    s << object_name << "->set_rand_scale_y(" << _rand_scale_y << ");\n";
    s << object_name << "->set_rand_scale_z(" << _rand_scale_z << ");\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


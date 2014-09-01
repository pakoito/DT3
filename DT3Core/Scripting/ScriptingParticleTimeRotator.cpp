//==============================================================================
///	
///	File: ScriptingParticleTimeRotator.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleTimeRotator.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleTimeRotator,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleTimeRotator)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleTimeRotator)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleTimeRotator::ScriptingParticleTimeRotator (void)
    :   _rotation_speed_mul	(3.0F),
		_rotation_phase_mul	(1.0F),
		_in					(PLUG_INFO_INDEX(_in)),
		_out				(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleTimeRotator::ScriptingParticleTimeRotator (const ScriptingParticleTimeRotator &rhs)
    :   ScriptingBase		(rhs),
		_rotation_speed_mul	(rhs._rotation_speed_mul),
		_rotation_phase_mul	(rhs._rotation_phase_mul),
		_in					(rhs._in),
		_out				(rhs._out)
{   

}

ScriptingParticleTimeRotator & ScriptingParticleTimeRotator::operator = (const ScriptingParticleTimeRotator &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_rotation_speed_mul = rhs._rotation_speed_mul;
		_rotation_phase_mul = rhs._rotation_phase_mul;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleTimeRotator::~ScriptingParticleTimeRotator (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleTimeRotator::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleTimeRotator::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_rotation_speed_mul, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_rotation_phase_mul, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleTimeRotator::compute (const PlugBase *plug)
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
		if (particles->rotations_stream().size() <= 0) {
			particles->build_rotations_stream();
		}
        
        DTint rs = MoreMath::random_seed();

		// Do processing
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();
		std::vector<DTfloat> &rotations = particles->rotations_stream();
		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			DTfloat time = lifetimes[i];
			DTfloat &rotation = rotations[i];
			
			// Generate random number
			MoreMath::set_random_seed(i);
			DTfloat rand1 = MoreMath::random_float() - 0.5F;
			DTfloat rand2 = MoreMath::random_float();
			
			rotation = (rand1 * time * _rotation_speed_mul) + (rand2 * _rotation_phase_mul);
		}
        
        MoreMath::set_random_seed(rs);

		_out = particles;
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

#ifdef DT3_EDITOR

void ScriptingParticleTimeRotator::dump_code(const std::string &object_name, Stream &s)
{
	PROFILER(PARTICLES);

    s << object_name << "->set_rotation_speed_mul(" << _rotation_speed_mul << ");\n";
    s << object_name << "->set_rotation_phase_mul(" << _rotation_phase_mul << ");\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


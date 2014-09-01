//==============================================================================
///	
///	File: ScriptingParticlePointAttractor.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticlePointAttractor.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticlePointAttractor,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticlePointAttractor)

IMPLEMENT_PLUG_INFO_INDEX(_target)
IMPLEMENT_PLUG_INFO_INDEX(_strength)
IMPLEMENT_PLUG_INFO_INDEX(_converge_time)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticlePointAttractor)

	PLUG_INIT(_strength,"Strength")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_converge_time,"Converge_Time")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_target,"Target")
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

ScriptingParticlePointAttractor::ScriptingParticlePointAttractor (void)
    :   _target			(PLUG_INFO_INDEX(_target), Vector3(0.0F,0.0F,0.0F)),
		_strength		(PLUG_INFO_INDEX(_strength), 10.0F),
		_converge_time	(PLUG_INFO_INDEX(_converge_time), 1.0F),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticlePointAttractor::ScriptingParticlePointAttractor (const ScriptingParticlePointAttractor &rhs)
    :   ScriptingBase	(rhs),
		_target			(rhs._target),
		_strength		(rhs._strength),
		_converge_time	(rhs._converge_time),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticlePointAttractor & ScriptingParticlePointAttractor::operator = (const ScriptingParticlePointAttractor &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_target = rhs._target;
		_strength = rhs._strength;
		_converge_time = rhs._converge_time;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticlePointAttractor::~ScriptingParticlePointAttractor (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticlePointAttractor::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticlePointAttractor::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_target, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_strength, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_converge_time, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticlePointAttractor::compute (const PlugBase *plug)
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
		//ArrayBlock<Vector3>* velocities = particles->velocity_stream();
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();

		DTfloat converge_time_inv = 1.0F / _converge_time;

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			DTfloat t = lifetimes[i] * converge_time_inv;
			t = std::pow(t,_strength);
			
			translations[i] =	translations[i] * (1.0F - t) + _target * t;
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


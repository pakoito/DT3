//==============================================================================
///	
///	File: ScriptingParticleKillZone.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleKillZone.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleKillZone,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleKillZone)

IMPLEMENT_PLUG_INFO_INDEX(_translation)
IMPLEMENT_PLUG_INFO_INDEX(_radius)
IMPLEMENT_PLUG_INFO_INDEX(_num_killed)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleKillZone)

	PLUG_INIT(_translation,"Translation")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_radius,"Radius")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_num_killed,"Num_Killed")
		.set_output(true);

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleKillZone::ScriptingParticleKillZone (void)
    :   _translation		(PLUG_INFO_INDEX(_translation), Vector3(0.0F,0.0F,0.0F)),
		_radius				(PLUG_INFO_INDEX(_radius), 1.0F),
		_num_killed         (PLUG_INFO_INDEX(_num_killed), 0),
		_in					(PLUG_INFO_INDEX(_in)),
		_out				(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleKillZone::ScriptingParticleKillZone (const ScriptingParticleKillZone &rhs)
    :   ScriptingBase		(rhs),
		_translation			(rhs._translation),
		_radius				(rhs._radius),
		_num_killed         (rhs._num_killed),
		_in					(rhs._in),
		_out				(rhs._out)
{   

}

ScriptingParticleKillZone & ScriptingParticleKillZone::operator = (const ScriptingParticleKillZone &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_translation = rhs._translation;
		_radius = rhs._radius;
		_num_killed = rhs._num_killed;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleKillZone::~ScriptingParticleKillZone (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleKillZone::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleKillZone::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_translation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_radius, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_num_killed, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleKillZone::compute (const PlugBase *plug)
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
					
        // Build the sizes stream
		if (particles->sizes_stream().size() <= 0) {
			particles->build_sizes_stream();
		}

		// Do processing
        std::vector<Vector3> &translations = particles->translations_stream();
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();
		std::vector<DTfloat> sizes = particles->sizes_stream();
		        
        DTfloat r2 = _radius * _radius;
        DTint num_killed = 0;

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			Vector3 &pt = translations[i];
			DTfloat &lifetime = lifetimes[i];
			DTfloat &size = sizes[i];
			
            Vector3 diff = (pt - _translation);
            if (diff.abs2() < r2) {
                // Lifetime to infinity and size to 0
                lifetime = std::numeric_limits<DTfloat>::infinity();
                size = 0.0F;
                ++num_killed;
            }
		}

        _num_killed = num_killed;
		_num_killed.set_clean();

		_out = particles;
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


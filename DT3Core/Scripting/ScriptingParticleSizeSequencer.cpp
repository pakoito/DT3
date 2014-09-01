//==============================================================================
///	
///	File: ScriptingParticleSizeSequencer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleSizeSequencer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleSizeSequencer,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleSizeSequencer)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleSizeSequencer)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleSizeSequencer::ScriptingParticleSizeSequencer (void)
    :   _in			(PLUG_INFO_INDEX(_in)),
		_out		(PLUG_INFO_INDEX(_out))
{  
	for (DTint i = 0; i < NUM_ENTRIES; ++i) {
		_t[i] = i;
		_s[i] = 1.0F;
	}
	
	// Last time to a big number
	_t[NUM_ENTRIES-1] = 100000.0F;
}
		
ScriptingParticleSizeSequencer::ScriptingParticleSizeSequencer (const ScriptingParticleSizeSequencer &rhs)
    :   ScriptingBase		(rhs),
		_in					(rhs._in),
		_out				(rhs._out)
{   
	for (DTint i = 0; i < NUM_ENTRIES; ++i) {
		_t[i] = rhs._t[i];
		_s[i] = rhs._s[i];
	}
}

ScriptingParticleSizeSequencer & ScriptingParticleSizeSequencer::operator = (const ScriptingParticleSizeSequencer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		for (DTint i = 0; i < NUM_ENTRIES; ++i) {
			_t[i] = rhs._t[i];
			_s[i] = rhs._s[i];
		}
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleSizeSequencer::~ScriptingParticleSizeSequencer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleSizeSequencer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleSizeSequencer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
		
	for (DTint i = 0; i < NUM_ENTRIES; ++i) {
		*archive << ARCHIVE_DATA_NAMED("Time" + MoreStrings::cast_to_string(i), _t[i], DATA_PERSISTENT | DATA_SETTABLE);
		*archive << ARCHIVE_DATA_NAMED("size" + MoreStrings::cast_to_string(i), _s[i], DATA_PERSISTENT | DATA_SETTABLE);
	}
	        
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleSizeSequencer::compute (const PlugBase *plug)
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
		
		// allocate the cache
		if (_cache.size() != particles->translations_stream().size())
			_cache.resize(particles->translations_stream().size(), 0);
		
		// Do processing
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();
		std::vector<DTfloat> &sizes = particles->sizes_stream();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			DTfloat &lifetime = lifetimes[i];
			DTint &cache = _cache[i];
			DTfloat &size = sizes[i];
			
			// If lifetime is less than the cache entry, just start over
			if (lifetime < _t[cache]) {
				cache = 0;
			}
			
			// Search the cache
			while (cache < NUM_ENTRIES-1 && lifetime >= _t[cache+1]) {
				++cache;
			}
		
			// lerp the color value
			DTfloat lerp = (lifetime - _t[cache]) / (_t[cache+1] - _t[cache]);
			DTfloat one_minus_lerp = 1.0F - lerp;
			
			size = _s[cache] * one_minus_lerp + _s[cache+1] * lerp;

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


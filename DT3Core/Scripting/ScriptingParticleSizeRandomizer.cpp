//==============================================================================
///	
///	File: ScriptingParticleSizeRandomizer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleSizeRandomizer.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleSizeRandomizer,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleSizeRandomizer)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleSizeRandomizer)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleSizeRandomizer::ScriptingParticleSizeRandomizer (void)
    :   _in			(PLUG_INFO_INDEX(_in)),
		_out		(PLUG_INFO_INDEX(_out)),
        _min_size   (1.0F),
        _max_size   (2.0F),
        _continuous (false)
{  

}
		
ScriptingParticleSizeRandomizer::ScriptingParticleSizeRandomizer (const ScriptingParticleSizeRandomizer &rhs)
    :   ScriptingBase		(rhs),
		_in					(rhs._in),
		_out				(rhs._out),
        _min_size           (rhs._min_size),
        _max_size           (rhs._max_size),
        _continuous         (rhs._continuous)
{   

}

ScriptingParticleSizeRandomizer & ScriptingParticleSizeRandomizer::operator = (const ScriptingParticleSizeRandomizer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);
        
        _min_size = rhs._min_size;
        _max_size = rhs._max_size;
        _continuous = rhs._continuous;

		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleSizeRandomizer::~ScriptingParticleSizeRandomizer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleSizeRandomizer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleSizeRandomizer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_DATA(_continuous, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_min_size, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_max_size, DATA_PERSISTENT | DATA_SETTABLE);
	        
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleSizeRandomizer::compute (const PlugBase *plug)
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
		std::vector<DTfloat> &sizes = particles->sizes_stream();
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();

        DTint rs = MoreMath::random_seed();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			if (_continuous || lifetimes[i] == 0.0F) {
                DTfloat &size = sizes[i];
                
                // Generate random number
                MoreMath::set_random_seed(i);
                size = MoreMath::random_float() * (_max_size - _min_size) + _min_size;
            }
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

} // DT3


//==============================================================================
///	
///	File: ScriptingParticlePositionRandomizer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ScriptingParticlePositionRandomizer.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticlePositionRandomizer,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticlePositionRandomizer)

IMPLEMENT_PLUG_INFO_INDEX(_min_x)
IMPLEMENT_PLUG_INFO_INDEX(_max_x)
IMPLEMENT_PLUG_INFO_INDEX(_min_y)
IMPLEMENT_PLUG_INFO_INDEX(_max_y)
IMPLEMENT_PLUG_INFO_INDEX(_min_z)
IMPLEMENT_PLUG_INFO_INDEX(_max_z)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticlePositionRandomizer)

	PLUG_INIT(_min_x,"MinX")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_max_x,"MaxX")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_min_y,"MinY")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_max_y,"MaxY")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_min_z,"MinZ")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_max_z,"MaxZ")
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

ScriptingParticlePositionRandomizer::ScriptingParticlePositionRandomizer (void)
    :   _continuous		(false),
		_min_x          (PLUG_INFO_INDEX(_min_x), -1.0F),
		_max_x          (PLUG_INFO_INDEX(_max_x), 1.0F),
		_min_y          (PLUG_INFO_INDEX(_min_y), -1.0F),
		_max_y          (PLUG_INFO_INDEX(_max_y), 1.0F),
		_min_z          (PLUG_INFO_INDEX(_min_z), -1.0F),
		_max_z          (PLUG_INFO_INDEX(_max_z), 1.0F),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticlePositionRandomizer::ScriptingParticlePositionRandomizer (const ScriptingParticlePositionRandomizer &rhs)
    :   ScriptingBase	(rhs),
		_continuous		(rhs._continuous),
		_min_x          (rhs._min_x),
		_max_x          (rhs._max_x),
		_min_y          (rhs._min_y),
		_max_y          (rhs._max_y),
		_min_z          (rhs._min_z),
		_max_z          (rhs._max_z),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticlePositionRandomizer & ScriptingParticlePositionRandomizer::operator = (const ScriptingParticlePositionRandomizer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_continuous = rhs._continuous;
		_min_x = rhs._min_x;
		_max_x = rhs._max_x;
		_min_y = rhs._min_y;
		_max_y = rhs._max_y;
		_min_z = rhs._min_z;
		_max_z = rhs._max_z;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticlePositionRandomizer::~ScriptingParticlePositionRandomizer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticlePositionRandomizer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticlePositionRandomizer::archive (const std::shared_ptr<Archive> &archive)
{
	archive->push_domain (class_id ());

    ScriptingBase::archive(archive);
	
	*archive << ARCHIVE_DATA(_continuous, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_min_x, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_max_x, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_min_y, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_max_y, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_min_z, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_max_z, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticlePositionRandomizer::compute (const PlugBase *plug)
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
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			if (_continuous || lifetimes[i] == 0.0F) {
				translations[i].x += MoreMath::random_float() * (_max_x - _min_x) + _min_x;
				translations[i].y += MoreMath::random_float() * (_max_y - _min_y) + _min_y;
				translations[i].z += MoreMath::random_float() * (_max_z - _min_z) + _min_z;
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


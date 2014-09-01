//==============================================================================
///	
///	File: ScriptingParticleColorRandomizer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleColorRandomizer.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleColorRandomizer,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleColorRandomizer)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleColorRandomizer)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleColorRandomizer::ScriptingParticleColorRandomizer (void)
    :   _r_mag		(0.1F),
		_g_mag		(0.1F),
		_b_mag		(0.1F),
		_a_mag		(0.0F),
		_in			(PLUG_INFO_INDEX(_in)),
		_out		(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleColorRandomizer::ScriptingParticleColorRandomizer (const ScriptingParticleColorRandomizer &rhs)
    :   ScriptingBase		(rhs),
		_r_mag				(rhs._r_mag),
		_g_mag				(rhs._g_mag),
		_b_mag				(rhs._b_mag),
		_a_mag				(rhs._a_mag),
		_in					(rhs._in),
		_out				(rhs._out)
{   

}

ScriptingParticleColorRandomizer & ScriptingParticleColorRandomizer::operator = (const ScriptingParticleColorRandomizer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_r_mag = rhs._r_mag;
		_g_mag = rhs._g_mag;
		_b_mag = rhs._b_mag;
		_a_mag = rhs._a_mag;

		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleColorRandomizer::~ScriptingParticleColorRandomizer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleColorRandomizer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleColorRandomizer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_r_mag, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_g_mag, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_b_mag, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_a_mag, DATA_PERSISTENT | DATA_SETTABLE);
		        
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleColorRandomizer::compute (const PlugBase *plug)
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
		
		// Build the color stream
		if (particles->color_stream().size() <= 0) {
			particles->build_colors_stream();
		}
				
		// Do processing
		std::vector<Color4b> &colors = particles->color_stream();
        
        DTint rs = MoreMath::random_seed();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			Color4b &color = colors[i];
			
			// Generate random number
			MoreMath::set_random_seed(i);
			DTfloat rm = (MoreMath::random_float() - 0.5F) * 2.0F * _r_mag;
			DTfloat gm = (MoreMath::random_float() - 0.5F) * 2.0F * _g_mag;
			DTfloat bm = (MoreMath::random_float() - 0.5F) * 2.0F * _b_mag;
			DTfloat am = (MoreMath::random_float() - 0.5F) * 2.0F * _a_mag;
			
			color.set_r( color.r_as_float() + rm);
			color.set_g( color.g_as_float() + gm);
			color.set_b( color.b_as_float() + bm);
			color.set_a( color.a_as_float() + am);
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


//==============================================================================
///	
///	File: ScriptingParticleColorSequencer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleColorSequencer.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleColorSequencer,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleColorSequencer)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleColorSequencer)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleColorSequencer::ScriptingParticleColorSequencer (void)
    :   _r_mask		(true),
		_g_mask		(true),
		_b_mask		(true),
		_a_mask		(true),
		_in			(PLUG_INFO_INDEX(_in)),
		_out		(PLUG_INFO_INDEX(_out))
{  
	for (DTint i = 0; i < NUM_ENTRIES; ++i) {
		_t[i] = i * 0.1F;
		_c[i] = Color4b (1.0F,1.0F,1.0F,1.0F);
	}
	
	// Last time to a big number
	_t[NUM_ENTRIES-1] = 100000.0F;
}
		
ScriptingParticleColorSequencer::ScriptingParticleColorSequencer (const ScriptingParticleColorSequencer &rhs)
    :   ScriptingBase		(rhs),
		_r_mask				(rhs._r_mask),
		_g_mask				(rhs._g_mask),
		_b_mask				(rhs._b_mask),
		_a_mask				(rhs._a_mask),
		_in					(rhs._in),
		_out				(rhs._out)
{   
	for (DTint i = 0; i < NUM_ENTRIES; ++i) {
		_t[i] = rhs._t[i];
		_c[i] = rhs._c[i];
	}
}

ScriptingParticleColorSequencer & ScriptingParticleColorSequencer::operator = (const ScriptingParticleColorSequencer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_r_mask = rhs._r_mask;
		_g_mask = rhs._g_mask;
		_b_mask = rhs._b_mask;
		_a_mask = rhs._a_mask;
		
		for (DTint i = 0; i < NUM_ENTRIES; ++i) {
			_t[i] = rhs._t[i];
			_c[i] = rhs._c[i];
		}

		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleColorSequencer::~ScriptingParticleColorSequencer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleColorSequencer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleColorSequencer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_r_mask, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_g_mask, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_b_mask, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_a_mask, DATA_PERSISTENT | DATA_SETTABLE);
	
	for (DTint i = 0; i < NUM_ENTRIES; ++i) {
		*archive << ARCHIVE_DATA_NAMED("Time" + MoreStrings::cast_to_string(i), _t[i], DATA_PERSISTENT | DATA_SETTABLE);
		*archive << ARCHIVE_DATA_NAMED("Color" + MoreStrings::cast_to_string(i), _c[i], DATA_PERSISTENT | DATA_SETTABLE);
	}
	        
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleColorSequencer::compute (const PlugBase *plug)
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
		
		// allocate the cache
		if (_cache.size() != particles->translations_stream().size())
			_cache.resize(particles->translations_stream().size(), 0);
		
		// Do processing
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();
		std::vector<Color4b> &colors = particles->color_stream();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			DTfloat &lifetime = lifetimes[i];
			DTint &cache = _cache[i];
			Color4b &color = colors[i];
			
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
			
			if (_r_mask)	color.set_r( (DTubyte) (_c[cache].r_as_float() * one_minus_lerp + _c[cache+1].r_as_float() * lerp));
			if (_g_mask)	color.set_g( (DTubyte) (_c[cache].g_as_float() * one_minus_lerp + _c[cache+1].g_as_float() * lerp));
			if (_b_mask)	color.set_b( (DTubyte) (_c[cache].b_as_float() * one_minus_lerp + _c[cache+1].b_as_float() * lerp));
			if (_a_mask)	color.set_a( (DTubyte) (_c[cache].a_as_float() * one_minus_lerp + _c[cache+1].a_as_float() * lerp));
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

void ScriptingParticleColorSequencer::dump_code(const std::string &object_name, Stream &s)
{
	PROFILER(PARTICLES);

    s << object_name << "->set_r_mask(" << _r_mask << ");\n";
    s << object_name << "->set_g_mask(" << _g_mask << ");\n";
    s << object_name << "->set_b_mask(" << _b_mask << ");\n";
    s << object_name << "->set_a_mask(" << _a_mask << ");\n";

    s << object_name << "->set_time_0(" << _t[0] << ");\n";
    s << object_name << "->set_color_0(Color4b(" << _c[0].r_as_float() << "," << _c[0].g_as_float() << "," << _c[0].b_as_float() << "," << _c[0].a_as_float() << "));\n";
    s << object_name << "->set_time_1(" << _t[1] << ");\n";
    s << object_name << "->set_color_1(Color4b(" << _c[1].r_as_float() << "," << _c[1].g_as_float() << "," << _c[1].b_as_float() << "," << _c[1].a_as_float() << "));\n";
    s << object_name << "->set_time_2(" << _t[2] << ");\n";
    s << object_name << "->set_color_2(Color4b(" << _c[2].r_as_float() << "," << _c[2].g_as_float() << "," << _c[2].b_as_float() << "," << _c[2].a_as_float() << "));\n";
    s << object_name << "->set_time_3(" << _t[3] << ");\n";
    s << object_name << "->set_color_3(Color4b(" << _c[3].r_as_float() << "," << _c[3].g_as_float() << "," << _c[3].b_as_float() << "," << _c[3].a_as_float() << "));\n";
    s << object_name << "->set_time_4(" << _t[4] << ");\n";
    s << object_name << "->set_color_4(Color4b(" << _c[4].r_as_float() << "," << _c[4].g_as_float() << "," << _c[4].b_as_float() << "," << _c[4].a_as_float() << "));\n";
    s << object_name << "->set_time_5(" << _t[5] << ");\n";
    s << object_name << "->set_color_5(Color4b(" << _c[5].r_as_float() << "," << _c[5].g_as_float() << "," << _c[5].b_as_float() << "," << _c[5].a_as_float() << "));\n";
    s << object_name << "->set_time_6(" << _t[6] << ");\n";
    s << object_name << "->set_color_6(Color4b(" << _c[6].r_as_float() << "," << _c[6].g_as_float() << "," << _c[6].b_as_float() << "," << _c[6].a_as_float() << "));\n";
    s << object_name << "->set_time_7(" << _t[7] << ");\n";
    s << object_name << "->set_color_7(Color4b(" << _c[7].r_as_float() << "," << _c[7].g_as_float() << "," << _c[7].b_as_float() << "," << _c[7].a_as_float() << "));\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


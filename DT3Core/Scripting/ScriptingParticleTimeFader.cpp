//==============================================================================
///	
///	File: ScriptingParticleTimeFader.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleTimeFader.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleTimeFader,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleTimeFader)

IMPLEMENT_PLUG_INFO_INDEX(_time_begin_fade)
IMPLEMENT_PLUG_INFO_INDEX(_time_end_fade)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleTimeFader)

	PLUG_INIT(_time_begin_fade,"Time_Begin_Fade")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_time_end_fade,"Time_End_Fade")
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

ScriptingParticleTimeFader::ScriptingParticleTimeFader (void)
    :   _time_begin_fade	(PLUG_INFO_INDEX(_time_begin_fade), 2.0F),
		_time_end_fade		(PLUG_INFO_INDEX(_time_end_fade), 3.0F),
		_in					(PLUG_INFO_INDEX(_in)),
		_out				(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleTimeFader::ScriptingParticleTimeFader (const ScriptingParticleTimeFader &rhs)
    :   ScriptingBase		(rhs),
		_time_begin_fade	(rhs._time_begin_fade),
		_time_end_fade		(rhs._time_end_fade),
		_in					(rhs._in),
		_out				(rhs._out)
{   

}

ScriptingParticleTimeFader & ScriptingParticleTimeFader::operator = (const ScriptingParticleTimeFader &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_time_begin_fade = rhs._time_begin_fade;
		_time_end_fade = rhs._time_end_fade;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleTimeFader::~ScriptingParticleTimeFader (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleTimeFader::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleTimeFader::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_time_begin_fade, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time_end_fade, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleTimeFader::compute (const PlugBase *plug)
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
		if (particles->color_stream().size() <= 0) {
			particles->build_colors_stream();
		}
		
		// Do processing
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();
		std::vector<Color4b> colors = particles->color_stream();
		
		DTfloat fade_time = _time_end_fade - _time_begin_fade;

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			Color4b &color = colors[i];
			DTfloat fade = 1.0F - (lifetimes[i] - _time_begin_fade) / fade_time;
			fade = MoreMath::clamp_zero_one(fade);
							
			color.set_a(fade);
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


//==============================================================================
///	
///	File: ScriptingParticleTimeScaler.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleTimeScaler.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleTimeScaler,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleTimeScaler)

IMPLEMENT_PLUG_INFO_INDEX(_constant_scale)
IMPLEMENT_PLUG_INFO_INDEX(_linear_scale)	
IMPLEMENT_PLUG_INFO_INDEX(_quadtratic_scale)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleTimeScaler)

	PLUG_INIT(_constant_scale,"Constant_Scale")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_linear_scale,"Linear_Scale")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_quadtratic_scale,"Quadratic_Scale")
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

ScriptingParticleTimeScaler::ScriptingParticleTimeScaler (void)
    :   _constant_scale		(PLUG_INFO_INDEX(_constant_scale), 0.0F),
		_linear_scale		(PLUG_INFO_INDEX(_linear_scale), 1.0F),
		_quadtratic_scale	(PLUG_INFO_INDEX(_quadtratic_scale), 0.0F),
		_in					(PLUG_INFO_INDEX(_in)),
		_out				(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleTimeScaler::ScriptingParticleTimeScaler (const ScriptingParticleTimeScaler &rhs)
    :   ScriptingBase		(rhs),
		_constant_scale		(rhs._constant_scale),
		_linear_scale		(rhs._linear_scale),
		_quadtratic_scale	(rhs._quadtratic_scale),
		_in					(rhs._in),
		_out				(rhs._out)
{   

}

ScriptingParticleTimeScaler & ScriptingParticleTimeScaler::operator = (const ScriptingParticleTimeScaler &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_constant_scale = rhs._constant_scale;
		_linear_scale = rhs._linear_scale;
		_quadtratic_scale = rhs._quadtratic_scale;
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleTimeScaler::~ScriptingParticleTimeScaler (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleTimeScaler::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleTimeScaler::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_constant_scale, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_linear_scale, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_quadtratic_scale, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticleTimeScaler::compute (const PlugBase *plug)
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
		std::vector<DTfloat> sizes = particles->sizes_stream();
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();
        
		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			DTfloat time = lifetimes[i];
			sizes[i] = _constant_scale + _linear_scale * time + _quadtratic_scale * time * time;
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

void ScriptingParticleTimeScaler::dump_code(const std::string &object_name, Stream &s)
{
	PROFILER(PARTICLES);

    s << object_name << "->set_constant_scale(" << _constant_scale << ");\n";
    s << object_name << "->set_linear_scale(" << _linear_scale << ");\n";
    s << object_name << "->set_quadratic_scale(" << _quadtratic_scale << ");\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


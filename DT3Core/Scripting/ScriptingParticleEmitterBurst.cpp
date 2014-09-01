//==============================================================================
///	
///	File: ScriptingParticleEmitterBurst.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleEmitterBurst.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleEmitterBurst,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleEmitterBurst)

IMPLEMENT_PLUG_INFO_INDEX(_num)
IMPLEMENT_PLUG_INFO_INDEX(_num_variation)	
IMPLEMENT_PLUG_INFO_INDEX(_spawn_translation)
IMPLEMENT_PLUG_INFO_INDEX(_max_lifetime)
IMPLEMENT_PLUG_INFO_INDEX(_active)
IMPLEMENT_PLUG_INFO_INDEX(_out)

IMPLEMENT_EVENT_INFO_INDEX(_active_e)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleEmitterBurst)

	PLUG_INIT(_num,"Num")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_num_variation,"Num_Variation")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_spawn_translation,"Spawn_Translation")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_max_lifetime,"Max_Lifetime")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_active,"Active")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
	EVENT_INIT(_active_e,"Active_Event")
        .set_input(true)
        .set_event(&ScriptingParticleEmitterBurst::eventActive);

        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleEmitterBurst::ScriptingParticleEmitterBurst (void)
    :   _num			(PLUG_INFO_INDEX(_num), 5),
		_num_variation	(PLUG_INFO_INDEX(_num_variation), 2),
		_spawn_translation	(PLUG_INFO_INDEX(_spawn_translation), Vector3(0.0F,0.0F,0.0F)),
		_max_lifetime	(PLUG_INFO_INDEX(_max_lifetime), 3.0F),
		_active			(PLUG_INFO_INDEX(_active), false),
		_last_active	(false),
		_max_num		(32),
		_out			(PLUG_INFO_INDEX(_out)),
        _active_e       (EVENT_INFO_INDEX(_active_e))

{  

}
		
ScriptingParticleEmitterBurst::ScriptingParticleEmitterBurst (const ScriptingParticleEmitterBurst &rhs)
    :   ScriptingParticleEmitter	(rhs),
		_num			(rhs._num),
		_num_variation	(rhs._num_variation),
		_spawn_translation	(rhs._spawn_translation),
		_max_lifetime	(rhs._max_lifetime),
		_active			(rhs._active),
		_last_active	(rhs._last_active),
		_max_num		(rhs._max_num),
		_out			(rhs._out),
        _active_e       (rhs._active_e)
{   

}

ScriptingParticleEmitterBurst & ScriptingParticleEmitterBurst::operator = (const ScriptingParticleEmitterBurst &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingParticleEmitter::operator = (rhs);

		_num = rhs._num;
		_num_variation = rhs._num_variation;
		_spawn_translation = rhs._spawn_translation;
		_max_lifetime = rhs._max_lifetime;
		_active = rhs._active;
		_last_active = rhs._last_active;
		_max_num = rhs._max_num;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleEmitterBurst::~ScriptingParticleEmitterBurst (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleEmitterBurst::initialize (void)
{
	ScriptingParticleEmitter::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleEmitterBurst::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingParticleEmitter::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA(_max_num, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_num, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_num_variation, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_spawn_translation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_max_lifetime, DATA_PERSISTENT | DATA_SETTABLE);
	
	*archive << ARCHIVE_PLUG(_active, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_last_active, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingParticleEmitterBurst::emitParticle (void)
{
	PROFILER(PARTICLES);

    DTint num = (DTint) (_num + MoreMath::random_float() * _num_variation);
    for (DTint i = 0; i < num; ++i) {

        std::shared_ptr<Particles> particles = _out;
        if (!particles || particles->translations_stream().size() <= 0) {
			_out.set_clean();
            return;
        }
            
        ASSERT(	_max_num > 0 &&
                static_cast<DTint>(particles->translations_stream().size()) == _max_num &&
                static_cast<DTint>(particles->lifetimes_stream().size()) == _max_num);
        
        DTint index = particles->active_end();
        ASSERT (index < static_cast<DTint>(particles->translations_stream().size()));
        
        DTint next_index = (index + 1) % particles->translations_stream().size();
                
        // Check if ring buffer all filled up
        if (next_index == particles->active_start())
            return;
            
        // set defaults
        particles->translations_stream()[index] = _spawn_translation;
        particles->lifetimes_stream()[index] = particles->default_lifetime();
        
        // reset default values of other streams
        if (particles->velocity_stream().size() > 0)
            particles->velocity_stream()[index] = particles->default_velocity();

        if (particles->sizes_stream().size() > 0)
            particles->sizes_stream()[index] = particles->default_size();

        if (particles->color_stream().size() > 0)
            particles->color_stream()[index] = particles->default_color();

        if (particles->rotations_stream().size() > 0)
            particles->rotations_stream()[index] = particles->default_rotation();
        
        particles->set_active_end(next_index);
    }
}

//==============================================================================
//==============================================================================

void ScriptingParticleEmitterBurst::eventActive (PlugNode *sender)
{
	PROFILER(PARTICLES);

    // Force emit on next tick
    _last_active = false;
    _active = true;
}

//==============================================================================
//==============================================================================

void ScriptingParticleEmitterBurst::tick (const DTfloat dt)
{
	PROFILER(PARTICLES);

    std::shared_ptr<Particles> particles = _out;

    // set up the particle system if we have to
    if (particles == NULL) {
        particles = std::shared_ptr<Particles>(Particles::create());
        _out = particles;		// Increases ref count
    }

    particles->resize (_max_num);
    
    // Build the lifetimes stream
    if (particles->lifetimes_stream().size() <= 0) {
        particles->build_lifetimes_stream();
    }
    
    
    ASSERT(	_max_num > 0 &&
            static_cast<DTint>(particles->translations_stream().size()) == _max_num &&
            static_cast<DTint>(particles->lifetimes_stream().size()) == _max_num);
    
    // Increment lifetimes and expire objects
    for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
        
        particles->lifetimes_stream()[i] += dt;
        if (particles->lifetimes_stream()[i] >= _max_lifetime)
            particles->set_active_start((i + 1) % particles->translations_stream().size());
        else if (particles->lifetimes_stream()[i] < 0.0F)
            particles->lifetimes_stream()[i] = 0.0F;
    }

    // Spawn particles if the time is right
    
    if (_active && !_last_active) {
        emitParticle();
    }
    
    _last_active = _active;
    
    // set outputs
    _out.set_dirty();
    _out = particles;
}

//==============================================================================
//==============================================================================

void ScriptingParticleEmitterBurst::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingParticleEmitterBurst::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

#ifdef DT3_EDITOR

void ScriptingParticleEmitterBurst::dump_code(const std::string &object_name, Stream &s)
{
	PROFILER(PARTICLES);

    s << object_name << "->set_max_num(" << _max_num << ");\n";
    s << object_name << "->set_num(" << _num << ");\n";
    s << object_name << "->set_num_variation(" << _num_variation << ");\n";
    s << object_name << "->set_spawn_translation(Vector3(" << _spawn_translation->x << "," << _spawn_translation->y << "," << _spawn_translation->z << "));\n";
    s << object_name << "->set_max_lifetime(" << _max_lifetime << ");\n";
    s << object_name << "->set_active(" << _active << ");\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


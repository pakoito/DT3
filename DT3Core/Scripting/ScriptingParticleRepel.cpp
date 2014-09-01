//==============================================================================
///	
///	File: ScriptingParticleRepel.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleRepel.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleRepel,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleRepel)

IMPLEMENT_PLUG_INFO_INDEX(_force)
IMPLEMENT_PLUG_INFO_INDEX(_translation)
IMPLEMENT_PLUG_INFO_INDEX(_distance)
IMPLEMENT_PLUG_INFO_INDEX(_active)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleRepel)

	PLUG_INIT(_force,"Force")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_translation,"Translation")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_distance,"Distance")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_active,"Active")
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

ScriptingParticleRepel::ScriptingParticleRepel (void)
    :   _force          (PLUG_INFO_INDEX(_force), 1.0F),
		_translation       (PLUG_INFO_INDEX(_translation), Vector3(0.0F,0.0F,0.0F)),
		_distance       (PLUG_INFO_INDEX(_distance), 1.0F),
		_active         (PLUG_INFO_INDEX(_active), true),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleRepel::ScriptingParticleRepel (const ScriptingParticleRepel &rhs)
    :   ScriptingBase	(rhs),
		_force          (rhs._force),
		_translation    (rhs._translation),
		_distance       (rhs._distance),
		_active         (rhs._active),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticleRepel & ScriptingParticleRepel::operator = (const ScriptingParticleRepel &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_force = rhs._force;
		_translation = rhs._translation;
		_distance = rhs._distance;
		_active = rhs._active;
        
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleRepel::~ScriptingParticleRepel (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleRepel::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleRepel::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_force, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_translation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_distance, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_active, DATA_PERSISTENT | DATA_SETTABLE);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingParticleRepel::tick (const DTfloat dt)
{
	PROFILER(PARTICLES);

    // Make sure there are input particles
    std::shared_ptr<Particles> particles = _in;
    if (!particles || particles->translations_stream().size() <= 0) {
        _out.set_clean();
        return;
    }
        
    // Build the velocities stream
    if (particles->velocity_stream().size() <= 0) {
        particles->build_velocity_stream();
    }
    
    // Do processing
    std::vector<Vector3> &velocities = particles->velocity_stream();
    std::vector<Vector3> &translations = particles->translations_stream();

    if (_active) {
        for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
            
            Vector3 diff = translations[i] - _translation;
            DTfloat dist2 = diff.abs2();
            
            if (dist2 < _distance * _distance && dist2 > 0.0F) {
                DTfloat dist = std::sqrt(dist2);
                diff = diff / dist;
                
                velocities[i] += _force * (1.0F - dist/_distance) * dt * diff;
            }
        }
    }

    _out = particles;
}

//==============================================================================
//==============================================================================

void ScriptingParticleRepel::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingParticleRepel::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


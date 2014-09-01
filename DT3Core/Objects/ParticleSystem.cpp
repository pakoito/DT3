//==============================================================================
///	
///	File: ParticleSystem.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/ParticleSystem.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Scripting/ScriptingParticleEmitter.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(ParticleSystem,"Object","EdPlaceableObjectAdapter")
IMPLEMENT_PLUG_NODE(ParticleSystem)

IMPLEMENT_PLUG_INFO_INDEX(_material)
IMPLEMENT_PLUG_INFO_INDEX(_particles)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ParticleSystem)

	PLUG_INIT(_particles,"Particles")
		.set_input(true);

	PLUG_INIT(_material,"Material")
		.set_input(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ParticleSystem::ParticleSystem (void)
    :   _run_up         (0.0F),
		_particles      (PLUG_INFO_INDEX(_particles)),
		_material       (PLUG_INFO_INDEX(_material))
{  

}
		
ParticleSystem::ParticleSystem (const ParticleSystem &rhs)
	:	PlaceableObject	(rhs),
        _run_up         (rhs._run_up),
		_particles		(rhs._particles),
		_material		(rhs._material)
{   

}

ParticleSystem & ParticleSystem::operator = (const ParticleSystem &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		PlaceableObject::operator = (rhs);

        _run_up = rhs._run_up;
    
		_material = rhs._material;
		_particles = rhs._particles;
	}
    return (*this);
}
			
ParticleSystem::~ParticleSystem (void)
{

}

//==============================================================================
//==============================================================================

void ParticleSystem::initialize (void)
{
	PlaceableObject::initialize();
}

//==============================================================================
//==============================================================================

void ParticleSystem::archive (const std::shared_ptr<Archive> &archive)
{
    PlaceableObject::archive(archive);

    archive->push_domain (class_id ()); 
    
    *archive << ARCHIVE_DATA_ACCESSORS("Run_Up", ParticleSystem::run_up, ParticleSystem::set_run_up, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_particles, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_material, DATA_PERSISTENT | DATA_SETTABLE);
	
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ParticleSystem::draw (const std::shared_ptr<CameraObject> &camera, const DTfloat lag)
{
	PROFILER(PARTICLES);
    
    std::shared_ptr<Particles> particles = particles_property();
    std::shared_ptr<MaterialResource> material = material_property();

    if (!particles || !material)  return;

	DTint start = particles->active_start();
	DTint end = particles->active_end();
	
	if (start == end)
		return;
		
		
//	// No wrapping required
//	if (start < end) {
//		DrawUtils::drawParticles(   getTransform(),
//                                    particles.get(),
//                                    material,
//                                    start, end-start);
//								
//	// Need to wrap around
//	} else {
//		DrawUtils::drawParticles(   getTransform(),
//                                    particles.get(),
//                                    material,
//                                    start, (DTint) particles->translations_stream().size() - start);
//								
//		DrawUtils::drawParticles(   getTransform(),
//                                    particles.get(),
//                                    material,
//                                    0, end);
//	}
}

//==============================================================================
//==============================================================================

void ParticleSystem::eval_particles (const DTfloat dt)
{
	std::shared_ptr<Particles> particles = particles_property();

	if (particles &&
		particles->translations_stream().size() > 0 &&
		particles->velocity_stream().size() > 0) {
		
		std::vector<Vector3> &translations = particles->translations_stream();
        std::vector<Vector3> &velocities = particles->velocity_stream();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
			Vector3 &translation = translations[i];
			Vector3 &velocity = velocities[i];
			translation += velocity * dt;
		}
		
	}
}

//==============================================================================
//==============================================================================

void ParticleSystem::tick (const DTfloat dt)
{
	PROFILER(PARTICLES);
    
	//
	// Run up
	//
	
	if (_run_up > 0.0F && dt > 0.0F && particles_property()) {
		
        // Walk up particle nodes until we get to an emitter
		PlugNode *node = this;
		PlugBase *plug = plug_by_name("Particles");
		
		while (plug && node) {
			// Check if connected
			if (!plug->incoming_connection())
				break;
				
			node = plug->incoming_connection()->owner();
            if (node->isa(ScriptingParticleEmitter::kind())) {
                break;
            }

			plug = node->plug_by_name("In");
		}
		
		// If we found an emitter
		if (node) {
		
            ScriptingParticleEmitter *emitter = checked_static_cast<ScriptingParticleEmitter*>(node);
		
			while (_run_up > 0.0F) {
				
				// Dirty the plug
				emitter->tick(dt);
				
				// Query to Run the particles
				eval_particles(dt);
				
				// Decrement runup
				_run_up -= dt;
			}
		}
		
	}

	eval_particles(dt);
}

//==============================================================================
//==============================================================================

void ParticleSystem::add_to_world(World *world)
{
    PlaceableObject::add_to_world(world);
        
    world->register_for_tick(this, make_callback(this, &type::tick));
    world->register_for_draw(this, make_callback(this, &type::draw));
}

void ParticleSystem::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));
    world()->unregister_for_draw(this, make_callback(this, &type::draw));

    PlaceableObject::remove_from_world();
}

//==============================================================================
//==============================================================================

#ifdef DT3_EDITOR

void ParticleSystem::dump_code(const std::string &object_name, Stream &s)
{
    s << object_name << "->set_translation(translation);\n";
    s << object_name << "->set_material_property(MaterialResource::import_resource(FilePath(\"" << (**_material).path().original_path() << "\")));\n";
    s << object_name << "->set_run_up(" << _run_up << ");\n";
}

#endif

//==============================================================================
//==============================================================================

} // DT3


#ifndef DT3_SCRIPTINGPARTICLEEMITTERPERIODIC
#define DT3_SCRIPTINGPARTICLEEMITTERPERIODIC
//==============================================================================
///	
///	File: ScriptingParticleEmitterPeriodic.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleEmitter.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Particles;

//==============================================================================
/// Periodic Particle Emitter for Particle System.
//==============================================================================

class ScriptingParticleEmitterPeriodic: public ScriptingParticleEmitter {
    public:
        DEFINE_TYPE(ScriptingParticleEmitterPeriodic,ScriptingParticleEmitter)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleEmitterPeriodic	(void);	
											ScriptingParticleEmitterPeriodic	(const ScriptingParticleEmitterPeriodic &rhs);
        ScriptingParticleEmitterPeriodic &	operator =							(const ScriptingParticleEmitterPeriodic &rhs);	
        virtual								~ScriptingParticleEmitterPeriodic	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        virtual void                tick                    (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Emits a particle
		void						emitParticle			(void);
		
		
		DEFINE_ACCESSORS(period, set_period, DTfloat, _period);
		DEFINE_ACCESSORS(variation, set_variation, DTfloat, _variation);
		DEFINE_ACCESSORS(spawn_translation, set_spawn_translation, Vector3, _spawn_translation);
		DEFINE_ACCESSORS(max_lifetime, set_max_lifetime, DTfloat, _max_lifetime);
		DEFINE_ACCESSORS(active, set_active, DTboolean, _active);
		DEFINE_ACCESSORS(max_num, set_max_num, DTint, _max_num);
		DEFINE_ACCESSORS(batch_size, set_batch_size, DTint, _batch_size);
		
	private:			
		Plug<DTfloat>								_period;	
		Plug<DTfloat>								_variation;	
		DTint										_batch_size;	

		Plug<Vector3>								_spawn_translation;
		Plug<DTfloat>								_max_lifetime;
		
		Plug<DTboolean>								_active;
		
		DTint										_max_num;	
		
		Plug<std::shared_ptr<Particles>>            _out;
		
		DTfloat										_timer;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_SCRIPTINGPARTICLEEMITTERBURST
#define DT3_SCRIPTINGPARTICLEEMITTERBURST
//==============================================================================
///	
///	File: ScriptingParticleEmitterBurst.hpp
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
/// Burst Particle Emitter for Particle System.
//==============================================================================

class ScriptingParticleEmitterBurst: public ScriptingParticleEmitter {
    public:
        DEFINE_TYPE(ScriptingParticleEmitterBurst,ScriptingParticleEmitter)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleEmitterBurst	(void);	
											ScriptingParticleEmitterBurst	(const ScriptingParticleEmitterBurst &rhs);
        ScriptingParticleEmitterBurst &		operator =						(const ScriptingParticleEmitterBurst &rhs);	
        virtual								~ScriptingParticleEmitterBurst	(void);
    
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
		void						eventActive             (PlugNode *sender);
		
		
		DEFINE_ACCESSORS(max_num, set_max_num, DTint, _max_num);
		DEFINE_ACCESSORS(num, set_num, DTint, _num);
		DEFINE_ACCESSORS(num_variation, set_num_variation, DTint, _num_variation);
		DEFINE_ACCESSORS(spawn_translation, set_spawn_translation, Vector3, _spawn_translation);
		DEFINE_ACCESSORS(max_lifetime, set_max_lifetime, DTfloat, _max_lifetime);
		DEFINE_ACCESSORS(active, set_active, DTboolean, _active);
		
#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                (const std::string &object_name, Stream &s);
#endif

	private:
        void                        emitParticle            (void);
                			
		Plug<DTint>									_num;	
		Plug<DTint>									_num_variation;	
		
		Plug<Vector3>								_spawn_translation;
		Plug<DTfloat>								_max_lifetime;
		
		Plug<DTboolean>								_active;
        Event                                       _active_e;
        
		DTboolean									_last_active;
		
		DTint										_max_num;	
		
		Plug<std::shared_ptr<Particles>>          _out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

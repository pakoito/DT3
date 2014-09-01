#ifndef DT3_SCRIPTINGPARTICLEREPEL
#define DT3_SCRIPTINGPARTICLEREPEL
//==============================================================================
///	
///	File: ScriptingParticleRepel.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Particles;

//==============================================================================
/// Particle Repellant for Particle System.
//==============================================================================

class ScriptingParticleRepel: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleRepel,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingParticleRepel	(void);	
									ScriptingParticleRepel	(const ScriptingParticleRepel &rhs);
        ScriptingParticleRepel &	operator =				(const ScriptingParticleRepel &rhs);	
        virtual						~ScriptingParticleRepel	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize				(void);
				
		DEFINE_ACCESSORS(force, set_force, DTfloat, _force);
		DEFINE_ACCESSORS(distance, set_distance, DTfloat, _distance);
		DEFINE_ACCESSORS(translation, set_translation, Vector3, _translation);
		DEFINE_ACCESSORS(active, set_active, DTboolean, _active);

	private:			
		Plug<DTfloat>								_force;	
		Plug<Vector3>								_translation;	
		Plug<DTfloat>								_distance;	
		Plug<DTboolean>								_active;	
        			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

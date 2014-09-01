#ifndef DT3_SCRIPTINGPARTICLEACCELERATOR
#define DT3_SCRIPTINGPARTICLEACCELERATOR
//==============================================================================
///	
///	File: ScriptingParticleAccelerator.hpp
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
/// Accelerator for Particle System.
//==============================================================================

class ScriptingParticleAccelerator: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleAccelerator,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                        ScriptingParticleAccelerator	(void);
                                        ScriptingParticleAccelerator	(const ScriptingParticleAccelerator &rhs);
        ScriptingParticleAccelerator &	operator =                      (const ScriptingParticleAccelerator &rhs);
        virtual                         ~ScriptingParticleAccelerator	(void);
    
        virtual void                    archive                         (const std::shared_ptr<Archive> &archive);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                    add_to_world                    (World *world);

		/// Object was removed from a world
        virtual void                    remove_from_world               (void);
		
	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        void                            tick                            (const DTfloat dt);

		/// Called to initialize the object
		virtual void                    initialize                      (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                       compute                         (const PlugBase *plug);

		DEFINE_ACCESSORS (acceleration, set_acceleration, Vector3, _acceleration);

	private:			
		Plug<Vector3>                           _acceleration;
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

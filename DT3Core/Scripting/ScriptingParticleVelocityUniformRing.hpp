#ifndef DT3_SCRIPTINGPARTICLEVELOCITYUNIFORMRING
#define DT3_SCRIPTINGPARTICLEVELOCITYUNIFORMRING
//==============================================================================
///	
///	File: ScriptingParticleVelocityUniformRing.hpp
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
/// Velocity Uniform Ring for Particle System.
//==============================================================================

class ScriptingParticleVelocityUniformRing: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleVelocityUniformRing,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleVelocityUniformRing	(void);	
											ScriptingParticleVelocityUniformRing	(const ScriptingParticleVelocityUniformRing &rhs);
        ScriptingParticleVelocityUniformRing &		operator =						(const ScriptingParticleVelocityUniformRing &rhs);	
        virtual								~ScriptingParticleVelocityUniformRing	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);


		DEFINE_ACCESSORS(normal, set_normal, Vector3, _normal);
		DEFINE_ACCESSORS(velocity, set_velocity, DTfloat, _velocity);

	private:			
		Plug<Vector3>								_normal;
		DTfloat										_velocity;
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

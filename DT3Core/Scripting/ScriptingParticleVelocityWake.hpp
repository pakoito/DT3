#ifndef DT3_SCRIPTINGPARTICLEVELOCITYRANDOMIZER
#define DT3_SCRIPTINGPARTICLEVELOCITYRANDOMIZER
//==============================================================================
///	
///	File: ScriptingParticleVelocityWake.hpp
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
/// Velocity Wake for Particle System.
//==============================================================================

class ScriptingParticleVelocityWake: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleVelocityWake,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleVelocityWake	(void);	
											ScriptingParticleVelocityWake	(const ScriptingParticleVelocityWake &rhs);
        ScriptingParticleVelocityWake &		operator =						(const ScriptingParticleVelocityWake &rhs);	
        virtual								~ScriptingParticleVelocityWake	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:			
		Plug<Vector3>								_up;	
		Plug<Vector3>								_in_velocity;	
		Plug<DTfloat>								_scale;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

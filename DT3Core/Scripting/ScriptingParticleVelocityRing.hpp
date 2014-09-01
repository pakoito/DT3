#ifndef DT3_SCRIPTINGPARTICLEVELOCITYRING
#define DT3_SCRIPTINGPARTICLEVELOCITYRING
//==============================================================================
///	
///	File: ScriptingParticleVelocityRing.hpp
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
/// Velocity Ring for Particle System.
//==============================================================================

class ScriptingParticleVelocityRing: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleVelocityRing,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleVelocityRing	(void);	
											ScriptingParticleVelocityRing	(const ScriptingParticleVelocityRing &rhs);
        ScriptingParticleVelocityRing &		operator =						(const ScriptingParticleVelocityRing &rhs);	
        virtual								~ScriptingParticleVelocityRing	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);


		DEFINE_ACCESSORS(normal, set_normal, Vector3, _normal);
		DEFINE_ACCESSORS(velocity, set_velocity, DTfloat, _velocity);
		DEFINE_ACCESSORS(velocity_variation, set_velocity_variation, DTfloat, _variation);

#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                (const std::string &object_name, Stream &s);
#endif

	private:			
		Plug<Vector3>								_normal;
		DTfloat										_velocity;
		DTfloat										_variation;
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

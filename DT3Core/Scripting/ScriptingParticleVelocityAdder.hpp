#ifndef DT3_SCRIPTINGPARTICLEVELOCITYADDER
#define DT3_SCRIPTINGPARTICLEVELOCITYADDER
//==============================================================================
///	
///	File: ScriptingParticleVelocityAdder.hpp
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
/// Velocity Adder for Particle System.
//==============================================================================

class ScriptingParticleVelocityAdder: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleVelocityAdder,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
												ScriptingParticleVelocityAdder	(void);	
												ScriptingParticleVelocityAdder	(const ScriptingParticleVelocityAdder &rhs);
        ScriptingParticleVelocityAdder &        operator =                      (const ScriptingParticleVelocityAdder &rhs);	
        virtual									~ScriptingParticleVelocityAdder (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(continuous, set_continuous, DTboolean, _continuous);
		DEFINE_ACCESSORS(velocity, set_velocity, Vector3, _velocity);

#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                (const std::string &object_name, Stream &s);
#endif

	private:
		DTboolean									_continuous;
			
		Plug<Vector3>								_velocity;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

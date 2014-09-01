#ifndef DT3_SCRIPTINGPARTICLEVELOCITYRANDOMIZER
#define DT3_SCRIPTINGPARTICLEVELOCITYRANDOMIZER
//==============================================================================
///	
///	File: ScriptingParticleVelocityRandomizer.hpp
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
/// Velocity Randomizer for Particle System.
//==============================================================================

class ScriptingParticleVelocityRandomizer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleVelocityRandomizer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
												ScriptingParticleVelocityRandomizer	(void);	
												ScriptingParticleVelocityRandomizer	(const ScriptingParticleVelocityRandomizer &rhs);
        ScriptingParticleVelocityRandomizer &	operator =							(const ScriptingParticleVelocityRandomizer &rhs);	
        virtual									~ScriptingParticleVelocityRandomizer(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(continuous, set_continuous, DTboolean, _continuous);
		DEFINE_ACCESSORS(rand_scale_x, set_rand_scale_x, DTfloat, _rand_scale_x);
		DEFINE_ACCESSORS(rand_scale_y, set_rand_scale_y, DTfloat, _rand_scale_y);
		DEFINE_ACCESSORS(rand_scale_z, set_rand_scale_z, DTfloat, _rand_scale_z);

#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                (const std::string &object_name, Stream &s);
#endif

	private:
		DTboolean								_continuous;
			
		Plug<DTfloat>							_rand_scale_x;
		Plug<DTfloat>							_rand_scale_y;
		Plug<DTfloat>							_rand_scale_z;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

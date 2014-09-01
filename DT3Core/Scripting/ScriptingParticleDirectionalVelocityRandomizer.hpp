#ifndef DT3_SCRIPTINGPARTICLEVELOCITYRANDOMIZER
#define DT3_SCRIPTINGPARTICLEVELOCITYRANDOMIZER
//==============================================================================
///	
///	File: ScriptingParticleDirectionalVelocityRandomizer.hpp
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
/// Directional Velocity Randomizer for Particle System.
//==============================================================================

class ScriptingParticleDirectionalVelocityRandomizer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleDirectionalVelocityRandomizer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                                            ScriptingParticleDirectionalVelocityRandomizer	(void);
                                                            ScriptingParticleDirectionalVelocityRandomizer	(const ScriptingParticleDirectionalVelocityRandomizer &rhs);
        ScriptingParticleDirectionalVelocityRandomizer &	operator =                                      (const ScriptingParticleDirectionalVelocityRandomizer &rhs);
        virtual                                             ~ScriptingParticleDirectionalVelocityRandomizer (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(continuous, set_continuous, DTboolean, _continuous);
		DEFINE_ACCESSORS(rand_scale_perp, set_rand_scale_perp, DTfloat, _rand_scale_perp);
		DEFINE_ACCESSORS(rand_scale_tangent, set_rand_scale_tangent, DTfloat, _rand_scale_tangent);

	private:
		DTboolean									_continuous;
			
		Plug<DTfloat>								_rand_scale_perp;	
		Plug<DTfloat>								_rand_scale_tangent;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

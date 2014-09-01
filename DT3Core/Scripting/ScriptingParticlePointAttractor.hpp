#ifndef DT3_SCRIPTINGPARTICLEPOINTATTRACTOR
#define DT3_SCRIPTINGPARTICLEPOINTATTRACTOR
//==============================================================================
///	
///	File: ScriptingParticlePointAttractor.hpp
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
/// Point Attractor for Particle System.
//==============================================================================

class ScriptingParticlePointAttractor: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticlePointAttractor,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticlePointAttractor		(void);	
											ScriptingParticlePointAttractor		(const ScriptingParticlePointAttractor &rhs);
        ScriptingParticlePointAttractor &	operator =							(const ScriptingParticlePointAttractor &rhs);	
        virtual								~ScriptingParticlePointAttractor	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(target, set_target, Vector3, _target);
		DEFINE_ACCESSORS(strength, set_strength, DTfloat, _strength);
		DEFINE_ACCESSORS(converge_time, set_converge_time, DTfloat, _converge_time);

	private:			
		Plug<Vector3>								_target;
		Plug<DTfloat>								_strength;
		Plug<DTfloat>								_converge_time;
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

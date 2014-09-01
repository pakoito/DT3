#ifndef DT3_SCRIPTINGPARTICLEEMITTER
#define DT3_SCRIPTINGPARTICLEEMITTER
//==============================================================================
///	
///	File: ScriptingParticleEmitter.hpp
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
/// Particle Emitter for Particle System.
//==============================================================================

class ScriptingParticleEmitter: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleEmitter,ScriptingBase)
		//DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                        ScriptingParticleEmitter	(void);	
                                        ScriptingParticleEmitter	(const ScriptingParticleEmitter &rhs);
        ScriptingParticleEmitter &		operator =					(const ScriptingParticleEmitter &rhs);	
        virtual							~ScriptingParticleEmitter	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        virtual void                tick                    (DTfloat dt) = 0;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

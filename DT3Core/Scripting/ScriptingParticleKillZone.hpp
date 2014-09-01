#ifndef DT3_ScriptingParticleKillZone
#define DT3_ScriptingParticleKillZone
//==============================================================================
///	
///	File: ScriptingParticleKillZone.hpp
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
/// Kill zone for Particle System.
//==============================================================================

class ScriptingParticleKillZone: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleKillZone,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingParticleKillZone	(void);
									ScriptingParticleKillZone	(const ScriptingParticleKillZone &rhs);
        ScriptingParticleKillZone &	operator =                  (const ScriptingParticleKillZone &rhs);	
        virtual                     ~ScriptingParticleKillZone  (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:			
		Plug<Vector3>								_translation;	
		Plug<DTfloat>								_radius;	
		Plug<DTint>                                 _num_killed;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

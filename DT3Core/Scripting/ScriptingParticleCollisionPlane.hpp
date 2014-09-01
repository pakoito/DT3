#ifndef DT3_SCRIPTINGPARTICLECOLLISIONPLANE
#define DT3_SCRIPTINGPARTICLECOLLISIONPLANE
//==============================================================================
///	
///	File: ScriptingParticleCollisionPlane.hpp
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
/// Collision Plane for Particle System.
//==============================================================================

class ScriptingParticleCollisionPlane: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleCollisionPlane,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleCollisionPlane	(void);	
											ScriptingParticleCollisionPlane	(const ScriptingParticleCollisionPlane &rhs);
        ScriptingParticleCollisionPlane &	operator =						(const ScriptingParticleCollisionPlane &rhs);	
        virtual								~ScriptingParticleCollisionPlane(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(normal, set_normal, Vector3, _normal);
		DEFINE_ACCESSORS(d, set_d, DTfloat, _d);
		DEFINE_ACCESSORS(restitution, set_restitution, DTfloat, _restitution);

	private:			
		Plug<Vector3>								_normal;	
		Plug<DTfloat>								_d;	
		Plug<DTfloat>								_restitution;	
		Plug<DTfloat>								_friction_damping;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

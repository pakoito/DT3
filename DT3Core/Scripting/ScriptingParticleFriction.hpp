#ifndef DT3_SCRIPTINGPARTICLEFRICTION
#define DT3_SCRIPTINGPARTICLEFRICTION
//==============================================================================
///	
///	File: ScriptingParticleFriction.hpp
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
/// Friction for Particle System.
//==============================================================================

class ScriptingParticleFriction: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleFriction,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingParticleFriction	(void);	
									ScriptingParticleFriction	(const ScriptingParticleFriction &rhs);
        ScriptingParticleFriction &	operator =					(const ScriptingParticleFriction &rhs);	
        virtual						~ScriptingParticleFriction	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(drag, set_drag, DTfloat, _drag);

	private:			
		Plug<DTfloat>                                       _drag;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

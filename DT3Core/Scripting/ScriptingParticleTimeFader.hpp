#ifndef DT3_SCRIPTINGPARTICLETIMEFADER
#define DT3_SCRIPTINGPARTICLETIMEFADER
//==============================================================================
///	
///	File: ScriptingParticleTimeFader.hpp
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
/// Time Fader for Particle System.
//==============================================================================

class ScriptingParticleTimeFader: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleTimeFader,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingParticleTimeFader	(void);	
									ScriptingParticleTimeFader	(const ScriptingParticleTimeFader &rhs);
        ScriptingParticleTimeFader&	operator =					(const ScriptingParticleTimeFader &rhs);	
        virtual						~ScriptingParticleTimeFader	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:			
		Plug<DTfloat>				_time_begin_fade;	
		Plug<DTfloat>				_time_end_fade;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

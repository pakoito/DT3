#ifndef DT3_SCRIPTINGPARTICLECOLORRANDOMIZER
#define DT3_SCRIPTINGPARTICLECOLORRANDOMIZER
//==============================================================================
///	
///	File: ScriptingParticleColorRandomizer.hpp
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
/// Color Randomizer for Particle System.
//==============================================================================

class ScriptingParticleColorRandomizer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleColorRandomizer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleColorRandomizer    (void);	
											ScriptingParticleColorRandomizer    (const ScriptingParticleColorRandomizer &rhs);
        ScriptingParticleColorRandomizer&	operator =							(const ScriptingParticleColorRandomizer &rhs);	
        virtual								~ScriptingParticleColorRandomizer	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);


		DEFINE_ACCESSORS(r_mag, set_r_mag, DTfloat, _r_mag);
		DEFINE_ACCESSORS(g_mag, set_g_mag, DTfloat, _g_mag);
		DEFINE_ACCESSORS(b_mag, set_b_mag, DTfloat, _b_mag);
		DEFINE_ACCESSORS(a_mag, set_a_mag, DTfloat, _a_mag);

	private:		
        static const DTint NUM_ENTRIES = 8;
		
		DTfloat		_r_mag;
		DTfloat		_g_mag;
		DTfloat		_b_mag;
		DTfloat		_a_mag;
								
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_SCRIPTINGPARTICLEPOSITIONRANDOMIZER
#define DT3_SCRIPTINGPARTICLEPOSITIONRANDOMIZER
//==============================================================================
///	
///	File: ScriptingParticlePositionRandomizer.hpp
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
/// Position Randomizer for Particle System.
//==============================================================================

class ScriptingParticlePositionRandomizer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticlePositionRandomizer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
												ScriptingParticlePositionRandomizer	(void);	
												ScriptingParticlePositionRandomizer	(const ScriptingParticlePositionRandomizer &rhs);
        ScriptingParticlePositionRandomizer &	operator =							(const ScriptingParticlePositionRandomizer &rhs);	
        virtual									~ScriptingParticlePositionRandomizer(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(continuous, set_continuous, DTboolean, _continuous);
		DEFINE_ACCESSORS(min_x, set_min_x, DTfloat, _min_x);
		DEFINE_ACCESSORS(max_x, set_max_x, DTfloat, _max_x);
		DEFINE_ACCESSORS(min_y, set_min_y, DTfloat, _min_y);
		DEFINE_ACCESSORS(max_y, set_max_y, DTfloat, _max_y);
		DEFINE_ACCESSORS(min_z, set_min_z, DTfloat, _min_z);
		DEFINE_ACCESSORS(max_z, set_max_z, DTfloat, _max_z);

	private:
		DTboolean									_continuous;
			
		Plug<DTfloat>								_min_x;	
		Plug<DTfloat>								_max_x;	
		Plug<DTfloat>								_min_y;	
		Plug<DTfloat>								_max_y;	
		Plug<DTfloat>								_min_z;	
		Plug<DTfloat>								_max_z;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

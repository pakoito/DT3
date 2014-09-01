#ifndef DT3_SCRIPTINGPARTICLETIMEROTATOR
#define DT3_SCRIPTINGPARTICLETIMEROTATOR
//==============================================================================
///	
///	File: ScriptingParticleTimeRotator.hpp
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
/// Time Rotator for Particle System.
//==============================================================================

class ScriptingParticleTimeRotator: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleTimeRotator,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingParticleTimeRotator	(void);	
										ScriptingParticleTimeRotator	(const ScriptingParticleTimeRotator &rhs);
        ScriptingParticleTimeRotator &	operator =						(const ScriptingParticleTimeRotator &rhs);	
        virtual							~ScriptingParticleTimeRotator	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(rotation_speed_mul, set_rotation_speed_mul, DTfloat, _rotation_speed_mul);
		DEFINE_ACCESSORS(rotation_phase_mul, set_rotation_phase_mul, DTfloat, _rotation_phase_mul);

#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                (const std::string &object_name, Stream &s);
#endif

	private:			
		DTfloat						_rotation_speed_mul;	
		DTfloat						_rotation_phase_mul;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

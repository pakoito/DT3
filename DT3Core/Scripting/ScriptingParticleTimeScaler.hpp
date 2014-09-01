#ifndef DT3_SCRIPTINGPARTICLETIMESCALER
#define DT3_SCRIPTINGPARTICLETIMESCALER
//==============================================================================
///	
///	File: ScriptingParticleTimeScaler.hpp
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
/// Time Scaler for Particle System.
//==============================================================================

class ScriptingParticleTimeScaler: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleTimeScaler,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingParticleTimeScaler	(void);	
									ScriptingParticleTimeScaler	(const ScriptingParticleTimeScaler &rhs);
        ScriptingParticleTimeScaler &	operator =				(const ScriptingParticleTimeScaler &rhs);	
        virtual						~ScriptingParticleTimeScaler(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);


		DEFINE_ACCESSORS(constant_scale, set_constant_scale, DTfloat, _constant_scale);
		DEFINE_ACCESSORS(linear_scale, set_linear_scale, DTfloat, _linear_scale);
		DEFINE_ACCESSORS(quadratic_scale, set_quadratic_scale, DTfloat, _quadtratic_scale);

#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                (const std::string &object_name, Stream &s);
#endif

	private:			
		Plug<DTfloat>								_constant_scale;	
		Plug<DTfloat>								_linear_scale;	
		Plug<DTfloat>								_quadtratic_scale;	
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_SCRIPTINGPARTICLESIZERANDOMIZER
#define DT3_SCRIPTINGPARTICLESIZERANDOMIZER
//==============================================================================
///	
///	File: ScriptingParticleSizeRandomizer.hpp
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
/// Size Randomizer for Particle System.
//==============================================================================

class ScriptingParticleSizeRandomizer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleSizeRandomizer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleSizeRandomizer	(void);	
											ScriptingParticleSizeRandomizer	(const ScriptingParticleSizeRandomizer &rhs);
        ScriptingParticleSizeRandomizer&    operator =						(const ScriptingParticleSizeRandomizer &rhs);	
        virtual								~ScriptingParticleSizeRandomizer(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);
		
	private:	
 		DTboolean                                           _continuous;
   
        DTfloat                                             _min_size;
        DTfloat                                             _max_size;

		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

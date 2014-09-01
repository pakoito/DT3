#ifndef DT3_SCRIPTINGPARTICLESIZESEQUENCER
#define DT3_SCRIPTINGPARTICLESIZESEQUENCER
//==============================================================================
///	
///	File: ScriptingParticleSizeSequencer.hpp
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
/// Size Sequencer for Particle System.
//==============================================================================

class ScriptingParticleSizeSequencer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleSizeSequencer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleSizeSequencer	(void);	
											ScriptingParticleSizeSequencer	(const ScriptingParticleSizeSequencer &rhs);
        ScriptingParticleSizeSequencer&		operator =						(const ScriptingParticleSizeSequencer &rhs);	
        virtual								~ScriptingParticleSizeSequencer	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);


		DEFINE_ACCESSORS(time_0, set_time_0, DTfloat, _t[0]);
		DEFINE_ACCESSORS(size_0, set_size_0, DTfloat, _s[0]);
		DEFINE_ACCESSORS(time_1, set_time_1, DTfloat, _t[1]);
		DEFINE_ACCESSORS(size_1, set_size_1, DTfloat, _s[1]);
		DEFINE_ACCESSORS(time_2, set_time_2, DTfloat, _t[2]);
		DEFINE_ACCESSORS(size_2, set_size_2, DTfloat, _s[2]);
		DEFINE_ACCESSORS(time_3, set_time_3, DTfloat, _t[3]);
		DEFINE_ACCESSORS(size_3, set_size_3, DTfloat, _s[3]);
		DEFINE_ACCESSORS(time_4, set_time_4, DTfloat, _t[4]);
		DEFINE_ACCESSORS(size_4, set_size_4, DTfloat, _s[4]);
		DEFINE_ACCESSORS(time_5, set_time_5, DTfloat, _t[5]);
		DEFINE_ACCESSORS(size_5, set_size_5, DTfloat, _s[5]);
		DEFINE_ACCESSORS(time_6, set_time_6, DTfloat, _t[6]);
		DEFINE_ACCESSORS(size_6, set_size_6, DTfloat, _s[6]);
		DEFINE_ACCESSORS(time_7, set_time_7, DTfloat, _t[7]);
		DEFINE_ACCESSORS(size_7, set_size_7, DTfloat, _s[7]);
		
	private:		
        static const DTint NUM_ENTRIES = 8;
			
		DTfloat                             _t[NUM_ENTRIES];	
		DTfloat                             _s[NUM_ENTRIES];
		
		std::vector<DTint>                  _cache;
			
		Plug<std::shared_ptr<Particles>>    _in;
		Plug<std::shared_ptr<Particles>>    _out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_SCRIPTINGPARTICLECOLORSEQUENCER
#define DT3_SCRIPTINGPARTICLECOLORSEQUENCER
//==============================================================================
///	
///	File: ScriptingParticleColorSequencer.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Particles;

//==============================================================================
/// Color Sequencer for Particle System.
//==============================================================================

class ScriptingParticleColorSequencer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleColorSequencer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
											ScriptingParticleColorSequencer		(void);	
											ScriptingParticleColorSequencer		(const ScriptingParticleColorSequencer &rhs);
        ScriptingParticleColorSequencer&	operator =							(const ScriptingParticleColorSequencer &rhs);	
        virtual								~ScriptingParticleColorSequencer	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);


		DEFINE_ACCESSORS(r_mask, set_r_mask, DTboolean, _r_mask);
		DEFINE_ACCESSORS(g_mask, set_g_mask, DTboolean, _g_mask);
		DEFINE_ACCESSORS(b_mask, set_b_mask, DTboolean, _b_mask);
		DEFINE_ACCESSORS(a_mask, set_a_mask, DTboolean, _a_mask);

		DEFINE_ACCESSORS(time_0, set_time_0, DTfloat, _t[0]);
		DEFINE_ACCESSORS(color_0, set_color_0, Color4b, _c[0]);
		DEFINE_ACCESSORS(time_1, set_time_1, DTfloat, _t[1]);
		DEFINE_ACCESSORS(color_1, set_color_1, Color4b, _c[1]);
		DEFINE_ACCESSORS(time_2, set_time_2, DTfloat, _t[2]);
		DEFINE_ACCESSORS(color_2, set_color_2, Color4b, _c[2]);
		DEFINE_ACCESSORS(time_3, set_time_3, DTfloat, _t[3]);
		DEFINE_ACCESSORS(color_3, set_color_3, Color4b, _c[3]);
		DEFINE_ACCESSORS(time_4, set_time_4, DTfloat, _t[4]);
		DEFINE_ACCESSORS(color_4, set_color_4, Color4b, _c[4]);
		DEFINE_ACCESSORS(time_5, set_time_5, DTfloat, _t[5]);
		DEFINE_ACCESSORS(color_5, set_color_5, Color4b, _c[5]);
		DEFINE_ACCESSORS(time_6, set_time_6, DTfloat, _t[6]);
		DEFINE_ACCESSORS(color_6, set_color_6, Color4b, _c[6]);
		DEFINE_ACCESSORS(time_7, set_time_7, DTfloat, _t[7]);
		DEFINE_ACCESSORS(color_7, set_color_7, Color4b, _c[7]);

#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                (const std::string &object_name, Stream &s);
#endif

	private:		
        static const DTint NUM_ENTRIES = 8;
		
		DTboolean		_r_mask;
		DTboolean		_g_mask;
		DTboolean		_b_mask;
		DTboolean		_a_mask;
			
		DTfloat			_t[NUM_ENTRIES];	
		Color4b			_c[NUM_ENTRIES];
		
		std::vector<DTint>	_cache;
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

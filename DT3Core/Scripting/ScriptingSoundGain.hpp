#ifndef DT3_SCRIPTINGSOUNDGAIN
#define DT3_SCRIPTINGSOUNDGAIN
//==============================================================================
///	
///	File: ScriptingSoundGain.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundBase.hpp"
#include "DT3Core/Types/Sound/SoundPacket.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class SoundResource;

//==============================================================================
/// Audio gain.
//==============================================================================

class ScriptingSoundGain: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundGain,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundGain      (void);	
									ScriptingSoundGain      (const ScriptingSoundGain &rhs);
        ScriptingSoundGain &		operator =				(const ScriptingSoundGain &rhs);	
        virtual                     ~ScriptingSoundGain     (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<SoundPacket>         _sound_packet_in;
		Plug<SoundPacket>         _sound_packet_out;
        
        Plug<DTfloat>                               _output_gain;
    };

//==============================================================================
//==============================================================================

} // DT3

#endif

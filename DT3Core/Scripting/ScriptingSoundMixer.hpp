#ifndef DT3_SCRIPTINGSOUNDMIXER
#define DT3_SCRIPTINGSOUNDMIXER
//==============================================================================
///	
///	File: ScriptingSoundMixer.hpp
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
/// Mixes two sound streams together.
//==============================================================================

class ScriptingSoundMixer: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundMixer,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundMixer     (void);	
									ScriptingSoundMixer     (const ScriptingSoundMixer &rhs);
        ScriptingSoundMixer &		operator =				(const ScriptingSoundMixer &rhs);	
        virtual                     ~ScriptingSoundMixer    (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<SoundPacket>         _sound_packet_in_1;
        Plug<DTfloat>                               _in_gain_1;
        
		Plug<SoundPacket>         _sound_packet_in_2;
        Plug<DTfloat>                               _in_gain_2;

		Plug<SoundPacket>         _sound_packet_out;
        Plug<DTfloat>                               _output_gain;
        
        SoundPacket                                 _sound_packet_buffer_1;
        SoundPacket                                 _sound_packet_buffer_2;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

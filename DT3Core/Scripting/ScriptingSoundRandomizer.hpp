#ifndef DT3_SCRIPTINGSOUNDRANDOMIZER
#define DT3_SCRIPTINGSOUNDRANDOMIZER
//==============================================================================
///	
///	File: ScriptingSoundRandomizer.hpp
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
/// Randomize the sound stream.
//==============================================================================

class ScriptingSoundRandomizer: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundRandomizer,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundRandomizer    (void);	
									ScriptingSoundRandomizer    (const ScriptingSoundRandomizer &rhs);
        ScriptingSoundRandomizer&   operator =                  (const ScriptingSoundRandomizer &rhs);	
        virtual                     ~ScriptingSoundRandomizer   (void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize                  (void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute                     (const PlugBase *plug);
        
		/// Toggle Event
        void                        randomizeInputs             (PlugNode *sender);


	private:		
		Plug<SoundPacket>           _sound_packet_in_1;
        Plug<SoundPacket>           _sound_packet_in_2;
		Plug<SoundPacket>           _sound_packet_in_3;
        Plug<SoundPacket>           _sound_packet_in_4;

        Plug<DTfloat>               _in_weight_1;
        Plug<DTfloat>               _in_weight_2;
        Plug<DTfloat>               _in_weight_3;
        Plug<DTfloat>               _in_weight_4;

        Event                       _randomize_inputs;
        DTuint                      _current_input;

		Plug<SoundPacket>           _sound_packet_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

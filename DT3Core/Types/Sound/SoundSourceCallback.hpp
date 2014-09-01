#ifndef DT3_SOUNDSOURCECALLBACK
#define DT3_SOUNDSOURCECALLBACK
//==============================================================================
///	
///	File: SoundSourceCallback.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Sound/SoundSource.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================

class SoundPacket;

//==============================================================================
/// Class
//==============================================================================

class SoundSourceCallback: public SoundSource {
    public:
        DEFINE_TYPE(SoundSourceCallback,SoundSource)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    SoundSourceCallback             (void);
                                    SoundSourceCallback             (const SoundSourceCallback &rhs);
        SoundSourceCallback &       operator =                      (const SoundSourceCallback &rhs);
        virtual                     ~SoundSourceCallback            (void);
                
	public:
		/// Sets a callback for retrieving the next sound packet
		/// \param callback_sound_packet Callback to retrieval funciton
        void                        set_next_sound_packet_callback  (std::shared_ptr<Callback<SoundPacket&>> callback_sound_packet);
        
		/// Returns the next sound packet for playing from this source
        virtual const SoundPacket&  next_sound_packet               (void);
        
	private:
        std::shared_ptr<Callback<SoundPacket&>>   _callback_sound_packet;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

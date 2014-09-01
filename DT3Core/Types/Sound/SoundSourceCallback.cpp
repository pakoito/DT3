//==============================================================================
///	
///	File: SoundSourceCallback.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Sound/SoundSourceCallback.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(SoundSourceCallback)
IMPLEMENT_PLUG_NODE(SoundSourceCallback)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(SoundSourceCallback)
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SoundSourceCallback::SoundSourceCallback (void)
{

}

SoundSourceCallback::SoundSourceCallback (const SoundSourceCallback& rhs)
	:	_callback_sound_packet   (rhs._callback_sound_packet){

}

SoundSourceCallback & SoundSourceCallback::operator = (const SoundSourceCallback& rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        SoundSource::operator = (rhs);

        _callback_sound_packet = rhs._callback_sound_packet;
    }

    return (*this);
}

SoundSourceCallback::~SoundSourceCallback (void)
{

}

//==============================================================================
//==============================================================================

void SoundSourceCallback::set_next_sound_packet_callback (std::shared_ptr<Callback<SoundPacket&>> callback_sound_packet)
{
    _callback_sound_packet = callback_sound_packet;
}

//==============================================================================
//==============================================================================

const SoundPacket& SoundSourceCallback::next_sound_packet (void)
{
    // If we fill the packet via callback
    if (_callback_sound_packet) {
        (*_callback_sound_packet)(_sound_packet.as_ref());
        _sound_packet.set_dirty();
    }
    
    return *_sound_packet;
}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: SoundSourceQueue.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Sound/SoundSourceQueue.hpp"
#include "DT3Core/Types/Sound/SoundPacket.hpp"
#include "DT3Core/System/Factory.hpp"
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(SoundSourceQueue)
IMPLEMENT_PLUG_NODE(SoundSourceQueue)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(SoundSourceQueue)
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SoundSourceQueue::SoundSourceQueue (void)
{

}

SoundSourceQueue::SoundSourceQueue (const SoundSourceQueue& rhs)
	:	SoundSource     (rhs)
{

}

SoundSourceQueue & SoundSourceQueue::operator = (const SoundSourceQueue& rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        SoundSource::operator = (rhs);

    }

    return (*this);
}

SoundSourceQueue::~SoundSourceQueue (void)
{

}

//==============================================================================
//==============================================================================

void SoundSourceQueue::push_packet (SoundPacket& packet)
{
    std::unique_lock<std::mutex> lock(_lock);
    
    SoundPacket p;
    p.copy(packet);
    
    _packets.push_back(p);
}

void SoundSourceQueue::clear_packets (void)
{
    std::unique_lock<std::mutex> lock(_lock);

    _packets.clear();
}

//==============================================================================
//==============================================================================

const SoundPacket& SoundSourceQueue::next_sound_packet (void)
{
    std::unique_lock<std::mutex> lock(_lock);
    
    if (_packets.size() > 0) {
        _sound_packet.as_ref() = _packets.front();
    
        _packets.pop_front();
        _sound_packet.set_dirty();
    } else {
        // Empty the packet
        _sound_packet->set_num_bytes(0);
    }

    return *_sound_packet;
}

//==============================================================================
//==============================================================================

} // DT3

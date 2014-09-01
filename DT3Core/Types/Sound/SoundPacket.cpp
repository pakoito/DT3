//==============================================================================
///	
///	File: SoundPacket.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Sound/SoundPacket.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(SoundPacket)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SoundPacket::SoundPacket (void)
    :   _logical_size   (0),
        _format         (SoundResource::FORMAT_STEREO16),
        _frequency      (0)
{

}

SoundPacket::SoundPacket (const SoundPacket &rhs)
    :   BaseClass       (rhs),
        _buffer         (rhs._buffer),
        _logical_size   (rhs._logical_size),
        _format         (rhs._format),
        _frequency      (rhs._frequency)
{

}
                            
SoundPacket& SoundPacket::operator = (const SoundPacket &rhs)
{
    
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        BaseClass::operator = (rhs);
        
        _logical_size = rhs._logical_size;
        _format = rhs._format;
        _frequency = rhs._frequency;
        _buffer = rhs._buffer;
    }

    return (*this);
}

SoundPacket::~SoundPacket (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const SoundPacket&v)
{
	return s;
}

Stream& operator >>(Stream &s, SoundPacket&v)
{
	return s;
}

//==============================================================================
//==============================================================================

void SoundPacket::copy (const SoundPacket &rhs)
{
    _logical_size = rhs._logical_size;
    _format = rhs._format;
    _frequency = rhs._frequency;

    set_num_bytes(_logical_size);
    
    ::memcpy(buffer(), rhs.buffer(), (size_t) _logical_size);
}

//==============================================================================
//==============================================================================

void SoundPacket::set_num_bytes (DTsize bytes)
{	
    if ( !_buffer) {
        _buffer = std::shared_ptr<std::vector<DTubyte>>(new std::vector<DTubyte>() );
        _buffer->resize(bytes);
        
    } else if (bytes > static_cast<DTsize>(_buffer->size()) ) {
        _buffer->resize(bytes);

    }
    
    _logical_size = bytes;
    
}

//==============================================================================
//==============================================================================

DTsize SoundPacket::num_samples (void) const
{
    switch (_format) {
        case SoundResource::FORMAT_MONO16:      return num_bytes() / 2;
        case SoundResource::FORMAT_STEREO16:    return num_bytes() / 4;
        default:                                return 0;
    };
}

void SoundPacket::set_num_samples (DTsize samples)
{
    switch (_format) {
        case SoundResource::FORMAT_MONO16:      set_num_bytes(samples*2);     break;
        case SoundResource::FORMAT_STEREO16:    set_num_bytes(samples*4);     break;
    };
}

//==============================================================================
//==============================================================================

void SoundPacket::append_bytes (Plug<SoundPacket> &plug, DTsize nb)
{
    // Note: Assumes plug has already been evaluated before entering the function
    SoundPacket &sound_packet = plug.as_ref_no_compute();
    
    set_format(sound_packet.format());
    set_frequency(sound_packet.frequency());
    
    while (1) {
        // Check if we are not getting any more data
        if (sound_packet.num_bytes() == 0) {
            // Fill remaining size with zeros
            DTsize remaining = nb - num_bytes();
            
            if (nb > 0) {
                set_num_bytes(nb);
                ::memset(buffer() + nb - remaining, 0, (size_t) remaining);
            }
            
            break;
        }
    
        // Append the data
        DTsize old_size = num_bytes();
        set_num_bytes(old_size + sound_packet.num_bytes());
        ::memcpy(buffer() + old_size, sound_packet.buffer(), (size_t) sound_packet.num_bytes());
                
        // Do we have enough data
        if (num_bytes() >= nb)
            break;
        
        // Read the plug again
        plug.compute();
    }
}

void SoundPacket::append_samples (Plug<SoundPacket> &plug, DTsize num_samples)
{
    switch (_format) {
        case SoundResource::FORMAT_MONO16:      append_bytes(plug, num_samples*2);     break;
        case SoundResource::FORMAT_STEREO16:    append_bytes(plug, num_samples*4);     break;
    };
}


void SoundPacket::append_packet (const SoundPacket &rhs)
{
    if (_frequency == 0) {
        set_frequency(rhs._frequency);
        set_format(rhs._format);
    }
    
    ASSERT(_format == rhs._format && _frequency == rhs._frequency);
    
    // Remember old logical size
    DTsize logical_size = _logical_size;
    
    set_num_bytes(num_bytes() + rhs.num_bytes());
    
    // Copy in the data
    ::memcpy(buffer() + logical_size, rhs.buffer(), rhs.num_bytes());
}
    
//==============================================================================
//==============================================================================

void SoundPacket::remove_bytes (DTsize nb)
{
    ASSERT(nb <= num_bytes());
    DTsize remaining = num_bytes() - nb;
    
    ::memcpy(buffer(), buffer() + nb, (size_t) remaining);
    set_num_bytes(remaining);
}

void SoundPacket::remove_samples (DTsize num_samples)
{
    switch (_format) {
        case SoundResource::FORMAT_MONO16:      remove_bytes(num_samples*2);     break;
        case SoundResource::FORMAT_STEREO16:    remove_bytes(num_samples*4);     break;
    };
}


//==============================================================================
//==============================================================================

} // DT3

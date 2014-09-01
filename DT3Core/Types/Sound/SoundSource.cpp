//==============================================================================
///	
///	File: SoundSource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Sound/SoundSource.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(SoundSource)
IMPLEMENT_PLUG_NODE(SoundSource)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet)

IMPLEMENT_PLUG_INFO_INDEX(_translation);
IMPLEMENT_PLUG_INFO_INDEX(_velocity);
IMPLEMENT_PLUG_INFO_INDEX(_pitch);
IMPLEMENT_PLUG_INFO_INDEX(_gain);
IMPLEMENT_PLUG_INFO_INDEX(_rolloff);

IMPLEMENT_EVENT_INFO_INDEX(_started);
IMPLEMENT_EVENT_INFO_INDEX(_stopped);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(SoundSource)
        
	PLUG_INIT(_sound_packet,"Sound_Packet")
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_translation,"Translation")
		.set_input(true)
		.set_output(true);

	PLUG_INIT(_velocity,"Velocity")
		.set_input(true)
		.set_output(true);
						
	PLUG_INIT(_pitch,"Pitch")
		.set_input(true)
		.set_output(true);
		
	PLUG_INIT(_gain,"Gain")
		.set_input(true)
		.set_output(true);
		
	PLUG_INIT(_rolloff,"Rolloff")
		.set_input(true)
		.set_output(true);
        
        
	EVENT_INIT(_started,"Started")
        .set_output(true);
        
	EVENT_INIT(_stopped,"Stopped")
        .set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SoundSource::SoundSource (void)
	:	_sound_packet   (PLUG_INFO_INDEX(_sound_packet)),
		_translation    (PLUG_INFO_INDEX(_translation), Vector3(0.0F,0.0F,0.0F)),
		_velocity       (PLUG_INFO_INDEX(_velocity), Vector3(0.0F,0.0F,0.0F)),
		_pitch          (PLUG_INFO_INDEX(_pitch), 1.0F),
		_gain           (PLUG_INFO_INDEX(_gain), 1.0F),
		_rolloff        (PLUG_INFO_INDEX(_rolloff), 0.0F),
        _ends_on_empty  (true),
        _started        (EVENT_INFO_INDEX(_started)),
        _stopped        (EVENT_INFO_INDEX(_stopped))
{

}

SoundSource::SoundSource (const SoundSource& rhs)
	:   PlugNode        (rhs),
        _sound_packet   (rhs._sound_packet),
        _translation	(rhs._translation),
		_velocity		(rhs._velocity),
		_pitch			(rhs._pitch),
		_gain			(rhs._gain),
		_rolloff		(rhs._rolloff),
        _ends_on_empty  (rhs._ends_on_empty),
        _started        (EVENT_INFO_INDEX(_started)),
        _stopped        (EVENT_INFO_INDEX(_stopped))
{

}

SoundSource & SoundSource::operator = (const SoundSource& rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        PlugNode::operator = (rhs);

        _sound_packet = rhs._sound_packet;
        _translation = rhs._translation;
        _velocity = rhs._velocity;
        _pitch = rhs._pitch;
        _gain = rhs._gain;
        _rolloff = rhs._rolloff;
        
        _ends_on_empty = rhs._ends_on_empty;
    }

    return (*this);
}

SoundSource::~SoundSource (void)
{

}

//==============================================================================
//==============================================================================

void SoundSource::initialize (void)
{
	PlugNode::initialize();
}

//==============================================================================
//==============================================================================

void SoundSource::start_sound (void)
{
    _started.send(this);
}
        
void SoundSource::stop_sound (void)
{
    _stopped.send(this);
}

//==============================================================================
//==============================================================================

const SoundPacket& SoundSource::next_sound_packet (void)
{
    return *_sound_packet;
}

//==============================================================================
//==============================================================================

} // DT3

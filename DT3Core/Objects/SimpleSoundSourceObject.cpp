//==============================================================================
///	
///	File: SimpleSoundSourceObject.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/SimpleSoundSourceObject.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Sound/SoundSource.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Scripting/ScriptingSound.hpp"
#include "DT3Core/Resources/ResourceTypes/SoundResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(SimpleSoundSourceObject,"Sound","EdPlaceableObjectAdapter")
IMPLEMENT_PLUG_NODE(SimpleSoundSourceObject)

IMPLEMENT_PLUG_INFO_INDEX(_gain)
IMPLEMENT_PLUG_INFO_INDEX(_pitch)
IMPLEMENT_PLUG_INFO_INDEX(_rolloff)

IMPLEMENT_EVENT_INFO_INDEX(_play)
IMPLEMENT_EVENT_INFO_INDEX(_stop)
IMPLEMENT_EVENT_INFO_INDEX(_stopped)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(SimpleSoundSourceObject)

	PLUG_INIT(_gain,"Gain")
		.set_input(true);

	PLUG_INIT(_pitch,"Pitch")
		.set_input(true);

	PLUG_INIT(_rolloff,"Rolloff")
		.set_input(true);

	EVENT_INIT(_play,"Play")
        .set_input(true)
        .set_event(&SimpleSoundSourceObject::play);

	EVENT_INIT(_stop,"Stop")
        .set_input(true)
        .set_event(&SimpleSoundSourceObject::stop);

	EVENT_INIT(_stopped,"Stopped")
        .set_input(true)
        .set_no_draw(true)
        .set_event(&SimpleSoundSourceObject::stopped);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SimpleSoundSourceObject::SimpleSoundSourceObject (void)
    :   _sound          (NULL),
        _gain           (PLUG_INFO_INDEX(_gain),1.0F),
        _pitch          (PLUG_INFO_INDEX(_pitch),1.0F),
        _rolloff        (PLUG_INFO_INDEX(_rolloff),0.0F),
        _play           (EVENT_INFO_INDEX(_play)),
        _stop           (EVENT_INFO_INDEX(_stop)),
        _stopped        (EVENT_INFO_INDEX(_stopped))
{

}
		
SimpleSoundSourceObject::SimpleSoundSourceObject (const SimpleSoundSourceObject &rhs)
	:	PlaceableObject	(rhs),
        _sound          (NULL),
        _gain           (rhs._gain),
        _pitch          (rhs._pitch),
        _rolloff        (rhs._rolloff),
        _play           (rhs._play),
        _stop           (rhs._stop),
        _stopped        (rhs._stopped),
        _sound_source   (NULL),
        _source         (NULL)
{

}

SimpleSoundSourceObject & SimpleSoundSourceObject::operator = (const SimpleSoundSourceObject &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		PlaceableObject::operator = (rhs);

        _gain = rhs._gain;
        _pitch = rhs._pitch;
        _rolloff = rhs._rolloff;
        
        _sound = rhs._sound;

        stop(NULL);
	}
    return (*this);
}
			
SimpleSoundSourceObject::~SimpleSoundSourceObject (void)
{
    stop(NULL);
}

//==============================================================================
//==============================================================================

void SimpleSoundSourceObject::archive (const std::shared_ptr<Archive> &archive)
{
	PlaceableObject::archive(archive);

	archive->push_domain (class_id ());
			
	*archive << ARCHIVE_DATA_ACCESSORS("Sound", SimpleSoundSourceObject::sound_property, SimpleSoundSourceObject::set_sound_property, DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_gain, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_pitch, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rolloff, DATA_PERSISTENT | DATA_SETTABLE);
	   
	*archive << ARCHIVE_EVENT(_play, DATA_SETTABLE);
	*archive << ARCHIVE_EVENT(_stop, DATA_SETTABLE);

	archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void SimpleSoundSourceObject::initialize (void)
{
	PlaceableObject::initialize();
    set_node_color(Color4f(0.360784F,0.729412F,0.92549F,1.0F));
}

//==============================================================================
//==============================================================================

void SimpleSoundSourceObject::set_sound_property (const std::shared_ptr<SoundResource> &attr)
{
	_sound = attr;
}

//==============================================================================
//==============================================================================

void SimpleSoundSourceObject::play (PlugNode *sender)
{
    stop(NULL);
    
    // Loader
    _sound_source = ScriptingSound::create();
    _sound_source->set_sound_property(_sound);
    
    // Source
    _source = SoundSource::create();
    
    PlugBase *s1 = _source->plug_by_name("Sound_Packet");
    PlugBase *s2 = _sound_source->plug_by_name("Sound_Packet");
    s1->set_incoming_connection(s2);
    
    PlugBase *p1 = _source->plug_by_name("Translation");
    PlugBase *p2 = plug_by_name("World_Translation");
    p1->set_incoming_connection(p2);
  
    Event *e1 = _source->event_by_name("Stopped");
    Event *e2 = event_by_name("Stopped");
    e2->add_incoming_connection(e1);
          
    // Start getting ticks
    world()->register_for_tick(this, make_callback(this, &type::tick));
    
    System::audio_renderer()->play(_source, checked_static_cast<World>(world()->shared_from_this()), NULL);

    tick (0.0F);    // Update gain and pitch
}

void SimpleSoundSourceObject::stop (PlugNode *sender)
{
    if (_source) {
        System::audio_renderer()->stop(_source);
        _source.reset();
        _sound_source.reset();
        
        // Stop getting ticks
        world()->unregister_for_tick(this, make_callback(this, &type::tick));
    }
}

void SimpleSoundSourceObject::stopped (PlugNode *sender)
{
    if (_source) {
        _source.reset();
        _sound_source.reset();

        // Stop getting ticks
        world()->unregister_for_tick(this, make_callback(this, &type::tick));
    }
}

//==============================================================================
//==============================================================================

void SimpleSoundSourceObject::tick (const DTfloat dt)
{
    if (_source) {
        _source->set_gain(_gain);
        _source->set_pitch(_pitch);
        _source->set_rolloff(_rolloff);
    }
}

//==============================================================================
//==============================================================================

void SimpleSoundSourceObject::add_to_world(World *world)
{
    PlaceableObject::add_to_world(world);
    
}

void SimpleSoundSourceObject::remove_from_world (void)
{
    stop(NULL);
    
    PlaceableObject::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: SoundBusSourceObject.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/SoundBusSourceObject.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Sound/SoundSource.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Scripting/ScriptingSoundBusIn.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(SoundBusSourceObject,"Sound","EdPlaceableObjectAdapter")
IMPLEMENT_PLUG_NODE(SoundBusSourceObject)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet)

IMPLEMENT_EVENT_INFO_INDEX(_play)
IMPLEMENT_EVENT_INFO_INDEX(_stop)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(SoundBusSourceObject)

	PLUG_INIT(_sound_packet,"Sound_Packet")
		.set_input(true)
		.set_output(true)
        .set_always_dirty(true);

	EVENT_INIT(_play,"Play")
        .set_input(true)
        .set_event(&SoundBusSourceObject::play);

	EVENT_INIT(_stop,"Stop")
        .set_input(true)
        .set_event(&SoundBusSourceObject::stop);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SoundBusSourceObject::SoundBusSourceObject (void)
    :   _sound_packet   (PLUG_INFO_INDEX(_sound_packet)),
        _play           (EVENT_INFO_INDEX(_play)),
        _stop           (EVENT_INFO_INDEX(_stop))
{  

}
		
SoundBusSourceObject::SoundBusSourceObject (const SoundBusSourceObject &rhs)
	:	PlaceableObject	(rhs),
		_sound_packet   (rhs._sound_packet),
        _play           (rhs._play),
        _stop           (rhs._stop),
        _bus_name       (rhs._bus_name)
{   

}

SoundBusSourceObject & SoundBusSourceObject::operator = (const SoundBusSourceObject &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		PlaceableObject::operator = (rhs);

        _sound_packet = rhs._sound_packet;
        _bus_name = rhs._bus_name;
    }
    return (*this);
}
			
SoundBusSourceObject::~SoundBusSourceObject (void)
{

}

//==============================================================================
//==============================================================================

void SoundBusSourceObject::archive (const std::shared_ptr<Archive> &archive)
{
	PlaceableObject::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_DATA(_bus_name, DATA_SETTABLE | DATA_PERSISTENT);

	*archive << ARCHIVE_EVENT(_play, DATA_SETTABLE);
	*archive << ARCHIVE_EVENT(_stop, DATA_SETTABLE);

	archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void SoundBusSourceObject::initialize (void)
{
	PlaceableObject::initialize();
    set_node_color(Color4f(0.360784F,0.729412F,0.92549F,1.0F));
}

//==============================================================================
//==============================================================================

void SoundBusSourceObject::play (PlugNode *sender)
{
    // Search for the bus
    std::shared_ptr<WorldNode> node = world()->node_by_name(_bus_name);
    
    if (node && node->isa(ScriptingSoundBusIn::kind())) {
        std::shared_ptr<ScriptingSoundBusIn> bus = checked_static_cast<ScriptingSoundBusIn>(node);
        
        bus->add_source(checked_static_cast<SoundBusSourceObject>(shared_from_this()));
    }
    
}

void SoundBusSourceObject::stop (PlugNode *sender)
{
    // Search for the bus
    std::shared_ptr<WorldNode> node = world()->node_by_name(_bus_name);
    
    if (node && node->isa(ScriptingSoundBusIn::kind())) {
        std::shared_ptr<ScriptingSoundBusIn> bus = checked_static_cast<ScriptingSoundBusIn>(node);
        
        bus->remove_source(checked_static_cast<SoundBusSourceObject>(shared_from_this()));
    }

}

//==============================================================================
//==============================================================================

void SoundBusSourceObject::add_to_world(World *world)
{
    PlaceableObject::add_to_world(world);
}

void SoundBusSourceObject::remove_from_world (void)
{    
    stop(NULL);

    PlaceableObject::remove_from_world();    
}

//==============================================================================
//==============================================================================

} // DT3


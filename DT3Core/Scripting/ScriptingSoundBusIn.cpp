//==============================================================================
///	
///	File: ScriptingSoundBusIn.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundBusIn.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Objects/SoundBusSourceObject.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundBusIn,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundBusIn)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundBusIn)

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundBusIn::ScriptingSoundBusIn (void)
    :   _sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out))
{  

}
		
ScriptingSoundBusIn::ScriptingSoundBusIn (const ScriptingSoundBusIn &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_out   (rhs._sound_packet_out)
{   

}	

ScriptingSoundBusIn & ScriptingSoundBusIn::operator = (const ScriptingSoundBusIn &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_out = rhs._sound_packet_out;
	}
    return (*this);
}
			
ScriptingSoundBusIn::~ScriptingSoundBusIn (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundBusIn::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundBusIn::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingSoundBusIn::add_source (const std::shared_ptr<SoundBusSourceObject> &source)
{
	PROFILER(SOUND);

    // Check and see if it's already in the list
    FOR_EACH (i,_bus_sources) {
        if (i->_bus_source == source)
            return;
    }

    
    BusSource bs;
    bs._bus_source = source;
    
    _bus_sources.push_back(bs);
}

void ScriptingSoundBusIn::remove_source (const std::shared_ptr<SoundBusSourceObject> &source)
{
	PROFILER(SOUND);

    // Check and see if it's already in the list
    FOR_EACH (i,_bus_sources) {
        if (i->_bus_source == source) {
            _bus_sources.erase(i);
            return;
        }
    }
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundBusIn::compute (const PlugBase *plug)
{
	PROFILER(SOUND);

    if (super_type::compute(plug))  return true;

	if (plug == &_sound_packet_out) {
    
        SoundPacket &sound_packet_out = _sound_packet_out.as_ref_no_compute();
        
        if (_bus_sources.size() == 0) {
            sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
            sound_packet_out.set_num_bytes(0);
            sound_packet_out.set_frequency(44100);
            return true;
        }
        
        //
        // Get data for all of inputs and figure out largest chunk size
        //
        
        DTsize max_num_samples = 0;
        
        FOR_EACH (i,_bus_sources) {
            PlugBase *p = i->_bus_source->plug_by_name("Sound_Packet");
            if (!p)
                continue;
                
            Plug<SoundPacket> *plug_packet = static_cast<Plug<SoundPacket> *>(p);
            
            plug_packet->compute();
            
            SoundPacket &packet = plug_packet->as_ref_no_compute();
            ASSERT(packet.frequency() == 44100);
            
            max_num_samples = MoreMath::max(max_num_samples, packet.num_samples());
        }
        
        //
        // Fill buffers with data
        //
        
        FOR_EACH (i,_bus_sources) {
            PlugBase *p = i->_bus_source->plug_by_name("Sound_Packet");
            if (!p)
                continue;
                
            Plug<SoundPacket> *plug_packet = static_cast<Plug<SoundPacket> *>(p);
            
            i->_buffer.append_samples(*plug_packet, max_num_samples);
        }

        //
        // Upconvert to 16-bit Stereo and mix into 32 bit buffers
        //
        
        std::vector<DTint> buffer;
        buffer.resize(max_num_samples*2, 0);    // Interleaved 32-but ints
        
        FOR_EACH (i,_bus_sources) {
            SoundPacket &packet = i->_buffer;

            if (packet.format() == SoundResource::FORMAT_MONO16) {
            
                DTshort *src = (DTshort*) packet.buffer();
                DTint *dst = &buffer[0];
                
                for (DTuint j = 0; j < max_num_samples; ++j) {
                    DTshort val = *src;
                    
                    *dst += val; ++dst;
                    *dst += val; ++dst;
                    ++src;
                }

            } else if (packet.format() == SoundResource::FORMAT_STEREO16) {
            
                DTshort *src = (DTshort*) packet.buffer();
                DTint *dst = &buffer[0];
                
                for (DTuint j = 0; j < max_num_samples; ++j) {
                    *dst += *src; 
                    ++dst;
                    ++src;
                    
                    *dst += *src; 
                    ++dst;
                    ++src;
                }

            }

            packet.remove_samples(max_num_samples);

        }
        
        //
        // Clamp and prepare output buffer
        //
        
        sound_packet_out.set_frequency(44100);
        sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
        sound_packet_out.set_num_samples(max_num_samples);   // 2 channels of 2 bytes per channel
        
        DTint *src = &buffer[0];
        DTshort *dst = (DTshort*) sound_packet_out.buffer();
        
        for (DTuint i = 0; i < max_num_samples; ++i) {
            // Left
            *dst = static_cast<DTshort>(CLAMP_16BIT(*src));
            ++dst;
            ++src;

            // Right
            *dst = static_cast<DTshort>(CLAMP_16BIT(*src));
            ++dst;
            ++src;
        }
                        
		_sound_packet_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


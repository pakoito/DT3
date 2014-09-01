//==============================================================================
///	
///	File: DeviceAudioOpenAL.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3OpenAL/DeviceAudioOpenAL.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Scripting/ScriptingSound.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include <limits>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceAudioOpenAL)

//==============================================================================
//==============================================================================

void CheckALMsg (const DTcharacter* file, const DTcharacter* func, const DTint line)
{
    ALenum errorcode;

    if ((errorcode = ::alGetError()) != AL_NO_ERROR) {
		std::stringstream message;
		message << "OpenAL error " << errorcode << " In file " << file << " in function " << func << " on line " << line;
		WARNINGMSG(message.str().c_str());
    }
}

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceAudioOpenAL::DeviceAudioOpenAL (void)
	:	_camera		(NULL),
		_gain		(1.0F),
        _al_context (NULL),
        _al_device  (NULL)
{

}

DeviceAudioOpenAL::~DeviceAudioOpenAL (void)
{
	stop_all();
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::initialize	(void)
{
    restart_audio ();
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::shutdown_audio (void)
{    
    // Save source for after reinitialize
    _save_channels.resize(_channels.size());
    
    for (DTuint c = 0; c < _channels.size(); ++c) {
        _save_channels[c]._source = _channels[c]._source;
        _save_channels[c]._sound_loader = _channels[c]._sound_loader;
        _save_channels[c]._packet = _channels[c]._packet;
        _save_channels[c]._world = _channels[c]._world;
    }
    
    // Don't stop sound channels so that we can begin playing them again
	destroy_channels();

	::alcDestroyContext(_al_context);
	::alcCloseDevice(_al_device);
	
	_al_context = NULL;
	_al_device = NULL;
}

void DeviceAudioOpenAL::restart_audio (void)
{
    if (!_al_device && !_al_context) {
        _al_device = ::alcOpenDevice(NULL);
        _al_context = ::alcCreateContext(_al_device,0);
        ::alcMakeContextCurrent(_al_context);

        set_number_of_channels(32);
    
        // Get all channels playing again
        for (DTuint c = 0; c < MoreMath::min(_channels.size(),_save_channels.size()); ++c) {
            play_on_channel (_channels[c], _save_channels[c]._source, _save_channels[c]._world, _save_channels[c]._sound_loader);
        }
        
        _save_channels.clear();

    }
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::set_camera (const std::shared_ptr<CameraObject> &camera)
{
	_camera = camera;
}

std::shared_ptr<CameraObject> DeviceAudioOpenAL::camera (void) const
{
	return _camera;
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::set_world_gain (World *world, DTfloat gain)
{
    // Set all of the gains for each of the worlds sound sources
    for (DTuint c = 0; c < _channels.size(); ++c) {
        Channel &channel = _channels[c];

        if (channel._world == world) {
            channel._world_gain = gain;
        }
    }
}

//==============================================================================
//==============================================================================

std::shared_ptr<SoundSource> DeviceAudioOpenAL::play_quick (const FilePath &pathname, World *world)
{
    LOG_MESSAGE << "Play quick " << pathname.full_path();

	std::shared_ptr<SoundResource> sound = SoundResource::import_resource(pathname);
    	
	if (sound) {
        LOG_MESSAGE << "Playing sound " << pathname.full_path();
		return play_quick (sound, world);
	}
	
	return NULL;
}

std::shared_ptr<SoundSource> DeviceAudioOpenAL::play_quick (const std::shared_ptr<SoundResource> &sound, World *world)
{
	std::shared_ptr<SoundSource> source = SoundSource::create();
    std::shared_ptr<ScriptingSound> sound_loader = ScriptingSound::create();
    
    // Build the sound file loader
    sound_loader->set_sound_property(sound);
    
    // Connect sound packet outputs
    PlugBase *s1 = source->plug_by_name("Sound_Packet");
    PlugBase *s2 = sound_loader->plug_by_name("Sound_Packet");
    s1->set_incoming_connection(s2);
    
    // Play the sound source
	if (play(source, world, sound_loader)) {
		return source;
	} else {
		return NULL;
	}
}

//==============================================================================
//==============================================================================

DTfloat DeviceAudioOpenAL::estimate_priority (const std::shared_ptr<SoundSource> &source)
{
    if (_camera) {
        Vector3 diff = camera()->translation() - source->translation();
        DTfloat dist_squared = diff.abs2();
        
        return (1.0F / dist_squared);
    } else {
        return 0.0F;
    }
}

//==============================================================================
//==============================================================================

DTboolean DeviceAudioOpenAL::load_chunk (ALuint buffer, Channel &channel)
{	
	PROFILER(SOUND);
    
    if (!channel._source)
        return false;
			
	// Stream chunk in	
	channel._packet = channel._source->next_sound_packet();
    if (channel._packet.num_bytes() == 0)
        return false;

	// Choose appropriate format
	ALenum format;
	switch(channel._packet.format()) {
		case SoundResource::FORMAT_MONO16:		format = AL_FORMAT_MONO16;		break;
		case SoundResource::FORMAT_STEREO16:	format = AL_FORMAT_STEREO16;	break;
		default:	return false;
	};
		
	// Store data into the buffer
	CheckAL();
	::alBufferData(buffer, format, channel._packet.buffer(), (ALsizei) channel._packet.num_bytes(), channel._packet.frequency());
	CheckAL();

	return true;
}

void DeviceAudioOpenAL::pump_all_streaming (void)
{
    std::unique_lock<std::mutex> lock(_mutex);

    for (DTuint c = 0; c < _channels.size(); ++c) {
        Channel &channel = _channels[c];
        std::shared_ptr<SoundSource> source = channel._source;
        
        if (!source)
            continue;

        // Get the number of buffers that have been processed
        ALint num_buffers;
        ::alGetSourcei(channel._al_source, AL_BUFFERS_PROCESSED, &num_buffers);
        
        // Unqueue all of the used buffers
        while (num_buffers > 0) {
        
            // Deque one buffer
            ALuint buffer;
            ::alSourceUnqueueBuffers(channel._al_source, 1, &buffer );
            CheckAL();
            
            // Toggle use flag
            for (DTint i = 0; i < ARRAY_SIZE(channel._buffer); ++i) {
                if (channel._buffer[i] == buffer) {
                    channel._buffer_in_use[i] = false;
                }
            }
            
            --num_buffers;
        }
        
        // Try to fill buffers
        for (DTint i = 0; i < ARRAY_SIZE(channel._buffer); ++i) {
            if (!channel._buffer_in_use[i]) {
        
                // Load the buffer
                if (load_chunk (channel._buffer[i], channel)) {
                    // Enqueue the buffer
                    ::alSourceQueueBuffers(channel._al_source, 1, &channel._buffer[i]);
                    channel._buffer_in_use[i] = true;
                    
                    CheckAL();
                }
                
            }
        }


    }
}

void DeviceAudioOpenAL::prime_streaming (Channel &channel)
{
    // Load the buffer	
    for (DTuint i = 0; i < ARRAY_SIZE(channel._buffer); ++i) {
        if (load_chunk (channel._buffer[i], channel)) {
            ::alSourceQueueBuffers(channel._al_source, 1, &channel._buffer[i]);
            channel._buffer_in_use[i] = true;

            CheckAL();
        } else {
            break;
        }
    }
    
    channel._needs_priming = false;
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::play_on_channel (   Channel &channel,
                                            const std::shared_ptr<SoundSource> &source,
                                            World *world,
                                            const std::shared_ptr<ScriptingSound> &sound_loader)
{
	// If new sound is valid
    if (source) {
		
		// Stop any existing sound
		stop_on_channel (channel);
	
		// Remember this source
		channel._source = source;
		channel._sound_loader = sound_loader;
        channel._world = world;
        channel._world_gain = 1.0F;

		// Tell the sound group we are plaing the sound
        source->start_sound();
		
		// Reset the chunk index to start streaming from the beginning
		channel._ready_to_start = true;
        channel._needs_priming = true;
		
		
		// NOTE: This happens later
        // primeStreaming();
    }
    
}

void DeviceAudioOpenAL::stop_on_channel (Channel &channel)
{
	std::shared_ptr<SoundSource> source = channel._source;
	
 	// Tell sound source that the sound is stopped
    if (source) {
        source->stop_sound();
	
		// Stop the existing source from playing
		::alSourceStop(channel._al_source);
		CheckAL();
		
		// Deque all buffers
		ALint num_buffers;

		// Deque all processed buffers. NOTE: Not sure if this has to be done but it can't hurt.
		::alGetSourcei(channel._al_source, AL_BUFFERS_PROCESSED, &num_buffers);
		while (num_buffers > 0) {
			ALuint buffer;
			::alSourceUnqueueBuffers(channel._al_source, 1, &buffer );
			--num_buffers;
		}

		// Deque all queued buffers
		::alGetSourcei(channel._al_source, AL_BUFFERS_QUEUED, &num_buffers);
		while (num_buffers > 0) {
			ALuint buffer;
			::alSourceUnqueueBuffers(channel._al_source, 1, &buffer );
			--num_buffers;
		}
        
        channel._buffer_in_use[0] = false;
        channel._buffer_in_use[1] = false;
        channel._buffer_in_use[2] = false;

		// Detach buffers
        // Don't do this on iOS 5 because of Apple bug
		//::alSourcei(channel._al_source, AL_BUFFER, 0);
		CheckAL();

		// Release the connected sources and sound
		channel._source.reset();
        channel._sound_loader.reset();
        channel._world = NULL;

        channel._needs_priming = true;
	}
}

//==============================================================================
//==============================================================================

DTboolean DeviceAudioOpenAL::play (const std::shared_ptr<SoundSource> &source, World *world, const std::shared_ptr<ScriptingSound> &sound_loader)
{
    std::unique_lock<std::mutex> lock(_mutex);

	PROFILER(SOUND);
	
	// Don't bother if gain is too low
	if (_gain < 0.01F)
		return false;
        

    // Search for this source already playing
    for (DTuint c = 0; c < _channels.size(); ++c) {
        Channel &channel = _channels[c];

        if (channel._source == source) {
            play_on_channel(channel, source, world, sound_loader);
            return true;
        }
    }  

    // Search for an empty channel
    for (DTuint c = 0; c < _channels.size(); ++c) {
		Channel &channel = _channels[c];

        if (channel._source == NULL) {
            play_on_channel(channel, source, world, sound_loader);
            return true;
        }
    }
    
    // No empty spots on list so we can check if we should
    // bump an existing sound. Find the lowest priority object.
    DTfloat smallest_pri = std::numeric_limits<DTfloat>::infinity();
    DTuint  smallest_pri_index = 0;
    
    for (DTuint c = 0; c < _channels.size(); ++c) {
        DTfloat pri = estimate_priority(_channels[c]._source);
        if (pri < smallest_pri) {
            smallest_pri_index = c;
            smallest_pri = pri;
        }
    }
    
    // Check if the new sound is more important
    if (smallest_pri < estimate_priority(source)) {
        play_on_channel(_channels[smallest_pri_index], source, world, sound_loader);
		return true;
    }
	
	return false;
}

void DeviceAudioOpenAL::stop (const std::shared_ptr<SoundSource> &source)
{
    std::unique_lock<std::mutex> lock(_mutex);

	PROFILER(SOUND);

    // Search for this source already playing
    for (DTuint c = 0; c < _channels.size(); ++c) {
		Channel &channel = _channels[c];

        if (channel._source == source) {            
            stop_on_channel(channel);
        }
    }
    
}

void DeviceAudioOpenAL::stop_all (void)
{
    std::unique_lock<std::mutex> lock(_mutex);

	PROFILER(SOUND);

    for (DTuint c = 0; c < _channels.size(); ++c) {
        stop_on_channel(_channels[c]);
    }
}

void DeviceAudioOpenAL::stop_all_world (World *world)
{
    std::unique_lock<std::mutex> lock(_mutex);

	PROFILER(SOUND);

    for (DTuint c = 0; c < _channels.size(); ++c) {
        if (_channels[c]._world == world) {
            stop_on_channel(_channels[c]);
        }
    }
}


DTboolean DeviceAudioOpenAL::is_playing (const std::shared_ptr<SoundSource> &source)
{
    std::unique_lock<std::mutex> lock(_mutex);

	PROFILER(SOUND);
    
    return channel(source) >= 0;
}

DTshort DeviceAudioOpenAL::channel (const std::shared_ptr<SoundSource> &source)
{
    std::unique_lock<std::mutex> lock(_mutex);

	PROFILER(SOUND);

    for (std::size_t c = 0; c < _channels.size(); ++c) {
		Channel &channel = _channels[c];

        if (channel._source == source)
            return static_cast<DTshort>(c);
    }
    
    return -1;
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::set_number_of_channels (const DTint num_channels)
{

	PROFILER(SOUND);

    clear_channels();

    std::unique_lock<std::mutex> lock(_mutex);

    _channels.resize(num_channels);

    for (DTuint c = 0; c < _channels.size(); ++c) {
		Channel &channel = _channels[c];
		
        ::alGenSources(1, &(channel._al_source));		
        CheckAL();

		::alGenBuffers(1, &(channel._buffer[0]) );
		::alGenBuffers(1, &(channel._buffer[1]) );
		::alGenBuffers(1, &(channel._buffer[2]) );
        CheckAL();
        
        channel._buffer_in_use[0] = false;
        channel._buffer_in_use[1] = false;
        channel._buffer_in_use[2] = false;

        channel._source = NULL;
        channel._sound_loader = NULL;
		channel._ready_to_start = false;
        channel._needs_priming = false;
    }
}

void DeviceAudioOpenAL::clear_channels (void)
{
	PROFILER(SOUND);

    stop_all();
	destroy_channels();
}

void DeviceAudioOpenAL::destroy_channels (void)
{
    std::unique_lock<std::mutex> lock(_mutex);

	PROFILER(SOUND);

    for (DTuint c = 0; c < _channels.size(); ++c) {
 		Channel &channel = _channels[c];

        ::alDeleteSources(1, &(channel._al_source));
		::alDeleteBuffers(1, &(channel._buffer[0]));
		::alDeleteBuffers(1, &(channel._buffer[1]));
		::alDeleteBuffers(1, &(channel._buffer[2]));
		CheckAL();
    }
	
    _channels.clear();
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::tick (const DTfloat dt)
{		

	PROFILER(SOUND);
    
    if (!_al_context || !_al_device)
        return;

    DTfloat sound_on = MoreStrings::cast_from_string<DTboolean>(Globals::global("SYS_SOUND")) ? 1.0F : 0.0F;
    DTfloat sound_gain = Globals::has_global("SYS_SOUND_GAIN") ? MoreStrings::cast_from_string<DTfloat>(Globals::global("SYS_SOUND_GAIN")) : 1.0F;

    _gain = sound_on * sound_gain;
    
    //
    // Update Camera
    //

    if (_camera) {
        // Update Listener
        Vector3 position = camera()->translation();
        Vector3 velocity = camera()->velocity();
        
        Vector3 forward = camera()->forwards();
        Vector3 up = camera()->up_axis();
        
        ::alListener3f(AL_POSITION, position.x, position.y, position.z);
        CheckAL();

        ::alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        CheckAL();	
        
        ::alListenerf(AL_GAIN, _gain);
        CheckAL();	

        Matrix3 orientation = camera()->orientation();
        ALfloat lo[] = {    forward.x,    forward.y,    forward.z,
                            up.x,         up.y,         up.z,	    };
        ::alListenerfv(AL_ORIENTATION, lo);
        CheckAL();	
    } else {
        ::alListener3f(AL_POSITION, 0.0F, 0.0F, 0.0F);
        CheckAL();

        ::alListener3f(AL_VELOCITY, 0.0F, 0.0F, 0.0F);
        CheckAL();	
        
        ::alListenerf(AL_GAIN, _gain);
        CheckAL();	

        ALfloat lo[] = {    0.0F, 0.0F, -1.0F,
                            0.0F, 1.0F, 0.0F    };
        ::alListenerfv(AL_ORIENTATION, lo);
    }


    //
    // Update channels
    //

    for (DTuint c = 0; c < _channels.size(); ++c) {
        Channel &channel = _channels[c];
        std::shared_ptr<SoundSource> source = channel._source;
    
        // If there's a source and a sound then this channel is playing
        if (source) {

            ALenum state;
            ::alGetSourcei(channel._al_source, AL_SOURCE_STATE, &state);
            CheckAL();
            
            // Check to see if sound buffer was starved
            if (!channel._ready_to_start && (channel._packet.num_bytes() > 0 || !channel._source->ends_on_empty()) && state != AL_PLAYING) {
                
                channel._ready_to_start = true;
                LOG_MESSAGE << "Audio stream starved";
            
            // Check if sound data has been depleted
            } else if (!channel._ready_to_start && channel._packet.num_bytes() == 0 && state != AL_PLAYING && channel._source->ends_on_empty()) {
                std::unique_lock<std::mutex> lock(_mutex);

                stop_on_channel(channel);
                LOG_MESSAGE << "Audio stream done";
                
            // Update sound
            } else {
                // Maybe channel needs to be started
                if (channel._ready_to_start) {
                    std::unique_lock<std::mutex> lock(_mutex);

                    // Fill buffers
                    if (channel._needs_priming)
                        prime_streaming (channel);
              
                    ::alSourcePlay(channel._al_source);
                    CheckAL();

                    channel._ready_to_start = false;
                }
                
                Vector3 position = source->translation();
                Vector3 velocity = source->velocity();
                //DTboolean looping = source->looping();	// Instead we loop sounds ourselves
                DTfloat pitch = source->pitch();
                DTfloat rolloff = source->rolloff();
                DTfloat gain = source->gain() * (channel._world ? channel._world_gain : 1.0F);

                // position sound
                if (!_camera) {
                    ::alSource3f(channel._al_source, AL_POSITION, 0.0F,0.0F,0.0F);					
                } else if (position == Vector3(0.0F,0.0F,0.0F)) {
                    Vector3 camera_position = camera()->translation();
                    ::alSource3f(channel._al_source, AL_POSITION, camera_position.x, camera_position.y, camera_position.z);					
                } else {
                    ::alSource3f(channel._al_source, AL_POSITION, position.x, position.y, position.z);
                }
                CheckAL();
                
                ::alSource3f(channel._al_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
                CheckAL();


                ::alSourcef(channel._al_source, AL_PITCH, pitch);
                CheckAL();

                //::alSourcei(channel._al_source, AL_LOOPING, looping);	// Instead we loop sounds ourselves
                //CheckAL();

                ::alSourcef(channel._al_source, AL_GAIN, gain);
                CheckAL();
                
                ::alSourcef(channel._al_source, AL_ROLLOFF_FACTOR, rolloff);
                CheckAL();
                
            }
        }
    }
    
    // Update Busses
    for (auto &bus : _busses) {
    
        // Get the number of buffers that have been processed
        ALint num_buffers;
        ::alGetSourcei(bus._al_source, AL_BUFFERS_PROCESSED, &num_buffers);
        bus._num_active -= num_buffers;
            
        // Pump all of the buffers
        while (bus._num_active < 3) {
        
            // Deque one buffer
            ALuint buffer;
            while (num_buffers > 0) {
                ::alSourceUnqueueBuffers(bus._al_source, 1, &buffer );
                --num_buffers;
                CheckAL();
            }
            
            // Stream chunk in	
            SoundPacket packet = bus._source->next_sound_packet();
            if (packet.num_bytes() == 0)
                break;

            // Choose appropriate format
            ALenum format;
            switch(packet.format()) {
                case SoundResource::FORMAT_MONO16:		format = AL_FORMAT_MONO16;		break;
                case SoundResource::FORMAT_STEREO16:	format = AL_FORMAT_STEREO16;	break;
            };
                
            // Store data into the buffer
            CheckAL();
            ::alBufferData(bus._buffer[0], format, packet.buffer(), (ALsizei) packet.num_bytes(), packet.frequency());
            CheckAL();
            
            // Enqueue the buffer
            ::alSourceQueueBuffers(bus._al_source, 1, &(bus._buffer[0]));
            
            // Rotate the ring buffer
            ALuint temp = bus._buffer[0];
            bus._buffer[0] = bus._buffer[1];
            bus._buffer[1] = bus._buffer[2];
            bus._buffer[2] = temp;

            ++bus._num_active;
        }
        
        if (bus._num_active > 0) {
            // Check to make sure source is playing if there is new data
            ALenum state;
            ::alGetSourcei(bus._al_source, AL_SOURCE_STATE, &state);
            CheckAL();
                
            // Check to see if sound buffer was starved
            if (state != AL_PLAYING) {
                ::alSourcePlay(bus._al_source);
            }
        }
    }
	
    pump_all_streaming();
}

//==============================================================================
//==============================================================================

void DeviceAudioOpenAL::add_bus (const std::shared_ptr<SoundSource> &source)
{
    std::unique_lock<std::mutex> lock(_mutex);
    
    Bus b;
    _busses.push_back(b);

    Bus &bus = _busses.back();
    
    bus._source = source;
    bus._num_active = 0;
    
    ::alGenSources(1, &(bus._al_source));		
    ::alGenBuffers(1, &(bus._buffer[0]) );
    ::alGenBuffers(1, &(bus._buffer[1]) );
    ::alGenBuffers(1, &(bus._buffer[2]) );
    CheckAL();

}

void DeviceAudioOpenAL::remove_bus (const std::shared_ptr<SoundSource> &source)
{
    std::unique_lock<std::mutex> lock(_mutex);

    FOR_EACH (i,_busses) {
        Bus &bus = *i;
        
        if (bus._source == source) {
            ::alDeleteSources(1, &(bus._al_source));
            ::alDeleteBuffers(1, &(bus._buffer[0]));
            ::alDeleteBuffers(1, &(bus._buffer[1]));
            ::alDeleteBuffers(1, &(bus._buffer[2]));
            CheckAL();
            
            _busses.erase(i);
            return;
        }
    
    }
}

//==============================================================================
//==============================================================================

} // DT3


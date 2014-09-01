//==============================================================================
///	
///	File: DeviceAudioDX11.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include 	"DeviceAudioDX11.hpp"

#include	"Factory.hpp"
#include	"FilePath.hpp"
#include	"System.hpp"
#include	"Error.hpp"

#include	"DeviceProfiler.hpp"
#include	"StringCast.hpp"

#include	"ResourceManager.hpp"
#include	"Globals.hpp"
#include	"ConsoleStream.hpp"
#include	"DeviceInput.hpp"

#include	"SoundSource.hpp"
#include	"SoundResource.hpp"
#include	"SoundPacket.hpp"
#include	"CameraObject.hpp"

#include    "ScriptingSound.hpp"
#include    "ThreadTaskQueue.hpp"

#include	<limits>

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceAudioDX11)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceAudioDX11::DeviceAudioDX11 (void)
	:	_camera		    (NULL),
		_gain		    (1.0F),
        _x_audio_2      (NULL),
        _x_master_voice (NULL)
{

}

DeviceAudioDX11::~DeviceAudioDX11 (void)
{
	stopAll();
	RELEASE(_camera);

    SAFE_RELEASE(_x_audio_2);
    _x_master_voice->DestroyVoice();
    _x_master_voice = NULL;
}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::initialize	(void)
{
    restartAudio ();
}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::shutdownAudio (void)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif
    
    // Save source for after reinitialize
    _save_channels.resize(_channels.size());
    
    for (DTuint c = 0; c < _channels.size(); ++c) {
        _save_channels[c]._source = RETAIN(_channels[c]._source);
        _save_channels[c]._sound_loader = RETAIN(_channels[c]._sound_loader);

        for (DTuint i = 0; i < ARRAY_SIZE(_save_channels[c]._packets); ++i) {
            _save_channels[c]._packets[i] = _channels[c]._packets[i];
        }
    }
    
    // Don't stop sound channels so that we can begin playing them again
	destroyChannels();   

    _x_master_voice->DestroyVoice();
    _x_master_voice = NULL;

    SAFE_RELEASE(_x_audio_2);
}

void DeviceAudioDX11::restartAudio (void)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

    if (!_x_audio_2) {
        // Initialize Audio system
        HRESULT hr = XAudio2Create( &_x_audio_2, 0, XAUDIO2_DEFAULT_PROCESSOR );

        Assert(SUCCEEDED(hr));

        // Initialize Mastering Voice
        hr = _x_audio_2->CreateMasteringVoice( &_x_master_voice );

        Assert(SUCCEEDED(hr));

        // Initialize 3D audio
        XAUDIO2_VOICE_DETAILS voice_details;
        _x_master_voice->GetVoiceDetails(&voice_details);

        // Initialize 3D audio subsystem
        X3DAudioInitialize( voice_details.InputChannels, X3DAUDIO_SPEED_OF_SOUND, _x3_instance );

        // Initialize listener
        ::memset(&_x3_listener,0,sizeof(X3DAUDIO_LISTENER));

        // Initialize DSP parameters
        ::memset(&_x3_dsp,0,sizeof(X3DAUDIO_DSP_SETTINGS));

        _x3_dsp_matrix.resize(voice_details.InputChannels * NUM_CHANNELS);

        _x3_dsp_delays.resize(voice_details.InputChannels);

        _x3_dsp.pMatrixCoefficients = &_x3_dsp_matrix[0];
        _x3_dsp.pDelayTimes = &_x3_dsp_delays[0];
        _x3_dsp.DstChannelCount = voice_details.InputChannels;
        _x3_dsp.SrcChannelCount = 0; // Changed on the fly when calculating


        // Initialize rest of this class
        setNumberOfChannels(32);
    
        // Get all channels playing again
        for (DTuint c = 0; c < min2(_channels.size(),_save_channels.size()); ++c) {                
            playOnChannel (_channels[c], _save_channels[c]._source, _save_channels[c]._sound_loader);
            RELEASE(_save_channels[c]._source);
            RELEASE(_save_channels[c]._sound_loader);
        }
        
        _save_channels.clear();
    }
}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::setCamera (CameraObject *camera)
{
	SAFE_ASSIGN(_camera, camera);
}

CameraObject*	DeviceAudioDX11::getCamera (void) const
{
	return _camera;
}

//==============================================================================
//==============================================================================

SoundSource* DeviceAudioDX11::playQuick (const FilePath &pathname)
{
    LOG_MESSAGE << "Play quick " << pathname.getPath();

	SoundResource *sound = SoundResource::getSound(pathname);
    	
	if (sound) {
        LOG_MESSAGE << "Playing sound " << pathname.getPath();
		return playQuick (sound);
	}
	
	return NULL;
}

SoundSource* DeviceAudioDX11::playQuick (SoundResource *sound)
{
	SoundSource* source = SoundSource::create();
    ScriptingSound* sound_loader = ScriptingSound::create();
    
    // Build the sound file loader
    sound_loader->setSoundProperty(sound);
    
    // Connect sound packet outputs
    PlugBase *s1 = source->getPlugByName("Sound_Packet");
    PlugBase *s2 = sound_loader->getPlugByName("Sound_Packet");
    s1->setIncomingConnection(s2);
    
    // Play the sound source
	if (play(source, sound_loader)) {
    
        if (source->countReferences() <= 1) {
            RELEASE(source);
        } else {
            source->release();	// Playing increments ref count so this pointer is still good
        }
        
        RELEASE(sound_loader);
		return source;
	} else {
        RELEASE(sound_loader);
		RELEASE(source);
		return NULL;
	}
}

//==============================================================================
//==============================================================================

DTfloat DeviceAudioDX11::estimatePriority (SoundSource *source)
{
    if (getCamera()) {
        Vector3 diff;
        diff = getCamera()->getTranslation() - source->getTranslation();
        
        DTfloat dist_squared = diff.abs2();
        
        return (1.0F / dist_squared);
    } else {
        return 0.0F;
    }
}

//==============================================================================
//==============================================================================

DTboolean DeviceAudioDX11::loadChunk (DeviceAudioDX11Channel &channel)
{	
	PROFILER(PROFILER_SOUND);
			
	// Stream chunk in	
    ++channel._current_packet;
    channel._current_packet = channel._current_packet % ARRAY_SIZE(channel._packets);

    SoundPacket &packet = channel._packets[channel._current_packet];
    	
	// Stream chunk in	
	packet.copy(channel._source->getNextSoundPacket());
    if (packet.getNumBytes() == 0)
        return false;


    DTuint num_channels;
    DTuint sample_size;

    // Update number of channels
    switch(packet.getFormat()) {
		case SoundResource::FORMAT_MONO8:
            sample_size = 8;
            num_channels = 1;
            break;	
        case SoundResource::FORMAT_MONO16:	
            sample_size = 16;
            num_channels = 1;
            break;	

		case SoundResource::FORMAT_STEREO8:
            sample_size = 8;
            num_channels = 2;
            break;	

		case SoundResource::FORMAT_STEREO16:
            sample_size = 16;
            num_channels = 2;
            break;	

		default:	return false;
	};

    // Rebuild the sound source if necessary
    initializeChannel(channel, packet.getFrequency(), sample_size, num_channels);

	return true;
}

void DeviceAudioDX11::pumpAllStreaming (void)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

    for (DTuint c = 0; c < _channels.size(); ++c) {
        DeviceAudioDX11Channel &channel = _channels[c];
        SoundSource *source = channel._source;
        
        if (!source)
            continue;

        XAUDIO2_VOICE_STATE state;

        channel._x_voice->GetState(&state);



        while (state.BuffersQueued < NUM_BUFFERS) {



            if (loadChunk (channel)) {
                SoundPacket &packet = channel._packets[channel._current_packet];

                // Enqueue the buffer
                XAUDIO2_BUFFER x_buffer;
                x_buffer.Flags = 0;

                x_buffer.AudioBytes = packet.getNumBytes();

                x_buffer.pAudioData = packet.getBuffer();

                x_buffer.PlayBegin = 0;

                x_buffer.PlayLength = 0;

                x_buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;

                x_buffer.LoopLength = 0;

                x_buffer.LoopCount = 0;

                x_buffer.pContext = &channel;

                channel._x_voice->SubmitSourceBuffer(&x_buffer);   
            } else {
                channel._x_voice->Discontinuity();
                break;
            }


            channel._x_voice->GetState(&state);

        }

    }
}

void DeviceAudioDX11::primeStreaming (DeviceAudioDX11Channel &channel)
{	
    
    // Load the buffer	
    for (DTuint i = 0; i < NUM_BUFFERS; ++i) {
        if (loadChunk (channel)) {
            SoundPacket &packet = channel._packets[channel._current_packet];

            // Enqueue the buffer
            XAUDIO2_BUFFER x_buffer;
            x_buffer.Flags = 0;

            x_buffer.AudioBytes = packet.getNumBytes();

            x_buffer.pAudioData = packet.getBuffer();

            x_buffer.PlayBegin = 0;

            x_buffer.PlayLength = 0;

            x_buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;

            x_buffer.LoopLength = 0;

            x_buffer.LoopCount = 0;

            x_buffer.pContext = &channel;

            channel._x_voice->SubmitSourceBuffer(&x_buffer);   
        } else {
            channel._x_voice->Discontinuity();
            break;
        }
    }
    
    channel._needs_priming = false;
}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::playOnChannel (DeviceAudioDX11Channel &channel, SoundSource *source, ScriptingSound *sound_loader)
{
	// If new sound is valid
    if (source) {
		
		// Stop any existing sound
		stopOnChannel (channel);
	
		// Remember this source
		SAFE_ASSIGN(channel._source, source);		
		SAFE_ASSIGN(channel._sound_loader, sound_loader);		

		// Tell the sound group we are plaing the sound
        source->startSound();
		
		// Reset the chunk index to start streaming from the beginning
		//channel._chunk_index = 0;
		channel._ready_to_start = true;
        channel._needs_priming = true;
		
		// NOTE: This happens later
        // primeStreaming();
    }
    
}

void DeviceAudioDX11::stopOnChannel (DeviceAudioDX11Channel &channel)
{
	SoundSource *source = channel._source;
	
 	// Tell sound source that the sound is stopped
    if (source) {
        source->stopSound();

        channel._x_voice->Stop();

		// Release the connected sources and sound
		RELEASE(channel._source); 
        RELEASE(channel._sound_loader);
        
        channel._needs_priming = true;
	}
}

//==============================================================================
//==============================================================================

DTboolean DeviceAudioDX11::play (SoundSource *source, ScriptingSound *sound_loader)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);
	
	// Don't bother if gain is too low
	if (_gain < 0.01F)
		return false;
        

    // Search for this source already playing
    for (DTuint c = 0; c < _channels.size(); ++c) {
        DeviceAudioDX11Channel &channel = _channels[c];

        if (channel._source == source) {
            playOnChannel(channel, source, sound_loader);
            return true;
        }
    }  

    // Search for an empty channel
    for (DTuint c = 0; c < _channels.size(); ++c) {
		DeviceAudioDX11Channel &channel = _channels[c];

        if (channel._source == NULL) {
            playOnChannel(channel, source, sound_loader);
            return true;
        }
    }
    
    // No empty spots on list so we can check if we should
    // bump an existing sound. Find the lowest priority object.
    DTfloat smallest_pri = std::numeric_limits<DTfloat>::infinity();
    DTuint  smallest_pri_index = 0;
    
    for (DTuint c = 0; c < _channels.size(); ++c) {
        DTfloat pri = estimatePriority(_channels[c]._source);
        if (pri < smallest_pri) {
            smallest_pri_index = c;
            smallest_pri = pri;
        }
    }
    
    // Check if the new sound is more important
    if (smallest_pri < estimatePriority(source)) {    
        playOnChannel(_channels[smallest_pri_index], source, sound_loader);
		return true;
    }
	
	return false;
}

void DeviceAudioDX11::stop (SoundSource *source)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);

    // Search for this source already playing
    for (DTuint c = 0; c < _channels.size(); ++c) {
		DeviceAudioDX11Channel &channel = _channels[c];

        if (channel._source == source) {            
            stopOnChannel(channel);
        }
    }
    
}

void DeviceAudioDX11::stopAll (void)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);

    for (DTuint c = 0; c < _channels.size(); ++c) {
        stopOnChannel(_channels[c]);
    }
}

DTboolean DeviceAudioDX11::isPlaying (SoundSource *source)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);
    
    return getChannel(source) >= 0;
}

DTshort DeviceAudioDX11::getChannel (SoundSource *source)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);

    for (DTuint c = 0; c < _channels.size(); ++c) {
		DeviceAudioDX11Channel &channel = _channels[c];

        if (channel._source == source)
            return c;
    }
    
    return -1;
}


//==============================================================================
//==============================================================================

void DeviceAudioDX11::initializeChannel(DeviceAudioDX11Channel &channel, DTuint sample_rate, DTuint sample_size, DTuint num_channels)
{
    if (sample_rate == channel._x_format.nSamplesPerSec &&
        sample_size == channel._x_format.wBitsPerSample &&
        num_channels == channel._x_format.nChannels)
        return;

    channel._x_format.wFormatTag = WAVE_FORMAT_PCM;
    channel._x_format.nChannels = num_channels;
    channel._x_format.nSamplesPerSec = sample_rate;
    channel._x_format.wBitsPerSample = sample_size;
    channel._x_format.nBlockAlign = (channel._x_format.nChannels * channel._x_format.wBitsPerSample)/8;
    channel._x_format.nAvgBytesPerSec = channel._x_format.nSamplesPerSec * channel._x_format.nBlockAlign;
    channel._x_format.cbSize = 0;

    if (channel._x_voice)   channel._x_voice->DestroyVoice();
    HRESULT hr = _x_audio_2->CreateSourceVoice( &channel._x_voice, 
                                                (WAVEFORMATEX*)&channel._x_format,
                                                0,
                                                XAUDIO2_DEFAULT_FREQ_RATIO,
                                                &channel);
    Assert(SUCCEEDED(hr));
}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::setNumberOfChannels (const DTint num_channels)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);

    clearChannels();
	
    _channels.resize(num_channels); 

    for (DTuint c = 0; c < _channels.size(); ++c) {
		DeviceAudioDX11Channel &channel = _channels[c];

        channel._x_voice = NULL;

        // Initialize channel
        ::memset(&channel._x_format, 0, sizeof(WAVEFORMATEX));
        initializeChannel(channel, 44100, 16, 1);

        // Initialize 3D emitter
        ::memset(&channel._x3_emitter,0,sizeof(X3DAUDIO_EMITTER));
        channel._x3_emitter.ChannelCount = 1;
        channel._x3_emitter.CurveDistanceScaler = FLT_MIN;

        channel._current_packet = 0;
        channel._source = NULL;
		channel._ready_to_start = false;
        channel._needs_priming = false;

        channel._is_playing = false;
    }
}

void DeviceAudioDX11::clearChannels (void)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);

    stopAll();
	destroyChannels();
}

void DeviceAudioDX11::destroyChannels (void)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

	PROFILER(PROFILER_SOUND);

    for (DTuint c = 0; c < _channels.size(); ++c) {
 		DeviceAudioDX11Channel &channel = _channels[c];

        channel._x_voice->DestroyVoice();
        channel._x_voice = NULL;

        RELEASE(channel._source);
        RELEASE(channel._sound_loader);
    }
	
    _channels.clear();
}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::tick (const DTfloat dt)
{		

	PROFILER(PROFILER_SOUND);
    
    if (!_x_audio_2)
        return;

    DTfloat sound_on = castFromString<DTboolean>(Globals::getGlobal("SYS_SOUND")) ? 1.0F : 0.0F;		
    DTfloat sound_gain = Globals::hasGlobal("SYS_SOUND_GAIN") ? castFromString<DTfloat>(Globals::getGlobal("SYS_SOUND_GAIN")) : 1.0F;

    _gain = sound_on * sound_gain;
    
    //
    // Update Camera
    //

    if (getCamera()) {
        // Update Listener
        Vector3 position = getCamera()->getTranslation();
        Vector3 velocity = getCamera()->getVelocity();
        
        Vector3 forward = getCamera()->getForwards();
        Vector3 up = getCamera()->getUp();

        _x3_listener.OrientFront = X3DAUDIO_VECTOR(forward.x,forward.y,forward.z);

        _x3_listener.OrientTop = X3DAUDIO_VECTOR(up.x,up.y,up.z);

        _x3_listener.Position = X3DAUDIO_VECTOR(position.x,position.y,position.z);

        _x3_listener.Velocity = X3DAUDIO_VECTOR(velocity.x,velocity.y,velocity.z);
        
        _x_master_voice->SetVolume(_gain);
    } else {
        _x3_listener.OrientFront = X3DAUDIO_VECTOR(0.0F,0.0F,-1.0F);

        _x3_listener.OrientTop = X3DAUDIO_VECTOR(0.0F,1.0F,0.0F);

        _x3_listener.Position = X3DAUDIO_VECTOR(0.0F,0.0F,0.0F);

        _x3_listener.Velocity = X3DAUDIO_VECTOR(0.0F,0.0F,0.0F);
        
        _x_master_voice->SetVolume(_gain);
    }


    //
    // Update channels
    //

    for (DTuint c = 0; c < _channels.size(); ++c) {
        DeviceAudioDX11Channel &channel = _channels[c];
        SoundSource *source = channel._source;
    
        // If there's a source and a sound then this channel is playing
        if (source) {
            SoundPacket &packet = channel._packets[channel._current_packet];

            // Check to see if sound buffer was starved
            if (!channel._ready_to_start && packet.getNumBytes() > 0 && !channel._is_playing) {
                
                channel._ready_to_start = true;
                LOG_MESSAGE << "Audio stream starved";
            
            // Check if sound data has been depleted
            } else if (!channel._ready_to_start && packet.getNumBytes() == 0 && !channel._is_playing) {
#if DT2_MULTITHREADED_AUDIO
                AutoSpinLockRecursive lock(&_lock);
#endif

                stopOnChannel(channel);
                LOG_MESSAGE << "Audio stream done";
                
            // Update sound
            } else {
                // Maybe channel needs to be started
                if (channel._ready_to_start) {
#if DT2_MULTITHREADED_AUDIO
                    AutoSpinLockRecursive lock(&_lock);
#endif

                    // Fill buffers
                    if (channel._needs_priming)
                        primeStreaming (channel);

                    channel._is_playing = true;
                    channel._x_voice->Start();
              
                    channel._ready_to_start = false;
                }
                
                Vector3 position = source->getTranslation();
                Vector3 velocity = source->getVelocity();
                //DTboolean looping = source->Get_Looping();	// Instead we loop sounds ourselves
                DTfloat pitch = source->getPitch();
                DTfloat rolloff = source->getRolloff();
                DTfloat gain = source->getGain();

                // position sound
                if (!getCamera()) {
                    channel._x3_emitter.Position = X3DAUDIO_VECTOR(0.0F,0.0F,0.0F);

                } else if (position == Vector3(0.0F,0.0F,0.0F)) {
                    Vector3 camera_position = getCamera()->getTranslation();
                    channel._x3_emitter.Position = X3DAUDIO_VECTOR(camera_position.x, camera_position.y, camera_position.z);

                } else {
                    channel._x3_emitter.Position = X3DAUDIO_VECTOR(position.x, position.y, position.z);

                }

                channel._x3_emitter.Velocity = X3DAUDIO_VECTOR(velocity.x, velocity.y, velocity.z);
                channel._x_voice->SetVolume(gain);

                //::alSourcef(channel._al_source, AL_PITCH, pitch);
                //CheckAL();

                //::alSourcef(channel._al_source, AL_ROLLOFF_FACTOR, rolloff);
                //CheckAL();
                
            }
        }
    }
    
    // Update Busses
    List<Bus>::iterator i;
    FOR_EACH (i,_busses) {
        Bus &bus = *i;
    
        // Get the number of buffers that have been processed
        //ALint num_buffers;
        //::alGetSourcei(bus._al_source, AL_BUFFERS_PROCESSED, &num_buffers);
        //bus._num_active -= num_buffers;
            
        // Pump all of the buffers
        //while (bus._num_active < 3) {
        //
        //    // Deque one buffer
        //    //ALuint buffer;
        //    //while (num_buffers > 0) {
        //    //    ::alSourceUnqueueBuffers(bus._al_source, 1, &buffer );
        //    //    --num_buffers;
        //    //    CheckAL();
        //    //}
        //    
        //    // Stream chunk in	
        //    SoundPacket packet = bus._source->getNextSoundPacket();
        //    if (packet.getNumBytes() == 0)
        //        break;

        //    // Choose appropriate format
        //    //ALenum format;
        //    //switch(packet.getFormat()) {
        //    //    case SoundResource::FORMAT_MONO8:		format = AL_FORMAT_MONO8;		break;
        //    //    case SoundResource::FORMAT_MONO16:		format = AL_FORMAT_MONO16;		break;
        //    //    case SoundResource::FORMAT_STEREO8:		format = AL_FORMAT_STEREO8;		break;
        //    //    case SoundResource::FORMAT_STEREO16:	format = AL_FORMAT_STEREO16;	break;
        //    //};
        //        
        //    // Store data into the buffer
        //    //CheckAL();
        //    //::alBufferData(bus._buffer[0], format, packet.getBuffer(), packet.getNumBytes(), packet.getFrequency());
        //    //CheckAL();
        //    
        //    // Enqueue the buffer
        //    //::alSourceQueueBuffers(bus._al_source, 1, &(bus._buffer[0]));
        //    
        //    // Rotate the ring buffer
        //    //ALuint temp = bus._buffer[0];
        //    //bus._buffer[0] = bus._buffer[1];
        //    //bus._buffer[1] = bus._buffer[2];
        //    //bus._buffer[2] = temp;

        //    ++bus._num_active;
        //}
        
        if (bus._num_active > 0) {
            // Check to make sure source is playing if there is new data
            //ALenum state;
            //::alGetSourcei(bus._al_source, AL_SOURCE_STATE, &state);
            //CheckAL();
            //    
            //// Check to see if sound buffer was starved
            //if (state != AL_PLAYING) {
            //    ::alSourcePlay(bus._al_source);
            //}
        }
    }
	
#if DT2_MULTITHREADED_AUDIO
    // Shedule pumping of streaming audio
    LatentCall0 *latent_call = new ("LatentCall0") LatentCall0(NULL, &_pumper_callback);
    ThreadTaskQueue::addTask(uniqueID(), latent_call);
    RELEASE(latent_call);
#else
    pumpAllStreaming();
#endif


}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::addBus (SoundSource *source)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

    Bus &bus = _busses.pushBack();
    
    bus._source = RETAIN(source);
    bus._num_active = 0;
    
    //::alGenSources(1, &(bus._al_source));		

}

void DeviceAudioDX11::removeBus (SoundSource *source)
{
#if DT2_MULTITHREADED_AUDIO
    AutoSpinLockRecursive lock(&_lock);
#endif

    List<Bus>::iterator i;
    FOR_EACH (i,_busses) {
        Bus &bus = *i;
        
        if (bus._source == source) {
            RELEASE(bus._source);
        
             bus._x_voice->DestroyVoice();
            
            _busses.erase(i);
            return;
        }
    
    }
}

//==============================================================================
//==============================================================================

void DeviceAudioDX11::emergencyFreeMemory	(void)
{

}

//==============================================================================
//==============================================================================

} // DT2


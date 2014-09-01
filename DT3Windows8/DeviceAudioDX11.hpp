#ifndef DT2_DEVICEAUDIODX11
#define DT2_DEVICEAUDIODX11
//==============================================================================
///	
///	File: DeviceAudioDX11.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "WinUtils.hpp"
#include "DeviceAudio.hpp"
#include "BaseInclude.hpp"
#include "Cache.hpp"
#include "SoundPacket.hpp"

#include "Array.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
//==============================================================================

void    CheckDX11Msg    (const DTcharacter* file, const DTcharacter* func, const DTint line);

#ifdef DT2_DEBUG
	#define CheckDX11()  	DT2::CheckDX11Msg   ((DT2::DTcharacter*)__FILE__, (DT2::DTcharacter*)__FUNCTION__, (DT2::DTint)__LINE__)
#else
	#define CheckDX11()  		
#endif

//==============================================================================
// Forward Declarations
//==============================================================================

class SoundResource;
class SoundSource;
class CameraObject;
class ScriptingSound;

//==============================================================================
//==============================================================================

class DeviceAudioDX11Channel : public IXAudio2VoiceCallback
{
    public:
        void __stdcall OnStreamEnd                (void)                                  {   _is_playing = false;    }
        void __stdcall OnVoiceProcessingPassEnd   (void)                                  {}
        void __stdcall OnVoiceProcessingPassStart (UINT32 SamplesRequired)                {}
        void __stdcall OnBufferEnd                (void *pBufferContext)                  {}
        void __stdcall OnBufferStart              (void *pBufferContext)                  {}
        void __stdcall OnLoopEnd                  (void *pBufferContext)                  {}
        void __stdcall OnVoiceError               (void *pBufferContext, HRESULT Error)   {   _is_playing = false;    }

    public:
        IXAudio2SourceVoice         *_x_voice;
        WAVEFORMATEX                _x_format;
        X3DAUDIO_EMITTER            _x3_emitter;

		DTboolean					_ready_to_start;
        DTboolean                   _needs_priming;

        SoundSource					*_source;
        ScriptingSound              *_sound_loader;
        SoundPacket                 _packets[3]; 
        DTint                       _current_packet;
            
        // State flags
        DTboolean                   _is_playing;  
};

//==============================================================================
//==============================================================================

class DeviceAudioDX11: public DeviceAudio {
		/// OpenAL based sound manager.

    public:
        DEFINE_TYPE(DeviceAudioDX11,DeviceAudio)
		DEFINE_CREATE

										DeviceAudioDX11		    (void);
	private:
										DeviceAudioDX11		    (const DeviceAudioDX11&);
        DeviceAudioDX11 &		        operator =              (const DeviceAudioDX11&);
	public:
        virtual							~DeviceAudioDX11		(void);
        
	public:
		/// Shuts down the OpenAL system
		virtual void					shutdownAudio			(void);

		/// Restarts the OpenAL system
		virtual void					restartAudio			(void);
		
		/// Called to initialize the object
		virtual void					initialize				(void);

		/// Sets the camera object which defines the properties of the listener
		/// \param camera listener camera
		virtual void					setCamera				(CameraObject *camera);

		/// Returns the camera object used as the listener
		/// \return listener camera
		virtual CameraObject *			getCamera				(void) const;	

		/// Plays the sound group
		/// \param source sound group to play
        virtual SoundSource *			playQuick				(const FilePath &pathname);

		/// Plays the sound group
		/// \param source sound group to play
        virtual SoundSource *			playQuick				(SoundResource *sound);
		
		/// Plays the sound source
		/// \param source sound source to play
        virtual DTboolean				play                    (SoundSource *source, ScriptingSound *sound_loader = NULL);

		/// Returns wether the sound source is playing or not
		/// \return playing or not
        virtual DTboolean				isPlaying               (SoundSource *source);

		/// Returns which channel a source is playing on
		/// \return channel index
        virtual DTshort                 getChannel              (SoundSource *source);

		/// Stops the sound source
		/// \param source sound source to stop
        virtual void					stop                    (SoundSource *source);

		/// Stops all sound sources
        virtual void					stopAll                 (void);
    
		/// Sets the number of channels to play on
		/// \param num_channels number of sound channels
        virtual void					setNumberOfChannels     (const DTint num_channels);
        
		/// Ticks the sound manager
		/// \param dt time step
        virtual void					tick					(const DTfloat dt);


		/// Adds a bus
		/// \param sound source
        virtual void                    addBus                  (SoundSource *source);

		/// Removes a bus
		/// \param sound source
        virtual void                    removeBus               (SoundSource *source);


		/// Called when an allocation has failed
		virtual void					emergencyFreeMemory     (void);

    private:
        friend class DeviceAudioDX11Callback;

        //
        // Standard audio
        //
        
		void                            clearChannels		    (void);
		void                            destroyChannels		    (void);

        enum {
            NUM_BUFFERS = 3,
            NUM_CHANNELS = 2
        };

		Array<DeviceAudioDX11Channel>					_channels;
		CameraObject					*_camera;

		IXAudio2*                       _x_audio_2;
        IXAudio2MasteringVoice*         _x_master_voice;

        X3DAUDIO_HANDLE                 _x3_instance;
        X3DAUDIO_DSP_SETTINGS           _x3_dsp;
        Array<FLOAT32>                  _x3_dsp_matrix;
        Array<FLOAT32>                  _x3_dsp_delays;
        X3DAUDIO_LISTENER               _x3_listener;
		
        void                            initializeChannel       (DeviceAudioDX11Channel &channel, DTuint sample_rate, DTuint sample_size, DTuint num_channels);

        void                            pumpAllStreaming        (void);
        void                            primeStreaming          (DeviceAudioDX11Channel &channel);

		DTboolean						loadChunk				(DeviceAudioDX11Channel &channel);

		void							playOnChannel			(DeviceAudioDX11Channel &channel, SoundSource *source, ScriptingSound *sound_loader = NULL);
		void							stopOnChannel			(DeviceAudioDX11Channel &channel);
        DTfloat							estimatePriority        (SoundSource *source);

		DTfloat							_gain;
        
        SpinLockRecursive               _lock;
        
        //
        // Busses
        //
        

        struct Bus {
            IXAudio2SourceVoice         *_x_voice;
            DTint                       _num_active;
            SoundSource					*_source;
        };
        
        List<Bus>                       _busses;
        
        //
        // Temporary backup
        //
        
		Array<DeviceAudioDX11Channel>   _save_channels;
};

//==============================================================================
//==============================================================================

} // DT2

#endif

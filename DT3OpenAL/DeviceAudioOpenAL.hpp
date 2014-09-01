#ifndef DT3_DEVICEAUDIOOPENAL
#define DT3_DEVICEAUDIOOPENAL
//==============================================================================
///	
///	File: DeviceAudioOpenAL.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include <mutex>
#include <vector>

#if DT3_OS == DT3_WINDOWS
    #include <al.h>
    #include <alc.h>
#elif DT3_OS == DT3_ANDROID
    #include "AL/al.h"
    #include "AL/alc.h"
#else
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#endif


//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

void    CheckALMsg    (const DTcharacter* file, const DTcharacter* func, const DTint line);

#ifdef DT3_DEBUG
	#if DT3_OS == DT3_WINDOWS
		#define CheckAL()  	DT3::CheckALMsg   ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__FUNCTION__, (DT3::DTint)__LINE__)
	#else
		#define CheckAL()  	DT3::CheckALMsg   ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__func__, (DT3::DTint)__LINE__)
	#endif
#else
	#if DT3_OS == DT3_WINDOWS
		#define CheckAL()  		
	#else
		#define CheckAL()  
	#endif
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

class DeviceAudioOpenAL: public DeviceAudio {
		/// OpenAL based sound manager.

    public:
        DEFINE_TYPE(DeviceAudioOpenAL,DeviceAudio)
		DEFINE_CREATE

                                                DeviceAudioOpenAL		(void);
	private:
                                                DeviceAudioOpenAL		(const DeviceAudioOpenAL&);
        DeviceAudioOpenAL &                     operator =              (const DeviceAudioOpenAL&);
	public:
        virtual                                 ~DeviceAudioOpenAL		(void);
        
	public:
        /// initialize sound manager
        virtual void                            initialize              (void);
    
		/// Shuts down the OpenAL system
		virtual void                            shutdown_audio          (void);

		/// Shuts down the OpenAL system
		virtual void                            restart_audio           (void);
    
	
		/// Sets the camera object which defines the properties of the listener
		/// \param camera listener camera
		virtual void                            set_camera              (const std::shared_ptr<CameraObject> &camera);

		/// Returns the camera object used as the listener
		/// \return listener camera
		virtual std::shared_ptr<CameraObject>   camera                  (void) const;
    

		/// Sets the gain for the entire world
		/// \param gain for the world
        virtual void                            set_world_gain          (World *world, DTfloat gain);
    

		/// Plays the sound group
		/// \param source sound group to play
        virtual std::shared_ptr<SoundSource>    play_quick              (const FilePath &pathname, World *world);
		
		/// Plays the sound group
		/// \param source sound group to play
        virtual std::shared_ptr<SoundSource>    play_quick              (const std::shared_ptr<SoundResource> &source, World *world);

		/// Plays the sound source
		/// \param source sound source to play
		/// \param sound_loader optional sound loader if you want it deleted
        virtual DTboolean                       play                    (const std::shared_ptr<SoundSource> &source, World *world, const std::shared_ptr<ScriptingSound> &sound_loader);

		/// Returns wether the sound source is playing or not
		/// \return playing or not
        virtual DTboolean                       is_playing              (const std::shared_ptr<SoundSource> &source);

		/// Returns which channel a source is playing on
		/// \return channel index
        virtual DTshort                         channel                 (const std::shared_ptr<SoundSource> &source);

		/// Stops the sound source
		/// \param source sound source to stop
        virtual void                            stop                    (const std::shared_ptr<SoundSource> &source);

		/// Stops all sound sources
        virtual void                            stop_all                (void);

		/// Stops the sources for the entire world
		/// \param world world to stop all of the sources on
        virtual void                            stop_all_world          (World *world);

		/// Sets the number of channels to play on
		/// \param num_channels number of sound channels
        virtual void                            set_number_of_channels  (const DTint num_channels);
        
		/// Adds a bus
		/// \param sound source
        virtual void                            add_bus                 (const std::shared_ptr<SoundSource> &source);

		/// Removes a bus
		/// \param sound source
        virtual void                            remove_bus              (const std::shared_ptr<SoundSource> &source);


        // Tick device
        /// \param dt delta time
        virtual void                            tick                    (const DTfloat dt);

    private:

        //
        // Standard audio
        //
        
		void                                    clear_channels		    (void);
		void                                    destroy_channels        (void);

		ALCcontext                              *_al_context;
		ALCdevice                               *_al_device;

        struct Channel {
            ALuint                              _al_source;
            ALuint                              _buffer[3];
            DTboolean                           _buffer_in_use[3];

			DTboolean                           _ready_to_start;
            DTboolean                           _needs_priming;

            std::shared_ptr<SoundSource>        _source;
            std::shared_ptr<ScriptingSound>     _sound_loader;
            SoundPacket                         _packet;
            
            DTfloat                             _world_gain;
            World                               *_world;
        };
        
		std::vector<Channel>                    _channels;
		std::shared_ptr<CameraObject>           _camera;
		
		
        void                                    pump_all_streaming      (void);
        void                                    prime_streaming         (Channel &channel);

		DTboolean                               load_chunk				(ALuint buffer, Channel &channel);

		void                                    play_on_channel			(   Channel &channel,
                                                                            const std::shared_ptr<SoundSource> &source,
                                                                            World *world,
                                                                            const std::shared_ptr<ScriptingSound> &sound_loader);
    
		void                                    stop_on_channel			(Channel &channel);
        DTfloat                                 estimate_priority       (const std::shared_ptr<SoundSource> &source);

		DTfloat                                 _gain;
    
        std::mutex                              _mutex;
    
        //
        // Busses
        //
        

        struct Bus {
            ALuint                              _al_source;
            ALuint                              _buffer[3];
            DTint                               _num_active;
            std::shared_ptr<SoundSource>        _source;
        };
        
        std::list<Bus>                          _busses;
        
        //
        // Temporary backup
        //
        
		std::vector<Channel>                    _save_channels;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

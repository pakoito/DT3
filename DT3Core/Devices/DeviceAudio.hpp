#ifndef DT3_DEVICEAUDIO
#define DT3_DEVICEAUDIO
//==============================================================================
///	
///	File: DeviceAudio.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Devices/DeviceBase.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Sound/SoundSource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================

class SoundResource;
class CameraObject;
class FilePath;
class ScriptingSound;
class World;

//==============================================================================
/// Class
//==============================================================================

class DeviceAudio: public DeviceBase {
		/// Base class for Audio Drivers.

    public:
        DEFINE_TYPE(DeviceAudio,DeviceBase)
		DEFINE_CREATE

                                                DeviceAudio             (void);
	private:
                                                DeviceAudio             (const DeviceAudio&);
        DeviceAudio &                           operator =              (const DeviceAudio&);
	public:
        virtual                                 ~DeviceAudio            (void);
        
	public:
		/// Shuts down the audio system
		virtual void                            shutdown_audio          (void)  {};

		/// Shuts down the audio system
		virtual void                            restart_audio           (void)  {};
    
	
		/// Sets the camera object which defines the properties of the listener
		/// \param camera listener camera
		virtual void                            set_camera              (const std::shared_ptr<CameraObject> &camera)   {}

		/// Returns the camera object used as the listener
		/// \return listener camera
		virtual std::shared_ptr<CameraObject>   camera                  (void) const    {   return NULL;    }
    

		/// Sets the gain for the entire world
		/// \param gain for the world
        virtual void                            set_world_gain          (const std::shared_ptr<World> &world, DTfloat gain) {}
    

		/// Plays the sound group
		/// \param source sound group to play
        virtual std::shared_ptr<SoundSource>    play_quick              (const FilePath &pathname, World *world)                        {   return NULL;    }
		
		/// Plays the sound group
		/// \param source sound group to play
        virtual std::shared_ptr<SoundSource>    play_quick              (const std::shared_ptr<SoundResource> &source, World *world)    {   return NULL;    }

		/// Plays the sound source
		/// \param source sound source to play
		/// \param sound_loader optional sound loader if you want it deleted
        virtual DTboolean                       play                    (const std::shared_ptr<SoundSource> &source, const std::shared_ptr<World> &world, const std::shared_ptr<ScriptingSound> &sound_loader) {    return false;   }

		/// Returns wether the sound source is playing or not
		/// \return playing or not
        virtual DTboolean                       is_playing              (const std::shared_ptr<SoundSource> &source)    {   return false;   }

		/// Returns which channel a source is playing on
		/// \return channel index
        virtual DTshort                         channel                 (const std::shared_ptr<SoundSource> &source)    {   return -1;      }

		/// Stops the sound source
		/// \param source sound source to stop
        virtual void                            stop                    (const std::shared_ptr<SoundSource> &source)    {}

		/// Stops all sound sources
        virtual void                            stop_all                (void)                                          {}

		/// Stops the sources for the entire world
		/// \param world world to stop all of the sources on
        virtual void                            stop_all_world          (const std::shared_ptr<World> &world)           {}

		/// Sets the number of channels to play on
		/// \param num_channels number of sound channels
        virtual void                            set_number_of_channels  (const DTint num_channels)                      {}
        
		/// Adds a bus
		/// \param sound source
        virtual void                            add_bus                 (const std::shared_ptr<SoundSource> &source)    {}

		/// Removes a bus
		/// \param sound source
        virtual void                            remove_bus              (const std::shared_ptr<SoundSource> &source)    {}

        // Tick device
        /// \param dt delta time
        virtual void                            tick                    (const DTfloat dt)                              {}

};

//==============================================================================
//==============================================================================

} // DT3

#endif

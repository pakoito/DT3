#ifndef DT3_DEVICEMUSIC
#define DT3_DEVICEMUSIC
//==============================================================================
///	
///	File: DeviceMusic.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Devices/DeviceBase.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class DeviceMusic: public DeviceBase {
		/// Base class for Music Drivers.

    public:
        DEFINE_TYPE(DeviceMusic,DeviceBase)
		DEFINE_CREATE

										DeviceMusic             (void);
	private:
										DeviceMusic             (const DeviceMusic&);
        DeviceMusic &					operator =              (const DeviceMusic&);
	public:
        virtual							~DeviceMusic            (void);
        
	public:
		/// Shuts down the music system
		virtual void					shutdown_audio			(void)                  {}

		/// Shuts down the music system
		virtual void					restart_audio			(void)                  {}
        
		/// Plays the music file at the specified path
		/// \param pathname path to music file
        virtual void					play_music				(const FilePath &pathname, DTfloat transition = 1.0F, DTboolean loop = true)    {};

		/// Stops the music
		/// \param pathname path to music file
        virtual void                    stop_music              (DTfloat transition)    {}
        virtual void					stop_music				(void)                  {}

		/// Sets the music volume
		/// \param volume music volume
        virtual void					set_volume				(DTfloat volume)        {}

		/// Returns how far the music has progressed as a fraction (0.0-1.0)
        /// \return progress
        virtual DTfloat                 progress                (void)                  {   return 0.0F;        }

		/// Returns whether a song is playing or not
        /// \return progress
        virtual DTboolean               playing                 (void)                  {   return false;       }
        
		/// Returns the currently playing song
        /// \return Song
        virtual FilePath                current_music           (void)                  {   return FilePath();  }
};

//==============================================================================
//==============================================================================

} // DT3

#endif

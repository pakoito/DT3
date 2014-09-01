#ifndef DT3_DEVICEMUSICAPPLEAQ
#define DT3_DEVICEMUSICAPPLEAQ
//==============================================================================
///	
///	File: DeviceMusicAppleAQ.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
/// NOTES: based on http://developer.apple.com/samplecode/AudioQueueTest/listing1.html
///
//==============================================================================

#include "DT3Core/Devices/DeviceMusic.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include <vector>

#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioFile.h>
#include <AudioToolbox/AudioToolbox.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================



//==============================================================================
//==============================================================================

class DeviceMusicAppleAQ: public DeviceMusic {
		/// Mac and iOS music driver.
    public:
        DEFINE_TYPE(DeviceMusicAppleAQ,DeviceMusic)
		DEFINE_CREATE

									DeviceMusicAppleAQ		(void);
	private:
									DeviceMusicAppleAQ		(const DeviceMusicAppleAQ&);
        DeviceMusicAppleAQ&         operator =				(const DeviceMusicAppleAQ&);
	public:
        virtual						~DeviceMusicAppleAQ		(void);
        
	public:
		/// Shuts down the music system
		virtual void				shutdown_audio			(void) {}

		/// Shuts down the music system
		virtual void				restart_audio			(void) {}
       
		/// Plays the music file at the specified path
		/// \param pathname path to music file
        virtual void				play_music				(const FilePath &pathname, DTfloat transition = 1.0F, DTboolean loop = true);

		/// Stops the music
		/// \param pathname path to music file
        virtual void				stop_music              (DTfloat transition);
        virtual void				stop_music				(void);
     
		/// Sets the music volume
		/// \param volume music volume
        virtual void				set_volume				(DTfloat volume);

		/// Returns how far the music has progressed
        /// \return progress
        virtual DTfloat             progress                (void);

		/// Returns whether a song is playing or not
        /// \return progress
        virtual DTboolean           playing                 (void);

		/// Returns the currently playing song
        /// \return Song
        virtual FilePath            current_music           (void);

		/// Ticks the sound manager
		/// \param dt time step
        virtual void				tick					(const DTfloat dt);

    private:
        void						start_music				(const FilePath &pathname);
        
        //
        // Helper functions
        //
		
		void						derive_buffer_size		(	AudioStreamBasicDescription &ASBDesc,
																UInt32                      maxPacketSize,
																Float64                     seconds,
																UInt32                      *outBufferSize,
																UInt32                      *outNumPacketsToRead);
	
        static OSStatus             read_proc               (   void     *inClientData,
                                                                SInt64   inPosition,
                                                                UInt32   requestCount,
                                                                void     *buffer,
                                                                UInt32   *actualCount);
                                                                
        static SInt64               size_proc               (   void  *inClientData );


        //
        // Streaming structure
        //

        static const DTint NUM_BUFFERS = 3;
		
		struct AQInfo {
			AudioFileID                                 _audio_file;
			AudioStreamBasicDescription                 _data_format;
			AudioQueueRef                               _queue;
			AudioQueueBufferRef                         _buffers[NUM_BUFFERS];
			UInt64                                      _packet_index;
			UInt32                                      _packets_to_read_per_buffer;
			UInt64                                      _num_packets;
			SInt32                                      _num_priming_frames;
			SInt32                                      _num_remainder_frames;
			std::vector<AudioStreamPacketDescription>	_packet_descs;
			DTboolean                                   _done;
            DTboolean                                   _loop;
		} _aq_info;
		
		// This function will be called upon to provide data when the audio queue calls it.
		// It can also be called manually shortly after initialization to prime to queue.
		static void aq_buffer_callback(	void					*inUserData,
										AudioQueueRef			inAQ,
										AudioQueueBufferRef		inCompleteAQBuffer);
			
        BinaryFileStream        _file;
		UInt32					_buffer_size;

		DTfloat					_music_master_volume;
		DTfloat					_music_volume;
		DTfloat					_music_volume_vel;
		
		DTfloat					_old_volume;
		
		FilePath				_music_current;
		FilePath				_music_pending;

		DTfloat					_tick_accum;
        DTboolean               _loop;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif

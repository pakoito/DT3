#ifndef DT3_HWVIDEOPLAYERFFINSTANCE
#define DT3_HWVIDEOPLAYERFFINSTANCE
//==============================================================================
///	
///	URL: 			HWVideoPlayerFFInstance.hpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFDataSourceBase.hpp"
#include "HWVideoPlayerFFPacketQueue.hpp"
#include "HWVideoPlayerFFCommandQueue.hpp"

#include "std::shared_ptr.hpp"
#include "TimerHires.hpp"
#include "TextureResource.hpp"

#include "SoundSourceQueue.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class HWVideoPlayerFFInstance: public BaseRefCounted {
    public:
        DEFINE_CREATE

                                        HWVideoPlayerFFInstance     (void);
    private:
                                        HWVideoPlayerFFInstance     (const HWVideoPlayerFFInstance &rhs);
        HWVideoPlayerFFInstance&        operator =                  (const HWVideoPlayerFFInstance &rhs);
    public:
        virtual                         ~HWVideoPlayerFFInstance	(void);
    
	public:
    
        /// Open a video file
		/// \param path path to video file
        /// \return Error
        DTerr                           open                (const FilePath &path);

		/// Open a video URL
		/// \param purl url of video file
        /// \return Error
        DTerr                           open                (const URL &url);

		/// Close the instance
        void                            close               (void);


        //
        // Playback controls
        //

        /// Start the movie playing
        void                            play                (void);
    
        /// Pause the movie
        void                            pause               (void);
    
        /// Seek the movie
        void                            seek                (DTdouble t);


        //
        // Info about movie
        //

        /// Returns the width of the video
        /// \return width
        DTint                           width               (void) const    {   return _width;  }
    
        /// Returns the height of the video
        /// \return height
        DTint                           height              (void) const    {   return _height; }
    
        /// Returns the texture that is being rendered
        std::shared_ptr<TextureResource>       getTexture          (void)          {   return _tex;    }
    
    
        /// Returns the Duration of the video
        DTdouble                        length              (void) const    {   return _length;  }

        /// Returns the current playback time of the video
        DTdouble                        currentTime         (void) const    {   return _current_time;  }


        /// Returns the start time of the buffer
        DTdouble                        bufferStartTime     (void) const;

        /// Returns the end time of the buffer
        DTdouble                        bufferEndTime       (void) const;

    
        enum State {
            STATE_IDLE,
            STATE_PAUSED,   // Loaded and waiting
            STATE_PLAYING,  // Currently playing
            STATE_CLOSING,
            STATE_ERROR
        };
    
        State                           getState            (void) const    {   return _state;  }

    private:
    
        DTerr                           setup               (void);
    
        static void                     playbackThread      (void *hwvp);
        static void                     dataThread          (void *hwvp);

    
        // Video meta data
        DTdouble                            _length;
    
        DTdouble                            _video_time_base;
        int64_t                             _video_time_start;

        DTdouble                            _audio_time_base;
        int64_t                             _audio_time_start;
    
        DTuint                              _width;
        DTuint                              _height;
    
        int64_t                             _buffer_start;
        int64_t                             _buffer_end;

        // Instance info
        HWVideoPlayerFFDataSourceBase       *_data_source;
    
        AVFrame                             *_src_frame;
        AVFrame                             *_dst_frame;
    
        AVFrame                             *_src_audio_frame;
        SoundPacket                         _dst_sound_packet;
    
        DTdouble                            _video_timestamp;
        DTdouble                            _audio_timestamp;
    
        SwsContext                          *_img_convert_ctx;
        SwrContext                          *_snd_convert_ctx;

        State                               _state;
        DTdouble                            _current_time;

        // Thread references
        HAL::ThreadType                     _playback_thread;
        HAL::ThreadType                     _data_thread;
        
        // Packet queue
        HWVideoPlayerFFPacketQueue          _video_packet_queue;
        HWVideoPlayerFFPacketQueue          _audio_packet_queue;
    
        // Command queue
        HWVideoPlayerFFCommandQueue         _data_command_queue;
        HWVideoPlayerFFCommandQueue         _playback_command_queue;    // This is only filled out after data thread
                                                                        // processes its command queue
    
        // Output texture
        std::shared_ptr<TextureResource>           _tex;
        std::shared_ptr<ArrayBlock<DTubyte> >      _tex_buffer;
        
        // System clock
        TimerHires                          _timer;
        DTdouble                            _playback_speed;
    
        // Audio Packets
        SoundSourceQueue                    _sound_source;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

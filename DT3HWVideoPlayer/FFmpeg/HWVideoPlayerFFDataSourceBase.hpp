#ifndef DT3_HWVIDEOPLAYERFFDATASOURCEBASE
#define DT3_HWVIDEOPLAYERFFDATASOURCEBASE
//==============================================================================
///	
///	File: 			HWVideoPlayerFFDataSourceBase.hpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "BaseRefCounted.hpp"
#include "SpinLock.hpp"

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libswresample/swresample.h"
}

//==============================================================================
/// Namespace
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class HWVideoPlayerFFDataSourceBase: public BaseRefCounted {
    public:
    
                                        HWVideoPlayerFFDataSourceBase   (void);
    private:
                                        HWVideoPlayerFFDataSourceBase   (const HWVideoPlayerFFDataSourceBase &rhs);
        HWVideoPlayerFFDataSourceBase&  operator =                      (const HWVideoPlayerFFDataSourceBase &rhs);
    public:
        virtual                         ~HWVideoPlayerFFDataSourceBase	(void);
    
	public:
    
		/// Open a video data source
        /// \return Error code
        virtual DTerr           open                (void) = 0;
    
		/// Closes a video source
        virtual void            close               (void) = 0;
    
        /// Returns AVIOContext
        AVIOContext*            avIOContext         (void)  {   return _av_io_context;  }

        /// Returns Format Context
        AVFormatContext*        formatContext       (void)  {   return _format_context;  }

        /// Index of video stream
        int                     videoStreamIndex    (void)  {   return _video_stream_index;  }

        /// Index of audio stream
        int                     audioStreamIndex    (void)  {   return _audio_stream_index;  }
    
        /// Returns Codec Context
        AVCodecContext*         videoCodecContext   (void)  {   return _video_codec_context;  }

        /// Returns Codec Context
        AVCodecContext*         audioCodecContext   (void)  {   return _audio_codec_context;  }
    
        /// Returns AVStream
        AVStream*               videoStream         (void)  {   return _video_stream_index >= 0 ? _format_context->streams[_video_stream_index] : NULL; }

        /// Returns AVStream
        AVStream*               audioStream         (void)  {   return _audio_stream_index >= 0 ? _format_context->streams[_audio_stream_index] : NULL; }
    
    protected:
        DTubyte                 *_buffer;
    
        AVIOContext             *_av_io_context;
        AVFormatContext         *_format_context;

        int                     _video_stream_index;
        int                     _audio_stream_index;
    
        AVCodecContext          *_video_codec_context;
        AVCodecContext          *_audio_codec_context;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

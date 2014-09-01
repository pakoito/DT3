#ifndef DT3_HWVIDEOPLAYERFF
#define DT3_HWVIDEOPLAYERFF
//==============================================================================
///	
///	File: 			HWVideoPlayerFF.hpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "BaseInclude.hpp"

#include "FilePath.hpp"
#include "URL.hpp"
#include "TextureResource.hpp"
#include "Callback.hpp"
#include "Rectangle.hpp"

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
}

//==============================================================================
/// Namespace
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TextureResource;

//==============================================================================
/// Class
//==============================================================================

class HWVideoPlayerFF {
    private:
                                        HWVideoPlayerFF     (void);
                                        HWVideoPlayerFF     (const HWVideoPlayerFF &rhs);
        HWVideoPlayerFF&                operator =          (const HWVideoPlayerFF &rhs);
        virtual                         ~HWVideoPlayerFF	(void);
    
	public:
    
        typedef void*                   HWVideoPlayerType;
    
		/// Initialize the FFMpeg Subsystem
        static void                         initialize              (void);

		/// Destroy the FFMpeg Subsystem
        static void                         destroy                 (void);

		/// Open a video file
		/// \param path path to video file
        /// \return video handle
        static HWVideoPlayerType            open                (const FilePath &path);

		/// Open a video URL
		/// \param purl url of video file
        /// \return video handle
        static HWVideoPlayerType            open                (const URL &url);

		/// Close the currently opened video
        /// \param hwvp video handle
        static void                         close               (HWVideoPlayerType hwvp);


        /// Checks if streaming video is likely to finish loading in time to play unterrupted
        /// \param hwvp video handle
        /// \return Is playback likely to succeed skip free
        static DTboolean                    isPlaybackLikelyToKeepUp    (HWVideoPlayerType hwvp);

		/// Play the currently opened video
        /// \param hwvp video handle
		/// \param sender pointer to node that fired the event
        static void                         play                (HWVideoPlayerType hwvp);

		/// Pause the currently opened video
        /// \param hwvp video handle
		/// \param sender pointer to node that fired the event
        static void                         pause               (HWVideoPlayerType hwvp);

		/// Rewinds the video to the beginning
        /// \param hwvp video handle
		/// \param sender pointer to node that fired the event
        static void                         rewind              (HWVideoPlayerType hwvp);

		/// Returns the length of the video
        /// \param hwvp video handle
        /// \return length (in seconds) of the video
        static DTfloat                      length              (HWVideoPlayerType hwvp);

		/// Returns the length of the video
        /// \param hwvp video handle
        /// \return length (in seconds) of the video
        static DTfloat                      currentTime         (HWVideoPlayerType hwvp);

		/// Seeks to a position in the video
        /// \param hwvp video handle
		/// \param time time to seek to
        static void                         seek                (HWVideoPlayerType hwvp, DTfloat time);
    
        enum State {
            STATE_IDLE,
            STATE_PAUSED,   // Loaded and waiting
            STATE_PLAYING,  // Currently playing
            STATE_CLOSING,
            STATE_ERROR
        };
    
		/// Returns the state of playback of the video
        /// \param hwvp video handle
        /// \return lstate of the video
        static State                        getState            (HWVideoPlayerType hwvp);

		/// Returns the size of the video
        /// \param hwvp video handle
		/// \param width width of video
		/// \param height height of video
        static void                         getSize             (HWVideoPlayerType hwvp, DTuint &width, DTuint &height);
    
		/// Returns the current frame for the video
        /// \param hwvp video handle
        /// \return current frame of the video
        static std::shared_ptr<TextureResource>    getFrame            (HWVideoPlayerType hwvp);
    
    private:
        static int                          lockManager         (void **mutex, enum AVLockOp op);
    
};

//==============================================================================
//==============================================================================

} // DT3

#endif

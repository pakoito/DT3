#ifndef DT3_HWVIDEOPLAYERFFCOMMANDQUEUE
#define DT3_HWVIDEOPLAYERFFCOMMANDQUEUE
//==============================================================================
///	
///	URL: 			HWVideoPlayerFFCommandQueue.hpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "BaseInclude.hpp"
#include "Mutex.hpp"
#include <list>

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
/// Class
//==============================================================================

class HWVideoPlayerFFCommandQueue {
    public:

                                        HWVideoPlayerFFCommandQueue     (void);
    private:
                                        HWVideoPlayerFFCommandQueue     (const HWVideoPlayerFFCommandQueue &rhs);
        HWVideoPlayerFFCommandQueue&    operator =                      (const HWVideoPlayerFFCommandQueue &rhs);
    public:
                                        ~HWVideoPlayerFFCommandQueue    (void);
    
	public:
    
        enum Command {
            CMD_PLAY,
            CMD_PAUSE,
            CMD_SEEK
        };
    
        /// Pushes the play command
        void            pushPlay        (void);
    
        /// Pushes the play command
        void            pushPause       (void);

        /// Pushes the seek command
        /// \param time Time to seek to
        void            pushSeek        (DTdouble time);
    
    
        /// Gets the next command in the queue
        DTboolean       popCommand      (Command &command, DTdouble &param);
    
    private:

        struct CommandEntry {
            Command     _command;
            DTdouble    _parameter;
        };

        std::list<CommandEntry>  _commands;
        Mutex               _lock;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

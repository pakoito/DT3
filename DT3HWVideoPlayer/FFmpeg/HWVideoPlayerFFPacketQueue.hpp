#ifndef DT3_HWVIDEOPLAYERFFPACKETQUEUE
#define DT3_HWVIDEOPLAYERFFPACKETQUEUE
//==============================================================================
///	
///	URL: 			HWVideoPlayerFFPacketQueue.hpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "BaseInclude.hpp"
#include "Mutex.hpp"

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

class HWVideoPlayerFFPacketQueue {
    public:

                                        HWVideoPlayerFFPacketQueue      (void);
    private:
                                        HWVideoPlayerFFPacketQueue      (const HWVideoPlayerFFPacketQueue &rhs);
        HWVideoPlayerFFPacketQueue&     operator =                      (const HWVideoPlayerFFPacketQueue &rhs);
    public:
                                        ~HWVideoPlayerFFPacketQueue     (void);
    
	public:
    
		/// Pushes a packet onto the back of the queue
        DTerr                           pushBack    (AVPacket *pkt);

		/// Pops a packet from the front of the queue
        DTboolean                       popFront    (AVPacket *pkt);

		/// Clears the packet queue
        void                            clear       (void);

		/// Returns the size of the queue
        DTsize                          size        (void) const    {   return _size;   }
    
    private:
        AVPacketList    *_first_pkt;
        AVPacketList    *_last_pkt;
        DTsize          _size;

        Mutex           _lock;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

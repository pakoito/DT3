//==============================================================================
///	
///	File: 			HWVideoPlayerFFPacketQueue.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFPacketQueue.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

HWVideoPlayerFFPacketQueue::HWVideoPlayerFFPacketQueue (void)
    :   _first_pkt  (NULL),
        _last_pkt   (NULL),
        _size       (0)
{
    
}

HWVideoPlayerFFPacketQueue::~HWVideoPlayerFFPacketQueue (void)
{

}

//==============================================================================
//==============================================================================

DTerr HWVideoPlayerFFPacketQueue::pushBack (AVPacket *pkt)
{
    if(av_dup_packet(pkt) < 0)
        return DT3_ERR_FILE_OPEN_FAILED;
    
    AVPacketList *packet_list = (AVPacketList *) ::av_malloc(sizeof(AVPacketList));
    
    packet_list->pkt = *pkt;
    packet_list->next = NULL;
    
    _lock.lock();
    
    // Queue is empty
    if (!_first_pkt) {
        _first_pkt = _last_pkt = packet_list;
    } else {
        _last_pkt->next = packet_list;
        _last_pkt = packet_list;
    }
    
    ++_size;
    
    _lock.unlock();
    
    return 0;
}

DTboolean HWVideoPlayerFFPacketQueue::popFront (AVPacket *pkt)
{
    _lock.lock();

    if (_first_pkt) {
        AVPacketList *first_pkt = _first_pkt;
        
        *pkt = _first_pkt->pkt;
        _first_pkt = _first_pkt->next;
        
        if (!_first_pkt)
            _last_pkt = NULL;
        
        ::av_free(first_pkt);
    
        --_size;

        _lock.unlock();
        return true;
    } else {

        _lock.unlock();
        return false;
    }
}

void HWVideoPlayerFFPacketQueue::clear (void)
{
    _lock.lock();

    while (_first_pkt) {
        AVPacketList *first_pkt = _first_pkt;

        _first_pkt = _first_pkt->next;
        
        ::av_free_packet(&first_pkt->pkt);
        ::av_free(first_pkt);
    }
    
    _first_pkt = NULL;
    _last_pkt = NULL;
    _size = 0;
    
    _lock.unlock();
}

//==============================================================================
//==============================================================================

} // DT3


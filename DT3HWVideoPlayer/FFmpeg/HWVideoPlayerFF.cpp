//==============================================================================
///	
///	File: 			HWVideoPlayerFF.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFF.hpp"

#include "StaticInitializer.hpp"

#include "HWVideoPlayerFFInstance.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,HWVideoPlayerFF::initialize())
GLOBAL_STATIC_DESTRUCTION(0,HWVideoPlayerFF::destroy())

//==============================================================================
//==============================================================================

int HWVideoPlayerFF::lockManager(void **mutex, enum AVLockOp op)
{
    if (mutex == NULL)
        return -1;

    switch(op) {
        case AV_LOCK_CREATE:
        {
            *mutex = static_cast<void*>(HAL::createMutex());
            break;
        }
        case AV_LOCK_OBTAIN:
        {
            HAL::MutexType m =  static_cast<HAL::MutexType>(*mutex);
            HAL::lockMutex(m);
            break;
        }
        case AV_LOCK_RELEASE:
        {
            HAL::MutexType m = static_cast<HAL::MutexType>(*mutex);
            HAL::unlockMutex(m);
            break;
        }
        case AV_LOCK_DESTROY:
        {
            HAL::MutexType m = static_cast<HAL::MutexType>(*mutex);
            HAL::destroyMutex(m);
            break;
        }
        default:
        break;
    }
    return 0;
}

void HWVideoPlayerFF::initialize (void)
{
#ifndef DT3_EDITOR
    ::av_register_all();
    ::av_lockmgr_register(&HWVideoPlayerFF::lockManager);
    //::av_log_set_level (AV_LOG_DEBUG);
#endif
}

void HWVideoPlayerFF::destroy (void)
{

}

//==============================================================================
//==============================================================================

HWVideoPlayerFF::HWVideoPlayerType HWVideoPlayerFF::open (const FilePath &path)
{
#ifndef DT3_EDITOR
    HWVideoPlayerFFInstance* hwvpi = new HWVideoPlayerFFInstance;
    hwvpi->open(path);
    
    return hwvpi;
#else
    return NULL;
#endif
}

HWVideoPlayerFF::HWVideoPlayerType HWVideoPlayerFF::open (const URL &url)
{
#ifndef DT3_EDITOR
    HWVideoPlayerFFInstance* hwvpi = new HWVideoPlayerFFInstance;
    hwvpi->open(url);

    return hwvpi;
#else
    return NULL;
#endif
}

void HWVideoPlayerFF::close (HWVideoPlayerFF::HWVideoPlayerType hwvp)
{
#ifndef DT3_EDITOR
    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    if (!hwvpi)
        return;
    
    hwvpi->close();
    RELEASE(hwvpi);
#endif
}

//==============================================================================
//==============================================================================

DTboolean HWVideoPlayerFF::isPlaybackLikelyToKeepUp (HWVideoPlayerType hwvp)
{
    Assert(hwvp);

    //HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);

    return true;
}

//==============================================================================
//==============================================================================

void HWVideoPlayerFF::play (HWVideoPlayerFF::HWVideoPlayerType hwvp)
{
#ifndef DT3_EDITOR
    Assert(hwvp);

    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    hwvpi->play();
#endif
}

void HWVideoPlayerFF::pause (HWVideoPlayerFF::HWVideoPlayerType hwvp)
{
#ifndef DT3_EDITOR
    Assert(hwvp);

    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    hwvpi->pause();
#endif
}

void HWVideoPlayerFF::rewind (HWVideoPlayerFF::HWVideoPlayerType hwvp)
{
    Assert(hwvp);

    seek(hwvp, 0.0F);
}

DTfloat HWVideoPlayerFF::length (HWVideoPlayerFF::HWVideoPlayerType hwvp)
{
#ifndef DT3_EDITOR
    Assert(hwvp);

    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    
    return hwvpi->length();
#else
    return 0.0F;
#endif
}

DTfloat HWVideoPlayerFF::currentTime (HWVideoPlayerFF::HWVideoPlayerType hwvp)
{
#ifndef DT3_EDITOR
    Assert(hwvp);

    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    
    return hwvpi->currentTime();
#else
    return 0.0F;
#endif
}

void HWVideoPlayerFF::seek (HWVideoPlayerFF::HWVideoPlayerType hwvp, DTfloat time)
{
#ifndef DT3_EDITOR
    Assert(hwvp);

    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    hwvpi->seek(time);
#endif
}

//==============================================================================
//==============================================================================

void HWVideoPlayerFF::getSize (HWVideoPlayerType hwvp, DTuint &width, DTuint &height)
{
#ifndef DT3_EDITOR
    Assert(hwvp);
    
    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    
    width = hwvpi->width();
    height = hwvpi->height();
#else
    width = 0;
    height = 0;
#endif
}

HWVideoPlayerFF::State HWVideoPlayerFF::getState (HWVideoPlayerFF::HWVideoPlayerType hwvp)
{
#ifndef DT3_EDITOR
    Assert(hwvp);
    
    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);

    switch (hwvpi->getState()) {
        case HWVideoPlayerFFInstance::STATE_IDLE:       return STATE_IDLE;
        case HWVideoPlayerFFInstance::STATE_PAUSED:     return STATE_PAUSED;
        case HWVideoPlayerFFInstance::STATE_PLAYING:    return STATE_PLAYING;
        case HWVideoPlayerFFInstance::STATE_CLOSING:    return STATE_CLOSING;
        case HWVideoPlayerFFInstance::STATE_ERROR:      return STATE_ERROR;
    }
#else
    return STATE_IDLE;
#endif
}

//==============================================================================
//==============================================================================

std::shared_ptr<TextureResource> HWVideoPlayerFF::getFrame (HWVideoPlayerType hwvp)
{
#ifndef DT3_EDITOR
    HWVideoPlayerFFInstance* hwvpi = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    
    return hwvpi->getTexture();
#else
    return std::shared_ptr<TextureResource>();
#endif
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: 			HWVideoPlayerFFCommandQueue.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFCommandQueue.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

HWVideoPlayerFFCommandQueue::HWVideoPlayerFFCommandQueue (void)
{
    
}

HWVideoPlayerFFCommandQueue::~HWVideoPlayerFFCommandQueue (void)
{

}

//==============================================================================
//==============================================================================

void HWVideoPlayerFFCommandQueue::pushPlay (void)
{
    CommandEntry ce;
    ce._command = CMD_PLAY;
    ce._parameter = 0.0;
    
    _lock.lock();
    _commands.pushBack(ce);
    _lock.unlock();
}

void HWVideoPlayerFFCommandQueue::pushPause (void)
{
    CommandEntry ce;
    ce._command = CMD_PAUSE;
    ce._parameter = 0.0;
    
    _lock.lock();
    _commands.pushBack(ce);
    _lock.unlock();
}

void HWVideoPlayerFFCommandQueue::pushSeek (DTdouble time)
{
    CommandEntry ce;
    ce._command = CMD_SEEK;
    ce._parameter = time;
    
    _lock.lock();
    
    // Remove any other pending seeks
    std::list<CommandEntry>::iterator i;
    
    for (i = _commands.begin(); i != _commands.end(); ) {
    
        if (i->_command == CMD_SEEK) {
            i = _commands.erase(i);
        } else {
            ++i;
        }
    
    }
    
    // Push new seek command
    _commands.pushBack(ce);
    
    _lock.unlock();
}

DTboolean HWVideoPlayerFFCommandQueue::popCommand (Command &command, DTdouble &param)
{
    _lock.lock();

    if (_commands.size() > 0) {
        CommandEntry ce;
        _commands.popFront(ce);
    
        command = ce._command;
        param = ce._parameter;
        
        _lock.unlock();
        return true;
    } else {
        _lock.unlock();
        return false;
    }

}

//==============================================================================
//==============================================================================

} // DT3


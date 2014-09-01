//==============================================================================
///	
///	File: ConsoleStream.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/Console.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Globals for handling messages
//==============================================================================

ConsoleErrorStream      ConsoleErrorVar;
ConsoleMessageStream	ConsoleMessageVar;
ConsoleDebugStream      ConsoleDebugVar;
ConsoleNullStream       ConsoleNullVar;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ConsoleStream::ConsoleStream(DTuint type)
{
    _ss = std::shared_ptr<SS>(new SS);
    _ss->_type = type;
}

ConsoleStream::ConsoleStream(ConsoleStream &rhs)
    :	_ss	(rhs._ss)
{

}
				
ConsoleStream::~ConsoleStream(void)
{
    if (_ss.use_count() <= 1) {
        
        switch (_ss->_type) {
            case SS_ERROR:		Console::log_error(_ss->_buffer.buffer());		break;
            case SS_MESSAGE:	Console::log_message(_ss->_buffer.buffer());   break;
            case SS_DEBUG:		Console::log_debug(_ss->_buffer.buffer());		break;
        };
        
    }
}

//==============================================================================
//==============================================================================

} // DT3

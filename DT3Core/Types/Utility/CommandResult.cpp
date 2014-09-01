//==============================================================================
///	
///	File: CommandResult.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CommandResult.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

CommandResult::CommandResult (void)
    :   _result         (false),
        _update_hint    (UPDATE_NONE)
{

}

CommandResult::CommandResult (const CommandResult& rhs)
    :   _message        (rhs._message),
        _result         (rhs._result),
        _update_hint    (rhs._update_hint),
        _undo_commands  (rhs._undo_commands)
{

}

CommandResult & CommandResult::operator = (const CommandResult& rhs)
{
    _message = rhs._message;
    _result = rhs._result;
    _update_hint = rhs._update_hint;
    _undo_commands = rhs._undo_commands;
    return (*this);
}

CommandResult::~CommandResult (void)
{

}

//==============================================================================
//==============================================================================

void CommandResult::append_undo_command (const CommandResult &r)
{
    _undo_commands.insert(_undo_commands.end(), r._undo_commands.begin(), r._undo_commands.end());
}

//==============================================================================
//==============================================================================


} // DT3

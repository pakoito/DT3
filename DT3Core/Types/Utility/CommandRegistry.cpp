//==============================================================================
///	
///	File: CommandRegistry.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::map<std::string, CommandRegistry::Commandfn> CommandRegistry::_commands;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

CommandRegistry::CommandRegistry (void)
{

}

CommandRegistry::CommandRegistry (const CommandRegistry& rhs)
{
    _commands = rhs._commands;
}

CommandRegistry & CommandRegistry::operator = (const CommandRegistry& rhs)
{
    _commands = rhs._commands;
    return (*this);
}

CommandRegistry::~CommandRegistry (void)
{

}

//==============================================================================
//==============================================================================

void CommandRegistry::register_command (const std::string &command, Commandfn fn)
{
    _commands[MoreStrings::lowercase(command)] = fn;
}

CommandRegistry::Commandfn CommandRegistry::command (const std::string &command)
{
    auto i = _commands.find(MoreStrings::lowercase(command));
    if (i != _commands.end()) {
        return i->second;
    } else {
        return NULL;
    }
}

//==============================================================================
//==============================================================================

std::list<std::string> CommandRegistry::command_list (void)
{
    std::list<std::string> cmds;
    
    for (auto &i : _commands)
        cmds.push_back(i.first);
    
    return cmds;
}

//==============================================================================
//==============================================================================

} // DT3

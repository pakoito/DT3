//==============================================================================
///	
///	File: Console_cmd.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Command.hpp"

#ifdef DT3_COMMANDS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Console.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Console_cmd: public Command {
    public:
        DEFINE_TYPE(Console_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("ListCommands", &Console_cmd::do_list_commands);
        }

        static CommandResult do_list_commands (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(Console_cmd)

//==============================================================================
//==============================================================================

CommandResult Console_cmd::do_list_commands (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: ListCommands", CommandResult::UPDATE_NONE);
    }

    std::string result;
    
    std::list<std::string> cmds = CommandRegistry::command_list();
    
    for (auto &i : cmds) {
        result += i + " ";
    }
    
    return CommandResult(true, result, CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS


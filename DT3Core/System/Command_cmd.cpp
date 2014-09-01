//==============================================================================
///	
///	File: Command_cmd.cpp
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
#include "DT3Core/Types/Utility/Time.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Command_cmd: public Command {
    public:
        DEFINE_TYPE(Command_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("Echo", &Command_cmd::do_echo);
            CommandRegistry::register_command("Date", &Command_cmd::do_date);
        }

        static CommandResult do_echo (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_date (CommandContext &ctx, const CommandParams &p);

};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(Command_cmd)

//==============================================================================
//==============================================================================

CommandResult Command_cmd::do_echo (CommandContext &ctx, const CommandParams &p)
{    
    return CommandResult(true, p[1], CommandResult::UPDATE_NONE);
}

CommandResult Command_cmd::do_date (CommandContext &ctx, const CommandParams &p)
{
    return CommandResult(true, Time::time_string(), CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


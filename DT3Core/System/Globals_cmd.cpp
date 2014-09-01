//==============================================================================
///	
///	File: Globals_cmd.cpp
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
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Globals_cmd: public Command {
    public:
        DEFINE_TYPE(Globals_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("SetGlobal", &Globals_cmd::do_set_global);
            CommandRegistry::register_command("GetGlobal", &Globals_cmd::do_get_global);
        }

        static CommandResult do_set_global (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_get_global (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(Globals_cmd)

//==============================================================================
//==============================================================================

CommandResult Globals_cmd::do_set_global (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3 && p.count() != 4) {
        return CommandResult(false, "Usage: SetGlobal name value [READ_ONLY,PERSISTENT,VOLATILE,PERSISTENT_OBFUSCATED]", CommandResult::UPDATE_NONE);
    }
    
    std::string name = p[1];
    std::string value = p[2];
    DTuint flags = Globals::VOLATILE;
    
    if (p[3].find("READ_ONLY") != std::string::npos)                     flags = Globals::READ_ONLY;
    else if (p[3].find("VOLATILE") != std::string::npos)                 flags = Globals::VOLATILE;
    else if (p[3].find("PERSISTENT") != std::string::npos)               flags = Globals::PERSISTENT;
    else if (p[3].find("PERSISTENT_OBFUSCATED") != std::string::npos)    flags = Globals::PERSISTENT_OBFUSCATED;
    
    Globals::set_global(name,value,flags);
    
    return CommandResult(true, "Set global " + p[1], CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD);
}

//==============================================================================
//==============================================================================

CommandResult Globals_cmd::do_get_global (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() == 2) {
        return CommandResult(false, "Usage: GetGlobal name", CommandResult::UPDATE_NONE);
    }
    
    std::string name = p[1];
    std::string result = Globals::global(name);
    
    return CommandResult(true, result, CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS


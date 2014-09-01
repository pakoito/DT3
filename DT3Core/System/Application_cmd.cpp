//==============================================================================
///	
///	File: Application_cmd.cpp
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
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Application.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Application_cmd: public Command {
    public:
        DEFINE_TYPE(Application_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("Quit", &Application_cmd::do_quit);
        }

        static CommandResult do_quit (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(Application_cmd)

//==============================================================================
//==============================================================================

CommandResult Application_cmd::do_quit (CommandContext &ctx, const CommandParams &p)
{    
    System::application()->quit();

    return CommandResult(true, p[1], CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


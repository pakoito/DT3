//==============================================================================
///	
///	File: FileManager_cmd.cpp
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
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class FileManager_cmd: public Command {
    public:
        DEFINE_TYPE(FileManager_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("RescanFiles", &FileManager_cmd::do_rescan_files);
            CommandRegistry::register_command("RegisterPackage", &FileManager_cmd::do_register_package);
        }

        static CommandResult do_rescan_files (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_register_package (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(FileManager_cmd)

//==============================================================================
//==============================================================================

CommandResult FileManager_cmd::do_rescan_files (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: RescanFiles", CommandResult::UPDATE_NONE);
    }

    FileManager::scan_files();
    
    return CommandResult(true, "Files rescanned", CommandResult::UPDATE_ALL);
}

CommandResult FileManager_cmd::do_register_package (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: RegisterPackage pathtopackage", CommandResult::UPDATE_NONE);
    }

    FileManager::register_package(FilePath(p[1]));
    
    return CommandResult(true, "Files rescanned", CommandResult::UPDATE_ALL);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS


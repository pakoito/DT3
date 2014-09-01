//==============================================================================
///	
///	File: ChecksumTools_cmd.cpp
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
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class ChecksumTools_cmd: public Command {
    public:
        DEFINE_TYPE(ChecksumTools_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("Checksum", &ChecksumTools_cmd::do_checksum);
        }

        static CommandResult do_checksum (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(ChecksumTools_cmd)

//==============================================================================
//==============================================================================

CommandResult ChecksumTools_cmd::do_checksum (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: Checksum file", CommandResult::UPDATE_NONE);
    }
    
    FilePath path(p[1]);
    
    if (!path.exists())
        return CommandResult(false, "File not found", CommandResult::UPDATE_NONE);
        
    std::string checksum = path.checksum();
        
    CommandResult cr;
    cr.set_message( checksum );
    cr.set_result(true);
    return cr;
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS


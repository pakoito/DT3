//==============================================================================
///	
///	File: MemoryAllocatorTrace_cmd.cpp
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
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Memory/MemoryAllocatorTrace.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class MemoryAllocatorTrace_cmd: public Command {
    public:
        DEFINE_TYPE(MemoryAllocatorTrace_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("CheckMemory", &MemoryAllocatorTrace_cmd::do_check_memory);
            CommandRegistry::register_command("EnableCheckMemory", &MemoryAllocatorTrace_cmd::do_enable_check_memory);
            CommandRegistry::register_command("DisableCheckMemory", &MemoryAllocatorTrace_cmd::do_disable_check_memory);
        }

        static CommandResult do_check_memory (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_enable_check_memory (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_disable_check_memory (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(MemoryAllocatorTrace_cmd)

//==============================================================================
//==============================================================================

CommandResult MemoryAllocatorTrace_cmd::do_check_memory (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: CheckMemory", CommandResult::UPDATE_NONE);
    }

    MemoryAllocatorTrace::check_allocations();

    return CommandResult(false, "CheckMemory Done", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

CommandResult MemoryAllocatorTrace_cmd::do_enable_check_memory (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: EnableCheckMemory", CommandResult::UPDATE_NONE);
    }

    MemoryAllocatorTrace::enable_check_allocations(true);

    return CommandResult(false, "Check Memory Enabled", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

CommandResult MemoryAllocatorTrace_cmd::do_disable_check_memory (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: DisableCheckMemory", CommandResult::UPDATE_NONE);
    }

    MemoryAllocatorTrace::enable_check_allocations(false);

    return CommandResult(false, "Check Memory Disabled", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS


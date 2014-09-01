//==============================================================================
///	
///	File: Console.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Console.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Command.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::mutex          Console::_console_mutex;

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(3,Console::initialize())
GLOBAL_STATIC_DESTRUCTION(3,Console::destroy())

//==============================================================================
//==============================================================================

#ifdef DT3_DEBUG
    #define RESET       ""
    #define BLACK       ""                      // Black
    #define RED         ""                      // Red
    #define GREEN       ""                      // Green
    #define YELLOW      ""                      // Yellow
    #define BLUE        ""                      // Blue
    #define MAGENTA     ""                      // Magenta
    #define CYAN        ""                      // Cyan
    #define WHITE       ""                      // White
    #define BOLDBLACK   ""                      // Bold Black
    #define BOLDRED     ""                      // Bold Red
    #define BOLDGREEN   ""                      // Bold Green
    #define BOLDYELLOW  ""                      // Bold Yellow
    #define BOLDBLUE    ""                      // Bold Blue
    #define BOLDMAGENTA ""                      // Bold Magenta
    #define BOLDCYAN    ""                      // Bold Cyan
    #define BOLDWHITE   ""                      // Bold White
#else
    #define RESET       "\033[0m"
    #define BLACK       "\033[30m"              // Black
    #define RED         "\033[31m"              // Red
    #define GREEN       "\033[32m"              // Green
    #define YELLOW      "\033[33m"              // Yellow
    #define BLUE        "\033[34m"              // Blue
    #define MAGENTA     "\033[35m"              // Magenta
    #define CYAN        "\033[36m"              // Cyan
    #define WHITE       "\033[37m"              // White 
    #define BOLDBLACK   "\033[1m\033[30m"       // Bold Black
    #define BOLDRED     "\033[1m\033[31m"       // Bold Red
    #define BOLDGREEN   "\033[1m\033[32m"       // Bold Green
    #define BOLDYELLOW  "\033[1m\033[33m"       // Bold Yellow
    #define BOLDBLUE    "\033[1m\033[34m"       // Bold Blue
    #define BOLDMAGENTA "\033[1m\033[35m"       // Bold Magenta
    #define BOLDCYAN    "\033[1m\033[36m"       // Bold Cyan
    #define BOLDWHITE   "\033[1m\033[37m"       // Bold White
#endif

//==============================================================================
//==============================================================================

void Console::initialize (void)
{
    // Register all commands with the registry
    std::map<std::string, std::shared_ptr<CreatorBase>>& cm = command_map();
    
    for (auto &i : cm) {
        std::shared_ptr<Command> cmd = checked_static_cast<Command>(i.second->create_object());
        cmd->register_commands();
    }
}

void Console::destroy (void)
{

}

//==============================================================================
//==============================================================================

void Console::log_error (const std::string &error)
{
    std::unique_lock<std::mutex> lock(_console_mutex);

#if defined(DT3_DEBUG) || defined(DT3_EDITOR)
    HAL::log( "ERR: " + error + RESET );
#endif	
    
    SystemCallbacks::error_cb().fire(error);
}

//==============================================================================
//==============================================================================

void Console::log_message (const std::string &message)
{
    std::unique_lock<std::mutex> lock(_console_mutex);

#if defined(DT3_DEBUG) || defined(DT3_EDITOR)
    HAL::log( "MSG: " + message );
#endif

    SystemCallbacks::message_cb().fire(message);
}

//==============================================================================
//==============================================================================

void Console::log_debug (const std::string &debug)
{
    std::unique_lock<std::mutex> lock(_console_mutex);

#if defined(DT3_DEBUG) || defined(DT3_EDITOR)
    HAL::log( "DBG: " CYAN + debug + RESET );
#endif

    SystemCallbacks::debug_cb().fire(debug);
}

//==============================================================================
//==============================================================================

CommandResult Console::do_command (CommandContext &ctx, const CommandParams &params)
{
    
    CommandResult (*fn)(CommandContext &, const CommandParams &);
    fn = CommandRegistry::command(params[0]);
    
    if (fn) {
        CommandResult result = fn(ctx, params);
        display_result(result);
        return result;
    } else {
        CommandResult result(false, "Unknown command: " + params[0], CommandResult::UPDATE_NONE);
        display_result(result);
        return result;
    }
}

//==============================================================================
//==============================================================================

void Console::display_result (const CommandResult &result)
{
    log_message(result.message());
}

//==============================================================================
//==============================================================================

} // DT3

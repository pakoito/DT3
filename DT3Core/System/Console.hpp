#ifndef DT3_CONSOLE
#define DT3_CONSOLE
//==============================================================================
///	
///	File: Console.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include <fstream>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class CommandContext;
class CommandParams;

//==============================================================================
//==============================================================================

class Console {
		/// Console implementation.

    private:
                                                Console             (void);	
                                                Console             (const Console &rhs);
        Console &                               operator =          (const Console &rhs);	
        virtual                                 ~Console            (void);
    
	public:
		/// Initialize the globals system
        static void                             initialize          (void);

		/// Destroy the globals system
        static void                             destroy             (void);

		/// Methods used to print messages
		/// \param log message to print
		static void                             log_error			(const std::string &error);
		static void                             log_message			(const std::string &message);
		static void                             log_debug			(const std::string &debug);

		/// Interface to command system. Commands are send through here.
		/// \param ctx Command context
		/// \param params Command parameters
		/// \return Command result
        static CommandResult                    do_command          (CommandContext &ctx, const CommandParams &params);
    
	private:		
        static void                             display_result      (const CommandResult &result);
    
        static std::mutex                       _console_mutex;
    
};

//==============================================================================
//==============================================================================

} // DT3

#endif

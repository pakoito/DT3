#ifndef DT3_COMMANDREGISTRY
#define DT3_COMMANDREGISTRY
//==============================================================================
///	
///	File: CommandRegistry.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <string>
#include <list>
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CommandResult;
class CommandContext;
class CommandParams;

//==============================================================================
/// Class
//==============================================================================

class CommandRegistry {
    private:
                                CommandRegistry			(void);
                                CommandRegistry			(const CommandRegistry &rhs);
        CommandRegistry &       operator =				(const CommandRegistry &rhs);
        virtual                 ~CommandRegistry		(void);
                
	public:
        typedef CommandResult (*Commandfn)(CommandContext &ctx, const CommandParams &p);
    
		/// Description
		/// \param param description
		/// \return description
        static void                         register_command        (const std::string &command, Commandfn fn);
        
		/// Description
		/// \param param description
		/// \return description
        static Commandfn                    command                 (const std::string &command);

		/// Description
		/// \param param description
		/// \return description
        static std::list<std::string>       command_list            (void);

    private:
        static std::map<std::string, Commandfn>    _commands;
};


//==============================================================================
//==============================================================================

} // DT3

#endif

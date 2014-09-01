#ifndef DT3_COMMANDRESULT
#define DT3_COMMANDRESULT
//==============================================================================
///	
///	File: CommandResult.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/CommandParams.hpp"
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class CommandResult {
    public:
    
        enum UpdateHint {
            UPDATE_SCRIPT = 1 << 0,
            UPDATE_WORLD = 1 << 1,
            UPDATE_HIERARCHY = 1 << 2,
            UPDATE_PROPERTIES = 1 << 3,
            UPDATE_LIBRARY = 1 << 4,
            UPDATE_RESOURCES = 1 << 5,
            UPDATE_ANIMATION = 1 << 6,
            UPDATE_CONSOLE = 1 << 7,
            UPDATE_PERFORMANCE = 1 << 8,
            UPDATE_SOUND = 1 << 9,
            UPDATE_ALL = 0xFFFFFFFF,
            UPDATE_NONE = 0
        };

                                        CommandResult			(void);	
                                        CommandResult           (DTboolean result, const std::string &m, DTuint h)   {   set_result(result); set_message(m); set_update_hint(h);    }
                                        CommandResult			(const CommandResult &rhs);
        CommandResult &                 operator =				(const CommandResult &rhs);	
        virtual                         ~CommandResult          (void);
                
	public:
    
        DEFINE_ACCESSORS(message, set_message, std::string, _message)
        DEFINE_ACCESSORS(update_hint, set_update_hint, DTuint, _update_hint)
        DEFINE_ACCESSORS(result, set_result, DTboolean, _result)
    
		/// Appends a command to the undo queue
		/// \param u command parameters
        void                            append_undo_command     (const CommandParams &u)    {   _undo_commands.push_back(u);  }
        
		/// Appends a command to the undo queue
		/// \param u command result (which contains the commands to undo)
        void                            append_undo_command     (const CommandResult &r);

		/// Returns the list of undo commands
		/// \return list of undo commands
        const std::list<CommandParams>& undo_commands           (void) const        {   return _undo_commands;  }
        
    private:
        std::string                 _message;
        DTboolean                   _result;
        DTuint                      _update_hint;
        std::list<CommandParams>    _undo_commands;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

#ifndef DT3_COMMANDPARAMS
#define DT3_COMMANDPARAMS
//==============================================================================
///	
///	File: CommandParams.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <vector>
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class CommandParams {
    public:
                                    CommandParams		(void);
                                    CommandParams		(const std::string &c);
                                    CommandParams		(const CommandParams &rhs);
        CommandParams &             operator =			(const CommandParams &rhs);
        virtual                     ~CommandParams      (void);
           
    public:
    
		/// Original command string
		/// \return returns the original command string
        const std::string &         original_string     (void) const            {   return _orig_string;    }

		/// Add a parsed command
		/// \param p command parameter
        void                        add                 (const std::string &p)   {   _params.push_back(p);  }

		/// Access a command parameter
		/// \param i index of command
		/// \return command parameter
        std::string                 operator[]          (DTuint i);
		const std::string           operator[]          (DTuint i) const;
    
		/// Returns the number of command parameters
		/// \return number of command parameters
        DTsize                      count               (void) const            {   return _params.size();  }
        
	private:
        std::string                 _orig_string;
        std::vector<std::string>    _params;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

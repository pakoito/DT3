#ifndef DT3_GLOBALS
#define DT3_GLOBALS
//==============================================================================
///	
///	File: Globals.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include <string>
#include <map>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Globals {         
	private:
									Globals         (void);	
									Globals         (const Globals &rhs);
        Globals &                   operator =      (const Globals &rhs);	
        virtual						~Globals        (void);
    
	public:

		/// Initialize the globals system
        static void                                         initialize              (void);

		/// Destroy the globals system
        static void                                         destroy                 (void);

        enum {
            READ_ONLY = 0,
            VOLATILE = 1,
            PERSISTENT = 2,
            PERSISTENT_OBFUSCATED = 3
        };
    
        struct GlobalsEntry {
            DTint           lifetime;
            StringCopier    name;
            StringCopier	value;
        };

		
		/// Checks to see if a global exists
		/// \param name name of global
		/// \return true or false if exists or not
		static DTboolean                                    has_global				(const std::string &name_with_case);
                                                 
		/// Gets value of global
		/// \param name name of global
		/// \return value of global
        static std::string                                  global                  (const std::string &name_with_case);

		/// Gets value of global
		/// \param name name of global
		/// \return value of global
        static const std::map<StringCopier, GlobalsEntry>&  all_globals             (void)  {   return _globals;    }

		/// Gets value of global
		/// \param name name of global
		/// \return value of global
        static DTboolean                                    global                  (const std::string &name_with_case, std::string &value);

		/// Sets the value of a global
		/// \param name name of global
		/// \param value value of global
		/// \param lifetime READ_ONLY, VOLATILE, or PERSISTENT
        static void                                         set_global              (const std::string &name_with_case, const std::string &value, const DTint lifetime);

		/// Sets the value of a global if not already set
		/// \param name name of global
		/// \param value value of global
		/// \param lifetime READ_ONLY, VOLATILE, or PERSISTENT
        static void                                         set_global_default		(const std::string &name_with_case, const std::string &value, const DTint lifetime);

        /// Substitutes values in strings with global variables. i.e. "My name is {NAME}" replaces
        /// {NAME} with the value of the global variable NAME.  Will substitute recursively.
		/// \param string to substitute
		/// \return result string
        static std::string                                  substitute_global		(const std::string &s);

        /// Substitutes values in strings with global variables. i.e. "My name is {NAME}" replaces
        /// {NAME} with the value of the global variable NAME.  Will substitute recursively.
		/// \param s string to substitute
		/// \param value result string
		/// \return success
        static DTboolean                                    substitute_global		(const std::string &s, std::string &value);

		/// Loads all of the globals
        static void                                         load                    (void);

		/// Saves all of the globals
        static void                                         save                    (void);
    
    private:
        static std::mutex                                   _globals_lock;
        static std::map<StringCopier, GlobalsEntry>         _globals;
};

//==============================================================================
//==============================================================================


} // DT3

#endif

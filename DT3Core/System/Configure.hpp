#ifndef DT3_CONFIGURE
#define DT3_CONFIGURE
//==============================================================================
///	
///	File: Configure.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Callback.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Tokenizer;
class FilePath;

//==============================================================================
//==============================================================================

class Configure {
	private:
									Configure					(void);
									Configure					(const Configure &rhs);
        Configure &                 operator =					(const Configure &rhs);	
        virtual						~Configure                  (void);

	public:
    
		/// Initialize the configuration system
        static void                 initialize                  (void);

		/// Destroy the configuration system
        static void                 destroy                     (void);

		/// Import a configuration file
		/// pathname path to configuration file
		/// cb optional callback to monitor values which are beign set
		static void                 import_config				(const FilePath &pathname);

		/// Setup system globals such as platform globals, language, region, etc.
		/// \return success
        static DTboolean            setup_system_globals        (void);

  	private:
        static void                 parse_package_block         (Tokenizer &tokenizer);
		static void                 parse_default_block			(Tokenizer &tokenizer);
		static void					parse_global_block			(Tokenizer &tokenizer);
    
		static void					parse_engine_block			(Tokenizer &tokenizer);
		static void					parse_application_block     (Tokenizer &tokenizer);
    

};

//==============================================================================
//==============================================================================

} // DT3

#endif

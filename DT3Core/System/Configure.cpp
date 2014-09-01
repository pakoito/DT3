//==============================================================================
///	
///	File: Configure.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Configure.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/Tokenizer.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(1,Configure::initialize())
GLOBAL_STATIC_DESTRUCTION(1,Configure::destroy())

//==============================================================================
//==============================================================================

void Configure::initialize (void)
{
    setup_system_globals();
}

void Configure::destroy (void)
{

}

//==============================================================================
//==============================================================================

void Configure::parse_package_block (Tokenizer &tokenizer)
{
	tokenizer.assume_next_token("{");
	
	std::string name;

	while (true) {
		std::string token = tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
	
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"path"))     {	tokenizer.assume_next_token("="); name = tokenizer.next_token_string_no_substitute();   continue;	}
		
		tokenizer.syntax_error("Unknown Token " + token);
	};

	FileManager::register_package(FilePath(name));
}

//==============================================================================
//==============================================================================

void Configure::parse_default_block (Tokenizer &tokenizer)
{
	tokenizer.assume_next_token("{");
	
	std::string name;
	std::string value;
	DTuint lifetime = Globals::VOLATILE;

	while (true) {
		std::string token = tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
	
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Name"))		{	tokenizer.assume_next_token("="); name = tokenizer.next_token_string();                         continue;	}
		if (MoreStrings::iequals(token,"Value"))	{	tokenizer.assume_next_token("="); value = tokenizer.next_token_string_no_substitute();          continue;	}
		if (MoreStrings::iequals(token,"Lifetime"))	{	tokenizer.assume_next_token("="); lifetime = static_cast<DTuint>(tokenizer.next_token_number());continue;	}
		
		tokenizer.syntax_error("Unknown Token " + token);
	};
	
	Globals::set_global_default(name,value,lifetime);
}

//==============================================================================
//==============================================================================

void Configure::parse_global_block (Tokenizer &tokenizer)
{
	tokenizer.assume_next_token("{");
		
	std::string name;
	std::string value;
    DTuint lifetime = Globals::VOLATILE;

	while (true) {
		std::string token = tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
	
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Name"))		{	tokenizer.assume_next_token("="); name = tokenizer.next_token_string();                 continue;	}
		if (MoreStrings::iequals(token,"Value"))	{	tokenizer.assume_next_token("="); value = tokenizer.next_token_string_no_substitute();  continue;	}
		if (MoreStrings::iequals(token,"Lifetime"))	{	tokenizer.assume_next_token("="); lifetime = (DTuint) tokenizer.next_token_number();	continue;	}
		
		tokenizer.syntax_error("Unknown Token " + token);
	};
	    
	Globals::set_global(name,value,lifetime);
}

//==============================================================================
//==============================================================================

void Configure::parse_application_block (Tokenizer &tokenizer)
{
	tokenizer.assume_next_token("{");

	while (true) {
		std::string token = tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
	
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"default"))	{	parse_default_block(tokenizer);		continue;	}
		if (MoreStrings::iequals(token,"global"))	{	parse_global_block(tokenizer);		continue;	}
		if (MoreStrings::iequals(token,"package"))  {	parse_package_block(tokenizer);		continue;	}

		tokenizer.syntax_error("Unknown Token " + token);
	};

}

void Configure::parse_engine_block (Tokenizer &tokenizer)
{
	tokenizer.assume_next_token("{");

	while (true) {
		std::string token = tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
	
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"default"))	{	parse_default_block(tokenizer);		continue;	}
		if (MoreStrings::iequals(token,"global"))	{	parse_global_block(tokenizer);		continue;	}
		
		tokenizer.syntax_error("Unknown Token " + token);
	};

}

//==============================================================================
//==============================================================================

void Configure::import_config(const FilePath &pathname)
{
	LOG_MESSAGE << "Importing config file " << pathname.full_path();

	Tokenizer tokenizer;
    
    // Load in defines for material
    const DTcharacter* header =
        "#define READ_ONLY 0\n"
        "#define VOLATILE 1\n"
        "#define PERSISTENT 2\n"
        "#define PERSISTENT_OBFUSCATED 3\n";
    
    tokenizer.set_token_stream(header, true);
    
    
	tokenizer.load_token_stream (pathname);
	
	while (!tokenizer.is_done()) {
		std::string token = tokenizer.next_token_string();

		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
		
		if (MoreStrings::iequals(token,"application"))    {	parse_application_block(tokenizer);	continue;	}
		if (MoreStrings::iequals(token,"engine"))         {	parse_engine_block(tokenizer);      continue;	}
	};

	LOG_MESSAGE << "Finished importing config file";
}

//==============================================================================
//==============================================================================

DTboolean Configure::setup_system_globals    (void)
{
    LOG_MESSAGE << "setup_system_globals entered";

    //
    // Standard defaults
    //

	Globals::set_global ("SYS_MUSIC", "1", Globals::PERSISTENT);

	Globals::set_global ("SYS_SOUND", "1", Globals::PERSISTENT);
	Globals::set_global ("SYS_MUSIC_GAIN", "1", Globals::PERSISTENT);
	Globals::set_global ("SYS_SOUND_GAIN", "1", Globals::PERSISTENT);

	Globals::set_global_default ("SYS_SCREEN_WIDTH", "800", Globals::PERSISTENT);
	Globals::set_global_default ("SYS_SCREEN_HEIGHT", "600", Globals::PERSISTENT);

	//
	// These globals describe the execution environment
	//

#if DT3_OS == DT3_WINDOWS
	Globals::set_global ("SYS_PLATFORM_WINDOWS", "1", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_MACOSX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_LINUX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_IOS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_ANDROID", "0", Globals::READ_ONLY); 

	Globals::set_global ("SYS_PLATFORM", "Windows", Globals::READ_ONLY); 
	Globals::set_global ("SYS_DEVICE", "Desktop", Globals::READ_ONLY); 
#elif DT3_OS == DT3_MACOSX
	Globals::set_global ("SYS_PLATFORM_WINDOWS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_MACOSX", "1", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_LINUX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_IOS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_ANDROID", "0", Globals::READ_ONLY); 

	Globals::set_global ("SYS_PLATFORM", "MacOSX", Globals::READ_ONLY); 
	Globals::set_global ("SYS_DEVICE", "Desktop", Globals::READ_ONLY); 
#elif DT3_OS == DT3_LINUX
	Globals::set_global ("SYS_PLATFORM_WINDOWS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_MACOSX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_LINUX", "1", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_IOS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_ANDROID", "0", Globals::READ_ONLY); 

	Globals::set_global ("SYS_PLATFORM", "Linux", Globals::READ_ONLY); 
	Globals::set_global ("SYS_DEVICE", "Desktop", Globals::READ_ONLY); 
#elif DT3_OS == DT3_IOS
	Globals::set_global ("SYS_PLATFORM_WINDOWS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_MACOSX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_LINUX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_IOS", "1", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_ANDROID", "0", Globals::READ_ONLY); 

	Globals::set_global ("SYS_PLATFORM", "iPhone", Globals::READ_ONLY); 
	Globals::set_global ("SYS_DEVICE", "Mobile", Globals::READ_ONLY); 
#elif DT3_OS == DT3_ANDROID
	Globals::set_global ("SYS_PLATFORM_WINDOWS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_MACOSX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_LINUX", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_IOS", "0", Globals::READ_ONLY); 
	Globals::set_global ("SYS_PLATFORM_ANDROID", "1", Globals::READ_ONLY); 

	Globals::set_global ("SYS_PLATFORM", "Android", Globals::READ_ONLY); 
	Globals::set_global ("SYS_DEVICE", "Mobile", Globals::READ_ONLY); 
#endif

    //
    // Determine Locale
    //


    std::string region = HAL::region();

    std::string language = HAL::language();

	Globals::set_global ("SYS_CURRENT_REGION", region, Globals::READ_ONLY); 
	Globals::set_global ("SYS_CURRENT_LANGUAGE", language, Globals::READ_ONLY);
    
    LOG_MESSAGE << "Region: " << region;
    LOG_MESSAGE << "Language: " << language;

	//
	// Determine Application and Save Dirs
	//

	Globals::set_global ("APPDIR", HAL::app_dir().full_path(), Globals::READ_ONLY);
	Globals::set_global ("SAVEDIR", HAL::save_dir().full_path(), Globals::READ_ONLY);
    
    //
    // Number of cores
    //
    
    DTuint num_cores = HAL::num_CPU_cores();
	Globals::set_global ("SYS_NUM_CORES", MoreStrings::cast_to_string(num_cores), Globals::READ_ONLY);
	Globals::set_global ("SYS_MULTICORE", num_cores > 1 ? "1" : "0", Globals::READ_ONLY);

    //
    // Screen size
    //
    
    Globals::set_global_default("SYS_SMALL_SCREEN", "0", Globals::VOLATILE);

    LOG_MESSAGE << "setup_system_globals done";

    return true;
}

//==============================================================================
//==============================================================================


} // DT3


#ifndef DT3_HAL
#define DT3_HAL
//==============================================================================
///	
///	File: HAL.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/System/Globals.hpp"
#include <map>
#include <vector>
#include <string>

#include <jni.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================

class DisplayMode;
class URL;
class FilePath;

//==============================================================================
//==============================================================================

class HAL {
    public:
        DEFINE_TYPE_SIMPLE_BASE(HAL)

	private:
									HAL                     (void);
									HAL                     (const HAL&);
        HAL&                        operator =				(const HAL&);
        virtual						~HAL                    (void);
        
	public:
    
		/// Initialize the HAL
        static void                 initialize              (void);

		/// Destroy the HAL
        static void                 destroy                 (void);

		/// Initialize the HAL jvm
        static void                 initialize              (JavaVM* vm, jobject activity);

        //
        // System Log
        //

		/// Outputs a message to the system log
        static void                 log                     (const std::string &message);

        //
        // Timers
        //

		/// Returns the program running time in seconds
        static DTdouble             program_running_time    (void);

        //
        // Command Line
        //

		/// Run a command on the command line
		/// \param command command to run
		/// \return args Array or arguments to command line
		static void					run_on_command_line     (const std::string &command, const std::vector<std::string> &args);
    
    
        //
        // Displays
        //

		/// Returns a list of supported display modes
		/// \param modes Display modes
        static void                 display_modes           (std::map<DTint, std::vector<DisplayMode>> &modes);

		/// Switches to a display mode
		/// \param display index of display
		/// \param mode resolution of display
        static void                 switch_display_mode     (DTint display, DisplayMode mode);

		/// Returns the rectangle of the display
		/// \param display index of display
		/// \param x x location
		/// \param y y location
		/// \param width width of display
		/// \param height height of display
        static void                 display_rect            (DTint display, DTint &x, DTint &y, DTint &width, DTint &height);

        //
        // Web Browser
        //

		static void                 launch_browser          (const URL &url);
		static void                 launch_editor           (const FilePath &path);
    
        //
        // CPU Query
        //
    
        static DTuint               num_CPU_cores           (void);

        //
        // File System
        //
    
        static DTboolean            move_file               (const FilePath &from, const FilePath &to);
        static DTboolean            delete_file             (const FilePath &file);
        static DTuint64             modification_date       (const FilePath &file);
        static DTboolean            is_dir                  (const FilePath &file);
        static std::string          path_separator          (void);
        static void                 list_directory          (const FilePath &pathname, DTboolean recursive, std::vector<FilePath> &paths);
    
        static FilePath             app_dir                 (void);
        static FilePath             save_dir                (void);

        //
        // Locale
        //

		/// Description
		/// \param param description
		/// \return description
		static std::string          region                  (void);

		/// Description
		/// \param param description
		/// \return description
		static std::string          language                (void);
    
        //
        // Globals
        //

		/// Description
		/// \param param description
		/// \return description
		static std::map<StringCopier, Globals::GlobalsEntry>  load_globals            (void);

		/// Description
		/// \param param description
		/// \return description
		static void                 save_globals            (std::map<StringCopier, Globals::GlobalsEntry> globals);
};

//==============================================================================
//==============================================================================

} // DT3

#endif

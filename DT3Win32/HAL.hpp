#ifndef DT2_HAL
#define DT2_HAL
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

#include "Config.hpp"
#include <thread>
#include <map>

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
// Forward Declarations
//==============================================================================

class String;
class DisplayMode;
class FilePath;
class URL;

template <class T> class Array;

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
    
        //
        // System Log
        //

		/// Outputs a message to the system log
        static void                 log                     (const String &message);

        //
        // Timers
        //

		/// Returns the program running time in seconds
        static DTdouble             getProgramRunningTime   (void);

        //
        // Command Line
        //

		/// Run a command on the command line
		/// \param command command to run
		/// \return args Array or arguments to command line
		static void					runOnCommandLine        (const String &command, const Array<String> &args);
    
    
        //
        // Displays
        //

		/// Returns a list of supported display modes
		/// \param modes Display modes
        static void                 getDisplayModes         (std::map<DTint, Array<DisplayMode> > &modes);

        //
        // Web Browser
        //

		static void                 launchBrowser           (const URL &url);
		static void                 launchEditor            (const FilePath &path)  {}

        //
        // Atomics
        //

        static void                 atomicIncrement32       (volatile DTint &val);
		static void                 atomicDecrement32       (volatile DTint &val);
		static void                 atomicIncrement64       (volatile DTint64 &val);
		static void                 atomicDecrement64       (volatile DTint64 &val);
		
		static DTboolean            compareAndSwap32        (volatile DTint &ptr, const DTint old_val, const DTint new_val);
		static DTboolean            compareAndSwap32        (volatile DTuint &ptr, const DTuint old_val, const DTuint new_val);
    
        //
        // Mutexes
        //
    
        typedef void*               MutexType;
    
        static MutexType            createMutex             (void);
        static void                 destroyMutex            (MutexType m);
        static void                 lockMutex               (MutexType m);
        static void                 unlockMutex             (MutexType m);
    
        //
        // Threads
        //
    
        typedef void*               ThreadType;
        typedef std::thread::id     ThreadIDType;
    
        static void                 createThread            (void (*fn)(void *), void *data, ThreadType &tp);
        static void                 deleteThread            (ThreadType t);
        static void                 joinThread              (ThreadType t);
        static void                 suspendThread           (ThreadType t);
        static void                 resumeThread            (ThreadType t);
        static void                 sleepThread             (DTuint milliseconds);
        static void                 yieldThread             (void);

        static ThreadIDType         getCurrentThreadID      (void);
        static DTboolean            isThreadIDsEqual        (const ThreadIDType &a, const ThreadIDType &b);
    
        static DTuint               numCPUCores             (void);

        //
        // File System
        //
    
        static DTboolean            moveFile                (const FilePath &from, const FilePath &to);
        static DTboolean            deleteFile              (const FilePath &file);
        static DTuint64             getModificationDate     (const FilePath &file);
        static DTboolean            isDir                   (const FilePath &file);
        static String               getPathSeparator        (void);
        static void                 listDirectory           (const FilePath &pathname, DTboolean recursive, Array<FilePath> &paths);
    
        static FilePath             getAppDir               (void);
        static FilePath             getSaveDir              (void);

        //
        // Locale
        //

		/// Description
		/// \param param description
		/// \return description
		static String                   getRegion				(void);

		/// Description
		/// \param param description
		/// \return description
		static String                   getLanguage             (void);

        //
        // Globals
        //

		/// Description
		/// \param param description
		/// \return description
		static void                     loadGlobals				(void);

		/// Description
		/// \param param description
		/// \return description
		static void                     saveGlobals             (void);
};

//==============================================================================
//==============================================================================

} // DT2

#endif

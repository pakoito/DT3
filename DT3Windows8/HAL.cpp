//==============================================================================
///	
///	File: HAL.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "HAL.hpp"
#include "Array.hpp"
#include "List.hpp"
#include "Error.hpp"
#include "DisplayMode.hpp"
#include "URL.hpp"
#include "FilePath.hpp"

#include <windows.h>
#include <Synchapi.h>
#include <sys/stat.h>
#include <shlobj.h>

#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <thread>
#include <condition_variable>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_CREATION(HAL)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

void s2s (Platform::String ^s1, String &s2)
{
    for (auto i:s1) {
        s2 += i;
    }
}

void s2s (String &s1, Platform::String ^&s2)
{
    for (DTuint i = 0; i < s1.size(); ++i) {
        wchar_t c = s1[i];
        s2 = Platform::String::Concat(s2, ref new Platform::String(&c,1));
    }
}

//==============================================================================
//==============================================================================

void HAL::log (const String &message)
{
	::OutputDebugStringA(message.cStr());
	::OutputDebugStringA("\n");
}

//==============================================================================
//==============================================================================

DTdouble HAL::getProgramRunningTime(void)
{
    unsigned __int64 pf,t;

    QueryPerformanceFrequency( (LARGE_INTEGER *)&pf );
    DTdouble freq = 1.0 / (DTdouble)pf;

    QueryPerformanceCounter( (LARGE_INTEGER *)&t );

    return t * freq;
}

//==============================================================================
//==============================================================================

void HAL::runOnCommandLine (const String &command, const Array<String> &args)
{
	//String params;
	//
	//for (DTuint i = 0; i < args.size(); ++i)
	//	params = params + args[i] + " ";

	//ShellExecuteA(NULL, "open", command.cStr(), params.cStr(), NULL, SW_SHOWNORMAL);
}

//==============================================================================
//==============================================================================

void HAL::getDisplayModes (Array<DisplayMode> &modes)
{
	modes.clear();
	//
	//// see http://www.gamedev.net/reference/articles/article1009.asp
	//
	//DTint  nModeExist;
	//DEVMODE devMode;
	//devMode.dmSize = sizeof(DEVMODE);
	//
	//for (int i=0; ;i++) {
	//	nModeExist = EnumDisplaySettings(NULL, i, &devMode);
	//	
	//	if (nModeExist != 1) {
	//		// end of modes.  bail out.
	//		break;
	//	} else {
	//		// add the driver to the list.			
	//		DTint width = devMode.dmPelsWidth;
	//		DTint height = devMode.dmPelsHeight;
	//		
	//		// Check if resolution already exists
	//		DTboolean found = false;
	//		
	//		for (DTuint k = 0; k < modes.size(); ++k)
	//			if (modes[k].getWidth() == width && modes[k].getHeight() == height)
	//				found = true;
	//		
	//		if (!found) {
	//			// add the mode to the list
	//			DisplayMode m;
	//					
	//			m.setWidth(width);
	//			m.setHeight(height);
	//			modes.pushBack(m);
	//		}

	//	}
	//}
 //   
 //   modes.sort();
}

//==============================================================================
//==============================================================================

void HAL::launchBrowser (const URL &url)
{

}

//==============================================================================
//==============================================================================

void HAL::atomicIncrement32 (volatile DTint &val)
{
	::InterlockedIncrement( (volatile LONG*) &val);
}
		
		
void HAL::atomicDecrement32 (volatile DTint &val)
{
	::InterlockedDecrement( (volatile LONG*) &val);
}
		
		
void HAL::atomicIncrement64 (volatile DTint64 &val)
{
	::InterlockedIncrement64(&val);
}
		
		
void HAL::atomicDecrement64 (volatile DTint64 &val)
{
	::InterlockedDecrement64(&val);
}
		
DTboolean HAL::compareAndSwap32(volatile DTint &ptr, const DTint old_val, const DTint new_val)
{		
	return InterlockedCompareExchange(reinterpret_cast<LONG volatile *>(&ptr), new_val, old_val) == old_val;
}

DTboolean HAL::compareAndSwap32(volatile DTuint &ptr, const DTuint old_val, const DTuint new_val)
{
	return InterlockedCompareExchange(reinterpret_cast<LONG volatile *>(&ptr), new_val, old_val) == old_val;
}

//==============================================================================
//==============================================================================

namespace {
    struct MutexInfo {
        HANDLE              mutex;
    };
}

HAL::MutexType HAL::createMutex (void)
{
    MutexInfo*    mp = new MutexInfo;

	mp->mutex = CreateMutexEx(0, NULL, FALSE, 0);

    return reinterpret_cast<MutexType>(mp);
}

void HAL::destroyMutex (MutexType m)
{
    MutexInfo*    mp = reinterpret_cast<MutexInfo*>(m);

	WaitForSingleObjectEx(mp->mutex, INFINITE, FALSE);
	CloseHandle(mp->mutex);

    delete mp;
}

void HAL::lockMutex (MutexType m)
{
    MutexInfo*    mp = reinterpret_cast<MutexInfo*>(m);

	WaitForSingleObjectEx(mp->mutex, INFINITE, FALSE);
}

void HAL::unlockMutex (MutexType m)
{
    MutexInfo*    mp = reinterpret_cast<MutexInfo*>(m);

	ReleaseMutex(mp->mutex);
}

//==============================================================================
//==============================================================================

namespace {
    struct ThreadInfo {
        void                        *data;
        void (*fn)(void *);
    
        std::thread                 thread;
        std::mutex                  lock;
        std::condition_variable     cond;
    };
    
    DWORD WINAPI threadHelper (LPVOID data)
    {
        ThreadInfo *tp = reinterpret_cast<ThreadInfo*>(data);
        tp->fn(tp->data);

        return 0;
    }

}

void HAL::createThread (void (*fn)(void *), void *data, ThreadType &thread)
{
    ThreadInfo *tp = new ThreadInfo;
    thread = reinterpret_cast<ThreadType>(tp);
    
    // Initialize structure
    tp->data = data;
    tp->fn = fn;

    std::thread new_thread(threadHelper, tp);
    std::swap(tp->thread, new_thread);
}

void HAL::deleteThread (ThreadType t)
{
    ThreadInfo* tp = reinterpret_cast<ThreadInfo*>(t);
    delete tp;
}

void HAL::joinThread (ThreadType t)
{
    ThreadInfo* tp = reinterpret_cast<ThreadInfo*>(t);
    tp->thread.join();
}

void HAL::suspendThread (ThreadType t)
{
    ThreadInfo* tp = reinterpret_cast<ThreadInfo*>(t);

    std::unique_lock<std::mutex> lock(tp->lock);
    tp->cond.wait(lock);
}

void HAL::resumeThread (ThreadType t)
{
    ThreadInfo* tp = reinterpret_cast<ThreadInfo*>(t);

    std::unique_lock<std::mutex> lock(tp->lock);
    tp->cond.notify_all();
}

void HAL::sleepThread (DTuint milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void HAL::yieldThread (void)
{
	std::this_thread::yield();
}

HAL::ThreadIDType HAL::getCurrentThreadID (void)
{
    return std::this_thread::get_id();
}

DTboolean HAL::isThreadIDsEqual (const ThreadIDType &a, const ThreadIDType &b)
{
    return a == b;
}

//==============================================================================
//==============================================================================

DTboolean HAL::moveFile (const FilePath &from, const FilePath &to)
{
	return MoveFileExA(from.getPath().cStr(), to.getPath().cStr(),0);

	return true;
}

DTboolean HAL::deleteFile (const FilePath &file)
{
	return DeleteFileA(file.getPath().cStr());

	return true;
}

DTuint64 HAL::getModificationDate (const FilePath &file)
{
	struct _stat buffer;	
	if (_stat(file.getPath().cStr(), &buffer))
		return 0;
	
	return (DTuint) buffer.st_mtime;
}

DTboolean HAL::isDir (const FilePath &file)
{
	struct _stat buffer;	
	if (_stat(file.getPath().cStr(), &buffer))
		return 0;
	
	return (DTboolean) (_S_IFDIR & buffer.st_mode);
}

String HAL::getPathSeparator (void)
{
	return "\\";
}


namespace {
    struct IterData {
        DTboolean			_recursive;
        Array<FilePath>     *_paths;
    };

    void iterProc (const FilePath &pathname, IterData *iter_data)
    {
        WIN32_FIND_DATAA	FindFileData;
        
        String dir_path = pathname.getPath() + "*";

        HANDLE find_file = FindFirstFileExA(dir_path.cStr(),
                                            FindExInfoStandard,
                                            &FindFileData,
                                            FindExSearchNameMatch,
                                            NULL,
                                            0
                                            );

        if (find_file == INVALID_HANDLE_VALUE)
            return;
            
        do {
        
            // Skip invisible files
            if (FindFileData.cFileName[0] != '.') {
                
                if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
                    iter_data->_paths->pushBack(FilePath(pathname.getPath() + FindFileData.cFileName));
                    
                    if (iter_data->_recursive) {
                        iterProc(FilePath(pathname.getPath() + FindFileData.cFileName + "\\"), iter_data);
                    }
                    
                } else {				
                    
                    iter_data->_paths->pushBack(FilePath(pathname.getPath() + FindFileData.cFileName));
                }
                
            }
        } while (FindNextFileA(find_file, &FindFileData));
    }

}

void HAL::listDirectory (const FilePath &pathname, DTboolean recursive, Array<FilePath> &paths)
{
	// set up and run platform specific iteration routines
	
	// Buid data to pass to iterator routines
	IterData iter_data;
	iter_data._paths = &paths;
	iter_data._recursive = recursive;

	iterProc(FilePath(pathname.getPath() + "\\"), &iter_data);
}


FilePath HAL::getAppDir (void)
{    
    Windows::Storage::StorageFolder ^install_folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

    String path;
    s2s(install_folder->Path, path);

    return FilePath(path);
}


FilePath HAL::getSaveDir (void)
{
    FilePath result;

    Windows::Storage::StorageFolder ^data_folder = Windows::Storage::ApplicationData::Current->LocalFolder;

    String path;
    s2s(data_folder->Path, path);

    return FilePath(path);
}

//==============================================================================
//==============================================================================

String HAL::getRegion (void)
{
    Platform::String ^region = Windows::Globalization::GeographicRegion().CodeTwoLetter;

    String region_s;
    s2s(region, region_s);

    return region_s;
}

String HAL::getLanguage (void)
{
    Platform::String ^language = Windows::Globalization::Language::CurrentInputMethodLanguageTag;

    String language_s;
    s2s(language, language_s);

    return language_s;
}

//==============================================================================
//==============================================================================

} // DT2

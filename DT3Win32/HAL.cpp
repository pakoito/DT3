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
#include "Algorithm.hpp"
#include "Globals.hpp"

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

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
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
	String params;
	
	for (DTuint i = 0; i < args.size(); ++i)
		params = params + args[i] + " ";

	ShellExecuteA(NULL, "open", command.cStr(), params.cStr(), NULL, SW_SHOWNORMAL);
}

//==============================================================================
//==============================================================================

BOOL CALLBACK MonitorEnumProc(_In_ HMONITOR hMonitor, _In_ HDC hdcMonitor, _In_ LPRECT lprcMonitor, _In_ LPARAM dwData)
{
    std::map<DTint, Array<DisplayMode> > &modes = *reinterpret_cast<std::map<DTint, Array<DisplayMode> >*>(dwData);

	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);
	
    Array<DisplayMode> modes_for_display;

	for (int i=0; ;i++) {
		DTint nModeExist = EnumDisplaySettings(NULL, i, &devMode);
		
		if (nModeExist != 1) {
			// end of modes.  bail out.
			break;
		} else {
			// Add the driver to the list.			
			DTint width = devMode.dmPelsWidth;
			DTint height = devMode.dmPelsHeight;
			
            DisplayMode m;
            m.setWidth(width);
            m.setHeight(height);
            
            if (modes_for_display.find(m) == modes_for_display.end()) {
                modes_for_display.pushBack(m);
            }

		}
	}

    Algorithm::bubblesort(modes_for_display);

    modes[modes.size()] = modes_for_display; 

    return true;
}

void HAL::getDisplayModes (std::map<DTint, Array<DisplayMode> > &modes)
{
	modes.clear();

    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM) &modes);  
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
    if (t) {
        ThreadInfo* tp = reinterpret_cast<ThreadInfo*>(t);
        tp->thread.join();
    }
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

DTuint HAL::numCPUCores (void)
{
    //may return 0 when not able to detect
    DTuint concurent_threads_supported = std::thread::hardware_concurrency();

    return (DTuint) concurent_threads_supported > 0 ? concurent_threads_supported : 2;
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
    /*Windows::Storage::StorageFolder ^install_folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

    String path;
    s2s(install_folder->Path, path);*/

	// get Current directory
	DTcharacter cstr[MAX_PATH];
	
	if (GetModuleFileNameA(NULL,cstr,MAX_PATH) == 0)
		return FilePath();
		
	String path(cstr);
	path = path.substr(0, path.findLastOf('\\'));

	// TODO: Fix me.
	if (    path.find("\\Debug") != String::NPOS || 
            path.find("\\Release") != String::NPOS) {
		path = path.substr(0, path.findLastOf('\\'));
	}

    return FilePath(path);
}


FilePath HAL::getSaveDir (void)
{
    /*Windows::Storage::StorageFolder ^data_folder = Windows::Storage::ApplicationData::Current->LocalFolder;

    String path;
    s2s(data_folder->Path, path);*/

    return FilePath();
}

//==============================================================================
//==============================================================================

String HAL::getRegion (void)
{
    /*Platform::String ^region = Windows::Globalization::GeographicRegion().CodeTwoLetter;

    String region_s;
    s2s(region, region_s);*/

    return "US";
}

String HAL::getLanguage (void)
{
    /*Platform::String ^language = Windows::Globalization::Language::CurrentInputMethodLanguageTag;

    String language_s;
    s2s(language, language_s);*/

    return "en";
}

//==============================================================================
//==============================================================================


void HAL::loadGlobals (void)
{
    HKEY hAppKey;

	if( RegOpenKeyEx( HKEY_CURRENT_USER, TEXT("SOFTWARE\\" PROJECT_NAME), 0, KEY_READ, &hAppKey) != ERROR_SUCCESS)
		return;
	
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD	retCode; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hAppKey,                 // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the key values. 
	for (DTuint i=0; i<cValues; ++i) { 
	
		DWORD   cchKey = MAX_VALUE_NAME;                   // size of name string 
		DWORD	cchValue = MAX_VALUE_NAME; 
	
		DTcharacter	achKeyA[MAX_VALUE_NAME];   // buffer for subkey name
		DTcharacter	achValueA[MAX_VALUE_NAME]; 
	
		retCode = RegEnumValueA(hAppKey, i, (LPSTR) achKeyA, &cchKey, NULL, NULL, (LPBYTE) achValueA, &cchValue);
		
		if (retCode == ERROR_SUCCESS) { 
			String name(achKeyA);
			String value(achValueA);
			DTuint mode = Globals::PERSISTENT;
				
			if (name[0] == '-') {
				name.erase(0,1);
				value = MoreMath::fromObfuscated(name,value);
				mode = Globals::PERSISTENT_OBFUSCATED;
			}
           
			log("Reading persistent value: " + name + " = " + value);
                
            Globals::setGlobal (name, value, mode);
		} 
	}
}

void HAL::saveGlobals (void)
{
    HKEY hk; 
	DWORD dwDisp; 

	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\" PROJECT_NAME), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hk, &dwDisp)) {
		return;
	}

    const std::map<String, Globals::GlobalsEntry>& globals = Globals::getAllGlobals ();
	        
	FOR_EACH (i, globals) {
        if (i->second.type == Globals::PERSISTENT || i->second.type == Globals::PERSISTENT_OBFUSCATED) {
			String name = i->second.name;
			String value = i->second.value;
			
			if (i->second.type == Globals::PERSISTENT_OBFUSCATED) {
				value = MoreMath::toObfuscated(name, value);
				name = "-" + name;

				log("Writing persistent value: " + name + " = " + i->second.value + " (Obfuscated form is " + value + ")");
			} else {
				log("Writing persistent value: " + name + " = " + value);
			}
			
            if (RegSetValueExA(	hk, name.cStr(), 0, REG_SZ, (const BYTE*) value.cStr(), (DWORD) value.size() + 1)) {
				RegCloseKey(hk); 
				return;
			}
        }
    }
		
	RegCloseKey(hk);
}

//==============================================================================
//==============================================================================

} // DT2

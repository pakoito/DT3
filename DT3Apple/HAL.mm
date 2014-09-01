//==============================================================================
///	
///	File: HAL.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Apple/HAL.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/DisplayMode.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Network/URL.hpp"
#include "DT3Core/System/Globals.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <sys/stat.h>
#include <dirent.h>

#if DT3_OS == DT3_IOS
    #include <AVFoundation/AVFoundation.h>
#endif

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,HAL::initialize())
GLOBAL_STATIC_DESTRUCTION(0,HAL::destroy())

//==============================================================================
//==============================================================================

#if DT3_OS == DT3_MACOSX
    // Restore state for displays
	const DTint         MAX_DISPLAYS = 32;
    CGDisplayModeRef    g_display_restore_state[MAX_DISPLAYS];
#endif

//==============================================================================
//==============================================================================

void HAL::initialize (void)
{
#if DT3_OS == DT3_IOS
	//
	// iPhone specific audio session initialization
	//

    // Allow background music
    NSError *setCategoryErr = nil;
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryAmbient error:&setCategoryErr];

    [AVAudioSession sharedInstance];
    [[AVAudioSession sharedInstance] setActive:YES error:NULL];



//	// Just set these to NULL if you don't want a callback function
//    ::AudioSessionInitialize (NULL,NULL,NULL,NULL);
//	    
//    // Set audio category
//    UInt32 sessionCategory = kAudioSessionCategory_SoloAmbientSound;
//    ::AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
//    ::AudioSessionSetActive(true);
#endif

#if DT3_OS == DT3_MACOSX
	CGDirectDisplayID display_array[MAX_DISPLAYS];
	CGDisplayCount num_displays;
	CGGetActiveDisplayList (MAX_DISPLAYS, display_array, &num_displays);
    
    // Restore any display states that have been changed
    for (CGDisplayCount i = 0; i < num_displays; ++i) {
        g_display_restore_state[i] = ::CGDisplayCopyDisplayMode(display_array[i]);
    }

#endif
}

void HAL::destroy (void)
{
#if DT3_OS == DT3_MACOSX

	CGDirectDisplayID display_array[MAX_DISPLAYS];
	CGDisplayCount numDisplays;
	CGGetActiveDisplayList (MAX_DISPLAYS, display_array, &numDisplays);
    
    // Restore any display states that have been changed
    for (CGDisplayCount i = 0; i < numDisplays; ++i) {

        ::CGDisplaySetDisplayMode (display_array[i], g_display_restore_state[i], NULL);
        ::CGDisplayModeRelease(g_display_restore_state[i]);
        
        if (CGDisplayIsCaptured(display_array[i]))
            ::CGDisplayRelease(display_array[i]);
    }
    
#endif
}

//==============================================================================
//==============================================================================

void HAL::log (const std::string &message)
{
	std::cout << message << std::endl;
}

//==============================================================================
//==============================================================================

DTdouble HAL::program_running_time(void)
{
    static CFTimeInterval	start_time 	= ::CFAbsoluteTimeGetCurrent();
    CFTimeInterval			new_time 	= ::CFAbsoluteTimeGetCurrent();
    
    return static_cast<DTdouble>(new_time - start_time);
}

//==============================================================================
//==============================================================================

void HAL::run_on_command_line (const std::string &command, const std::vector<std::string> &args)
{

#if DT3_OS == DT3_MACOSX

	// Build args array
	std::vector<char *> args_array;
	
	args_array.push_back(const_cast<char*>(command.c_str()));	// TODO: Icky const_cast
	for (DTuint i = 0; i < args.size(); ++i)
		args_array.push_back(const_cast<char*>(args[i].c_str()));	// TODO: Icky const_cast
	args_array.push_back(NULL);

	// Fork and run
	pid_t pid;

	switch(pid = fork()) {
		case -1: 
			ERRORMSG("Unable to start Game");
			break;

		case 0:	
			execv(const_cast<char*>(command.c_str()), &args_array[0]);
			ERRORMSG("Unable to start Game");
			exit(1);
			break;

		default:
			// Hopefully a success
			break;
	}

#endif
}

//==============================================================================
//==============================================================================

void HAL::display_modes (std::map<DTint, std::vector<DisplayMode>> &modes)
{
	modes.clear();
	
#if DT3_OS == DT3_MACOSX

	// see http://www.carbondev.com/site/?page=CGDisplaymode_list
	
	CGDirectDisplayID display_array[MAX_DISPLAYS];
	CGDisplayCount numDisplays;
	CGGetActiveDisplayList (MAX_DISPLAYS, display_array, &numDisplays);
    
	
	for(DTuint i = 0; i < numDisplays; i++) { 
	
		// get a list of display modes for the display
		CFArrayRef mode_list = ::CGDisplayCopyAllDisplayModes(display_array[i],NULL);
		CFIndex count = ::CFArrayGetCount(mode_list);
        
        std::vector<DisplayMode> modes_for_display;
		
		for(DTuint j = 0; j < count; j++) { 

			// look at each mode in the available list
			CGDisplayModeRef cg_mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(mode_list, j);
            						
			//CFBooleanRef stretched = (CFBooleanRef) CFDictionaryGetValue(mode, kCGDisplayModeIsStretched);
			//if (!stretched || !CFBooleanGetValue(stretched))	continue;

			//CFNumberRef bitsPerPixel = (CFNumberRef)CFDictionaryGetValue(mode, kCGDisplayBitsPerPixel);
			DTint width = static_cast<DTint>(::CGDisplayModeGetWidth(cg_mode));
			DTint height = static_cast<DTint>(::CGDisplayModeGetHeight(cg_mode));
            DTdouble refresh = static_cast<DTdouble>(::CGDisplayModeGetRefreshRate(cg_mode));
            
            CFStringRef pixelEncoding = ::CGDisplayModeCopyPixelEncoding(cg_mode);
     
            // Allow only 32-bit
            if (CFStringCompare(pixelEncoding,CFSTR(IO32BitDirectPixels),0) == kCFCompareEqualTo) {
                CFRelease(pixelEncoding);
            } else {
                CFRelease(pixelEncoding);
                continue;
            }
            
            DisplayMode m;
            m.set_width(width);
            m.set_height(height);
            m.set_refresh_rate(refresh);
            
            if ( std::find(modes_for_display.begin(), modes_for_display.end(), m) == modes_for_display.end()) {
                modes_for_display.push_back(m);
            }
						
		}
        
        std::sort(modes_for_display.begin(), modes_for_display.end());
        
        modes[i] = modes_for_display;
        
        CFRelease(mode_list);
	}
#endif
    
}

void HAL::switch_display_mode (DTint display, DisplayMode mode)
{
#if DT3_OS == DT3_MACOSX
	
	CGDirectDisplayID display_array[MAX_DISPLAYS];
	CGDisplayCount numDisplays;
	CGGetActiveDisplayList (MAX_DISPLAYS, display_array, &numDisplays);
    
    // Get the mode again
    CGDisplayModeRef cg_mode;
    CFIndex index, count;
    CFArrayRef mode_list;
 
    mode_list = ::CGDisplayCopyAllDisplayModes (display_array[display], NULL);
    count = ::CFArrayGetCount (mode_list);
 
    for (index = 0; index < count; index++) {
    
        cg_mode = (CGDisplayModeRef)CFArrayGetValueAtIndex (mode_list, index);
             
        DTint width = (DTint) ::CGDisplayModeGetWidth(cg_mode);
        DTint height = (DTint) ::CGDisplayModeGetHeight(cg_mode);
        CFStringRef pixelEncoding = ::CGDisplayModeCopyPixelEncoding(cg_mode);
     
        if (height == mode.height() && width == mode.width() && CFStringCompare(pixelEncoding,CFSTR(IO32BitDirectPixels),0) == kCFCompareEqualTo) {
            CFRelease(pixelEncoding);
        } else {
            CFRelease(pixelEncoding);
            continue;
        }
        
        // Time to capture the display
        if (!::CGDisplayIsCaptured(display_array[display])) {
            ::CGDisplayCapture(display_array[display]);
        }
        
        ::CGDisplaySetDisplayMode (display_array[display], cg_mode, NULL);
        
    }
    CFRelease(mode_list);

#endif
    
}

void HAL::display_rect (DTint display, DTint &x, DTint &y, DTint &width, DTint &height)
{
#if DT3_OS == DT3_MACOSX
	
	CGDirectDisplayID display_array[MAX_DISPLAYS];
	CGDisplayCount numDisplays;
	::CGGetActiveDisplayList (MAX_DISPLAYS, display_array, &numDisplays);

    CGRect r = ::CGDisplayBounds (display_array[display]);
    x = static_cast<DTint>(r.origin.x);
    y = static_cast<DTint>(r.origin.y);
    width = static_cast<DTint>(r.size.width);
    height = static_cast<DTint>(r.size.height);
    
#endif
}

//==============================================================================
//==============================================================================

void HAL::launch_browser (const URL &url)
{
#if DT3_OS == DT3_MACOSX	
	NSString *url_ns = [NSString stringWithUTF8String:url.full_url().c_str()];
	NSURL *nsurl = [NSURL URLWithString: url_ns];

	LSOpenCFURLRef( (CFURLRef)nsurl, NULL);
#elif DT3_OS == DT3_IOS
	NSString *url_ns = [NSString stringWithCString:url.full_url().c_str()];
	NSURL *nsurl = [NSURL URLWithString: url_ns];

	[[UIApplication sharedApplication] openURL: nsurl];
#endif
}

//==============================================================================
//==============================================================================

void HAL::launch_editor (const FilePath &path)
{
#if DT3_OS == DT3_MACOSX	
	NSString* url_ns = [NSString stringWithUTF8String:path.full_path().c_str()];
    [[NSWorkspace sharedWorkspace] openFile:url_ns];
#endif
}

//==============================================================================
//==============================================================================

DTuint HAL::num_CPU_cores (void)
{
    return std::thread::hardware_concurrency();
}

//==============================================================================
//==============================================================================

DTboolean HAL::move_file (const FilePath &from, const FilePath &to)
{
	if (link(from.full_path().c_str(), to.full_path().c_str()) == -1)
		return false;
		
	if (unlink(from.full_path().c_str()) == -1)
		return false;

	return true;
}

DTboolean HAL::delete_file (const FilePath &file)
{
	if (unlink(file.full_path().c_str()) == -1)
		return false;

	return true;
}

DTuint64 HAL::modification_date (const FilePath &file)
{
	struct stat buffer;	
	if (stat(file.full_path().c_str(), &buffer))
		return 0;
	
	return static_cast<DTuint>(buffer.st_mtime);
}

DTboolean HAL::is_dir (const FilePath &file)
{
	struct stat buffer;	
	if (stat(file.full_path().c_str(), &buffer))
		return 0;
	
	return (DTboolean) S_ISDIR(buffer.st_mode);
}

std::string HAL::path_separator (void)
{
	return "/";
}

void HAL::list_directory (const FilePath &pathname, DTboolean recursive, std::vector<FilePath> &paths)
{
	// set up and run platform specific iteration routines
		
	// Prime the queue
	std::list<std::string> dir_queue;
	dir_queue.push_back(pathname.full_path());
	
	while (dir_queue.size()) {
		std::string dir = dir_queue.back();
		dir_queue.pop_back();

		// Build the name list
		struct dirent **namelist = NULL;
		DTint n = scandir(dir.c_str(), &namelist, NULL, NULL);
		
		// process and free the name list
		for (DTint i = 0; i < n; ++i) {
			// Skip invisible files
			if (namelist[i]->d_name[0] != '.') {
				
				if (namelist[i]->d_type == DT_DIR) {
                    std::string entry = dir + "/" + namelist[i]->d_name;
                        
                    paths.push_back(FilePath(entry));
															
					if (recursive) {
                        dir_queue.push_back(entry);
                    }
                    
				} else {
					
                    std::string entry = dir + "/" + namelist[i]->d_name;
                    paths.push_back(FilePath(entry));
                
				}
				
			}
			::free(namelist[i]);
		}
		if (namelist) ::free(namelist);
		
	}

}


FilePath HAL::app_dir (void)
{
    FilePath result;
    
#if DT3_OS == DT3_MACOSX
	// get Current directory
	DTcharacter cstr[1024];	//MAXPATHLEN??
	
	// get Application path
	CFURLRef mainURL = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFURLRef tempURL = CFURLCreateCopyDeletingLastPathComponent(NULL, mainURL);
	CFStringRef text = CFURLCopyFileSystemPath( tempURL, kCFURLPOSIXPathStyle );
	
	if (CFStringGetCString( text, cstr, sizeof(cstr), kCFStringEncodingUTF8 )) {
		
		std::string path(cstr);
		
		// TODO: Fix me.
		if (path.find("/Debug") != std::string::npos ||
            path.find("/Release") != std::string::npos) {
			path = path.substr(0, path.find_last_of('/'));
		}
		
        result = FilePath(path);
	}
	
	CFRelease(text);
	CFRelease(mainURL);
	CFRelease(tempURL);
	
#elif DT3_OS == DT3_IOS
	// get Current directory
	DTcharacter cstr[1024];	//MAXPATHLEN??
	
	// get Application path
	CFURLRef mainURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	CFStringRef text = CFURLCopyFileSystemPath( mainURL, kCFURLPOSIXPathStyle );
	
	if (CFStringGetCString( text, cstr, sizeof(cstr), kCFStringEncodingUTF8 )) {
        result = FilePath(cstr);
    }
	
	CFRelease(text);
	CFRelease(mainURL);
#endif

    return result;
}


FilePath HAL::save_dir (void)
{
    FilePath result;

	//
	// Determine save Files Dir
	//
	
#if DT3_OS == DT3_MACOSX
	FSRef prefs_dir_ref;
	OSErr err = FSFindFolder (kUserDomain,kPreferencesFolderType,false,&prefs_dir_ref);
	if (err == noErr) {
        DTcharacter cstr[1024];	//MAXPATHLEN??
        CFStringRef text;

		CFURLRef prefsURL = CFURLCreateFromFSRef (kCFAllocatorDefault, &prefs_dir_ref);
		text = CFURLCopyFileSystemPath( prefsURL, kCFURLPOSIXPathStyle );
		
		if (CFStringGetCString( text, cstr, sizeof(cstr), kCFStringEncodingUTF8 )) {
            result = FilePath(cstr);
		}
		
		CFRelease(text);
		CFRelease(prefsURL);	
	}
#elif DT3_OS == DT3_IOS
    result = FilePath("{APPDIR}/../Documents");
#endif	

    return result;
}

//==============================================================================
//==============================================================================

std::string HAL::region (void)
{
    CFLocaleRef locale_ref = ::CFLocaleCopyCurrent();
    CFTypeRef countryRef = ::CFLocaleGetValue(locale_ref, kCFLocaleCountryCode);
   
    const DTuint MAX_LOCALE = 128;
    DTcharacter countrycode_c[MAX_LOCALE];
    ::CFStringGetCString((CFStringRef) countryRef, countrycode_c, MAX_LOCALE, kCFStringEncodingASCII);

    CFRelease(locale_ref);

    return countrycode_c ? countrycode_c : NULL;
}

std::string HAL::language (void)
{
    CFLocaleRef locale_ref = CFLocaleCopyCurrent();

    CFArrayRef langs = ::CFLocaleCopyPreferredLanguages();
    CFStringRef langCode = (CFStringRef) ::CFArrayGetValueAtIndex (langs, 0);

    const DTuint MAX_LANG = 128;
    DTcharacter languagecode_c[MAX_LANG];
    ::CFStringGetCString((CFStringRef) langCode, languagecode_c, MAX_LANG, kCFStringEncodingASCII);

    CFRelease(locale_ref);
    CFRelease(langs);

    return languagecode_c ? languagecode_c : NULL;
}

//==============================================================================
//==============================================================================

std::map<StringCopier, Globals::GlobalsEntry> HAL::load_globals (void)
{
    CFStringRef app = ::CFStringCreateWithCString(NULL, PROJECT_NAME, kCFStringEncodingASCII);

    CFDictionaryRef prefs = ::CFPreferencesCopyMultiple(NULL, app, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);

    std::map<StringCopier, Globals::GlobalsEntry> g;
    
    if (prefs) {
        CFIndex dict_count;
        CFIndex dict_index;
        
        dict_count = CFDictionaryGetCount(prefs);
        
        std::vector<CFTypeRef> keys;
        std::vector<CFTypeRef> values;
        
        keys.resize(dict_count);
        values.resize(dict_count);
        
        ::CFDictionaryGetKeysAndValues(prefs, &keys[0], &values[0]);
    
        for (dict_index = 0; dict_index < dict_count; dict_index++) {
            if (keys[dict_index] && values[dict_index]) {
                DTcharacter name_c[MAX_KEY_LENGTH];
                DTcharacter value_c[MAX_VALUE_NAME];
                
                ::CFStringGetCString((CFStringRef) keys[dict_index], name_c, MAX_KEY_LENGTH, kCFStringEncodingASCII);
                ::CFStringGetCString((CFStringRef) values[dict_index], value_c, MAX_VALUE_NAME, kCFStringEncodingASCII);
     
				std::string name(name_c);
				std::string value(value_c);
				DTuint lifetime = Globals::PERSISTENT;
				
				if (name[0] == '-') {
					name.erase(0,1);
					value = MoreStrings::from_obfuscated(name,value);
					lifetime = Globals::PERSISTENT_OBFUSCATED;
				}
           
				log("Reading persistent value: " + name + " = " + value);
                
                Globals::GlobalsEntry e;
                e.name = name;
                e.value = value;
                e.lifetime = lifetime;
                
                g[name] = e;
            }
            
        }
            
         ::CFRelease(prefs);
   
    }

    ::CFRelease(app);
    
    return g;
}

void HAL::save_globals (std::map<StringCopier, Globals::GlobalsEntry> globals)
{
    CFStringRef app = ::CFStringCreateWithCString(NULL, PROJECT_NAME, kCFStringEncodingASCII);
        
	for (auto &i : globals) {
        if (i.second.lifetime == Globals::PERSISTENT || i.second.lifetime == Globals::PERSISTENT_OBFUSCATED) {
			std::string name = i.second.name;
			std::string value = i.second.value;
            
            if (name.size() <= 0 || value.size() <= 0)
                continue;
			
			if (i.second.lifetime == Globals::PERSISTENT_OBFUSCATED) {
				value = MoreStrings::to_obfuscated(name, value);
				name = "-" + name;

				log("Writing persistent value: " + name + " = " + std::string(i.second.value) + " (Obfuscated form is " + value + ")");
			} else {
				log("Writing persistent value: " + name + " = " + value);
			}
			
            CFStringRef key = ::CFStringCreateWithCString(NULL, name.c_str(), kCFStringEncodingASCII);
            CFStringRef val = ::CFStringCreateWithCString(NULL, value.c_str(), kCFStringEncodingASCII);

            ::CFPreferencesSetAppValue(key, val, app); 

            ::CFRelease(key); 
            ::CFRelease(val); 
        }
    }

    ::CFPreferencesAppSynchronize(app); 
	
	::CFRelease(app); 
}

//==============================================================================
//==============================================================================

} // DT3

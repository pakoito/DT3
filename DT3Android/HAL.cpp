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

#include "DT3Android/HAL.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/DisplayMode.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Network/URL.hpp"
#include "DT3Core/System/Globals.hpp"

#include <thread>
#include <sys/time.h>
#include <android/log.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define  LOG_TAG    "JNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,HAL::initialize())
GLOBAL_STATIC_DESTRUCTION(0,HAL::destroy())

//==============================================================================
//==============================================================================

JavaVM* gJavaVM = NULL;
jobject gJavaObjActivity;
jobject gJavaObjContext;

//==============================================================================
//==============================================================================

jclass load_class (JNIEnv* env, const char *clazz)
{
    jclass c_activity = env->GetObjectClass(gJavaObjActivity);
    ASSERT(c_activity);

    jclass c_class_loader = env->FindClass("java/lang/ClassLoader");
    ASSERT(c_class_loader);
    
    jmethodID m_get_class_loader = env->GetMethodID(c_activity, "getClassLoader", "()Ljava/lang/ClassLoader;");
    ASSERT(m_get_class_loader);
    jobject o_class_loader = env->CallObjectMethod(gJavaObjActivity, m_get_class_loader);
    ASSERT(o_class_loader);

    jmethodID m_load_class = env->GetMethodID(c_class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    ASSERT(m_load_class);
    
    jstring o_clazz = env->NewStringUTF(clazz);

    jclass c_clazz = (jclass)(env->CallObjectMethod(o_class_loader, m_load_class, o_clazz));
    ASSERT(c_clazz);
    
    return c_clazz;
}

//==============================================================================
//==============================================================================

void HAL::initialize (void)
{

}

void HAL::destroy (void)
{
    gJavaVM->DetachCurrentThread();
}

//==============================================================================
//==============================================================================

void HAL::initialize (JavaVM* vm, jobject activity)
{
    gJavaVM = vm;
    
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    gJavaObjActivity = env->NewGlobalRef(activity);

    // Get current context
    jclass c_activity = env->GetObjectClass(activity);
    jmethodID m_context = env->GetMethodID(c_activity, "getApplicationContext", "()Landroid/content/Context;");
    gJavaObjContext = env->NewGlobalRef(env->CallObjectMethod(activity, m_context));    

    if (needs_detach)
        gJavaVM->DetachCurrentThread();
}

//==============================================================================
//==============================================================================

void HAL::log (const std::string &message)
{
	LOGE(message.c_str());
}

//==============================================================================
//==============================================================================

DTdouble HAL::program_running_time(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (DTdouble)( (DTdouble) now.tv_sec + (DTdouble) now.tv_usec/1000000.0);
}

//==============================================================================
//==============================================================================

void HAL::run_on_command_line (const std::string &command, const std::vector<std::string> &args)
{

}

//==============================================================================
//==============================================================================

void HAL::display_modes (std::map<DTint, std::vector<DisplayMode>> &modes)
{
	modes.clear();
}

void HAL::switch_display_mode (DTint display, DisplayMode mode)
{

}

void HAL::display_rect (DTint display, DTint &x, DTint &y, DTint &width, DTint &height)
{

}

//==============================================================================
//==============================================================================

void HAL::launch_browser (const URL &url)
{
	// Java: 
	//	final Intent intent = new Intent(Intent.ACTION_VIEW).setData(Uri.parse(url));
	//	_context.startActivity(intent);
    
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jstring o_url = env->NewStringUTF(url.full_url().c_str());
	ASSERT(o_url);

    jclass c_uri = env->FindClass("android/net/Uri");
	ASSERT(c_uri);

    jmethodID m_parse = env->GetStaticMethodID(c_uri, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	ASSERT(m_parse);
    jobject o_uri = (jstring) env->CallStaticObjectMethod(c_uri, m_parse, o_url);
	ASSERT(o_uri);

    jclass c_intent = env->FindClass("android/content/Intent");
	ASSERT(c_intent);
    jfieldID f_ACTION_VIEW = env->GetStaticFieldID(c_intent, "ACTION_VIEW", "Ljava/lang/String;");
	ASSERT(f_ACTION_VIEW);
    jobject o_ACTION_VIEW = env->GetStaticObjectField(c_intent, f_ACTION_VIEW);
	ASSERT(o_ACTION_VIEW);

    jmethodID m_new_intent = env->GetMethodID(c_intent, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
	ASSERT(m_new_intent);
    jobject o_intent = env->AllocObject(c_intent);
	ASSERT(o_intent);
    env->CallVoidMethod(o_intent, m_new_intent, o_ACTION_VIEW, o_uri);
    
    jclass c_activity = env->FindClass("android/app/Activity");
	ASSERT(c_activity);
    jmethodID m_start_activity = env->GetMethodID(c_activity, "startActivity", "(Landroid/content/Intent;)V");
	ASSERT(m_start_activity);
    env->CallVoidMethod(gJavaObjActivity,m_start_activity,o_intent);
    
    if (needs_detach)
        gJavaVM->DetachCurrentThread();
}

//==============================================================================
//==============================================================================

void HAL::launch_editor (const FilePath &path)
{

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
	
	return (DTuint) buffer.st_mtime;
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
	// Java:
	//	return context.getFilesDir().tostd::string();
    
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jclass c_context = env->FindClass("android/content/Context");
	ASSERT(c_context);

    jmethodID m_getFilesDir = env->GetMethodID(c_context, "getFilesDir", "()Ljava/io/File;");
    jobject o_dir = (jstring) env->CallObjectMethod(gJavaObjContext, m_getFilesDir);
    
    jclass c_file = env->FindClass("java/io/File");
	ASSERT(c_file);
    
    jmethodID m_toString = env->GetMethodID(c_file, "toString", "()Ljava/lang/String;");
    jstring s_dir = (jstring) env->CallObjectMethod(o_dir, m_toString);

    // Convert to UTF-8
    FilePath path = FilePath(env->GetStringUTFChars(s_dir, 0));
    
    if (needs_detach)
        gJavaVM->DetachCurrentThread();

	return path;
}


FilePath HAL::save_dir (void)
{
	// Java:
	//	return context.getFilesDir().toString();
    
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jclass c_context = env->FindClass("android/content/Context");
	ASSERT(c_context);
	
    jmethodID m_getFilesDir = env->GetMethodID(c_context, "getFilesDir", "()Ljava/io/File;");
	ASSERT(m_getFilesDir);

    jobject o_dir = env->CallObjectMethod(gJavaObjContext, m_getFilesDir);

    jclass c_file = env->FindClass("java/io/File");
	ASSERT(c_file);

    jmethodID m_toString = env->GetMethodID(c_file, "toString", "()Ljava/lang/String;");
	ASSERT(m_toString);

    jstring s_dir = (jstring) env->CallObjectMethod(o_dir, m_toString);

    // Convert to UTF-8
    FilePath path = FilePath(env->GetStringUTFChars(s_dir, 0));
    
    if (needs_detach)
        gJavaVM->DetachCurrentThread();

	return path;
}

//==============================================================================
//==============================================================================

std::string HAL::region (void)
{
	// Java: 
	//	return Locale.getDefault().getISO3Country();
    
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

	// Get Locale class
	jclass c_Locale = env->FindClass("java/util/Locale");
	ASSERT(c_Locale);

	// Call getDefault
    jmethodID m_getDefault = env->GetStaticMethodID(c_Locale, "getDefault", "()Ljava/util/Locale;");
    jobject o_Locale = env->CallStaticObjectMethod(c_Locale, m_getDefault);

	// Call getISO3Country
    jmethodID m_getISO3Country = env->GetMethodID(c_Locale, "getISO3Country", "()Ljava/lang/String;");
    jstring s_region = (jstring) env->CallObjectMethod(o_Locale, m_getISO3Country);
    
    // Convert to UTF-8
    std::string s = std::string(env->GetStringUTFChars(s_region, 0));
    
    if (needs_detach)
        gJavaVM->DetachCurrentThread();

	return s;
}

std::string HAL::language (void)
{
	// Java: 
	//	return Locale.getDefault().getISO3Language();

    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

	// Get Locale class
	jclass c_Locale = env->FindClass("java/util/Locale");
	ASSERT(c_Locale);

	// Call getDefault
    jmethodID m_getDefault = env->GetStaticMethodID(c_Locale, "getDefault", "()Ljava/util/Locale;");
    jobject o_Locale = env->CallStaticObjectMethod(c_Locale, m_getDefault);

	// Call getISO3Country
    jmethodID m_getISO3Language = env->GetMethodID(c_Locale, "getISO3Language", "()Ljava/lang/String;");
    jstring s_language = (jstring) env->CallObjectMethod(o_Locale, m_getISO3Language);
    
    // Convert to UTF-8
    std::string s = std::string(env->GetStringUTFChars(s_language, 0));
    
    if (needs_detach)
        gJavaVM->DetachCurrentThread();

	return s;
}

//==============================================================================
//==============================================================================

std::map<StringCopier, Globals::GlobalsEntry> HAL::load_globals (void)
{
	// Java: 
	//	SharedPreferences           settings;
	//	SharedPreferences.Editor    editor;
	//	Map<String,String>          globals = new TreeMap();
	//	settings = _context.getSharedPreferences(PREFS_NAME, 0);
	//	Map<String,?> mp = settings.getAll();
	//	Iterator it = mp.entrySet().iterator();
	//	while (it.hasNext()) {
	//	    Map.Entry pairs = (Map.Entry)it.next();
	//	    globals.put( (String) pairs.getKey(), (String) pairs.getValue() );
	//	}
	//	return globals;
	
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jstring o_prefs_name = env->NewStringUTF(PROJECT_NAME);
    
	//	settings = _context.getSharedPreferences(PREFS_NAME, 0);
    jclass c_context = env->FindClass("android/content/Context");
	ASSERT(c_context);
    jmethodID m_get_shared_preferences = env->GetMethodID(c_context, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
	ASSERT(m_get_shared_preferences);
    jobject o_settings = env->CallObjectMethod(gJavaObjContext, m_get_shared_preferences, o_prefs_name, 0);
	ASSERT(o_settings);

	//	Map<String,?> mp = settings.getAll();
    jclass c_shared_preferences = env->FindClass("android/content/SharedPreferences");
	ASSERT(c_shared_preferences);
    jmethodID m_get_all = env->GetMethodID(c_shared_preferences, "getAll", "()Ljava/util/Map;");
	ASSERT(m_get_all);
    jobject o_mp = env->CallObjectMethod(o_settings, m_get_all);
	ASSERT(o_mp);

	//	Iterator it = mp.entrySet().iterator();
    jclass c_map = env->FindClass("java/util/Map");
	ASSERT(c_map);
    jmethodID m_entry_set = env->GetMethodID(c_map, "entrySet", "()Ljava/util/Set;");
    ASSERT(m_entry_set);
    
    jclass c_set = env->FindClass("java/util/Set");
    ASSERT(c_set);
    jmethodID m_iterator = env->GetMethodID(c_set, "iterator", "()Ljava/util/Iterator;");
    ASSERT(m_iterator);

    jclass c_iterator = env->FindClass("java/util/Iterator");
    ASSERT(c_iterator);

    jclass c_entry = env->FindClass("java/util/Map$Entry");
    ASSERT(c_entry);

    jmethodID m_has_next = env->GetMethodID(c_iterator, "hasNext", "()Z");
    jmethodID m_next = env->GetMethodID(c_iterator, "next", "()Ljava/lang/Object;");
    jmethodID m_get_key = env->GetMethodID(c_entry, "getKey", "()Ljava/lang/Object;");
    jmethodID m_get_value = env->GetMethodID(c_entry, "getValue", "()Ljava/lang/Object;");
    
    jobject o_set = env->CallObjectMethod(o_mp, m_entry_set);
    jobject o_iter = env->CallObjectMethod(o_set, m_iterator);

    std::map<StringCopier, Globals::GlobalsEntry> globals;

	//	while (it.hasNext()) {
    while (env->CallBooleanMethod(o_iter, m_has_next)) {
        jobject o_entry = env->CallObjectMethod(o_iter, m_next);
        jstring o_key = (jstring) env->CallObjectMethod(o_entry, m_get_key);
        jstring o_value = (jstring) env->CallObjectMethod(o_entry, m_get_value);
        
        const char *name_c = env->GetStringUTFChars(o_key, 0);
        const char *value_c = env->GetStringUTFChars(o_value, 0);
                
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
        
        globals[name] = e;

        env->DeleteLocalRef(o_entry);
        env->DeleteLocalRef(o_key);
        env->DeleteLocalRef(o_value);
    }
    
    if (needs_detach)
        gJavaVM->DetachCurrentThread();

    return globals;
}

void HAL::save_globals (std::map<StringCopier, Globals::GlobalsEntry> globals)
{
	// Java: 
	//	SharedPreferences           settings;
	//	SharedPreferences.Editor    editor;
	//	settings = _context.getSharedPreferences(PREFS_NAME, 0);
	//	editor = settings.edit();
	//	Iterator it = globals.entrySet().iterator();
	//	while (it.hasNext()) {
	//	    Map.Entry pairs = (Map.Entry)it.next();
	//	    editor.putString( (String) pairs.getKey(), (String) pairs.getValue());
	//	}
	//	editor.commit();

    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jstring o_prefs_name = env->NewStringUTF(PROJECT_NAME);

	//	settings = _context.getSharedPreferences(PREFS_NAME, 0);
    jclass c_context = env->FindClass("android/content/Context");
	ASSERT(c_context);
    jmethodID m_get_shared_preferences = env->GetMethodID(c_context, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
	ASSERT(m_get_shared_preferences);
    jobject o_settings = env->CallObjectMethod(gJavaObjContext, m_get_shared_preferences, o_prefs_name, 0);
	ASSERT(o_settings);

	//	editor = settings.edit();
    jclass c_shared_preferences = env->FindClass("android/content/SharedPreferences");
	ASSERT(c_shared_preferences);
    jmethodID m_edit = env->GetMethodID(c_shared_preferences, "edit", "()Landroid/content/SharedPreferences$Editor;");
	ASSERT(m_edit);
    jobject o_editor = env->CallObjectMethod(o_settings, m_edit);
	ASSERT(o_editor);
	
    jclass c_editor = env->FindClass("android/content/SharedPreferences$Editor");
	ASSERT(c_editor);
    jmethodID m_put_string = env->GetMethodID(c_editor, "putString", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
	ASSERT(m_put_string);
	
	// Iterate through all globals
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
			
            jstring j_name = env->NewStringUTF( name.c_str() );
            jstring j_value = env->NewStringUTF( value.c_str() );
        
            env->CallObjectMethod(o_editor, m_put_string, j_name, j_value);

            env->DeleteLocalRef(j_name);
            env->DeleteLocalRef(j_value);
        }

	}
	
	//	editor.commit();
    jmethodID m_commit = env->GetMethodID(c_editor, "commit", "()Z");
	ASSERT(m_put_string);
	env->CallBooleanMethod(o_editor, m_commit);
	
    if (needs_detach)
        gJavaVM->DetachCurrentThread();
}

//==============================================================================
//==============================================================================

} // DT3

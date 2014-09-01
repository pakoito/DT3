//==============================================================================
///	
///	File: GameMainThread.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Entry/GameMainThread.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Configure.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Application.hpp"
#include "DT3Core/System/UnitTest.hpp"
#include "DT3Core/System/Console.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/CommandParams.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Threads/ThreadTaskQueue.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3Core/Devices/DeviceInput.hpp"
#include "DT3Core/Devices/DeviceMusic.hpp"
#include "DT3Core/Devices/DeviceNetwork.hpp"

#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID || DT3_OS == DT3_MACOSX
    #include "DT3OpenGL/DT3OpenGL.hpp"
#else
    #include "DT3DX11/DT3GLDX11.hpp"
#endif

#include DT3_HAL_INCLUDE_PATH

#ifdef DTP_USE_PORTAL
    #include "DTPortalSDK.hpp"
#endif


//==============================================================================
//==============================================================================

namespace DT3 {
	
//==============================================================================
//==============================================================================

GameMainThread::State                   GameMainThread::_state = STATE_IDLE;
DTboolean                               GameMainThread::_stop = true;

std::thread                             GameMainThread::_event_queue_thread;
std::mutex                              GameMainThread::_event_queue_mutex;

std::list<std::shared_ptr<LatentCall>>  GameMainThread::_event_queue;

//==============================================================================
//==============================================================================

void GameMainThread::initialize_engine_impl (void)
{
    LOG_DEBUG << "initialize_engine_impl called";
    
    //
    // Initialize engine
    //
    
#ifdef DTP_USE_PORTAL
    // Start portal SDK
    DTPortal::initialize(DTP_PUBLIC_ID, DTP_PRIVATE_KEY, HAL::save_dir().full_path().c_str());
#endif

    // Turn off sync_with_stdio
    // http://stackoverflow.com/questions/9371238/why-is-reading-lines-from-stdin-much-slower-in-c-than-python
    std::ios_base::sync_with_stdio(false);    

    // initialize the static initializer
	StaticInitializer::initialize();

	// Import a config file for the engine
	//Configure::import_config(FilePath("{config.txt}"));
    std::thread import_config_thread(Configure::import_config, FilePath("{config.txt}"));

    // Start up thread task queue threads
    ThreadTaskQueue::set_num_threads(HAL::num_CPU_cores());

	// Warning if in debug mode
#if DT3_DEBUG == 1
	LOG_MESSAGE << "WARNING: Debug mode enabled.";
#endif

    // Warning if next run after crash
	DTboolean crashed = MoreStrings::cast_from_string<DTboolean>(Globals::global("SYS_CRASHED"));
    if (crashed) {
        LOG_MESSAGE << "WARNING: Game crashed on last run.";
        Globals::set_global("SYS_WAS_CRASHED","1",Globals::VOLATILE);
    } else {
        Globals::set_global("SYS_WAS_CRASHED","0",Globals::VOLATILE);
    }

    // Crash detection. Save the crashed flag to a file and save it. Clear it on quit.
	Globals::set_global("SYS_CRASHED","1",Globals::PERSISTENT);
	Globals::save();    // Immediately save crash flag

    // Wait for config to finish loading
    import_config_thread.join();

	//
	// Create Managers
	//
				
	LOG_MESSAGE << "Creating Network Manager...";
	std::shared_ptr<DeviceNetwork> network_manager = checked_cast<DeviceNetwork>(Factory::create_object("{CLASS_NETWORK_DEVICE}"));
    if (network_manager)    System::set_network_manager(network_manager);
    else                    System::set_network_manager(DeviceNetwork::create());   // Default NULL device
    LOG_MESSAGE << " Created " << System::network_manager()->class_id_child();
	
	LOG_MESSAGE << "Creating Input Manager...";
	std::shared_ptr<DeviceInput> input_manager = checked_cast<DeviceInput>(Factory::create_object("{CLASS_INPUT_DEVICE}"));
    if (input_manager)      System::set_input_manager(input_manager);
    else                    System::set_input_manager(DeviceInput::create());       // Default NULL device
    LOG_MESSAGE << " Created " << System::input_manager()->class_id_child();
	
	LOG_MESSAGE << "Creating Music Device...";
	std::shared_ptr<DeviceMusic> music_device = checked_cast<DeviceMusic>(Factory::create_object("{CLASS_MUSIC_DEVICE}"));
    if (music_device)       System::set_music_renderer(music_device);
    else                    System::set_music_renderer(DeviceMusic::create());      // Default NULL device
    LOG_MESSAGE << " Created " << System::music_renderer()->class_id_child();

	LOG_MESSAGE << "Creating Audio Device...";
	std::shared_ptr<DeviceAudio> audio_device = checked_cast<DeviceAudio>(Factory::create_object("{CLASS_AUDIO_DEVICE}"));
    if (audio_device)       System::set_audio_renderer(audio_device);
    else                    System::set_audio_renderer(DeviceAudio::create());      // Default NULL device
    LOG_MESSAGE << " Created " << System::audio_renderer()->class_id_child();
				
	//
	// Application object
	//
	
	// Build the application class
	LOG_MESSAGE << "Creating Application...";
	std::shared_ptr<Application> application = checked_cast<Application>(Factory::create_object("{CLASS_APPLICATION}"));
    if (application)        System::set_application(application);
    else                    System::set_application(Application::create());      // Default NULL device
    LOG_MESSAGE << " Created " << System::application()->class_id_child();
    
    _state = STATE_INITIALIZED;
    
	// Setup Screen
	LOG_DEBUG << "initialize_engine_impl done";
}

void GameMainThread::initialize_engine (void)
{
    append(make_latent_call(&GameMainThread::initialize_engine_impl));
}

//==============================================================================
//==============================================================================

void GameMainThread::destroy_engine_impl (void)
{
    LOG_DEBUG << "destroy_engine_impl called";

    _state = STATE_IDLE;

	//
	// Destroy Managers
	//
	
	LOG_MESSAGE << "Cleaning up Application...";
	System::set_application(NULL);
	
	LOG_MESSAGE << "Cleaning up Audio Device...";
   	System::set_audio_renderer(NULL);
	
	LOG_MESSAGE << "Cleaning up Music Device...";
   	System::set_music_renderer(NULL);
    
	LOG_MESSAGE << "Cleaning up Input Manager...";
    System::set_input_manager(NULL);
		
	LOG_MESSAGE << "Cleaning up Network Manager...";
	System::set_network_manager(NULL);
			
    // Set crashed flag. Globals get saved below in StaticInitializer::destroy()
	Globals::set_global("SYS_CRASHED","0",Globals::PERSISTENT);

    // Destroy statically initialized data
	StaticInitializer::destroy();
    
    
#ifdef DTP_USE_PORTAL
    // Uninitialize the portal
    DTPortal::uninitialize();
#endif
    
    LOG_DEBUG << "destroy_engine_impl done";
}

void GameMainThread::destroy_engine (void)
{
    append(make_latent_call(&GameMainThread::destroy_engine_impl));
}

//==============================================================================
//==============================================================================

void GameMainThread::process_arguments_impl (std::vector<std::string> args)
{
    for (std::size_t i = 0; i < args.size(); ++i) {

        if (args[i] == "-testall") {
            UnitTest::run_all_tests();
            continue;
        }

        if (args[i] == "-testone" && args.size() > (i+1)) {
            UnitTest::run_one_test(args[i+1]);
            i += 1; // Skip 1 parameter
            continue;
        }

        if (args[i] == "-loadlevel" && args.size() > (i+1)) {   // Plus 1 Parameter
            System::application()->transition_to_world(FilePath(args[i+1]), "TransitionFadeOutIn", 1.0F, NULL, NULL);
            i += 1; // Skip 1 parameter
            continue;
        }

        if (args[i] == "-command" && args.size() > (i+1)) {   // Plus 1 Parameter
            CommandParams params(args[i+1]);
            i += 1; // Skip 1 parameter
            
            CommandContext ctx; // Default no world, no selection
            Console::do_command(ctx, params);
            continue;
        }
        
        if (args[i] == "-resolution" && args.size() > (i+1)) {   // Plus 1 Parameter
            std::vector<std::string> res = MoreStrings::split(args[i+1],"x");
            
            if (res.size() == 2) {
                Globals::set_global("PLATFORM_RES_WIDTH", res[0], Globals::PERSISTENT);
                Globals::set_global("PLATFORM_RES_HEIGHT", res[1], Globals::PERSISTENT);
                Globals::set_global("PLATFORM_WINDOWED", "1", Globals::PERSISTENT);
            }

            i += 1; // Skip 1 parameter
            continue;
        }
        
    }
}

void GameMainThread::process_arguments (std::vector<std::string> args)
{
    append(make_latent_call(&GameMainThread::process_arguments_impl, args));
}

//==============================================================================
//==============================================================================

void GameMainThread::show_engine_impl (DTint width, DTint height)
{
    LOG_DEBUG << "show_engine_impl called";

    if (!System::renderer()) {
        // Renderer
        LOG_MESSAGE << "Creating Renderer...";
        std::shared_ptr<DeviceGraphics> renderer = checked_cast<DeviceGraphics>(Factory::create_object("{CLASS_RENDERER_DEVICE}"));
        if (renderer) {
            System::set_renderer(renderer);
            
        } else {
#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID || DT3_OS == DT3_MACOSX
            System::set_renderer(DT3OpenGL::create());    // Default OpenGL device
#else
            System::set_renderer(DT3GLDX11::create());      // Default DX11 device
#endif
        }
        
        LOG_MESSAGE << " Created " << System::renderer()->class_id_child();
        
        // Open the display
        System::renderer()->open_display(width, height);
    } else {
        // Change the display
        System::renderer()->change_display(width, height);
    }
    
    _state = STATE_INITIALIZED_AND_SHOWN;

    LOG_DEBUG << "show_engine_impl done";
}

void GameMainThread::show_engine (DTint width, DTint height)
{
    append(make_latent_call(&GameMainThread::show_engine_impl, width, height));
}

//==============================================================================
//==============================================================================

void GameMainThread::hide_engine_impl (void)
{
    LOG_DEBUG << "hide_engine_impl called";

    _state = STATE_INITIALIZED;
    
    Globals::save();

#if DT3_CLEAR_GRAPHICS_ON_SUSPEND
    if (System::renderer()) {
        System::renderer()->close_display();
        System::set_renderer(NULL);
    }
#endif

    LOG_DEBUG << "hide_engine_impl done";
}

void GameMainThread::hide_engine (void)
{
    append(make_latent_call(&GameMainThread::hide_engine_impl));
}

//==============================================================================
//==============================================================================

void GameMainThread::resize_impl (DTint width, DTint height)
{
    LOG_DEBUG << "resize_impl called";
    
    // Resize the display
    System::renderer()->change_display(width, height);
        
    LOG_DEBUG << "resize_impl done";
}

void GameMainThread::resize (DTint width, DTint height)
{
    append(make_latent_call(&GameMainThread::resize_impl, width, height));
}

//==============================================================================
//==============================================================================

void GameMainThread::touch_event_impl (TouchEvent event)
{
	if (System::input_manager() && System::renderer())
		System::input_manager()->set_touch_event (&event);
}

void GameMainThread::touch_event (TouchEvent event)
{
    append(make_latent_call(&GameMainThread::touch_event_impl, event));
}

//==============================================================================
//==============================================================================

void GameMainThread::key_down_event_impl (DTuint modifiers, DTushort key)
{
	if (System::input_manager() && System::renderer())
		System::input_manager()->set_key_down_event (modifiers, key);
}

void GameMainThread::key_down_event (DTuint modifiers, DTushort key)
{
    append(make_latent_call(&GameMainThread::key_down_event_impl, modifiers, key));
}

//==============================================================================
//==============================================================================

void GameMainThread::key_up_event_impl (DTuint modifiers, DTushort key)
{
	if (System::input_manager())
		System::input_manager()->set_key_up_event (modifiers, key);
}

void GameMainThread::key_up_event (DTuint modifiers, DTushort key)
{
    append(make_latent_call(&GameMainThread::key_up_event_impl, modifiers, key));
}

//==============================================================================
//==============================================================================

void GameMainThread::do_back_button_impl (void)
{
	if (System::input_manager())
		System::input_manager()->do_back_button();
}

void GameMainThread::do_back_button (void)
{
    append(make_latent_call(&GameMainThread::do_back_button_impl));
}

//==============================================================================
//==============================================================================

void GameMainThread::do_menu_button_impl (void)
{
	if (System::input_manager())
		System::input_manager()->do_menu_button();
}

void GameMainThread::do_menu_button (void)
{
    append(make_latent_call(&GameMainThread::do_menu_button_impl));
}

//==============================================================================
//==============================================================================

void GameMainThread::do_accelerometer_impl (Vector3 a)
{
	if (System::input_manager())
		System::input_manager()->set_acceleration_event (a);
}

void GameMainThread::do_accelerometer (Vector3 a)
{
    append(make_latent_call(&GameMainThread::do_accelerometer_impl, a));
}

//==============================================================================
//==============================================================================

void GameMainThread::do_gyro_impl (Vector3 w)
{
	if (System::input_manager())
		System::input_manager()->set_gyro_event (w);
}

void GameMainThread::do_gyro (Vector3 w)
{
    append(make_latent_call(&GameMainThread::do_gyro_impl, w));
}

//==============================================================================
//==============================================================================

void GameMainThread::do_magnetometer_impl (Vector3 m)
{
	if (System::input_manager())
		System::input_manager()->set_magnetometer_event (m);
}

void GameMainThread::do_magnetometer (Vector3 m)
{
    append(make_latent_call(&GameMainThread::do_magnetometer_impl, m));
}

//==============================================================================
//==============================================================================

void GameMainThread::start (void)
{
    _stop = false;
    _event_queue_thread = std::thread(GameMainThread::do_thread);
}

void GameMainThread::stop (void)
{
    _stop = true;
    if (_event_queue_thread.joinable())
        _event_queue_thread.join();
}

void GameMainThread::append (std::shared_ptr<LatentCall> lc)
{
    // If thread is stopped, do actions sychronously
    if (_stop == true) {
        lc->fire();
    
    // Add to queue
    } else {
        _event_queue_mutex.lock();
        _event_queue.push_back(lc);
        _event_queue_mutex.unlock();        
    }
}

//==============================================================================
//==============================================================================

void GameMainThread::loop (void)
{
	// Are there pending events?
	GameMainThread::_event_queue_mutex.lock();
	while (GameMainThread::_event_queue.size() > 0) {
		std::shared_ptr<LatentCall> lc = GameMainThread::_event_queue.front();
		GameMainThread::_event_queue.pop_front();

		lc->fire();
	}
	GameMainThread::_event_queue_mutex.unlock();

	// If engine is fully initialized
	if (GameMainThread::_state == GameMainThread::STATE_INITIALIZED_AND_SHOWN) {
        System::application()->run_event_loop ();
		
#ifdef DTP_USE_PORTAL
		DTPortal::pump_results();
#endif
	}
}

//==============================================================================
//==============================================================================

void GameMainThread::do_thread (void)
{
    // This is the main event loop. It handles all of the events going into the engine.
    while (!GameMainThread::_stop || GameMainThread::_event_queue.size() > 0) {
        GameMainThread::loop();
    }
}

//==============================================================================
//==============================================================================

} // DT3

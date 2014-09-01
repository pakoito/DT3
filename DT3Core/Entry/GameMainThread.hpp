#ifndef DT3_GAMEMAINTHREAD
#define DT3_GAMEMAINTHREAD
//==============================================================================
///	
///	File: GameMainThread.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/TouchEvent.hpp"
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class LatentCall;

//==============================================================================
/// Class
//==============================================================================

class GameMainThread {
    private:
                                GameMainThread          (void);
                                GameMainThread          (const GameMainThread &rhs);
        GameMainThread &        operator =              (const GameMainThread &rhs);	
                                ~GameMainThread         (void);

    public:

        /// Initialize the engine
        static void             initialize_engine       (void);

        /// Destroy the engine
        static void             destroy_engine          (void);
    
        /// Initialize the engine
        static void             process_arguments       (std::vector<std::string> args);

        /// Tell the engine that the screen has been shown
        static void             show_engine             (DTint width, DTint height);

        /// Tell the engine that the screen has been hidden
        static void             hide_engine             (void);


        /// Tell the engine that the screen has been hidden
        static void             resize                  (DTint width, DTint height);
    
            
        /// Send a touch input event
        /// \param event Touch input event
        static void             touch_event             (TouchEvent event);

        /// Send a key input event
        /// \param event Key input event
        static void             key_down_event          (DTuint modifiers, DTushort key);

        /// Send a key input event
        /// \param event Key input event
        static void             key_up_event            (DTuint modifiers, DTushort key);


        /// Send a back button event (Android only)
        static void             do_back_button			(void);

        /// Send a menu button event (Android only)
        static void             do_menu_button			(void);
    

        /// Send a orientation changed event
        /// \param orientation new orientation
        static void             set_orientation         (DTuint orientation);


        /// Send an accelerometer event
        /// \param a acceleration
        static void             do_accelerometer        (Vector3 a);

        /// Send a gyroscope event
        /// \param a acceleration
        static void             do_gyro                 (Vector3 w);

        /// Send a gyroscope event
        /// \param a acceleration
        static void             do_magnetometer         (Vector3 m);

    
    
        //
        // Game thread management
        //
    
        /// Start the main loop thread
        static void             start                   (void);

        /// Stop the main loop thread
        static void             stop                    (void);


		//
		// If not running in a thread
		//
		
        /// Synchronous loop iteration
		static void				loop                    (void);
		
    private:    
        static void             initialize_engine_impl  (void);
        static void             destroy_engine_impl     (void);
        static void             process_arguments_impl  (std::vector<std::string> args);
        static void             show_engine_impl        (DTint width, DTint height);
        static void             hide_engine_impl        (void);
        static void             resize_impl             (DTint width, DTint height);
        static void             loop_impl               (void);
        static void             touch_event_impl        (TouchEvent event);
        static void             key_down_event_impl     (DTuint modifiers, DTushort key);
        static void             key_up_event_impl       (DTuint modifiers, DTushort key);
        static void             do_back_button_impl     (void);
        static void             do_menu_button_impl     (void);
        static void             set_orientation_impl    (DTuint orientation);
        static void             do_accelerometer_impl   (Vector3 a);
        static void             do_gyro_impl            (Vector3 w);
        static void             do_magnetometer_impl    (Vector3 m);
    
        static void             append                  (std::shared_ptr<LatentCall> lc);

        static void             do_thread               (void);

        enum State {
            STATE_IDLE,
            STATE_INITIALIZED,
            STATE_INITIALIZED_AND_SHOWN
        };

        static State                                    _state;
        static DTboolean                                _stop;
    
        static std::thread                              _event_queue_thread;
        static std::mutex                               _event_queue_mutex;
    
        static std::list<std::shared_ptr<LatentCall>>  _event_queue;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

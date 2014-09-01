//==============================================================================
///	
///	File: Application.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Application.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Session.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Transitions/TransitionBase.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/Objects/GameController.hpp"
#include "DT3Core/Types/Animation/PropertyAnimator.hpp"
#include "DT3Core/Types/Threads/ThreadMainTaskQueue.hpp"
#include "DT3Core/Types/Threads/ThreadTaskQueue.hpp"

#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
//#include "DT3Core/Devices/DeviceInput.hpp"
//#include "DT3Core/Devices/DeviceMusic.hpp"
//#include "DT3Core/Devices/DeviceNetwork.hpp"

#include <unistd.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Application::Application (void)
	:	_done					(false),
		_show_console			(false),
		_show_profiler			(false),
		_obj_world              (NULL),
		_obj_pending_world      (NULL),
		_transition				(NULL),
		_pending_session		(NULL),
        _pending_progress       (NULL),
		_transition_time		(0.0F),
		_transition_timer		(0.0F)
{
	// update all Lores timers
	TimerLores::update_program_running_time();
    
    SystemCallbacks::touch_event_cb().add(make_callback(this, &type::forward_touch_gui));
}

Application::~Application (void)
{    		
    SystemCallbacks::touch_event_cb().remove(make_callback(this, &type::forward_touch_gui));
}

//==============================================================================
//==============================================================================

void Application::initialize (void)
{
    transition_to_world (FilePath("{APP_FIRST_LEVEL}"), "TransitionFadeOutIn", 1.0F, nullptr, nullptr);
}

//==============================================================================
//==============================================================================

void Application::tick_game (const DTfloat dt)
{
	// Tick the world
	if (_obj_world) {
		_obj_world->tick(dt);
	}
}

void Application::draw_game (const DTfloat lag)
{
	// Draw the world
	if (_obj_world) {
		_obj_world->draw(lag);
	}
}

//==============================================================================
//==============================================================================

void Application::quit(void)
{
	_done = true;
}

//==============================================================================
//==============================================================================

void Application::clear	(void)
{
	System::renderer()->clear_viewport(true,true,false);
}

//==============================================================================
//==============================================================================

DTfloat Application::transition_progress (void) const
{	
    return MoreMath::clamp_zero_one(_transition_timer / _transition_time);
}

//==============================================================================
//==============================================================================

void Application::run_event_loop (void)
{
	PROFILER(MAIN_LOOP);
    
	// update all Lores timers
	TimerLores::update_program_running_time();
            
	// tick and draw
	DTfloat dt = static_cast<DTfloat>(_game_time.delta_time());
    _game_time.reset_abs_time();

    // Add entropy from random source
    MoreMath::add_entropy ( (DTubyte*) &dt, sizeof(dt));

	// Clamp dt to a tenth of a second
	if (dt > DT3_MAX_TICK) {
		dt = DT3_MAX_TICK;
	}

    // System routines
    ThreadTaskQueue::run_queue();
    ThreadMainTaskQueue::run_queue(dt);
    
    // Tick sub systems that need it
    System::audio_renderer()->tick(dt);
    
	DTfloat step_size = dt;


    //
    // tick Game
    //
    
    PropertyAnimator::tick(dt);
    
    // tick application
    if (_obj_pending_world) {
        tick_game(step_size);
        
        std::shared_ptr<World> save_world = world();
        set_world(_obj_pending_world);
        
        tick_game(step_size);
        
        set_world(save_world);
    } else {
        tick_game(step_size);
    }

    // If all caught up
    _transition_timer += step_size;
          
    Profiler::mark_tick();


    //
    // Draw Game
    //

    clear();
    
    // If there is a transition
    if (_transition) {
        
        DTfloat fade = MoreMath::clamp_zero_one(_transition_timer / _transition_time);
        _transition->transition(fade);
        
        // Update gains
        System::audio_renderer()->set_world_gain(world(), 1.0F - fade);
        System::audio_renderer()->set_world_gain(_obj_pending_world, fade);
        

        // If the back layer is not drawn, then delete the original world
        if (_transition->begin_old_world()) {
            draw_game(0.0F);
            _transition->end_old_world();
        } else {
            set_world(NULL);
        }

        // If the front layer is drawn, ensure pending world is loaded
        if (_transition->begin_new_world()) {
        
            // load pending world if not loaded
            if (!_obj_pending_world)
                load_pending_world();
        
            // Temporarily set pending world as real world for drawing
            std::shared_ptr<World> save_world = world();
            set_world(_obj_pending_world);

            draw_game(0.0F);
            _transition->end_new_world();
    
            set_world(save_world);
        }
                        
        _transition->draw();
                    
        // If transition is complete then Yay!
        if (_transition_timer >= _transition_time) {
            set_world(_obj_pending_world);
            _obj_pending_world.reset();
            _transition.reset();
            
            // Remember current world name
            _current_pathname = _pending_pathname;

            // Notify derived classes that the world was loaded
            world_loaded(world());
            System::audio_renderer()->set_world_gain(world(), 1.0F);

        }
        
    } else {
        if (_obj_pending_world) {
            set_world(_obj_pending_world);
            _obj_pending_world.reset();
            
            // Remember current world name
            _current_pathname = _pending_pathname;
            
            // Notify derived classes that the world was loaded
            world_loaded(world());
            System::audio_renderer()->set_world_gain(world(), 1.0F);
        }

        // draw application
        draw_game(0.0F);
    }
                            
    SystemCallbacks::screen_swap_cb().fire();
                        
    Profiler::mark_frame();
    Profiler::display_to_console();

    DTfloat sleep_time = DT3_UPDATE_RATE_PERIOD - static_cast<DTfloat>(_game_time.abs_time());
    if (sleep_time > 0.0F) {
        ::usleep(sleep_time*1000*1000);
    }

}

//==============================================================================
//==============================================================================

void Application::set_world (const std::shared_ptr<World> &world)
{
    _obj_world = world;
}

const std::shared_ptr<World>& Application::world (void) const
{
    return _obj_world;
}

//==============================================================================
//==============================================================================

void Application::touch_gui (const TouchEvent *event)
{
    if (_obj_world)
        _obj_world->touch_gui(event);
}

//==============================================================================
//==============================================================================

void Application::transition_to_world ( const FilePath &pathname,
                                        std::string transition,
                                        DTfloat time,
                                        const std::shared_ptr<Session> &session_override,
                                        std::shared_ptr<Progress> progress,
                                        std::string args)
{
    LOG_MESSAGE << "Transition to world: " << pathname.full_path();

	// Ensure any previous transition is complete
	//Set_World(_obj_pending_world);
	//RELEASE(_obj_pending_world);
	//RELEASE(_transition);
    
    if (!pathname.exists()) {
        LOG_MESSAGE << " File does not exist: " << pathname.full_path();
        return;
    }

	// Store world we are transitioning to into the globals
	Globals::set_global("SYS_TRANSITION_DB", pathname.original_path(), Globals::VOLATILE);

	// If there is a transition specified, then use it
	std::shared_ptr<TransitionBase> tr = checked_cast<TransitionBase>(Factory::create_object(transition));
		
	// set up deferred load of world
	_pending_pathname = pathname;
	_pending_session = session_override;
    _pending_progress = progress;
    _pending_args = args;

	if (tr) {
		_transition = tr;
		_transition_time = time;
		_transition_timer = 0.0F;
	} else {
		// No transition so just finish the deferred load immediately
		load_pending_world();
		
	}
			
	_game_time.reset_abs_time();
	_game_time.delta_time();
	
}

void Application::load_pending_world	(void)
{
    LOG_MESSAGE << "Loading deferred world: " << _pending_pathname.full_path();
    
	std::shared_ptr<World> world = checked_cast<World>(Factory::create_object_from_stream(_pending_pathname, _pending_progress));
    _pending_progress.reset();
	
	// If there is a session override, use it
	if (world && _pending_session) {
		world->set_session(_pending_session);
        _pending_session.reset();
		
	// Build default session
	} else if (world && !world->session()) {
		std::shared_ptr<Session> session = checked_cast<Session>(Factory::create_object("{CLASS_SESSION}"));
		world->set_session(session);
	}
	
	_obj_pending_world = world;
    _obj_pending_world->process_args(_pending_args);
	
	// Force a tick
	if (_obj_pending_world)
		_obj_pending_world->tick(0.01F);
	
	_game_time.reset_abs_time();
	_game_time.delta_time();
	
}

const std::shared_ptr<World>& Application::pending_world		(void) const
{
    return _obj_pending_world;
}

//==============================================================================
//==============================================================================

} // DT3


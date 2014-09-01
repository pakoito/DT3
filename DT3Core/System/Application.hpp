#ifndef DT3_APPLICATION
#define DT3_APPLICATION
//==============================================================================
///	
///	File: Application.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Utility/TimerLores.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class World;
class Session;
class Progress;
class TransitionBase;
class TouchEvent;

//==============================================================================
//==============================================================================

class Application: public BaseClass {
    public:
        DEFINE_TYPE(Application,BaseClass)
		DEFINE_CREATE

                                        Application					(void);
	private:
                                        Application					(const Application &rhs);
        Application &                   operator =					(const Application &rhs);
	public:
        virtual                         ~Application				(void);

                                        
	public:
		/// Initialize the configuration system
        void                            initialize                  (void);

		/// Sets the current world
		/// \param world new current world
		void                            set_world                   (const std::shared_ptr<World> &world);

		/// Returns the current world
		/// \return Current world
		const std::shared_ptr<World>&   world                       (void) const;

		/// Transitions to a world from the current world
		/// \param world Path to world file to load
		/// \param transition std::string classname for transition
		/// \param time transition time
		/// \param session_override Session override for transferring data from one world to another
		virtual void                    transition_to_world         (const FilePath &pathname, std::string transition, DTfloat time, const std::shared_ptr<Session> &session_override, std::shared_ptr<Progress> progress, std::string args = "");

		/// Description
		/// \param param description
		/// \return description
		void                            load_pending_world          (void);

		/// Called when a world has been loaded via transitionToWorld. This won't get called when setWorld
        /// is used instead.
		/// \param world new current world
		virtual void                    world_loaded                (const std::shared_ptr<World> &world){}

		/// Description
		/// \param param description
		/// \return description
		const std::shared_ptr<World>&   pending_world               (void) const;

		/// Description
		/// \param param description
		/// \return description
		const FilePath &                current_world_path          (void) const	{	return _current_pathname;	}

		/// Description
		/// \param param description
		/// \return description
		const FilePath &                pending_world_path          (void) const     {	return _pending_pathname;	}


		/// Description
		/// \param param description
		/// \return description
        std::shared_ptr<TransitionBase> transition                  (void)          {   return _transition;         }

		/// Description
		/// \param param description
		/// \return description
        DTfloat                         transition_time             (void)          {   return _transition_time;    }

		/// Description
		/// \param param description
		/// \return description
		DTboolean                       is_transitioning            (void) const	{	return _transition != NULL;	}

		/// Description
		/// \param param description
		/// \return description
		DTfloat                         transition_progress         (void) const;
    
    
		/// Description
		/// \param param description
		/// \return description
		void                            run_event_loop              (void);
	
		/// Description
		/// \param param description
		/// \return description
		virtual void                    tick_game					(const DTfloat dt);

		/// Description
		/// \param param description
		/// \return description
		virtual void                    draw_game					(const DTfloat lag);
    
		/// Description
		/// \param param description
		/// \return description
        virtual void                    touch_gui                   (const TouchEvent *event);


		/// Description
		/// \param param description
		/// \return description
		virtual void                    clear						(void);

		/// Description
		/// \param param description
		/// \return description		
		void                            quit						(void);
		
	private:
        // Virtual function forwarding so the callbacks follow virtual inheritance rules
        void                            forward_touch_gui           (const TouchEvent *event)   {   touch_gui(event);    }
		
		DTboolean						_done;
		
		TimerLores						_game_time;
				
		DTboolean						_show_console;
		DTboolean						_show_profiler;
		
		std::shared_ptr<World>          _obj_world;
		std::shared_ptr<World>          _obj_pending_world;
		std::shared_ptr<TransitionBase> _transition;
		
		FilePath						_current_pathname;

		FilePath						_pending_pathname;
		std::shared_ptr<Session>        _pending_session;
        std::shared_ptr<Progress>       _pending_progress;
        std::string                     _pending_args;
				
		DTfloat							_transition_time;
		DTfloat							_transition_timer;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

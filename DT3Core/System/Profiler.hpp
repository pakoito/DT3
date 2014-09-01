#ifndef DT3_PROFILER
#define DT3_PROFILER
//==============================================================================
///	
///	File: Profiler.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/TimerHires.hpp"
#include "DT3Core/Types/Utility/TimerLores.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include <list>
#include <atomic>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward declarations
//==============================================================================


//==============================================================================
//==============================================================================

class ProfilerCategory {
    public:
    
        enum Classification {
            CATEGORY_PER_FRAME,
            CATEGORY_PER_TICK
        };
    
        ProfilerCategory (const DTcharacter *c, Classification classification)
            :   _category       (c),
                _classification (classification),
                _seconds        (0.0F)
        {

        }
        
		/// Description
		/// \param param description
		/// \return description
        void                clear               (void)          {   _seconds = 0.0;         }
    
		/// Description
		/// \param param description
		/// \return description
        const DTcharacter*  category            (void) const    {   return _category;       }
    
		/// Description
		/// \param param description
		/// \return description
        Classification      classification      (void) const    {   return _classification; }
    
		/// Description
		/// \param param description
		/// \return description
        DTdouble            accumulated_time    (void) const    {   return _seconds.load(); }
    
    private:
        friend class Profiler;
    
        const DTcharacter           *_category;
        Classification              _classification;
        std::atomic<DTdouble>       _seconds;
};

//==============================================================================
//==============================================================================

class ProfilerTimer {
    public:
        ProfilerTimer         (ProfilerCategory* category);
        ~ProfilerTimer        (void);
    
    private:
        friend class Profiler;
    
        ProfilerTimer               *_previous;
        ProfilerCategory            *_category;
};

//==============================================================================
//==============================================================================

class Profiler {
		/// Profiler implementation.
         
	private:
									Profiler         (void);	
									Profiler         (const Profiler &rhs);
        Profiler &                  operator =       (const Profiler &rhs);	
        virtual						~Profiler        (void);
    
    public:
        
		/// Description
		/// \param param description
		/// \return description
        static void                 display_to_console  (void);
    
		/// Description
		/// \param param description
		/// \return description
        static void                 mark_frame          (void);

		/// Description
		/// \param param description
		/// \return description
        static void                 mark_tick           (void);
    
    private:
        friend class ProfilerTimer;

        static void                 push_timer          (ProfilerCategory* category, ProfilerTimer *timer);
        static void                 pop_timer           (void);

    private:
        DT3_THREAD_LOCAL static ProfilerTimer   *_timer_stack;
    
        static DTuint                           _num_ticks;
        static DTuint                           _num_frames;
        static TimerLores                       _display_timer;
        static TimerHires                       _profiler_timer;
};

//==============================================================================
//==============================================================================

#if DT3_USE_PROFILER    

    extern std::list<ProfilerCategory*>&  profiler_categories (void);
    
    #define DEFINE_PROFILER_CATEGORY(C)                                         \
        extern ProfilerCategory gCategory_##C;
    
    
    #define IMPLEMENT_PROFILER_CATEGORY(C,N,CL)                                 \
        ProfilerCategory gCategory_##C(N,CL);                                   \
        namespace {                                                             \
            struct registration_class##C: public StaticInitializerCallback {    \
                void initialize() {                                             \
                    DT3::profiler_categories().push_back(&gCategory_##C);      \
                }                                                               \
            } registration_obj##C;                                              \
        }


	#define PROFILER(C)                                                         \
        ProfilerTimer __profiler_timer(&gCategory_##C);
        
#else
    #define DEFINE_PROFILER_CATEGORY(C)        
    #define IMPLEMENT_PROFILER_CATEGORY(C,N,CL)
	#define PROFILER(C)	
#endif

//==============================================================================
//==============================================================================

DEFINE_PROFILER_CATEGORY(SOUND)
DEFINE_PROFILER_CATEGORY(GRAPHICS)
DEFINE_PROFILER_CATEGORY(SCRIPTING)
DEFINE_PROFILER_CATEGORY(PARTICLES)
DEFINE_PROFILER_CATEGORY(MAIN_LOOP)
DEFINE_PROFILER_CATEGORY(TICK)
DEFINE_PROFILER_CATEGORY(DRAW)
DEFINE_PROFILER_CATEGORY(DRAW_DIAGNOSTICS)
DEFINE_PROFILER_CATEGORY(PHYSICS)
DEFINE_PROFILER_CATEGORY(COLLISION)

//==============================================================================
//==============================================================================

} // DT3

#endif

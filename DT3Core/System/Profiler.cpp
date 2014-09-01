//==============================================================================
///	
///	File: Profiler.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_PROFILER_CATEGORY(SOUND,"Sound",ProfilerCategory::CATEGORY_PER_TICK)
IMPLEMENT_PROFILER_CATEGORY(GRAPHICS,"Graphics",ProfilerCategory::CATEGORY_PER_FRAME)
IMPLEMENT_PROFILER_CATEGORY(SCRIPTING,"Scripting",ProfilerCategory::CATEGORY_PER_TICK)
IMPLEMENT_PROFILER_CATEGORY(PARTICLES,"Particles",ProfilerCategory::CATEGORY_PER_FRAME)
IMPLEMENT_PROFILER_CATEGORY(MAIN_LOOP,"MainLoop",ProfilerCategory::CATEGORY_PER_TICK)
IMPLEMENT_PROFILER_CATEGORY(TICK,"Tick",ProfilerCategory::CATEGORY_PER_TICK)
IMPLEMENT_PROFILER_CATEGORY(DRAW,"Draw",ProfilerCategory::CATEGORY_PER_FRAME)
IMPLEMENT_PROFILER_CATEGORY(DRAW_DIAGNOSTICS,"Draw Diagnostics",ProfilerCategory::CATEGORY_PER_FRAME)
IMPLEMENT_PROFILER_CATEGORY(PHYSICS,"Physics",ProfilerCategory::CATEGORY_PER_TICK)
IMPLEMENT_PROFILER_CATEGORY(COLLISION,"Collision",ProfilerCategory::CATEGORY_PER_TICK)

//==============================================================================
//==============================================================================
    
DT3_THREAD_LOCAL ProfilerTimer  *Profiler::_timer_stack = NULL;
DTuint                          Profiler::_num_ticks = 0;
DTuint                          Profiler::_num_frames = 0;
TimerLores                      Profiler::_display_timer;
TimerHires                      Profiler::_profiler_timer;

//==============================================================================
//==============================================================================

std::list<ProfilerCategory*>&  profiler_categories (void)
{
    static std::list<ProfilerCategory*> categories;
    return categories;
}

//==============================================================================
//==============================================================================

ProfilerTimer::ProfilerTimer (ProfilerCategory* category)
{
    ASSERT(category);

    Profiler::push_timer(category, this);
}

ProfilerTimer::~ProfilerTimer (void)
{
    Profiler::pop_timer();
}

//==============================================================================
//==============================================================================

void Profiler::display_to_console (void)
{
#if DT3_USE_PROFILER    

    // only display once every 5 seconds
    if (_display_timer.abs_time() < 5.0F)
        return;
    _display_timer.reset_abs_time();

    LOG_MESSAGE << "Profiler";
    
    // Display all categories
    std::list<ProfilerCategory*>&  categories = profiler_categories();
    
    // Per Tick categories
    if (_num_ticks) {
        LOG_MESSAGE << " Per Tick Categories";
        
        for (auto i = categories.begin(); i != categories.end(); ++i) {
            ProfilerCategory *category = *i;
            
            if ( category->classification() == ProfilerCategory::CATEGORY_PER_TICK) {
                LOG_MESSAGE << "  " << category->category() << ":  " << category->accumulated_time()/_num_ticks*1000.0F << " ms/tick";
                category->clear();
            }
        }
    }
    
    if (_num_frames) {
        // Per Draw categories
        LOG_MESSAGE << " Per Draw Categories";

        for (auto i = categories.begin(); i != categories.end(); ++i) {
            ProfilerCategory *category = *i;

            if (category->classification() == ProfilerCategory::CATEGORY_PER_FRAME) {
                LOG_MESSAGE << "  " << category->category() << ":  " << category->accumulated_time()/_num_frames*1000.0F << " ms/frame";
                category->clear();
            }
        }
    }
    
    // Reset tick and frame counter
    _num_ticks = 0;
    _num_frames = 0;
    
#endif
}
    
void Profiler::mark_frame (void)
{
    ++_num_frames;
}

void Profiler::mark_tick (void)
{
    ++_num_ticks;
}

//==============================================================================
//==============================================================================

void Profiler::push_timer (ProfilerCategory* category, ProfilerTimer *timer)
{
    ASSERT(category);
    
    // Empty timer into previous active timer
    if (_timer_stack) {
        DTdouble expected, desired;
        do {
            expected = _timer_stack->_category->_seconds.load();
            desired = expected + _profiler_timer.abs_time();
        } while (!_timer_stack->_category->_seconds.compare_exchange_weak(expected, desired));
    }

    
    // Set the category
    timer->_category = category;
    
    // Add to stack
    timer->_previous = _timer_stack;
    _timer_stack = timer;
    
    // Reset the timer
    _profiler_timer.reset_abs_time();
}

void Profiler::pop_timer (void)
{
    ASSERT(_timer_stack->_category);

    // Empty timer into this active timer
    DTdouble expected, desired;
    do {
        expected = _timer_stack->_category->_seconds.load();
        desired = expected + _profiler_timer.abs_time();
    } while (!_timer_stack->_category->_seconds.compare_exchange_weak(expected, desired));
    
    // Pop from stack
    _timer_stack = _timer_stack->_previous;
    
    // Reset the timer
    _profiler_timer.reset_abs_time();
}

//==============================================================================
//==============================================================================

} // DT3

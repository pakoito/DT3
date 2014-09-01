//==============================================================================
///	
///	File: ThreadMainTaskQueue.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Threads/ThreadMainTaskQueue.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTuint                                  ThreadMainTaskQueue::_loop_counter = 0;
std::mutex                              ThreadMainTaskQueue::_queue_lock;
std::list<ThreadMainTaskQueue::Task>    ThreadMainTaskQueue::_queue;

//==============================================================================
//==============================================================================

std::shared_ptr<ThreadTaskRef> ThreadMainTaskQueue::add_task (std::shared_ptr<LatentCall> lc, DTfloat priority, DTfloat delay)
{
    std::shared_ptr<ThreadTaskRef> ref(ThreadTaskRef::create());

    Task t;
    t._ref = ref;
    t._priority = priority;
    t._delay = delay;
    t._lc = lc;
    t._loop_counter = _loop_counter;
    
    // Add the task
    std::unique_lock<std::mutex> lock(_queue_lock);
    
    // Set status to queued
    ref->set_status(ThreadTaskRef::STATUS_QUEUED);

    _queue.push_back(t);
    

    return ref;
}

void ThreadMainTaskQueue::remove_task (const std::shared_ptr<ThreadTaskRef> &ref)
{
    std::unique_lock<std::mutex> lock(_queue_lock);
        
    auto e = std::remove_if(_queue.begin(), _queue.end(), [&ref](Task &t) { return t._ref == ref; } );
    _queue.erase(e, _queue.end());
}
        
void ThreadMainTaskQueue::remove_all_tasks (void)
{
    std::unique_lock<std::mutex> lock(_queue_lock);
    _queue.clear();
}

DTsize ThreadMainTaskQueue::count_tasks (void)
{
    return _queue.size();
}

void ThreadMainTaskQueue::run_queue (const DTfloat dt)
{
    ++_loop_counter;

    while (1) {
        std::unique_lock<std::mutex> lock(_queue_lock);
        
        std::shared_ptr<LatentCall> lc;
        std::shared_ptr<ThreadTaskRef> ref;

        if (ThreadMainTaskQueue::_queue.size() > 0) {
        
            std::list<ThreadMainTaskQueue::Task>::iterator j = ThreadMainTaskQueue::_queue.end();
            DTfloat max_priorty = -std::numeric_limits<DTfloat>::infinity();
            
            FOR_EACH (i,ThreadMainTaskQueue::_queue) {
                i->_delay -= dt;
            
                if ( (i->_loop_counter < _loop_counter) && (i->_delay <= 0.0F) && (i->_priority > max_priorty) ) {
                    j = i;
                    max_priorty = i->_priority;
                }
            }
        
            if (j != ThreadMainTaskQueue::_queue.end()) {
                lc = j->_lc;
                ref = j->_ref;
                ThreadMainTaskQueue::_queue.erase(j);
            }
        }
        
        lock.unlock();

        // Check if done
        if (lc) {
            // Do callback
            ref->set_status(ThreadTaskRef::STATUS_RUNNING);
            lc->fire();
            ref->set_status(ThreadTaskRef::STATUS_COMPLETE);
        } else {
            break;
        }
    }


}

//==============================================================================
//==============================================================================

} // DT3

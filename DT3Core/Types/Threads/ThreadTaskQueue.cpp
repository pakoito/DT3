//==============================================================================
///	
///	File: ThreadTaskQueue.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Threads/ThreadTaskQueue.hpp"
#include "DT3Core/Types/Utility/TimerHires.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::mutex                                              ThreadTaskQueue::_queue_lock;
std::list<ThreadTaskQueue::Task>                        ThreadTaskQueue::_queue;
DTint                                                   ThreadTaskQueue::_max_num_threads = 1;
std::list<std::shared_ptr<ThreadTaskQueue::Helper>>    ThreadTaskQueue::_threads;

//==============================================================================
//==============================================================================

std::shared_ptr<ThreadTaskRef> ThreadTaskQueue::add_task (std::shared_ptr<LatentCall> lc, DTfloat priority, DTfloat delay)
{
    std::shared_ptr<ThreadTaskRef> ref(ThreadTaskRef::create());
    
    Task t;
    t._priority = priority;
    t._delay = delay;
    t._lc = lc;
    t._ref = ref;
    
    // Status is now queued
    ref->set_status(ThreadTaskRef::STATUS_QUEUED);

    // Add the task
    std::unique_lock<std::mutex> lock(_queue_lock);
    _queue.push_back(t);
    lock.unlock();
    
    // Bump start threads
    run_queue();
    
    return ref;
}

void ThreadTaskQueue::remove_task (const std::shared_ptr<ThreadTaskRef> &ref)
{
    std::unique_lock<std::mutex> lock(_queue_lock);
    
    auto e = std::remove_if(_queue.begin(), _queue.end(),
        [&ref](Task &t) {
            t._ref->set_status(ThreadTaskRef::STATUS_CANCELLED);
            return t._ref == ref;
        }
    );
    _queue.erase(e, _queue.end());

}
    
void ThreadTaskQueue::remove_all_tasks (void)
{
    std::unique_lock<std::mutex> lock(_queue_lock);
    
    for_each(_queue.begin(), _queue.end(),
        [](Task &t) {
            t._ref->set_status(ThreadTaskRef::STATUS_CANCELLED);
        }
    );
    
    _queue.clear();
}

void ThreadTaskQueue::set_num_threads (DTuint n)
{
    // Join old threads
    std::for_each(_threads.begin(), _threads.end(),
        [](std::shared_ptr<Helper> &h) {
            h->_helper_done = true;
            h->_helper_thread.join();
        }
    );

    // Start new threads
    _threads.clear();
    _threads.resize(n);
    
    std::for_each(_threads.begin(), _threads.end(),
        [](std::shared_ptr<Helper> &h) {
            h = std::shared_ptr<Helper>(new Helper());
            h->_helper_done = false;
            h->_helper_thread = std::thread(std::bind(ThreadTaskQueue::do_thread, h));
        }
    );

}

DTsize ThreadTaskQueue::count_tasks (void)
{
    return _queue.size();
}

void ThreadTaskQueue::run_queue (void)
{
    _queue_lock.lock();
    DTsize queue_size = _queue.size();
    _queue_lock.unlock();

    if (queue_size > 0) {
        std::for_each(_threads.begin(), _threads.end(),
            [](std::shared_ptr<Helper> &h) {
                resume(h);
            }
        );
    }
    
}

//==============================================================================
//==============================================================================

void ThreadTaskQueue::suspend (std::shared_ptr<Helper> &h)
{
    std::unique_lock<std::mutex> lock(h->_helper_lock);
    h->_helper_cond.wait(lock);
}

void ThreadTaskQueue::resume (std::shared_ptr<Helper> &h)
{
    std::unique_lock<std::mutex> lock(h->_helper_lock);
    h->_helper_cond.notify_all();
}

//==============================================================================
//==============================================================================

void ThreadTaskQueue::do_thread (std::shared_ptr<Helper> &h)
{
    TimerHires timer;
    
    while (!h->_helper_done) {
        std::unique_lock<std::mutex> lock(_queue_lock);

        std::shared_ptr<LatentCall> lc;
        std::shared_ptr<ThreadTaskRef> ref;

        if (ThreadTaskQueue::_queue.size() > 0) {
        
            std::list<ThreadTaskQueue::Task>::iterator j = ThreadTaskQueue::_queue.end();
            DTfloat max_priorty = -std::numeric_limits<DTfloat>::infinity();
            
            FOR_EACH (i,ThreadTaskQueue::_queue) {
                DTfloat dt = static_cast<DTfloat>(timer.delta_time());
                if (dt > DT3_MAX_TICK)
                    dt = DT3_MAX_TICK;

                i->_delay -= dt;
            
                if ( (i->_delay <= 0.0F) && (i->_priority > max_priorty) ) {
                    j = i;
                    max_priorty = i->_priority;
                }
            }
        
            if (j != ThreadTaskQueue::_queue.end()) {
                lc = j->_lc;
                ref = j->_ref;
                ThreadTaskQueue::_queue.erase(j);
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
            suspend(h);
        }
    }
}

//==============================================================================
//==============================================================================

} // DT3

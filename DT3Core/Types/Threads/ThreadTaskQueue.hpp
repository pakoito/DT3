#ifndef DT3_THREADTASKQUEUE
#define DT3_THREADTASKQUEUE
//==============================================================================
///	
///	File: ThreadTaskQueue.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/LatentCall.hpp"
#include "DT3Core/Types/Threads/ThreadTaskRef.hpp"
#include <memory>
#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class ThreadTaskQueue {
    public:
        DEFINE_TYPE_BASE(ThreadTaskQueue)

	private:
                                                    ThreadTaskQueue (void);
                                                    ThreadTaskQueue (const ThreadTaskQueue &rhs);
        ThreadTaskQueue &                           operator =      (const ThreadTaskQueue &rhs);
        virtual                                     ~ThreadTaskQueue(void);

	public:
		/// Add a task to the task queue
		/// \param lc Latent Call
		/// \param priority Priority of task. higher = sooner
		/// \return Reference to task
		static std::shared_ptr<ThreadTaskRef>       add_task        (std::shared_ptr<LatentCall> lc, DTfloat priority = 1.0F, DTfloat delay = 0.0F);

		/// Remove a task from the queue
		/// \param ref Reference to task to remove
        static void                                 remove_task     (const std::shared_ptr<ThreadTaskRef> &ref);
    
		/// Removes all the tasks from the queue
        static void                                 remove_all_tasks(void);

		/// Sets the number of threads working on the queue
		/// \param n number of threads
        static void                                 set_num_threads (DTuint n);
    
		/// Counts all of the tasks in the queue
		/// \return number of tasks
        static DTsize                               count_tasks     (void);
    
		/// Resumes the next thread in the list of threads
        static void                                 run_queue       (void);

    private:
    
        struct Task {        
            DTuint                                  _id;
            DTfloat                                 _priority;
            DTfloat                                 _delay;
            std::shared_ptr<ThreadTaskRef>          _ref;
            std::shared_ptr<LatentCall>             _lc;
        };
        
        static std::mutex                           _queue_lock;
        static std::list<Task>                      _queue;
    
    
        struct Helper {
            DTboolean                               _helper_done;
            std::thread                             _helper_thread;
            std::mutex                              _helper_lock;
            std::condition_variable                 _helper_cond;
        };
    
        static void                                 do_thread       (std::shared_ptr<Helper> &h);
        static void                                 suspend         (std::shared_ptr<Helper> &h);
        static void                                 resume          (std::shared_ptr<Helper> &h);

        static std::list<std::shared_ptr<Helper>>  _threads;
        static DTint                                _max_num_threads;
};

//==============================================================================
//==============================================================================


} // DT3

#endif

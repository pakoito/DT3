#ifndef DT3_THREADMAINTASKQUEUE
#define DT3_THREADMAINTASKQUEUE
//==============================================================================
///	
///	File: ThreadMainTaskQueue.hpp
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

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class ThreadMainTaskQueue {
    public:
        DEFINE_TYPE_BASE(ThreadMainTaskQueue)

	private:
                                                ThreadMainTaskQueue		(void);
                                                ThreadMainTaskQueue		(const ThreadMainTaskQueue &rhs);
        ThreadMainTaskQueue &                   operator =              (const ThreadMainTaskQueue &rhs);
        virtual                                 ~ThreadMainTaskQueue	(void);

	public:
		/// Add a task to the task queue
		/// \param lc Latent Call
		/// \param priority Priority of task. higher = sooner
		/// \return Reference to task
		static std::shared_ptr<ThreadTaskRef>   add_task                (std::shared_ptr<LatentCall> lc, DTfloat priority = 1.0F, DTfloat delay = 0.0F);

		/// Remove a task from the queue
		/// \param ref Reference to task to remove
        static void                             remove_task             (const std::shared_ptr<ThreadTaskRef> &ref);
        
		/// Removes all the tasks from the queue
        static void                             remove_all_tasks        (void);
    
		/// Counts all of the tasks in the queue
		/// \return number of tasks
        static DTsize                           count_tasks             (void);

		/// Runs the queue to completion
        static void                             run_queue               (const DTfloat dt);

    private:
        friend class ThreadTaskThread;
        
        struct Task {
            std::shared_ptr<ThreadTaskRef>      _ref;
            DTfloat                             _priority;
            DTfloat                             _delay;
            std::shared_ptr<LatentCall>         _lc;
            DTuint                              _loop_counter;   // For tracking newly added tasks
        };
    
        static DTuint                           _loop_counter;
        
        static std::mutex                       _queue_lock;
        static std::list<Task>                  _queue;
        
};

//==============================================================================
//==============================================================================


} // DT3

#endif

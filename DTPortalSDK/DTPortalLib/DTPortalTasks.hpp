#ifndef DTPORTAL_TASKS
#define DTPORTAL_TASKS
//==============================================================================
///	
///	File: DTPortalTasks.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalSDK.hpp"
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
//==============================================================================

template<class T>
class DTPortalSmartPtr {
    public:
                                    DTPortalSmartPtr    (void)                                              {   _pointee = NULL;                    }
                                    DTPortalSmartPtr    (const DTPortalSmartPtr<T> &rhs);
		explicit                    DTPortalSmartPtr    (T *rhs);
        DTPortalSmartPtr<T> &       operator =          (const DTPortalSmartPtr<T> &rhs);
                                    ~DTPortalSmartPtr   (void);
        
    public:
        T*							operator->			(void) const                                        {	return _pointee;					}
        T&							operator *			(void) const                                        {	return *_pointee;					}
    
        T*                          get                 (void) const                                        {   return _pointee;                    }
        void                        reset               (void)                                              {   if (_pointee)   _pointee->release();    _pointee = NULL;    }

//		inline friend DTboolean		operator ==			(const std::shared_ptr<T> &lhs, const T* rhs)              {	return lhs._pointee == rhs;			}
//		inline friend DTboolean		operator ==			(const T* lhs, const std::shared_ptr<T> &rhs)              {	return lhs == rhs._pointee;			}
//		inline friend DTboolean		operator ==			(const std::shared_ptr<T> &lhs, const std::shared_ptr<T> &rhs)	{	return lhs._pointee == rhs._pointee;}
//
//		inline friend DTboolean		operator !=			(const std::shared_ptr<T> &lhs, const T* rhs)              {	return lhs._pointee != rhs;			}
//		inline friend DTboolean		operator !=			(const T* lhs, const std::shared_ptr<T> &rhs)              {	return lhs != rhs._pointee;			}
//		inline friend DTboolean		operator !=			(const std::shared_ptr<T> &lhs, const std::shared_ptr<T> &rhs)	{	return lhs._pointee != rhs._pointee;}
	
	private:
		T*                          _pointee;
};

template<class T>
DTPortalSmartPtr<T>::DTPortalSmartPtr (const DTPortalSmartPtr<T> &rhs)
{
	_pointee = rhs._pointee;
    if (_pointee)   _pointee->retain();
}

template<class T>
DTPortalSmartPtr<T>::DTPortalSmartPtr (T *rhs)
{
	_pointee = rhs;
}

template<class T>
DTPortalSmartPtr<T>& DTPortalSmartPtr<T>::operator = (const DTPortalSmartPtr<T> &rhs)
{
    if (_pointee)   _pointee->release();
    _pointee = rhs._pointee;
    if (_pointee)   _pointee->retain();
	return (*this);
}

template<class T>
DTPortalSmartPtr<T>::~DTPortalSmartPtr (void)
{
    if (_pointee) {
        _pointee->release();
    }
}

//==============================================================================
/// Base class for all of the background tasks.
//==============================================================================

class DTPortalTask {
    
    public:
                                    DTPortalTask            (void);

    private:
									DTPortalTask            (const DTPortalTask &rhs);
        DTPortalTask &              operator =              (const DTPortalTask &rhs);
    public:
    
        virtual                     ~DTPortalTask           (void);
    
    public:
    
        /// Increments the reference count for task
        void                        retain                  (void);

        /// Decrements the reference count for task and frees if 0
        void                        release                 (void);

        /// Returns the name for serialization for the task
        /// \return Name for serialization
        virtual std::string         task_name               (void) const = 0;
    
        /// Serializes the object to a stream
        /// \param ss Stream for serialized data
        /// \return was serialized or not
        virtual DTPboolean          serialize_out           (std::string &s) {  return false;   }

        /// Serializes the object from a stream
        /// \param ss Stream for serialized data
        virtual DTPboolean          serialize_in            (std::string s)  {  return false;   }
    
    
        /// Runs the task. This is where the web server communication would happen
        /// and the results would be stored in this object for later. This happens
        // in a separate thread.
        /// \return Error code
        virtual DTPerror            run_task                (void) = 0;

        /// Pumps the results. Causes any callbacks to be fired. This would happen
        /// in the main thread after the task is finished running.
        virtual void                pump_results            (void) = 0;

        /// Saves the task to a file for later processing. All of the tasks that
        /// have not been processed or have errored out get saved to try
        /// again later.
        virtual void                save                    (std::ofstream &outfile) {};
    
    
        /// Delay running the task again
        void                        delay                   (long delay);
    
        /// Check if we're ready to run this task
        bool                        is_delay_done           (void) const;
    
    private:
        DTPuint                     _refcount;
        long                        _expire_time;
    
};

//==============================================================================
/// Task factory
//==============================================================================

class DTPortalTaskFactory {
		/// Base object for the different component types in the engine.

    private:
                                    DTPortalTaskFactory     (void);	
									DTPortalTaskFactory     (const DTPortalTaskFactory &rhs);
        DTPortalTaskFactory &       operator =              (const DTPortalTaskFactory &rhs);	
        virtual                     ~DTPortalTaskFactory    (void);
    		
	public:

        /// Creates a task by a task name string
        /// \return task or NULL on error
        static DTPortalTask*        create_task             (const std::string &task_name);
    
        /// Registers a task creator by a task name string
        /// \param task_name name of task
        /// \param task_creator task creator function        
        static void                 register_task_creator   (const std::string &task_name, DTPortalTask* (*task_creator)(void));

    private:
        static std::map<std::string, DTPortalTask* (*)(void)>&  task_factory_map (void);

};

#define REGISTER_TASK(T)                                                            \
struct reg_##T {                                                                     \
    static DTPortalTask* create (void) {    return new T(); }                       \
    reg_##T(void)    {   DTPortalTaskFactory::register_task_creator(#T, &create);  }   \
} g_reg_##T;                                                                 

//==============================================================================
/// Tasks queue implementation
//==============================================================================

class DTPortalTasks {
		/// Base object for the different component types in the engine.

    private:
                                    DTPortalTasks           (void);	
									DTPortalTasks           (const DTPortalTasks &rhs);
        DTPortalTasks &             operator =              (const DTPortalTasks &rhs);	
        virtual                     ~DTPortalTasks          (void);
    		
	public:

        /// Initializes the task queue. 
        static void                 initialize              (void);
    
        /// Uninitializes the task queue. 
        static void                 uninitialize            (void);
    
        /// Adds a task to the pending task queue
        /// \param task Task to add to the queue
        static void                 queueTask               (DTPortalSmartPtr<DTPortalTask> task);
    
        /// Causes all of the completed tasks to report their results
        static void                 pump_results            (void);
    
    private:
        static void                 suspend_tasks           (void);
        static void                 resume_tasks            (void);
    
        static void                 consume_queue           (void);

        static std::list<DTPortalSmartPtr<DTPortalTask>>    _pending_queue;
        static std::list<DTPortalSmartPtr<DTPortalTask>>    _complete_queue;
    
        static DTPboolean               _done;
    
        static std::thread              _thread;
        static std::mutex               _thread_mutex;
        static std::condition_variable  _thread_cond;

        static std::mutex               _queue_mutex;
};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


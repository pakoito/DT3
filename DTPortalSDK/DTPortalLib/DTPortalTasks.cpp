//==============================================================================
///	
///	File: DTPortalTasks.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalTasks.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"
#include <fstream>
#include <unistd.h>
#include <cassert>

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
//==============================================================================

DTPortalTask::DTPortalTask (void)
    :   _expire_time    (0),
        _refcount       (1)
{

}

DTPortalTask::~DTPortalTask (void)
{

}

void DTPortalTask::retain (void)
{
    ++_refcount;
}

void DTPortalTask::release (void)
{
    --_refcount;
    if (_refcount == 0)
        delete this;
}

void DTPortalTask::delay (long delay)
{
    _expire_time = ::time(NULL) + delay;
}

bool DTPortalTask::is_delay_done (void) const
{
    return ::time(NULL) > _expire_time;
}

//==============================================================================
//==============================================================================

DTPortalTask* DTPortalTaskFactory::create_task (const std::string &task_name)
{
    std::map<std::string, DTPortalTask* (*)(void)> &task_map = task_factory_map();
    auto i = task_map.find(task_name);
    if (i == task_map.end())
        return NULL;
    
    return (i->second)();
}
    
void DTPortalTaskFactory::register_task_creator (const std::string &task_name, DTPortalTask* (*task_creator)(void))
{
    std::map<std::string, DTPortalTask* (*)(void)> &task_map = task_factory_map();
    task_map[task_name] = task_creator;
}

std::map<std::string, DTPortalTask* (*)(void)>& DTPortalTaskFactory::task_factory_map (void)
{
    static std::map<std::string, DTPortalTask* (*)(void)> task_map;
    return task_map;
}

//==============================================================================
//==============================================================================

std::list<DTPortalSmartPtr<DTPortalTask>> DTPortalTasks::_pending_queue;
std::list<DTPortalSmartPtr<DTPortalTask>> DTPortalTasks::_complete_queue;

DTPboolean DTPortalTasks::_done = false;

std::thread             DTPortalTasks::_thread;
std::mutex              DTPortalTasks::_thread_mutex;
std::condition_variable DTPortalTasks::_thread_cond;
std::mutex              DTPortalTasks::_queue_mutex;

//==============================================================================
//==============================================================================

void DTPortalTasks::initialize (void)
{
    // Serialize in the unprocessed tasks
    std::string path = DTPortalCommon::save_dir() + "/" + "serialized.txt";
    
    std::ifstream infile (path.c_str());
    
    std::stringstream ss;
    ss << infile.rdbuf();
    infile.close();

    // Delete the original file
    ::unlink(path.c_str());

    while (1) {
        
        std::string task_name;
        unsigned int task_data_size = 0;
        
        ss >> task_name;
        ss >> task_data_size;
        
        if (ss.eof() || task_data_size == 0)
            break;
        
        std::string data;
        data.resize(task_data_size);
        
        ss.read(&data[0], task_data_size);
        
        DTPortalSmartPtr<DTPortalTask> task(DTPortalTaskFactory::create_task(task_name));
        if ( task.get() == NULL)
            break;
        
        DTPboolean success = task->serialize_in(data);
        if (!success)
            break;
        
        _pending_queue.push_back(task);
        
    }
    
    _thread = std::thread(DTPortalTasks::consume_queue);
}
    
void DTPortalTasks::uninitialize (void)
{
    _done = true;
    
    _thread.join();

    // Pump any final results
    pump_results();
    
    // Serialize out the unprocessed tasks
    std::string path = DTPortalCommon::save_dir() + "/" + "serialized.txt";
    std::ofstream outfile (path.c_str());
    
    std::list<DTPortalSmartPtr<DTPortalTask>>::iterator i;
    for (i = _pending_queue.begin(); i != _pending_queue.end(); ++i) {
        
        std::string task_name = (**i).task_name();
        
        std::string task_data;
        DTPboolean success = (**i).serialize_out(task_data);
        
        if (success) {
            outfile << task_name << " " << task_data.size() << std::endl;
            outfile << task_data << std::endl;
        }
        
    }
    
    _pending_queue.clear();
    _complete_queue.clear();
}
    
void DTPortalTasks::queueTask (DTPortalSmartPtr<DTPortalTask> task)
{
    _queue_mutex.lock();
    _pending_queue.push_back(task);
    _queue_mutex.unlock();

    resume_tasks();
}

//==============================================================================
//==============================================================================

void DTPortalTasks::suspend_tasks (void)
{
    std::unique_lock<std::mutex> lock(_thread_mutex);
    _thread_cond.wait(lock);
}

void DTPortalTasks::resume_tasks (void)
{
    std::unique_lock<std::mutex> lock(_thread_mutex);
    _thread_cond.notify_all();
}

//==============================================================================
//==============================================================================

void DTPortalTasks::consume_queue (void)
{
    while (!_done) {
    
        _queue_mutex.lock();
        size_t queue_size = _pending_queue.size();
        _queue_mutex.unlock();

        // Suspend the thread if there are no pending tasks
        if (queue_size == 0) {
            suspend_tasks();
            continue;   // Start the loop over
        }

        // Pop the next task off of the top of the queue
        _queue_mutex.lock();

        DTPortalSmartPtr<DTPortalTask> task = _pending_queue.front();
        _pending_queue.pop_front();
        _queue_mutex.unlock();
        
        // Run the task
        DTPerror error = -1;    // -1 signifies that task was not run
        if (task->is_delay_done()) {
            error = task->run_task();
        }
        
        // Add the task to the error queue
        _queue_mutex.lock();

        // Process result
        if (error == -1) {
            _pending_queue.push_back(task);
            
        // Error happened
        } else if (error != DTP_NO_ERROR) {     // Includes DTP_NO_ERROR_BUT_RUN_TASK_AGAIN
            task->delay(60*10); // 10 min
            _pending_queue.push_back(task);
            
        // No error
        } else {
            assert(task.get());
            _complete_queue.push_back(task);
        }
                
        _queue_mutex.unlock();
        
        ::usleep(100*1000);
    }
    
}

//==============================================================================
//==============================================================================

void DTPortalTasks::pump_results (void)
{
    _queue_mutex.lock();
    
    while (_complete_queue.size()) {
        DTPortalSmartPtr<DTPortalTask> task = _complete_queue.front();
        task->pump_results();
        _complete_queue.pop_front();
    }
    
    _complete_queue.clear();
    
    _queue_mutex.unlock();
}

//==============================================================================
//==============================================================================

} // DTPortal

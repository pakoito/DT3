//==============================================================================
///	
///	File: DTPortalAnalytics.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalAnalytics.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalTasks.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalNetwork.hpp"
#include <iostream>
#include <sstream>
#include <ctime>
#include <cassert>

extern "C" {
    #include "jansson.h"
}

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
//==============================================================================

DTPortalSmartPtr<DTPortalTask>    DTPortalAnalytics::_task;

//==============================================================================
// Submit high scores task
//==============================================================================

class SubmitAnalyticsTask : public DTPortalTask
{
    public:
                    SubmitAnalyticsTask            (void);
        virtual     ~SubmitAnalyticsTask           (void);
    
    public:
        /// Returns the name for serialization for the task
        /// \return Name for serialization
        virtual std::string         task_name               (void) const            {   return "SubmitAnalyticsTask";  }
    
        /// Serializes the object to a stream
        /// \param ss Stream for serialized data
        /// \return was serialized or not
        virtual DTPboolean          serialize_out           (std::string &s);

        /// Serializes the object from a stream
        /// \param ss Stream for serialized data
        virtual DTPboolean          serialize_in            (std::string s);

        /// Runs the task. This is where the web server communication would happen
        /// and the results would be stored in this object for later. This happens
        // in a separate thread.
        /// \return Error code
        virtual DTPerror            run_task                (void);

        /// Pumps the results. Causes any callbacks to be fired. This would happen
        /// in the main thread after the task is finished running.
        virtual void                pump_results            (void) {}

        // Append a new event to the list of events that will be submitted
        void                        append_event            (const std::string &context, const std::string &key, const std::string &value);
    
    private:
        struct Event {
            std::string     context;
            std::string     key;
            std::string     value;
            DTPuint         timestamp;
        };
        std::list<Event>    _events;
        std::mutex          _events_mutex;
};


SubmitAnalyticsTask::SubmitAnalyticsTask (void)
{

}

SubmitAnalyticsTask::~SubmitAnalyticsTask (void)
{

}


DTPboolean SubmitAnalyticsTask::serialize_out (std::string &s)
{
    json_t *root = ::json_array();
    
    std::list<Event>::iterator i;
    
    for (i = _events.begin(); i != _events.end(); ++i) {
    
        json_t *element = ::json_object();
    
        ::json_object_set_new(element, "context", json_string(i->context.c_str()) );
        ::json_object_set_new(element, "key", json_string(i->key.c_str()) );
        ::json_object_set_new(element, "value", json_string(i->value.c_str()) );
        ::json_object_set_new(element, "timestamp", json_integer(i->timestamp) );

        ::json_array_append_new(root, element);
    }
    
    s = ::json_dumps(root, JSON_COMPACT);

    return true;
}

DTPboolean SubmitAnalyticsTask::serialize_in (std::string s)
{
    json_error_t error;
    json_t *root = ::json_loads(s.c_str(), 0, &error);

    if (!root)
        return false;

    if (!json_is_array(root)) {
        ::json_decref(root);
        return false;
    }
    
    for(int i = 0; i < json_array_size(root); ++i) {
    
        json_t *element = ::json_array_get(root, i);
        if (!json_is_object(element)) {
            ::json_decref(root);
            return false;
        }
    
        json_t *context = ::json_object_get(root, "context");
        if (!json_is_string(context)) {
            ::json_decref(root);
            return false;
        }

        json_t *key = ::json_object_get(root, "key");
        if (!json_is_string(key)) {
            ::json_decref(root);
            return false;
        }

        json_t *value = ::json_object_get(root, "value");
        if (!json_is_string(value)) {
            ::json_decref(root);
            return false;
        }

        json_t *timestamp = ::json_object_get(root, "timestamp");
        if (!json_is_integer(timestamp)) {
            ::json_decref(root);
            return false;
        }
        
        Event e;
        e.context = ::json_string_value(context);
        e.key = ::json_string_value(key);
        e.value = ::json_string_value(value);
        e.timestamp = (DTPuint) ::json_integer_value(timestamp);

        _events.push_back(e);
    
    }
    
    return true;
}

DTPerror SubmitAnalyticsTask::run_task (void)
{
    std::string response;
    std::string header_and_contents;
    std::string url;
    
    //
    // Fill out the JSON request data
    //
    
    // Make a copy of the current list of events
    _events_mutex.lock();
    std::list<Event> events(_events);
    _events_mutex.unlock();
    
    // Check if we have nothing to report
    if (events.size() == 0)
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    
    
    // Build JSON data

    json_t *request_data = ::json_array();
    
    std::list<Event>::iterator i;
    
    for (i = _events.begin(); i != _events.end(); ++i) {
    
        json_t *element = ::json_object();
    
        ::json_object_set_new(element, "context", json_string(i->context.c_str()) );
        ::json_object_set_new(element, "key", json_string(i->key.c_str()) );
        ::json_object_set_new(element, "value", json_string(i->value.c_str()) );
        ::json_object_set_new(element, "timestamp", json_integer(i->timestamp) );

        ::json_array_append_new(request_data, element);
    }
    
    std::string request_data_string = ::json_dumps(request_data, JSON_COMPACT);

    ::json_decref(request_data);
    
    std::string request_data_string_checksum;
    std::string request_data_string_encoded;
    
    DTPortalNetwork::encode (request_data_string, request_data_string_checksum, request_data_string_encoded);

    //
    // Fill out the JSON request
    //
    
    json_t *request = ::json_object();
    
    ::json_object_set_new(request, "public_id", json_string(DTPortalCommon::public_id()) );
    ::json_object_set_new(request, "checksum", json_string(request_data_string_checksum.c_str()) );
    ::json_object_set_new(request, "data", json_string(request_data_string_encoded.c_str()) );
    
    std::string request_string = ::json_dumps(request, JSON_COMPACT);

    ::json_decref(request);
    
    DTPerror error = DTPortalNetwork::post(DTP_BASE_URL, "80", "sdk/sdk_submit_analytics.php", request_string, response);
    if (error != DTP_NO_ERROR)
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    
    //
    // Process the result
    //
    
    if (response.find("ERROR") != std::string::npos) {
        std::cout << response << std::endl;
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    }
    
    //
    // Remove the transferred items
    //
    
    _events_mutex.lock();

    while (events.size()) {
        events.pop_front();
        _events.pop_front();
    }

    _events_mutex.unlock();

    return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
}

void SubmitAnalyticsTask::append_event (const std::string &context, const std::string &key, const std::string &value)
{
    Event e;
    e.context = context;
    e.key = key;
    e.value = value;
    e.timestamp = (unsigned int) ::time(NULL);
    
    // Append the event
    _events_mutex.lock();
    _events.push_back(e);
    _events_mutex.unlock();
}

//==============================================================================
//==============================================================================

void DTPortalAnalytics::submit_analytics_event (const std::string &context, const std::string &key, const std::string &value)
{
    assert(_task.get());
    
    SubmitAnalyticsTask *t = (SubmitAnalyticsTask*)( _task.get() ); // No RTTI required
    t->append_event(context,key,value);
}

//==============================================================================
//==============================================================================

void DTPortalAnalytics::initialize (void)
{
    _task = DTPortalSmartPtr<DTPortalTask>(new SubmitAnalyticsTask());
    DTPortalTasks::queueTask(_task);
}
    
void DTPortalAnalytics::uninitialize (void)
{
    _task.reset();
}

//==============================================================================
//==============================================================================

REGISTER_TASK(SubmitAnalyticsTask)

} // DTPortal

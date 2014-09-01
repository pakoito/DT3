//==============================================================================
///	
///	File: DTPortalStrings.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalStrings.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalTasks.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalNetwork.hpp"
#include <iostream>
#include <sstream>
#include <ctime>

extern "C" {
    #include "jansson.h"
}

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
// Get high scores task
//==============================================================================

class GetStringsTask : public DTPortalTask
{
    public:
                    GetStringsTask              (void)                          {     _cb = NULL;   _error = DTP_NO_ERROR;  }
                    GetStringsTask              (getStringsCB cb, void *data)   {     _cb = cb; _data = data; _error = DTP_NO_ERROR;  }
        virtual     ~GetStringsTask             (void)                          {}
    
    public:
    
        /// Returns the name for serialization for the task
        /// \return Name for serialization
        virtual std::string         task_name       (void) const    {   return "GetStringsTask";  }
    
        /// Runs the task. This is where the web server communication would happen
        /// and the results would be stored in this object for later. This happens
        // in a separate thread.
        /// \return Error code
        virtual DTPerror            run_task        (void);

        /// Pumps the results. Causes any callbacks to be fired. This would happen
        /// in the main thread after the task is finished running.
        virtual void                pump_results    (void);

    
    private:
        std::map<std::string,std::string>   _strings;
        getStringsCB                        _cb;
        DTPerror                            _error;
        void                                *_data;
};

DTPerror GetStringsTask::run_task (void)
{
    std::string response;
    std::string header_and_contents;
    std::string url;
    
    //
    // Fill out the JSON request data
    //
    
    json_t *request_data = ::json_object();
    
    ::json_object_set_new(request_data, "dummy", json_string("Dummy") );

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
    
    DTPerror error = DTPortalNetwork::post(DTP_BASE_URL, "80", "sdk/sdk_get_strings.php", request_string, response);
    if (error != DTP_NO_ERROR)
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    
    //
    // Decode the JSON data
    //
    
    json_error_t json_error;
    json_t *root = ::json_loads(response.c_str(), 0, &json_error);

    if (!root)
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    
    
    if (!json_is_array(root)) {
        ::json_decref(root);
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    }

    for(int s = 0; s < json_array_size(root); ++s) {
    
        json_t *element = ::json_array_get(root, s);
        if (!json_is_object(element)) {
            ::json_decref(root);
            return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
        }

        json_t *key = ::json_object_get(element, "key");
        if (!json_is_string(key)) {
            ::json_decref(root);
            return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
        }

        json_t *value = ::json_object_get(element, "value");
        if (!json_is_string(value)) {
            ::json_decref(root);
            return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
        }
        
        _strings[::json_string_value(key)] = ::json_string_value(value);
    }
    
    
    ::json_decref(root);
    
    return DTP_NO_ERROR;
}

void GetStringsTask::pump_results (void)
{
    if (_cb)
        (*_cb)(_strings, _error, _data);
}

//==============================================================================
//==============================================================================

void DTPortalStrings::strings(getStringsCB cb, void *data)
{
    DTPortalSmartPtr<DTPortalTask> task(new GetStringsTask(cb,data));
    DTPortalTasks::queueTask(task);
}

//==============================================================================
//==============================================================================

REGISTER_TASK(GetStringsTask)

} // DTPortal

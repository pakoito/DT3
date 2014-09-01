//==============================================================================
///	
///	File: DTPortalTextBlob.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalTextBlob.hpp"
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
// Submit high scores task
//==============================================================================

class SubmitTextBlobTask : public DTPortalTask
{
    public:
                    SubmitTextBlobTask            (void)    {    _cb = NULL; _error = DTP_NO_ERROR;  }
                    SubmitTextBlobTask            (const std::string &title, const std::string &blob, submitTextBlobCB cb, void *data)
                                                            {    _title = title; _blob = blob; _cb = cb; _data = data; _error = DTP_NO_ERROR;  }
        virtual     ~SubmitTextBlobTask           (void)    {}
    
    public:
        /// Returns the name for serialization for the task
        /// \return Name for serialization
        virtual std::string         task_name               (void) const            {   return "SubmitTextBlobTask";  }
    
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
        virtual void                pump_results            (void);

    
    private:
        std::string                 _title;
        std::string                 _blob;
        submitTextBlobCB            _cb;
        DTPerror                    _error;
        void                        *_data;
};

DTPboolean SubmitTextBlobTask::serialize_out (std::string &s)
{
    json_t *root = ::json_object();
    
    ::json_object_set_new(root, "title", json_string(_title.c_str()));
    ::json_object_set_new(root, "blob", json_string(_blob.c_str()));
    
    s = ::json_dumps(root, JSON_COMPACT);
    
    ::json_decref(root);
    
    return true;
}

DTPboolean SubmitTextBlobTask::serialize_in (std::string s)
{
    json_error_t error;
    json_t *root = ::json_loads(s.c_str(), 0, &error);
    
    if (!root)
        return false;
    
    json_t *title = ::json_object_get(root, "title");
    if (!json_is_string(title)) {
        ::json_decref(root);
        return false;
    }
    
    json_t *blob = ::json_object_get(root, "blob");
    if (!json_is_string(blob)) {
        ::json_decref(root);
        return false;
    }
    
    _title = ::json_string_value(title);
    _blob = ::json_string_value(blob);
    
    ::json_decref(root);
    
    return true;
}

DTPerror SubmitTextBlobTask::run_task (void)
{
    std::string response;
    std::string header_and_contents;
    std::string url;
    
    //
    // Fill out the JSON request data
    //
    
    json_t *request_data = ::json_object();
    
    ::json_object_set_new(request_data, "timestamp", json_integer((unsigned int) ::time(NULL)) );
    ::json_object_set_new(request_data, "title", json_string(_title.c_str()) );
    ::json_object_set_new(request_data, "blob", json_string(_blob.c_str()) );

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
    
    DTPerror error = DTPortalNetwork::post(DTP_BASE_URL, "80", "sdk/sdk_submit_text_blob.php", request_string.c_str(), response);
    if (error != DTP_NO_ERROR)
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    
    //
    // Process the result
    //
    
    if (response.find("ERROR") != std::string::npos) {
        std::cout << response << std::endl;
        return DTP_ERROR;
    }
   
    return DTP_NO_ERROR;
}

void SubmitTextBlobTask::pump_results (void)
{
    if (_cb)
        (*_cb)(_error, _data);
}

//==============================================================================
//==============================================================================

void DTPortalTextBlob::submit_text_blob (const std::string &title, const std::string &blob, submitTextBlobCB cb, void *data)
{
    DTPortalSmartPtr<DTPortalTask> task(new SubmitTextBlobTask(title,blob,cb,data));
    DTPortalTasks::queueTask(task);
}

//==============================================================================
//==============================================================================

REGISTER_TASK(SubmitTextBlobTask)

} // DTPortal

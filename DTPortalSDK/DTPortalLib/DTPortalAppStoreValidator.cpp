//==============================================================================
///	
///	File: DTPortalAppStoreValidator.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalAppStoreValidator.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalTasks.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalNetwork.hpp"
#include <iostream>
#include <sstream>
#include <ctime>

extern "C" {
    #include "jansson.h"
    #include "tomcrypt.h"
}

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
// Get high scores task
//==============================================================================

class GetAppStoreValidatorTask : public DTPortalTask
{
    public:
                    GetAppStoreValidatorTask        (void)                                                              {     _cb = NULL; _error = DTP_NO_ERROR;  }
                    GetAppStoreValidatorTask        (const std::string &request, appStoreValidatorCB cb, void *data)    {     _request = request; _cb = cb; _data = data; _error = DTP_NO_ERROR;  }
        virtual     ~GetAppStoreValidatorTask       (void)                                                              {}
    
    public:
    
        /// Returns the name for serialization for the task
        /// \return Name for serialization
        virtual std::string         task_name       (void) const    {   return "GetAppStoreValidatorTask";  }
    
        /// Runs the task. This is where the web server communication would happen
        /// and the results would be stored in this object for later. This happens
        // in a separate thread.
        /// \return Error code
        virtual DTPerror            run_task        (void);

        /// Pumps the results. Causes any callbacks to be fired. This would happen
        /// in the main thread after the task is finished running.
        virtual void                pump_results    (void);

    
    private:
        std::string                         _request;
        appStoreValidatorCB                 _cb;
        DTPerror                            _error;
        void                                *_data;
};

DTPerror GetAppStoreValidatorTask::run_task (void)
{
    std::string response;
    std::string header_and_contents;
    std::string url;
    
    //
    // Fill out the JSON request data
    //
    
    json_t *request_data = ::json_object();
    
    // Base64 encode Reciept
    std::string encoded;
    unsigned long outlen = _request.size() + _request.size()/3 + 16;
    encoded.resize(outlen, 0);
    
    ::base64_encode((unsigned char*) &_request[0], _request.size(), (unsigned char*) &encoded[0], &outlen);
    encoded.resize(outlen, 0);


    ::json_object_set_new(request_data, "receipt-data", json_string(encoded.c_str()) );

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
    
    DTPerror error = DTPortalNetwork::post(DTP_BASE_URL, "80", "sdk/sdk_app_store_validate.php", request_string, response);
    if (error != DTP_NO_ERROR)
        return error;
    
    //
    // Decode the JSON data
    //
    
    json_error_t json_error;
    json_t *root = ::json_loads(response.c_str(), 0, &json_error);

    if (!root)
        return false;
    
    json_t *success = ::json_object_get(root, "success");
    if (!json_is_boolean(success)) {
        ::json_decref(root);
        return false;
    }
    
    if (::json_integer_value(success)) {
        _error = DTP_NO_ERROR;
    } else {
        _error = DTP_RECEIPT_INVALID;
    }
    
    ::json_decref(root);
    
    return DTP_NO_ERROR;
}

void GetAppStoreValidatorTask::pump_results (void)
{
    if (_cb)
        (*_cb)(_request, _error, _data);
}

//==============================================================================
//==============================================================================

void DTPortalAppStoreValidator::validate_app_store_purchase (const std::string &request, appStoreValidatorCB cb, void *data)
{
    DTPortalSmartPtr<DTPortalTask> task(new GetAppStoreValidatorTask(request,cb,data));
    DTPortalTasks::queueTask(task);
}

//==============================================================================
//==============================================================================

REGISTER_TASK(GetAppStoreValidatorTask)

} // DTPortal

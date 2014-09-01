//==============================================================================
///	
///	File: DTPortalHighScores.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalHighScores.hpp"
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

class GetHighScoresTask : public DTPortalTask
{
    public:
                    GetHighScoresTask               (void)                             {     _cb = NULL;   _error = DTP_NO_ERROR;  }
                    GetHighScoresTask               (getHighScoresCB cb, void *data)   {     _cb = cb; _data = data; _error = DTP_NO_ERROR;  }
        virtual     ~GetHighScoresTask              (void)                             {}
    
    public:
    
        /// Returns the name for serialization for the task
        /// \return Name for serialization
        virtual std::string         task_name       (void) const    {   return "GetHighScoresTask";  }
    
        /// Runs the task. This is where the web server communication would happen
        /// and the results would be stored in this object for later. This happens
        // in a separate thread.
        /// \return Error code
        virtual DTPerror            run_task        (void);

        /// Pumps the results. Causes any callbacks to be fired. This would happen
        /// in the main thread after the task is finished running.
        virtual void                pump_results    (void);

    
    private:
        std::map<std::string,std::vector<HighScore>>    _high_scores;
        getHighScoresCB                                 _cb;
        DTPerror                                        _error;
        void                                            *_data;
};

DTPerror GetHighScoresTask::run_task (void)
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
    
    DTPerror error = DTPortalNetwork::post(DTP_BASE_URL, "80", "sdk/sdk_get_high_scores.php", request_string, response);
    if (error != DTP_NO_ERROR)
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    
    //
    // Decode the JSON data
    //
    
    json_error_t json_error;
    json_t *root = ::json_loads(response.c_str(), 0, &json_error);

    if (!root)
        return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
    
    
    const char *category;
    json_t *scores;

    json_object_foreach(root, category, scores) {
    
        if (!json_is_array(scores)) {
            ::json_decref(root);
            return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
        }
    
        for(int s = 0; s < json_array_size(scores); ++s) {
        
            json_t *element = ::json_array_get(scores, s);
            if (!json_is_object(element)) {
                ::json_decref(root);
                return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
            }

            json_t *name = ::json_object_get(element, "name");
            if (!json_is_string(name)) {
                ::json_decref(root);
                return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
            }

            json_t *score = ::json_object_get(element, "score");
            if (!json_is_integer(score)) {
                ::json_decref(root);
                return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
            }

            json_t *censored = ::json_object_get(element, "censored");
            if (!json_is_integer(censored)) {
                ::json_decref(root);
                return DTP_NO_ERROR_BUT_RUN_TASK_AGAIN;
            }
        
            HighScore hs;
            hs.name = ::json_string_value(name);
            hs.score = (DTPuint) ::json_integer_value(score);
            hs.censored = ::json_integer_value(censored);
            
            _high_scores[category].push_back(hs);
        }
    
    }
    
    ::json_decref(root);
    
    return DTP_NO_ERROR;
}

void GetHighScoresTask::pump_results (void)
{
    if (_cb)
        (*_cb)(_high_scores, _error, _data);
}

//==============================================================================
// Submit high scores task
//==============================================================================

class SubmitHighScoresTask : public DTPortalTask
{
    public:
                    SubmitHighScoresTask            (void)  {   _cb = NULL; _data = NULL; _error = DTP_NO_ERROR;  }
                    SubmitHighScoresTask            (const std::string &category, const HighScore &hs, submitHighScoreCB cb, void *data)
                                                            {   _category = category; _hs = hs; _cb = cb; _data = data; _error = DTP_NO_ERROR;  }
        virtual     ~SubmitHighScoresTask           (void)  {}
    
    public:
        /// Returns the name for serialization for the task
        /// \return Name for serialization
        virtual std::string         task_name               (void) const            {   return "SubmitHighScoresTask";  }
    
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
        std::string                 _category;
        HighScore                   _hs;
        submitHighScoreCB           _cb;
        DTPerror                    _error;
        void                        *_data;
};

DTPboolean SubmitHighScoresTask::serialize_out (std::string &s)
{
    json_t *root = ::json_object();
    
    ::json_object_set_new(root, "category", json_string(_category.c_str()));
    ::json_object_set_new(root, "name", json_string(_hs.name.c_str()));
    ::json_object_set_new(root, "score", json_integer(_hs.score));
    
    s = ::json_dumps(root, JSON_COMPACT);
    
    ::json_decref(root);

    return true;
}

DTPboolean SubmitHighScoresTask::serialize_in (std::string s)
{
    json_error_t error;
    json_t *root = ::json_loads(s.c_str(), 0, &error);
    
    if (!root)
        return false;
    
    json_t *category = ::json_object_get(root, "category");
    if (!json_is_string(category)) {
        ::json_decref(root);
        return false;
    }
    
    json_t *name = ::json_object_get(root, "name");
    if (!json_is_string(name)) {
        ::json_decref(root);
        return false;
    }

    json_t *score = ::json_object_get(root, "score");
    if (!json_is_integer(score)) {
        ::json_decref(root);
        return false;
    }
    
    _category = ::json_string_value(category);
    _hs.name = ::json_string_value(name);
    _hs.score = (DTPuint) ::json_integer_value(score);

    ::json_decref(root);
    
    return true;
}

DTPerror SubmitHighScoresTask::run_task (void)
{
    std::string response;
    std::string header_and_contents;
    std::string url;
    
    //
    // Fill out the JSON request data
    //
    
    json_t *request_data = ::json_object();
    
    ::json_object_set_new(request_data, "timestamp", json_integer((unsigned int) ::time(NULL)) );
    ::json_object_set_new(request_data, "category", json_string(_category.c_str()) );
    ::json_object_set_new(request_data, "name", json_string(_hs.name.c_str()) );
    ::json_object_set_new(request_data, "score", json_integer(_hs.score) );

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
    
    DTPerror error = DTPortalNetwork::post(DTP_BASE_URL, "80", "sdk/sdk_submit_high_score.php", request_string, response);
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

void SubmitHighScoresTask::pump_results (void)
{
    if (_cb)
        (*_cb)(_error, _data);
}

//==============================================================================
//==============================================================================

void DTPortalHighScores::high_scores (getHighScoresCB cb, void *data)
{
    DTPortalSmartPtr<DTPortalTask> task(new GetHighScoresTask(cb,data));
    DTPortalTasks::queueTask(task);
}

//==============================================================================
//==============================================================================

void DTPortalHighScores::submit_high_score (const std::string &category, const HighScore &hs, submitHighScoreCB cb, void *data)
{
    DTPortalSmartPtr<DTPortalTask> task(new SubmitHighScoresTask(category,hs,cb,data));
    DTPortalTasks::queueTask(task);
}

//==============================================================================
//==============================================================================

REGISTER_TASK(GetHighScoresTask)
REGISTER_TASK(SubmitHighScoresTask)

} // DTPortal

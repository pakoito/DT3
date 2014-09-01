#ifndef DTPORTAL_DTPORTALSDK
#define DTPORTAL_DTPORTALSDK
//==============================================================================
///	
///	File: DTPortalSDK.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <list>
#include <vector>
#include <map>
#include <string>

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
// Types used by this system
//==============================================================================

typedef bool DTPboolean;
typedef int DTPerror;
typedef char DTPcharacter;
typedef unsigned char DTPubyte;
typedef int DTPint;
typedef unsigned int DTPuint;
typedef long long DTPint64;
typedef unsigned long long DTPuint64;

//==============================================================================
// Error codes returned bby this system
//==============================================================================

const DTPerror DTP_NO_ERROR = 0;                    // No error
const DTPerror DTP_NO_ERROR_BUT_RUN_TASK_AGAIN = 1; // No error but run the task again

const DTPerror DTP_NO_CONNECTION = 2;           // No connection could be established

const DTPerror DTP_BAD_PUBLIC_ID = 3;           // The public ID is not correct
const DTPerror DTP_BAD_SHARED_SECRET = 4;       // The shared secret is not correct

const DTPerror DTP_DECODING_ERROR = 5;          // The data cannot be decoded
const DTPerror DTP_ERROR = 6;                   // Misc error

const DTPerror DTP_RECEIPT_INVALID = 6;         // Misc error

//==============================================================================
// The URL of the web service
//==============================================================================

#define DTP_BASE_URL "sdk.smellslikedonkey.com"

//==============================================================================
// Debug flag
//==============================================================================

#if defined(_DEBUG) || defined(NDEBUG) || defined(DEBUG)
    #define DTP_DEBUG
#else
    #undef DTP_DEBUG
#endif

//==============================================================================
// Initialization routines
//==============================================================================

/// Initializes the entire system and enables communication with the
/// Donkey portal. 
/// \param public_id The Public App ID supplied by the Donkey portal
/// \param shared_secret The Shared Secret supplied by the Donkey portal
/// \param save_dir Path to where temporary files can be saved
/// \return Error code if there was an error, DTP_NO_ERROR if none
DTPerror initialize (const DTPcharacter *public_id, const DTPcharacter *shared_secret, const std::string &save_dir);

/// Shuts down the DTportal system and closes all communication with
/// the portal.
void uninitialize (void);

/// Causes all of the callbacks that are pending to be called.
void pump_results (void);

//==============================================================================
// High Scores
//==============================================================================

struct HighScore {
	std::string 		name;
    DTPboolean          censored;
	DTPuint             score;
};

typedef void (*getHighScoresCB)(std::map<std::string,std::vector<HighScore>> &, DTPerror error, void *data);
typedef void (*submitHighScoreCB)(DTPerror error, void *data);

/// Retrieves the high scores from the web service
/// \param cb Callback that will be called when the request completes.
/// \param data user data
void high_scores (getHighScoresCB cb, void *data);

/// Submits a high score to the web service
/// \param category The high score category to submit to
/// \param hs The high score to submit
/// \param cb Callback that will be called when the request completes.
/// \param data user data
void submit_high_score (const std::string &category, const HighScore &hs, submitHighScoreCB cb, void *data);

//==============================================================================
// Text Blob
//==============================================================================

typedef void (*submitTextBlobCB)(DTPerror error, void *data);

/// Submits a text blob to the service
/// \param title The title of the tex blob
/// \param blob The contents of the tex blob
/// \param cb Callback that will be called when the request completes.
/// \param data user data
void submit_text_blob (const std::string &title, const std::string &blob, submitTextBlobCB cb, void *data);

//==============================================================================
// Strings
//==============================================================================

typedef void (*getStringsCB)(std::map<std::string,std::string> &, DTPerror error, void *data);

/// Retrieves the strings from the web service
/// \param cb Callback that will be called when the request completes.
/// \param data user data
void strings (getStringsCB cb, void *data);

//==============================================================================
// Analytics
//==============================================================================

/// Submits an analytics event to the service
/// \param context The event context
/// \param key The event key
/// \param value The event value
void submit_analytics_event (const std::string &context, const std::string &key, const std::string &value);

//==============================================================================
// App Store Validator
//==============================================================================

typedef void (*appStoreValidatorCB)(std::string &request, DTPerror error, void *data);

/// Validates the app store purchase
/// \param request The request
/// \param cb Callback that will be called when the request completes.
/// \param data user data
void validate_app_store_purchase (const std::string &request, appStoreValidatorCB cb, void *data);

//==============================================================================
//==============================================================================

} // DTPortal

#endif


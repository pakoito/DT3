//==============================================================================
///	
///	File: DTPortalSDK.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalSDK.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalHighScores.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalTextBlob.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalStrings.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalAnalytics.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalAppStoreValidator.hpp"

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
//==============================================================================

DTPerror    initialize (const DTPcharacter *public_id, const DTPcharacter *shared_secret, const std::string &save_dir)
{
    return DTPortalCommon::initialize(public_id, shared_secret, save_dir);
}

void        uninitialize (void)
{
    DTPortalCommon::uninitialize();
}

//==============================================================================
//==============================================================================

void        pump_results (void)
{
    DTPortalCommon::pump_results();
}

//==============================================================================
//==============================================================================


void    high_scores (getHighScoresCB cb, void *data)
{
    DTPortalHighScores::high_scores(cb,data);
}

void    submit_high_score (const std::string &category, const HighScore &hs, submitHighScoreCB cb, void *data)
{
    DTPortalHighScores::submit_high_score(category,hs,cb,data);
}

//==============================================================================
//==============================================================================

void strings (getStringsCB cb, void *data)
{
    DTPortalStrings::strings(cb,data);
}

//==============================================================================
//==============================================================================

void    submit_text_blob (const std::string &title, const std::string &blob, submitTextBlobCB cb, void *data)
{
    DTPortalTextBlob::submit_text_blob(title,blob,cb,data);
}

//==============================================================================
//==============================================================================

void submit_analytics_event (const std::string &context, const std::string &key, const std::string &value)
{
    DTPortalAnalytics::submit_analytics_event(context,key,value);
}

//==============================================================================
//==============================================================================

void validate_app_store_purchase (const std::string &request, appStoreValidatorCB cb, void *data)
{
    DTPortalAppStoreValidator::validate_app_store_purchase(request,cb,data);
}

//==============================================================================
//==============================================================================

} // DTPortal

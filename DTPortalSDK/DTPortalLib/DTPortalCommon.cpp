//==============================================================================
///	
///	File: DTPortalCommon.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalTasks.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalHighScores.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalTextBlob.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalAnalytics.hpp"
#include <iostream>
#include <stdlib.h>

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
//==============================================================================

DTPcharacter        DTPortalCommon::_public_id[32+1] = {0};           // Plus 1 for NULL
DTPcharacter        DTPortalCommon::_shared_secret[64+1] = {0};       // Plus 1 for NULL
DTPubyte            DTPortalCommon::_shared_secret_key[32] = {0};     // Binary version of shared secret
std::string         DTPortalCommon::_save_dir;

//==============================================================================
//==============================================================================

DTPerror    DTPortalCommon::initialize (const DTPcharacter *public_id, const DTPcharacter *shared_secret, const std::string &save_dir)
{
    //
    // Validate Public ID
    //
    
    if (::strlen(public_id) != 32) {
        log_error ("Public ID is not the correct length. It must be 32 hexadecimal characters long.");
        return DTP_BAD_PUBLIC_ID;
    }
    
    // Check to make sure string is hex
    if (std::string(public_id).find_first_not_of("0123456789abcdefABCDEF") != std::string::npos) {
        log_error ("Public ID contains invalid characters. It must be 32 hexadecimal characters long.");
        return DTP_BAD_PUBLIC_ID;
    }

    //
    // Validate Shared Secret
    //
    
    if (::strlen(shared_secret) != 64) {
        log_error ("Shared Secret is not the correct length. It must be 64 hexadecimal characters long.");
        return DTP_BAD_SHARED_SECRET;
    }
    
    // Check to make sure string is hex
    if (std::string(shared_secret).find_first_not_of("0123456789abcdefABCDEF") != std::string::npos) {
        log_error ("Shared Secret contains invalid characters. It must be 64 hexadecimal characters long.");
        return DTP_BAD_SHARED_SECRET;
    }
    
    
    //
    // Store Public ID and Shared Secret
    //
    
    ::memcpy(_public_id, public_id, sizeof(_public_id));
    ::memcpy(_shared_secret, shared_secret, sizeof(_shared_secret));
    
    // Convert shared secret to binary
    for (DTPint src=0, dst=0; src < 64; src+=2, dst+=1) {
        DTPcharacter c[] = { shared_secret[src], shared_secret[src+1], 0 };
        _shared_secret_key[dst] = ::strtoul (c, NULL, 16);
    }
    
    _save_dir = save_dir;
    
    // Initialize queue
    DTPortalTasks::initialize ();

    // Initialize subsystems
    DTPortalHighScores::initialize();
    DTPortalTextBlob::initialize();
    DTPortalAnalytics::initialize();
    

    return DTP_NO_ERROR;
}

//==============================================================================
//==============================================================================

void        DTPortalCommon::uninitialize (void)
{
    // Uninitialize subsystems
    DTPortalHighScores::uninitialize();
    DTPortalTextBlob::uninitialize();
    DTPortalAnalytics::uninitialize();

    // Uninitialize queue
    DTPortalTasks::uninitialize ();
}

//==============================================================================
//==============================================================================

void        DTPortalCommon::pump_results (void)
{
    DTPortalTasks::pump_results();
}

//==============================================================================
//==============================================================================

void        DTPortalCommon::log_error (const std::string &error)
{
    std::cout << "DTPortalSDK Error: " << error << std::endl;
}

void        DTPortalCommon::log_warning (const std::string &warning)
{
    std::cout << "DTPortalSDK Warning: " << warning << std::endl;
}

//==============================================================================
//==============================================================================

} // DTPortal

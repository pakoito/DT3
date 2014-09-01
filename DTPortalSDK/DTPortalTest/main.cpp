//==============================================================================
///	
///	File: main.c
///
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalSDK.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalNetwork.hpp"

#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <cassert>

//==============================================================================
//==============================================================================

void submitHighScoreCB (DTPortal::DTPerror error, void *data)
{
    std::cout << "-> submitHighScoreCB returned " << error << std::endl;
}

void getHighScoresCB(std::map<std::string,std::vector<DTPortal::HighScore> > &scores, DTPortal::DTPerror error, void *data)
{
    std::cout << "-> getHighScoresCB returned " << error << std::endl;
    
    for (auto i : scores) {
        std::cout << i.first << " " << i.second.size() << std::endl;
    }
}

void submitTextBlobCB (DTPortal::DTPerror error, void *data)
{
    std::cout << "-> submitTextBlobCB returned " << error << std::endl;
}

void getStringsCB(std::map<std::string,std::string> &strings, DTPortal::DTPerror error, void *data)
{
    std::cout << "-> getStrings returned " << error << std::endl;
    
    for (auto i : strings) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}

//==============================================================================
//==============================================================================

int main(int argc, char *argv[])
{
    // https://portal.smellslikedonkey.com/web_high_scores.php?id=db4ac89a27d1800b14cd56bcb37398b3
    
    // Initialize the Portal SDK
    DTPortal::DTPerror err = DTPortal::initialize ( "db4ac89a27d1800b14cd56bcb37398b3",
                                                    "8f084198cc3bfd70583e81a4b5d4dacc319a8371f4928a769248dcdf42f18a49",
                                                    "/tmp");



    //
    // Check encryption and decryption
    //
    
    std::string plaintext = "This message brought to you by DTPortal...";
    std::cout << plaintext << std::endl << std::endl;

    std::string encoded;
    std::string decoded;
    std::string checksum;

    DTPortal::DTPortalNetwork::encode (plaintext, checksum, encoded);
    std::cout << encoded << "  " << encoded.size() << std::endl << std::endl;

    err = DTPortal::DTPortalNetwork::decode (encoded, checksum, decoded);
    if (err != DTPortal::DTP_NO_ERROR) {
        std::cout << " ERROR!! " << std::endl << std::endl;
    }
    
    std::cout << decoded << "  " << decoded.size() << std::endl << std::endl;

    assert(plaintext == decoded);

    
    
    for (int i = 0; i < 5000; ++i) {
    
        //
        // Try to submit new high score
        //
        
//        DTPortal::HighScore hs;
//        hs.name = "DTPortalTest";
//        hs.score = rand();
//
//        std::cout << "Submitting a new high score: " << hs.name << " " << hs.score << std::endl;
//        DTPortal::submit_high_score ("TestCategory", hs, &submitHighScoreCB, NULL);

        //
        // Try to retrieve high scores
        //
        
        std::cout << "Retrieving high scores" << std::endl;
        DTPortal::high_scores (&getHighScoresCB, NULL);
        
        //
        // Submit a text blob
        //
        
//        std::cout << "Submit a text blob" << std::endl;
//        DTPortal::submit_text_blob("A nice story","Once upon a time there was a giant computer...", &submitTextBlobCB, NULL);

        //
        // Submit Analytics event
        //
        
//        std::cout << "Submit analytics event" << std::endl;
//        DTPortal::submit_analytics_event ("Test", "TestKey", "TestValue");

        //
        // Get strings
        //
        
        DTPortal::strings(&getStringsCB, NULL);

        //
        // Pump tasks
        //
        
        DTPortal::pump_results();
        
        ::usleep(1000*1000);
    }

    // Uninitialize the Portal SDK
    DTPortal::uninitialize();

    return 0;
}

//==============================================================================
//==============================================================================


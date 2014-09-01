#ifndef DTPORTAL_HIGHSCORES
#define DTPORTAL_HIGHSCORES
//==============================================================================
///	
///	File: DTPortalHighScores.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalSDK.hpp"

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
/// Class
//==============================================================================

class DTPortalHighScores {
    private:
                                    DTPortalHighScores		(void);	
									DTPortalHighScores		(const DTPortalHighScores &rhs);
        DTPortalHighScores &        operator =              (const DTPortalHighScores &rhs);	
        virtual                     ~DTPortalHighScores		(void);
    		
	public:

        /// Initializes the high scores system. 
        static void                 initialize              (void)  {}
    
        /// Uninitializes the high scores system. 
        static void                 uninitialize            (void)  {}

        /// Retrieves the high scores from the web service
        /// \param cb Callback that will be called when the request completes.
        /// \param data user data
        static void                 high_scores             (getHighScoresCB cb, void *data);

        /// Submits a high score to the web service
        /// \param category The high score category to submit to
        /// \param hs The high score to submit
        /// \param cb Callback that will be called when the request completes.
        /// \param data user data
        static void                 submit_high_score       (const std::string &category, const HighScore &hs, submitHighScoreCB cb, void *data);
    
    
};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


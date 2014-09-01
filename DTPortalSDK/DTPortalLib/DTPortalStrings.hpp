#ifndef DTPORTAL_STRINGS
#define DTPORTAL_STRINGS
//==============================================================================
///	
///	File: DTPortalStrings.hpp
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

class DTPortalStrings {
    private:
                                    DTPortalStrings         (void);
									DTPortalStrings         (const DTPortalStrings &rhs);
        DTPortalStrings &           operator =              (const DTPortalStrings &rhs);
        virtual                     ~DTPortalStrings		(void);
    		
	public:

        /// Initializes the high scores system. 
        static void                 initialize              (void)  {}
    
        /// Uninitializes the high scores system. 
        static void                 uninitialize            (void)  {}

        /// Retrieves the strings from the web service
        /// \param cb Callback that will be called when the request completes.
        /// \param data user data
        static void                 strings                 (getStringsCB cb, void *data);

};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


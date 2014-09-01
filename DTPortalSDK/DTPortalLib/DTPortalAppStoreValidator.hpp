#ifndef DTPORTAL_APPSTOREVALIDATOR
#define DTPORTAL_APPSTOREVALIDATOR
//==============================================================================
///	
///	File: DTPortalAppStoreValidator.hpp
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

class DTPortalAppStoreValidator {
    private:
                                    DTPortalAppStoreValidator   (void);
									DTPortalAppStoreValidator   (const DTPortalAppStoreValidator &rhs);
        DTPortalAppStoreValidator&  operator =                  (const DTPortalAppStoreValidator &rhs);
        virtual                     ~DTPortalAppStoreValidator  (void);
    		
	public:

        /// Initializes the high scores system. 
        static void                 initialize                  (void)  {}
    
        /// Uninitializes the high scores system. 
        static void                 uninitialize                (void)  {}

        /// Validates the app store purchase
        /// \param request The request
        /// \param cb Callback that will be called when the request completes.
        /// \param data user data
        static void                 validate_app_store_purchase (const std::string &request, appStoreValidatorCB cb, void *data);

};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


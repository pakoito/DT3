#ifndef DTPORTAL_TEXTBLOB
#define DTPORTAL_TEXTBLOB
//==============================================================================
///	
///	File: DTPortalTextBlob.hpp
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

class DTPortalTextBlob {
    private:
                                    DTPortalTextBlob		(void);	
									DTPortalTextBlob		(const DTPortalTextBlob &rhs);
        DTPortalTextBlob &          operator =              (const DTPortalTextBlob &rhs);	
        virtual                     ~DTPortalTextBlob		(void);
    		
	public:    
        /// Initializes the text blob system. 
        static void                 initialize              (void)  {}
    
        /// Uninitializes the text blob system. 
        static void                 uninitialize            (void)  {}

        /// Submits a text blob to the service
        /// \param blob The contents of the tex blob
        /// \param cb Callback that will be called when the request completes.
        /// \param data user data
        static void                 submit_text_blob        (const std::string &title, const std::string &blob, submitTextBlobCB cb, void *data);
    
    
};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


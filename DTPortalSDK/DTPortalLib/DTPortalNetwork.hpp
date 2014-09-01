#ifndef DTPORTAL_NETWORK
#define DTPORTAL_NETWORK
//==============================================================================
///	
///	File: DTPortalNetwork.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalSDK.hpp"

#include <memory>

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
/// Networking implementation
//==============================================================================

class DTPortalNetwork {
    private:
                                    DTPortalNetwork         (void);	
									DTPortalNetwork         (const DTPortalNetwork &rhs);
        DTPortalNetwork &           operator =              (const DTPortalNetwork &rhs);	
        virtual                     ~DTPortalNetwork        (void);
    		
	public:

        /// Initializes the network layer. 
        static void                 initialize              (void);
    
        /// Uninitializes the network layer. 
        static void                 uninitialize            (void);


        /// Performs HTTP post.
        /// \param url URL to sent the request to
        /// \param header_and_contents The actual request
        /// \param response The resonse from the server
        /// \return Error code if there was an error, DTP_NO_ERROR if none
        static DTPerror             post                    (std::string host, std::string port, std::string location, std::string contents, std::string &response);
    
    
        /// Encode sensitive data
        /// \param decoded Original decoded data
        /// \param checksum Calculated checksum
        /// \param encoded Encoded data
        static void                 encode                  (std::string decoded, std::string &checksum, std::string &encoded);

        /// Decode sensitive data
        /// \param encoded Encoded data
        /// \param checksum Calculated checksum
        /// \param decoded Decoded data
        /// \return Error code if there was an error, DTP_NO_ERROR if none
        static DTPerror             decode                  (std::string encoded, std::string checksum, std::string &decoded);

    private:
        static std::string          url_encode              (const std::string &value);

};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


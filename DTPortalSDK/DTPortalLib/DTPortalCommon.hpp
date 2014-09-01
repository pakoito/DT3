#ifndef DTPORTAL_COMMON
#define DTPORTAL_COMMON
//==============================================================================
///	
///	File: DTPortalCommon.hpp
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

class DTPortalCommon {
    private:
                                    DTPortalCommon		(void);	
									DTPortalCommon		(const DTPortalCommon &rhs);
        DTPortalCommon &            operator =			(const DTPortalCommon &rhs);	
        virtual                     ~DTPortalCommon		(void);
    		
	public:
    
        /// Initializes the entire system and enables communication with the
        /// Donkey portal. 
        /// \param public_id The Public App ID supplied by the Donkey portal
        /// \param shared_secret The Shared Secret supplied by the Donkey portal
        /// \param save_dir Path to where temporary files can be saved
        /// \return Error code if there was an error, DTP_NO_ERROR if none
        static DTPerror             initialize          (const DTPcharacter *public_id, const DTPcharacter *shared_secret, const std::string &save_dir);

        /// Shuts down the DTportal system and closes all communication with
        /// the portal.
        static void                 uninitialize        (void);

        /// Causes all of the callbacks that are pending to be called.
        static void                 pump_results         (void);
    
    
        //
        // Error message Logging
        //

        /// Logs an error to the console
        /// \param error Error message to display
        static void                 log_error           (const std::string &error);

        /// Logs a warning to the console
        /// \param error Warning message to display
        static void                 log_warning         (const std::string &warning);


        //
        // App ID's and Shared Secrets
        //
    
        /// Returns the Public ID as ascii string.
        /// \return Pointer to Public ID.
        static const DTPcharacter*  public_id               (void)      {   return _public_id;  }
    
        /// Returns the Shared Secret as ascii string.
        /// \return Pointer to Shared Secret String.
        static const DTPcharacter*  shared_secret_as_string (void)      {   return _shared_secret;  }

        /// Returns the Shared Secret as bytes.
        /// \return Pointer to Shared Secret Bytes.
        static const DTPubyte*      shared_secret_as_bytes  (void)      {   return _shared_secret_key;  }

        /// Returns the location where temporary files can be saved.
        /// \return Path to save dir.
        static const std::string&   save_dir                (void)      {   return _save_dir;  }

    private:
        static DTPcharacter         _public_id[32+1];           // Plus 1 for NULL
    
        static DTPcharacter         _shared_secret[64+1];       // Plus 1 for NULL
        static DTPubyte             _shared_secret_key[32];     // Binary version of shared secret
        
        static std::string          _save_dir;
        
        
};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


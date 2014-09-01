#ifndef DTPORTAL_ANALYTICS
#define DTPORTAL_ANALYTICS
//==============================================================================
///	
///	File: DTPortalAnalytics.hpp
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
//==============================================================================

class DTPortalTask;
template<class T> class DTPortalSmartPtr;

//==============================================================================
/// Class
//==============================================================================

class DTPortalAnalytics {
    private:
                                    DTPortalAnalytics		(void);	
									DTPortalAnalytics		(const DTPortalAnalytics &rhs);
        DTPortalAnalytics &         operator =              (const DTPortalAnalytics &rhs);	
        virtual                     ~DTPortalAnalytics		(void);
    		
	public:
    
        /// Initializes the analytics system. 
        static void                 initialize              (void);
    
        /// Uninitializes the analytics system. 
        static void                 uninitialize            (void);


        /// Submits an analytics event to the service
        /// \param context The event context
        /// \param key The event key
        /// \param value The event value
        static void                 submit_analytics_event  (const std::string &context, const std::string &key, const std::string &value);
    
    private:
        static DTPortalSmartPtr<DTPortalTask>    _task;
};

//==============================================================================
//==============================================================================

} // DTPortal

#endif


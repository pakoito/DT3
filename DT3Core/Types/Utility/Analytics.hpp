#ifndef DT3_ANALYTICS
#define DT3_ANALYTICS
//==============================================================================
///	
///	File: Analytics.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Analytics {
    private:	
		inline					Analytics			(void)  {}
								Analytics			(const Analytics &rhs);
        Analytics &				operator =			(const Analytics &rhs);		
								~Analytics			(void);
    
    public:	
    
        /// Record analytics event
        /// \param context context
        /// \param key event key
        /// \param value event value
        static void             record_event        (const std::string &context, const std::string &key, const std::string &value);
    
};

//==============================================================================
//==============================================================================

} // DT3

#endif


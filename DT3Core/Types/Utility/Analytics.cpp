//==============================================================================
///	
///	File: Analytics.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Analytics.hpp"

#if DT3_USE_ANALYTICS && DTP_USE_PORTAL
    #include "DTPortalSDK/DTPortalLib/DTPortalSDK.hpp"
#endif

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

void Analytics::record_event (const std::string &context, const std::string &key, const std::string &value)
{
#if DT3_USE_ANALYTICS && DTP_USE_PORTAL
    DTPortal::submit_analytics_event (context, key, value);
#endif
}
    
//==============================================================================
//==============================================================================

} // DT3

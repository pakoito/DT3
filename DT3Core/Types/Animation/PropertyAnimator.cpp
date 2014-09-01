//==============================================================================
///	
///	File: PropertyAnimator.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Animation/PropertyAnimator.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::list<std::shared_ptr<PropertyAnimatorHandleBase>>  PropertyAnimator::_properties;

//==============================================================================
//==============================================================================

void PropertyAnimator::tick (const DTfloat dt)
{
//    LOG_MESSAGE << "PropertyAnimator tick: " << dt;

    for (auto i = _properties.begin(); i != _properties.end();) {
    
        // Clean out the list as we go
        DTboolean success = (**i).tick(dt);
        if (success) {
            ++i;
        } else {
            i = _properties.erase(i);
        }
    }
    
}

//==============================================================================
//==============================================================================

} // DT3

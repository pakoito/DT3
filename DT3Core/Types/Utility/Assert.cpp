//==============================================================================
///	
///	File: Assert.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/Debug.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint>>   AssertImpl::_callback_assert_msg;

//==============================================================================
//==============================================================================

void AssertImpl::set_callback_assert_msg (std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint>> cb)
{
    _callback_assert_msg = cb;
}

void AssertImpl::call_assert_msg (const DTcharacter* file, const DTcharacter* func, const DTint line)
{
    if (_callback_assert_msg)
        (*_callback_assert_msg)(file, func, line);  
    else {
        LOG_MESSAGE << "ASSERT: In file " << file << " in function " << func << " on line " << line;
        DT3_DEBUG_BREAK
        exit(1);
    }
}

void AssertImpl::call_assert_break (const DTcharacter* file, const DTcharacter* func, const DTint line)
{
    DT3_DEBUG_BREAK
    exit(1);
}

//==============================================================================
//==============================================================================

} // DT3


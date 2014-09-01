//==============================================================================
///	
///	File: Error.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter*>>   ErrorImpl::_callback_error_msg;
std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter*>>   ErrorImpl::_callback_warning_msg;

//==============================================================================
//==============================================================================

void ErrorImpl::set_callback_error_msg (std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter*>> cb)
{   
    _callback_error_msg = cb;
}

void ErrorImpl::call_error_msg (const DTcharacter* file, const DTcharacter* func, DTint line, const DTcharacter *msg)
{
    if (_callback_error_msg)
        (*_callback_error_msg)(file, func, line, msg);  
    else {
        LOG_ERROR << "In file " << file << " in function " << func << " on line " << line << ": " << msg;
    }
}

//==============================================================================
//==============================================================================

void ErrorImpl::set_callback_warning_msg (std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter*>> cb)
{   
    _callback_error_msg = cb;
}

void ErrorImpl::call_warning_msg (const DTcharacter* file, const DTcharacter* func, DTint line, const DTcharacter *msg)
{
    if (_callback_warning_msg)
        (*_callback_warning_msg)(file, func, line, msg);  
    else {
        LOG_MESSAGE << "In file " << file << " in function " << func << " on line " << line << ": " << msg;
    }
}

//==============================================================================
//==============================================================================

} // DT3


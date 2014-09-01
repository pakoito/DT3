//==============================================================================
///	
///	File: CommandParams.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CommandParams.hpp"
#include "DT3Core/Types/Utility/Tokenizer.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

CommandParams::CommandParams (void)
{

}

CommandParams::CommandParams (const std::string &c)
{
    _orig_string = c;
    
    Tokenizer t;
    t.set_token_stream(c);
    
    while (!t.is_done()) {
        _params.push_back(t.next_token_string_no_substitute());
    }
}

CommandParams::CommandParams (const CommandParams& rhs)
    :   _orig_string    (rhs._orig_string),
        _params         (rhs._params)
{

}

CommandParams& CommandParams::operator = (const CommandParams& rhs)
{
    _orig_string = rhs._orig_string;
    _params = rhs._params;
    
    return (*this);
}

CommandParams::~CommandParams (void)
{

}

//==============================================================================
//==============================================================================

std::string CommandParams::operator[] (DTuint i)
{	
    if (i >= _params.size())
        return std::string();
        
    return _params[i];	
}

const std::string CommandParams::operator[] (DTuint i) const
{	
    if (i >= _params.size())
        return std::string();

    return _params[i];	
}
        
//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: CommandContext.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CommandContext.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

CommandContext::CommandContext (void)
    :   _rectangle  (0.0F,0.0F,0.0F,0.0F)
{

}

CommandContext::CommandContext (const CommandContext& rhs)
{
    _world = rhs._world;
    _selection = rhs._selection;
    _rectangle = rhs._rectangle;
}

CommandContext& CommandContext::operator = (const CommandContext& rhs)
{
    _world = rhs._world;
    _selection = rhs._selection;
    _rectangle = rhs._rectangle;

    return (*this);
}

CommandContext::~CommandContext (void)
{

}

//==============================================================================
//==============================================================================

void CommandContext::set_world (std::shared_ptr<World> world)
{
    _world = world;
}

void CommandContext::set_selection (const std::list<std::shared_ptr<PlugNode>> &selection)
{
    _selection = selection;
}

void CommandContext::clear_selection (void)
{
    _selection.clear();
}

void CommandContext::set_selection_rectangle (const Rectangle &rectangle)
{
    _rectangle = rectangle;
}
//==============================================================================
//==============================================================================

} // DT3

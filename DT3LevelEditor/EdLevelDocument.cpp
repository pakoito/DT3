//==============================================================================
///	
///	File: EdLevelDocument.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelDocument.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelDocument::EdLevelDocument(void)
{
    _world = World::create();
}

EdLevelDocument::~EdLevelDocument(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelDocument::setWorld (const std::shared_ptr<World> &w)
{
    _world = w;
}

void EdLevelDocument::clearSelection (void)
{
    _selection.clear();
}

void EdLevelDocument::setSelection (const std::list<std::shared_ptr<PlugNode>> &s)
{
    clearSelection();
    std::copy(s.begin(), s.end(), std::back_inserter(_selection));
}

DTboolean EdLevelDocument::isSelected (const std::shared_ptr<PlugNode> &node) const
{
    auto i = std::find(_selection.begin(), _selection.end(), node);
    if (i != _selection.end())
        return true;
        
    return false;
}

//==============================================================================
//==============================================================================

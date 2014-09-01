//==============================================================================
///	
///	File: EdLevelHierarchyWindowMIME.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelHierarchyWindowMIME.hpp"

// Qt include
// Engine includes

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelHierarchyWindowMIME::EdLevelHierarchyWindowMIME(void)
{

}

EdLevelHierarchyWindowMIME::~EdLevelHierarchyWindowMIME(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindowMIME::addObject (const std::shared_ptr<PlugNode> &node)
{
    if (std::find(_nodes.begin(), _nodes.end(), node) == _nodes.end()) {
        _nodes.push_back(node);
    }
}

const std::list<std::shared_ptr<PlugNode>>& EdLevelHierarchyWindowMIME::getNodes (void) const
{
    return _nodes;
}

//==============================================================================
//==============================================================================

bool EdLevelHierarchyWindowMIME::hasFormat (const QString &mimeType) const
{
    if (mimeType == "DT3/HierItem")
        return true;
    else
        return false;
}

QStringList EdLevelHierarchyWindowMIME::formats (void) const
{
    QStringList m;
    m << "DT3/HierItem";
    return m;
}

QVariant EdLevelHierarchyWindowMIME::retrieveData (const QString & mimeType, QVariant::Type type ) const
{
    if (mimeType == "DT3/HierItem") {
        return QString("DT3/HierItem");
    } else {
        return QMimeData::retrieveData (mimeType, type);
    }
        
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelHierarchyWindowMIME.cpp"


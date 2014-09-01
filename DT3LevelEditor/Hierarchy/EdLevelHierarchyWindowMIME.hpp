#ifndef EDLEVELHIERARCHYWINDOWMIME
#define EDLEVELHIERARCHYWINDOWMIME
//==============================================================================
///	
///	File: EdLevelHierarchyWindowMIME.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtCore/QMimeData>
#include <QtCore/QStringList>

// Engine includes
#include <list>

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlugNode;
}

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelHierarchyWindowMIME : public QMimeData
{
    Q_OBJECT

	public:
                                EdLevelHierarchyWindowMIME          (void);
                                ~EdLevelHierarchyWindowMIME         (void);

    public:
        void                                        addObject       (const std::shared_ptr<PlugNode> &node);
        const std::list<std::shared_ptr<PlugNode>>& getNodes        (void) const;
        
        bool                                        hasFormat       (const QString & mimeType) const;
        QStringList                                 formats         (void) const;
        QVariant                                    retrieveData    (const QString & mimeType, QVariant::Type type ) const;
        
    private:
        std::list<std::shared_ptr<PlugNode>>         _nodes;
};

//==============================================================================
//==============================================================================

#endif

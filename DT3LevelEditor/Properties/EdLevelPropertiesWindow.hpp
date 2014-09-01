#ifndef EDLEVELPROPERTIESWINDOW
#define EDLEVELPROPERTIESWINDOW
//==============================================================================
///	
///	File: EdLevelPropertiesWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QToolBar>

// Engine includes
#include "ArchivePropertyReaderWriter.hpp"
#include <list>

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class ComponentBase;
    class PlugNode;
}

class EdLevelDocument;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelPropertiesWindow : public QListWidget
{
    Q_OBJECT

	public:
										EdLevelPropertiesWindow		(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);

	private:
        EdLevelDocument                                 *_document;
        QToolBar                                        *_toolbar;

        std::shared_ptr<PlugNode>                       _node;
        std::shared_ptr<ArchivePropertyReaderWriter>    _properties;

        void                            buildPropertiesForNode      (std::shared_ptr<PlugNode> node);
        void                            buildProperties             (std::shared_ptr<PlugNode> node);
                
        void                            clearProperties             (void);
        
    public slots:
        void                            onRefreshProperties         (void);
        void                            onSelectionChanged          (const std::list<std::shared_ptr<PlugNode>> &selection_list);

        void                            onAddComponent              (ComponentBase* node);
        void                            onRemoveComponent           (ComponentBase* node);
        
        void                            onCommand                   (QString command, bool recache);
        void                            onKeyMenu                   (QString property, const QPointF &global_pos);

        void                            onSetKey                    (void);
        void                            onClearAllKeys              (void);
    
        // Clipboard
        void                            cut                         (void);
        void                            copy                        (void);
        void                            paste                       (void);

    signals:
        void                            doCommand                   (QString command);
        void                            doUndoBlock                 (void);

        void                            doReadAllProperties();
};

//==============================================================================
//==============================================================================

#endif

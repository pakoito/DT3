#ifndef EDLEVELHIERARCHYWINDOW
#define EDLEVELHIERARCHYWINDOW
//==============================================================================
///	
///	File: EdLevelHierarchyWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QToolBar>
#include <QtCore/QTimer>
#include <QtCore/QRegExp>

// Engine includes
#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/Event.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class WorldNode;
    class PlugNode;
    class ComponentBase;
}
class EdLevelDocument;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelHierarchyWindow: public QTreeWidget
{
    Q_OBJECT

	public:
										EdLevelHierarchyWindow		(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);

        void                            selectionChanged            (const QItemSelection & selected, const QItemSelection & deselected);


        Qt::DropActions                 supportedDropActions        (void) const    {   return Qt::MoveAction;  }
        
        QMimeData*                      mimeData                    (const QList<QTreeWidgetItem *> items) const;
        QStringList                     mimeTypes                   (void) const;
    
        bool                            dropMimeData                (QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action);
        void                            dropEvent                   (QDropEvent *event);

        void                            dragEnterEvent              (QDragEnterEvent *event);
        void                            dragLeaveEvent              (QDragLeaveEvent *event);
        void                            dragMoveEvent               (QDragMoveEvent *event);

        void                            keyPressEvent               (QKeyEvent *event);

	private:
        static const DTuint AUTO_SCROLL_TIMER = 30;
        static const DTuint AUTO_SCROLL_MARIGN = 30;

        EdLevelDocument                 *_document;
        EdLevelLineEdit                 *_filter;
        QToolBar                        *_toolbar;
        QTreeWidgetItem                 *_top_level;
        
		QAction                         *_show_objects;
		QAction                         *_show_scripting;
		QAction                         *_show_all;
        QRegExp                         _reg_exp_filter;
    
        void                            startAutoScroll             (void);
        void                            stopAutoScroll              (void);
    
        QTimer                          _auto_scroll_timer;
        DTint                           _auto_scroll_x;
        DTint                           _auto_scroll_y;
      
        // Internal representation of scene
        struct NodeCache {
            NodeCache(void)                                         {   _list_object = NULL; }
            NodeCache(const std::shared_ptr<WorldNode> &node)       {   _node = node; _list_object = NULL; }
            
            DTboolean   operator == (const NodeCache& rhs) const    {	return _node == rhs._node;		}

            std::shared_ptr<WorldNode>      _node;
            QTreeWidgetItem                 *_list_object;
        };
        
        std::list<NodeCache>                _node_cache;
        
        std::shared_ptr<WorldNode>      itemToNode                  (const QTreeWidgetItem *item) const;
    
        QTreeWidgetItem*                nodeToItem                  (WorldNode *node) const;
        QTreeWidgetItem*                nodeToItem                  (const std::shared_ptr<WorldNode> &node) const;
    
        
        enum Visibility {
            VIS_ALL,
            VIS_OBJECTS,
            VIS_SCRIPTING
        } _visibility;
        
        void                            setVisibility               (NodeCache *c);
	
    public slots:
        void                            onAddNode                   (WorldNode *node);
        void                            onRemoveNode                (WorldNode *node);
        void                            onReparentNode              (WorldNode *node, WorldNode *old_parent, WorldNode *new_parent);

        void                            onRefreshHierarchy          (void);
        void                            onSelectionChanged          (const std::list<std::shared_ptr<PlugNode>> &selection_list);

        void                            onConnectPlug               (PlugBase *outgoing, PlugBase *incoming);
        void                            onDisconnectPlug            (PlugBase *outgoing, PlugBase *incoming);
        void                            onConnectEvent              (Event *outgoing, Event *incoming);
        void                            onDisconnectEvent           (Event *outgoing, Event *incoming);
        
        void                            onShowObjects               (void);
        void                            onShowCalc                  (void);
        void                            onShowAll                   (void);
        void                            onShowFilter                (const QString & text);

        void                            onAutoScroll                (void);
    
        void                            onItemChanged               (QTreeWidgetItem *item, int column);

    signals:
        void                            doCommand                   (QString command);
        void                            doUndoBlock                 (void);
        void                            doSelectionChanged          (const std::list<std::shared_ptr<PlugNode>> &selection_list);
};

//==============================================================================
//==============================================================================

#endif

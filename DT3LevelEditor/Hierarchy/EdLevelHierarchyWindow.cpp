//==============================================================================
///	
///	File: EdLevelHierarchyWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelHierarchyWindow.hpp"
#include "EdLevelHierarchyWindowMIME.hpp"
#include "EdLevelDocument.hpp"

// Qt include
#include <QtGui/QDropEvent>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QAction>
#include <QtWidgets/QScrollBar>

// Engine includes
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelHierarchyWindow::EdLevelHierarchyWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   QTreeWidget         (parent),
        _visibility         (VIS_ALL)
{
	_document = document;
    _toolbar = toolbar;
    
    //
    // Actions and toolbar
    //
    
    _show_objects = new QAction(tr("Show Objects"), this);
    _show_objects->setIcon(QIcon(":/images/hierobj.png"));
    //_show_objects->setShortcut(QKeySequence::New);
    _show_objects->setStatusTip(tr("Show Object nodes"));
    connect(_show_objects, SIGNAL(triggered()), this, SLOT(onShowObjects()));

    _show_scripting = new QAction(tr("Show Scripting"), this);
    _show_scripting->setIcon(QIcon(":/images/hiercalc.png"));
    //_show_scripting->setShortcut(QKeySequence::New);
    _show_scripting->setStatusTip(tr("Show Scripting Nodes"));
    connect(_show_scripting, SIGNAL(triggered()), this, SLOT(onShowCalc()));

    _show_all = new QAction(tr("Show All"), this);
    _show_all->setIcon(QIcon(":/images/hierall.png"));
    //_show_all->setShortcut(QKeySequence::New);
    _show_all->setStatusTip(tr("Show All Nodes"));
    connect(_show_all, SIGNAL(triggered()), this, SLOT(onShowAll()));
    

    toolbar->addAction(_show_objects);
    toolbar->addAction(_show_scripting);
    toolbar->addAction(_show_all);

    _filter = new EdLevelLineEdit(toolbar);
    _filter->setMinimumWidth(50);
    toolbar->addWidget(_filter);
    
    // For Typing
    connect(_filter,   SIGNAL(textChanged(const QString &)),
            this,       SLOT(onShowFilter(const QString &)));


    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
        
    setAutoScroll(false);            // Doesn't work!!!!
    
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    
    setColumnCount(2);
    
    //setAlternatingRowColors(true);
    
    QTreeWidgetItem *header_item = new QTreeWidgetItem();
	header_item->setText(0, "Name");
	header_item->setText(1, "Kind");
    header_item->setFlags (0);

    setHeaderItem (header_item);
    header()->resizeSection(0, 200);

    QTreeWidgetItem *root_item = invisibleRootItem();
    root_item->setFlags (0);

	_top_level = new QTreeWidgetItem();
	_top_level->setText(0, _document->world()->name().c_str());
	_top_level->setText(1, _document->world()->class_id_child());
    _top_level->setFlags (Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    addTopLevelItem(_top_level);
    
    // For autoscroll
    connect(&_auto_scroll_timer,    SIGNAL(timeout()),
            this,                   SLOT(onAutoScroll()));
    
    // For renaming
    connect(this,                   SIGNAL(itemChanged(QTreeWidgetItem*, int) ),
            this,                   SLOT(onItemChanged(QTreeWidgetItem*, int) ) );
            
    _auto_scroll_timer.setSingleShot(false);
    
    // Set case insensitive
    _reg_exp_filter.setCaseSensitivity(Qt::CaseInsensitive);
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::onShowObjects (void)
{
    _visibility = VIS_OBJECTS;

    FOR_EACH (i, _node_cache) {
        setVisibility(&(*i));
    }
}

void EdLevelHierarchyWindow:: onShowCalc (void)
{
    _visibility = VIS_SCRIPTING;

    FOR_EACH (i, _node_cache) {
        setVisibility(&(*i));
    }
}

void EdLevelHierarchyWindow::onShowAll (void)
{
    _visibility = VIS_ALL;

    FOR_EACH (i, _node_cache) {
        setVisibility(&(*i));
    }
}

void EdLevelHierarchyWindow::onShowFilter (const QString &text)
{
    if (text.length() == 0) {
        _reg_exp_filter.setPattern("*");
    } else {
        _reg_exp_filter.setPattern(text);
    }

    FOR_EACH (i, _node_cache) {
        setVisibility(&(*i));
    }
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::setVisibility   (NodeCache *c)
{
    DTboolean match = _reg_exp_filter.indexIn(  QString(c->_node->name().c_str()) + " " +
                                                QString(c->_node->class_id_child()) ) >= 0;

    if (_reg_exp_filter.isValid() && !match) {
        c->_list_object->setHidden(true);
        return;
    }

    if (_visibility == VIS_ALL) {
        c->_list_object->setHidden(false);
        return;
    }
    
    if (_visibility == VIS_OBJECTS && c->_node->isa(ObjectBase::kind()) ) {
        c->_list_object->setHidden(false);
        return;
    }
    
    if (_visibility == VIS_SCRIPTING && c->_node->isa(ScriptingBase::kind()) ) {
        c->_list_object->setHidden(false);
        return;
    }
    
    c->_list_object->setHidden(true);
}

//==============================================================================
//==============================================================================

QMimeData* EdLevelHierarchyWindow::mimeData (const QList<QTreeWidgetItem *> items) const
{
    if (items.size() == 0)
        return NULL;

    EdLevelHierarchyWindowMIME *mime = new EdLevelHierarchyWindowMIME();
    
    FOR_EACH (i, items) {
    
        std::shared_ptr<WorldNode> node = itemToNode(*i);
        
        if (node)
            mime->addObject(node);
        
    }

    
    return mime;
}

QStringList EdLevelHierarchyWindow::mimeTypes (void) const
{
    QStringList s;
    s << "DT3/HierItem";
    return s;
}

bool EdLevelHierarchyWindow::dropMimeData (QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{
    emit doUndoBlock();

    // Only accept move actions
    if (action != Qt::MoveAction)
        return false;

    // Only accepts our custom data
    const EdLevelHierarchyWindowMIME *mime = qobject_cast<const EdLevelHierarchyWindowMIME*>(data);
    if (!mime)
        return false;
        
    // Get the new parent
    std::shared_ptr<PlugNode> new_parent_node = itemToNode(parent);
      
    // Send command to reparent
    const std::list<std::shared_ptr<PlugNode>>& nodes = mime->getNodes ();
    
    FOR_EACH (i, nodes) {
    
        if (new_parent_node) {
            std::string cmd = "Parent \"" + (**i).name() + "\" \"" + new_parent_node->name() + "\"";
            emit doCommand(cmd.c_str());
        } else {
            std::string cmd = "Unparent \"" + (**i).name() + "\"";
            emit doCommand(cmd.c_str());
        }
        
    }
    
    return true;
}

void EdLevelHierarchyWindow::dropEvent (QDropEvent *event)
{
    emit doUndoBlock();

    // Only accept move actions
    if (event->dropAction() != Qt::MoveAction) {
        event->setDropAction(Qt::IgnoreAction);
        QTreeWidget::dropEvent(event);
    }
    
    // Only accepts our custom data
    const EdLevelHierarchyWindowMIME *mime = qobject_cast<const EdLevelHierarchyWindowMIME*>(event->mimeData());
    if (!mime) {
        event->setDropAction(Qt::IgnoreAction);
        QTreeWidget::dropEvent(event);
    }
            
    // Get the new parent
    std::shared_ptr<PlugNode> new_parent_node = itemToNode(itemAt(event->pos()));
      
    // Send command to reparent
    const std::list<std::shared_ptr<PlugNode>>& nodes = mime->getNodes ();
    
    FOR_EACH (i, nodes) {
    
        if (new_parent_node) {
            std::string cmd = "Parent \"" + (**i).name() + "\" \"" + new_parent_node->name() + "\"";
            emit doCommand(cmd.c_str());
        } else {
            std::string cmd = "Unparent \"" + (**i).name() + "\"";
            emit doCommand(cmd.c_str());
        }
        
    }
    
    //event->accept(); // uncommenting this will cause Qt bugs where it deletes items
    
    //event->setDropAction(Qt::IgnoreAction);
    QTreeWidget::dropEvent(event);

    stopAutoScroll();
}

void EdLevelHierarchyWindow::dragEnterEvent (QDragEnterEvent *event)
{
    startAutoScroll();

    QTreeWidget::dragEnterEvent(event);
}

void EdLevelHierarchyWindow::dragLeaveEvent (QDragLeaveEvent *event)
{
    stopAutoScroll();
    
    QTreeWidget::dragLeaveEvent(event);
}

void EdLevelHierarchyWindow::dragMoveEvent(QDragMoveEvent *event)
{
	QPoint end_point = event->pos();
    
    // Autoscroll
    if(end_point.x() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_x = AUTO_SCROLL_MARIGN - end_point.x();
    } else if(width()-end_point.x() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_x = -(AUTO_SCROLL_MARIGN - (width()-end_point.x()));
    } else {
        _auto_scroll_x = 0;
    }
    
    if(end_point.y() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_y = AUTO_SCROLL_MARIGN - end_point.y();
    } else if(height()-end_point.y() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_y = -(AUTO_SCROLL_MARIGN - (height()-end_point.y()));
    } else {
        _auto_scroll_y = 0;
    }
    
    LOG_MESSAGE << _auto_scroll_x << " " << _auto_scroll_y;
    
    QTreeWidget::dragMoveEvent(event);
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::startAutoScroll (void)
{
    _auto_scroll_x = 0;
    _auto_scroll_y = 0;
    _auto_scroll_timer.start(AUTO_SCROLL_TIMER);

    LOG_MESSAGE << "Autoscroll Started";
}

void EdLevelHierarchyWindow::stopAutoScroll (void)
{
    _auto_scroll_timer.stop();
    
    LOG_MESSAGE << "Autoscroll Stopped";
}

void EdLevelHierarchyWindow::onAutoScroll (void)
{
    LOG_MESSAGE << "onAutoScroll";

    if (_auto_scroll_x == 0 && _auto_scroll_y == 0)
        return;
        
    //QScrollBar *hBar = horizontalScrollBar();
    QScrollBar *vBar = verticalScrollBar();
    //hBar->setValue(hBar->value() - _auto_scroll_x);
    vBar->setValue(vBar->value() - _auto_scroll_y);
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::onItemChanged (QTreeWidgetItem *item, int column)
{
    std::shared_ptr<WorldNode> world_node = itemToNode(item);
    if (world_node) {
    
        if (column == 0) {
            std::string old_name = world_node->name();
            std::string new_name = item->text(column).toUtf8().data();
            
            if (new_name != old_name) {
            
                // Check if name is already taken
                if (_document->world()->node_by_name(new_name) == NULL) {

                    std::string cmd = "SetName \"" + old_name + "\" \"" + new_name + "\"";
                    emit doCommand(cmd.c_str());
                    
                // Restore old name to the widget
                } else {
                
                    blockSignals(true);
                    item->setText(0, old_name.c_str());
                    blockSignals(false);
                }
                
            }
        }

        blockSignals(true);
        item->setText(1, world_node->class_id_child());
        blockSignals(false);
    }
    
}

//==============================================================================
//==============================================================================

std::shared_ptr<WorldNode> EdLevelHierarchyWindow::itemToNode(const QTreeWidgetItem *item) const
{
    FOR_EACH (i, _node_cache) {
        if (item == i->_list_object)
            return i->_node;
    }

    return NULL;
}

QTreeWidgetItem* EdLevelHierarchyWindow::nodeToItem(WorldNode *node) const
{
    FOR_EACH (i, _node_cache) {
        if (node == i->_node.get())
            return i->_list_object;
    }

    return NULL;
}

QTreeWidgetItem* EdLevelHierarchyWindow::nodeToItem(const std::shared_ptr<WorldNode> &node) const
{
    return nodeToItem(node.get());
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::selectionChanged (const QItemSelection & selected, const QItemSelection & deselected)
{
    QTreeWidget::selectionChanged(selected, deselected);
    
    std::list<std::shared_ptr<PlugNode>> selection_list;
    QList<QTreeWidgetItem *> selected_items = selectedItems();
    
    FOR_EACH (i,selected_items) {
    
        if (*i == _top_level) {
            selection_list.push_back(_document->world());
        } else {
            std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(itemToNode(*i));
            if (node)
                selection_list.push_back(node);
        }
        
    }
    
    emit doSelectionChanged(selection_list);
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::onAddNode (WorldNode *node_raw)
{
    std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(node_raw->shared_from_this());

	QTreeWidgetItem *item = new QTreeWidgetItem();

	item->setText(0, node->name().c_str());
	item->setText(1, node->class_id_child());

    if (node->isa(ObjectBase::kind())) {
        item->setFlags (Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setIcon(0,QIcon(":/images/hierobj.png"));
    } else {
        item->setFlags (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setIcon(0,QIcon(":/images/hiercalc.png"));
    }
            
    _top_level->addChild(item);
            
    NodeCache c(node);
    c._list_object = item;
    _node_cache.push_back(c);
    
    setVisibility(&c);
    
    // Fix parenting
    if (node && node->isa(PlaceableObject::kind()) ) {
        std::shared_ptr<PlaceableObject> obj = checked_static_cast<PlaceableObject>(node);
    
        if (obj->has_parent()) {
            onReparentNode (node.get(), NULL, obj->parent());
        }
        
        std::list<PlaceableObject*> children = obj->children();
        
        FOR_EACH (i,children) {
            onReparentNode ( *i, NULL, node.get());
        }
        
    }
    
}

void EdLevelHierarchyWindow::onRemoveNode (WorldNode *node_raw)
{
    std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(node_raw->shared_from_this());

    NodeCache c(node);

    auto i = std::find(_node_cache.begin(), _node_cache.end(), c);
    if (i != _node_cache.end()) {
        delete i->_list_object;
        i->_list_object = NULL;
        
        _node_cache.erase(i);
    }

}

void EdLevelHierarchyWindow::onReparentNode (WorldNode *node, WorldNode *old_parent, WorldNode *new_parent)
{
    blockSignals(true);

    QTreeWidgetItem* node_item = nodeToItem(node);
    QTreeWidgetItem* old_parent_item = nodeToItem(old_parent);
    QTreeWidgetItem* new_parent_item = nodeToItem(new_parent);
    
    if (old_parent_item)
        old_parent_item->removeChild(node_item);
    else
        _top_level->removeChild(node_item);

    if (new_parent_item)
        new_parent_item->addChild(node_item);
    else
        _top_level->addChild(node_item);
    
    blockSignals(false);
    
    update();
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::onRefreshHierarchy (void)
{
    FOR_EACH (i, _node_cache) {
        i->_list_object->setText(0, i->_node->name().c_str());
        setVisibility(&(*i));
    }
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::onSelectionChanged (const std::list<std::shared_ptr<PlugNode>> &selection_list)
{
    blockSignals(true);

    clearSelection();
    
    FOR_EACH (i,selection_list) {
    
        if (*i == _document->world()) {
            _top_level->setSelected(true);
        } else {
            
            std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(*i);

            if (node) {
                QTreeWidgetItem *item = nodeToItem(node);
                
                if (item) {
                    item->setSelected(true);
                    scrollToItem (item);
                }
            }
        }

    }

    blockSignals(false);
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::keyPressEvent (QKeyEvent *event)
{
    emit doUndoBlock();
    
    int key = event->key();

    if (event->matches(QKeySequence::Delete) || key == 0x1000003) {
    
        QList<QTreeWidgetItem *> selected_items = selectedItems();
    
        FOR_EACH (i,selected_items) {
        
            std::shared_ptr<WorldNode>node = itemToNode(*i);
            if (node) {
                std::string nodename = node->name();

                std::string cmd = "Remove \"" + nodename + "\"";
                emit doCommand(cmd.c_str());
            }
        }

    }
}

//==============================================================================
//==============================================================================

void EdLevelHierarchyWindow::onConnectPlug (PlugBase *outgoing, PlugBase *incoming)
{
    
}

void EdLevelHierarchyWindow::onDisconnectPlug (PlugBase *outgoing, PlugBase *incoming)
{

}

void EdLevelHierarchyWindow::onConnectEvent (Event *outgoing, Event *incoming)
{

}

void EdLevelHierarchyWindow::onDisconnectEvent (Event *outgoing, Event *incoming)
{

}

//==============================================================================
//==============================================================================

#include "moc_EdLevelHierarchyWindow.cpp"


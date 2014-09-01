//==============================================================================
///	
///	File: EdSpriteJointList.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteJointList.hpp"
#include "EdSpritePropertyDefaultField.hpp"
#include "EdSpritePropertyBoolField.hpp"

#include "ArchiveData.hpp"
#include "Algorithm.hpp"
#include "ConsoleStream.hpp"

using namespace DT2;

//==============================================================================
//==============================================================================

EdSpriteJointList::EdSpriteJointList(QWidget *parent, QToolBar *toolbar, EdSpriteMainWindow *main_window)
    :   QListWidget     (parent),
        _order          (ORDER_BY_ORDER)
{
    _toolbar = toolbar;
	_main_window = main_window;
    
    //
    // Actions and toolbar
    //

    _order_by_name = new QAction(tr("Order By Name"), this);
    _order_by_name->setIcon(QIcon(":/images/order_by_name.png"));
    _order_by_name->setStatusTip(tr("Order the items by their name"));
    connect(_order_by_name, SIGNAL(triggered()), this, SLOT(onOrderByName()));

    _order_by_order = new QAction(tr("Order By Order"), this);
    _order_by_order->setIcon(QIcon(":/images/order_by_order.png"));
    _order_by_order->setStatusTip(tr("Order the items by their order"));
    connect(_order_by_order, SIGNAL(triggered()), this, SLOT(onOrderByOrder()));

    toolbar->addAction(_order_by_name);
    toolbar->addAction(_order_by_order);

    _filter = new EdSpriteLineEdit(toolbar);
    _filter->setMinimumWidth(50);
    toolbar->addWidget(_filter);
    
    // For Typing
    connect(_filter,   SIGNAL(textChanged(const QString &)),
            this,       SLOT(onShowFilter(const QString &)));


    setMinimumWidth(200);
    
    connect(	this,       SIGNAL(itemChanged(QListWidgetItem *)),
				this,       SLOT(onItemChanged(QListWidgetItem *))	);
    
    connect(    this,       SIGNAL(itemSelectionChanged()),
                this,       SLOT(onItemSelectionChanged()));

    
    //setDragDropMode(QAbstractItemView::InternalMove);
    
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    
    _reg_exp_filter.setPattern("*");
}

//==============================================================================
//==============================================================================

void EdSpriteJointList::onItemChanged(QListWidgetItem *item)
{
    _main_window->pushUndo();

    KeyedSpriteAnimationPoseJoint *data = (KeyedSpriteAnimationPoseJoint *) item->data(Qt::UserRole).value<void*>();

    data->setVisible(item->checkState() == Qt::Checked ? true : false);
    
    _main_window->keyJoint(data);
    _main_window->fireSpriteChanged();
}

void EdSpriteJointList::onItemSelectionChanged  (void)
{
    _main_window->pushUndo();

    QList<QListWidgetItem*> selected_items = selectedItems();
    List<KeyedSpriteAnimationPoseJoint*>   selected_joints;
    
    FOR_EACH (i,selected_items) {
        KeyedSpriteAnimationPoseJoint *data = (KeyedSpriteAnimationPoseJoint *) (**i).data(Qt::UserRole).value<void*>();
        selected_joints.pushBack(data);
        
        // Add a keyframe
        _main_window->keyJoint(data);
    }
    
    _main_window->setPartSelection(selected_joints);
    
    _main_window->fireSpriteChanged();
    _main_window->fireAnimationsChanged();
}

//==============================================================================
//==============================================================================

void EdSpriteJointList::onOrderByName (void)
{
    _order = ORDER_BY_NAME;
    refreshList();
}

void EdSpriteJointList::onOrderByOrder (void)
{
    _order = ORDER_BY_ORDER;
    refreshList();
}

void EdSpriteJointList::onShowFilter (const QString &text)
{
    if (text.length() == 0) {
        _reg_exp_filter.setPattern("*");
    } else {
        _reg_exp_filter.setPattern(text);
    }

    refreshList();
}

//==============================================================================
//==============================================================================

int EdSpriteJointList::compare(KeyedSpriteAnimationPoseJoint *a, KeyedSpriteAnimationPoseJoint *b)
{
    if (a->getName() < b->getName()) return 1;
    else if (a->getName() > b->getName()) return -1;
    else return 0;
}

//==============================================================================
//==============================================================================

void EdSpriteJointList::refreshList (void)
{
    blockSignals(true);
    setUpdatesEnabled(false);

    clear();

    KeyedSpriteResource *sprite = _main_window->getSprite();
    if (!sprite)
        return;

    Array<KeyedSpriteAnimationPoseJoint*> joints = sprite->getJoints();
    
    if (_order == ORDER_BY_ORDER) {
        // Do nothing
    } else if (_order == ORDER_BY_NAME) {
        Algorithm::quicksort(joints,&EdSpriteJointList::compare);
    }
    
    // Add all the items
    for (DTsize i = joints.size()-1; i >= 0; --i) {
    
        QListWidgetItem* item = new QListWidgetItem(joints[i]->getName().cStr(), this);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag

        item->setData(Qt::UserRole, qVariantFromValue( (void*) joints[i] ));
        
        DTboolean match = _reg_exp_filter.indexIn(QString(joints[i]->getName().cStr())) >= 0;

        if (_reg_exp_filter.isValid() && !match) {
            item->setHidden(true);
        }

    }
    
    // Set Visible and selected
    for (DTint i = 0; i < count();++i) {
        QListWidgetItem *cur_item = item(i);
        KeyedSpriteAnimationPoseJoint *data = (KeyedSpriteAnimationPoseJoint *) cur_item->data(Qt::UserRole).value<void*>();

        cur_item->setCheckState(data->getVisible() ? Qt::Checked : Qt::Unchecked);
        
        if (_main_window->isPartSelected(data)) {
            cur_item->setSelected(true);
            scrollToItem(cur_item);
        }

        
    }


    blockSignals(false);
    setUpdatesEnabled(true);
}

//==============================================================================
//==============================================================================

void EdSpriteJointList::dropEvent (QDropEvent *event)
{
//    // Only accept move actions
//    if (event->dropAction() != Qt::MoveAction) {
//        event->setDropAction(Qt::IgnoreAction);
//        QListWidget::dropEvent(event);
//    }
//
//    event->accept(); // uncommenting this will cause Qt bugs where it deletes items
//    event->setDropAction(Qt::IgnoreAction);

    QListWidget::dropEvent(event);
}

//==============================================================================
//==============================================================================

void EdSpriteJointList::onSpriteChanged (void)
{
    refreshList();
}

void EdSpriteJointList::onSelectionChanged (void)
{
    refreshList();
}

//==============================================================================
//==============================================================================


#include "moc_EdSpriteJointList.cpp"


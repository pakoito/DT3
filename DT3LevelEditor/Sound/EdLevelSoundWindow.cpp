//==============================================================================
///	
///	File: EdLevelSoundWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelSoundWindow.hpp"
#include "EdLevelDocument.hpp"
#include "EdLevelSoundBus.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtCore/QTimerEvent>

// Engine includes
#include "DT3Core/Scripting/ScriptingSoundBusOut.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelSoundWindow::EdLevelSoundWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   QWidget   (parent)
{
    _document = document;
    _toolbar = toolbar;
        
    _layout = new QHBoxLayout();
    _layout->addStretch();
    
    // Add scroll area
    _scroll_area = new QScrollArea(this);
    _scroll_area->setWidgetResizable(true);

    _scroll_widget = new QWidget(_scroll_area);
    _scroll_widget->setLayout(_layout);
    
    _scroll_area->setWidget(_scroll_widget);

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setHorizontalSpacing(1);
    layout->setVerticalSpacing(1);
    layout->addWidget(_scroll_area);
    setLayout(layout);
    
    _timer.start(200, this);
}

//==============================================================================
//==============================================================================

void EdLevelSoundWindow::resizeEvent (QResizeEvent *event)
{
    
}

//==============================================================================
//==============================================================================

void EdLevelSoundWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timer.timerId()) {
        
        // Update Busses
        FOR_EACH (i,_bus_cache) {
            i->_widget->updateBus();
        }
        
    } else {
        QWidget::timerEvent(event);
    }

}

//==============================================================================
//==============================================================================

void EdLevelSoundWindow::onAddNode (WorldNode *node_raw)
{
    std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(node_raw->shared_from_this());

    // Add the node to the bus cache
    if (node->isa(ScriptingSoundBusOut::kind())) {    
        std::shared_ptr<ScriptingSoundBusOut> bus = checked_static_cast<ScriptingSoundBusOut>(node);
    
        BusCache c(bus);
        
        c._widget = new EdLevelSoundBus(_scroll_widget, bus);
        
        QObject::connect(	c._widget,      SIGNAL(doCommand(QString)),
                            this,           SLOT(onCommand(QString))	);

        _layout->insertWidget(0,c._widget);
        
        _bus_cache.push_back(c);
    }
    
}

void EdLevelSoundWindow::onRemoveNode (WorldNode *node_raw)
{
    std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(node_raw->shared_from_this());

    if (node->isa(ScriptingSoundBusOut::kind())) {    
        std::shared_ptr<ScriptingSoundBusOut> bus = checked_static_cast<ScriptingSoundBusOut>(node);

        // Remove the node from the bus cache
        BusCache c(bus);

        auto i = std::find(_bus_cache.begin(), _bus_cache.end(), c);
        if (i != _bus_cache.end()) { 
            _layout->removeWidget(i->_widget);
            delete i->_widget;
            _bus_cache.erase(i);
        }
    }
}

//==============================================================================
//==============================================================================

void EdLevelSoundWindow::onCommand (QString command)
{    
    emit doCommand(command);
}

//==============================================================================
//==============================================================================

void EdLevelSoundWindow::onRefreshSound (void)
{
    // Update Busses
    FOR_EACH (i,_bus_cache) {
        i->_widget->updateList();
    }
    
    onRefreshProperties();
}

void EdLevelSoundWindow::onRefreshProperties (void)
{
    // Update Busses
    FOR_EACH (i,_bus_cache) {
        i->_widget->updateProperties();
    }
}

void EdLevelSoundWindow::onSelectionChanged (const std::list<std::shared_ptr<PlugNode>> &selection_list)
{

}

//==============================================================================
//==============================================================================

#include "moc_EdLevelSoundWindow.cpp"


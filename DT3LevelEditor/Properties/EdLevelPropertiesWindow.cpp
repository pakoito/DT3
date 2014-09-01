//==============================================================================
///	
///	File: EdLevelPropertiesWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertiesWindow.hpp"
#include "EdLevelPropertyDefaultField.hpp"
#include "EdLevelPropertyVector2Field.hpp"
#include "EdLevelPropertyVector3Field.hpp"
#include "EdLevelPropertyMatrix3Field.hpp"
#include "EdLevelPropertyColorField.hpp"
#include "EdLevelPropertyBoolField.hpp"
#include "EdLevelPropertyTitleField.hpp"
#include "EdLevelPropertyEventField.hpp"
#include "EdLevelPropertyEnumField.hpp"
#include "EdLevelPropertyRangeField.hpp"
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>

// Engine includes
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelPropertiesWindow::EdLevelPropertiesWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   QListWidget     (parent),
        _node           (NULL)
{
	_document = document;
    _toolbar = toolbar;
    _properties = ArchivePropertyReaderWriter::create();

    setMinimumWidth(350);
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::buildPropertiesForNode (std::shared_ptr<PlugNode> node)
{

    //
    // Add Components
    //

    DTsize start_of_properties = _properties->properties().size();
    
    node->archive(_properties);
    
    // Get the properties
    std::vector<std::shared_ptr<ArchiveData>>& properties = _properties->properties();

    for (DTuint i = (DTuint)start_of_properties; i < properties.size(); ++i) {
        std::shared_ptr<ArchiveData> data = properties[i];
        const DTcharacter *type = data->data_kind();
    
        QListWidgetItem *list_item = new QListWidgetItem();
        addItem(list_item);
        
        if (data->title() == "_name") {
            EdLevelPropertyTitleField *itemWidget = new EdLevelPropertyTitleField(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());
            
            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));

        } else if ( data->is_enum() ) {
            EdLevelPropertyEnumField *itemWidget = new EdLevelPropertyEnumField(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());
            
            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));

            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );

        } else if ( data->is_range() ) {
            EdLevelPropertyRangeField *itemWidget = new EdLevelPropertyRangeField(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());
            
            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));

            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );
            
        } else if ( data->is_bitfield() ) {
            ASSERT(0);

        } else if ( std::string(type) == std::string(Color4f::class_id_static())) {
            EdLevelPropertyColorField *itemWidget = new EdLevelPropertyColorField(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());
            
            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));

            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );
            
        } else if ( std::string(type) == std::string(Vector3::class_id_static())) {
            EdLevelPropertyVector3Field *itemWidget = new EdLevelPropertyVector3Field(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());

            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));
            
            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );

        } else if ( std::string(type) == std::string(Vector2::class_id_static())) {
            EdLevelPropertyVector2Field *itemWidget = new EdLevelPropertyVector2Field(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());

            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));
            
            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );

        } else if ( std::string(type) == std::string(Matrix3::class_id_static())) {
            EdLevelPropertyMatrix3Field *itemWidget = new EdLevelPropertyMatrix3Field(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());

            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));
            
            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );

        } else if ( std::string(type) == std::string("bool")) {
            EdLevelPropertyBoolField *itemWidget = new EdLevelPropertyBoolField(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());

            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));
            
            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );

        } else if ( std::string(type) == std::string(Event::class_id_static())) {
            EdLevelPropertyEventField *itemWidget = new EdLevelPropertyEventField(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());

            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));
            
            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );

        } else {
            EdLevelPropertyDefaultField *itemWidget = new EdLevelPropertyDefaultField(this, node, data);
            setItemWidget(list_item,itemWidget);
            list_item->setSizeHint(itemWidget->minimumSize());

            connect (   this,       SIGNAL(doReadAllProperties()),
                        itemWidget, SLOT(doReadParams()));
            

            connect (   itemWidget, SIGNAL(doKeyMenu(QString, const QPointF &)),
                        this,       SLOT(onKeyMenu(QString, const QPointF &)) );
        }
        
    }

}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::buildProperties(std::shared_ptr<PlugNode> node)
{    
    LOG_MESSAGE << "EdLevelPropertiesWindow::buildProperties";

    // Get properties for selection
    buildPropertiesForNode(node);
    
    // Do components if necessary
    std::shared_ptr<ObjectBase> base_object = checked_cast<ObjectBase>(node);
    if (base_object) {
        std::list<std::shared_ptr<ComponentBase>> c = base_object->all_components();
        
        FOR_EACH (i,c) {
            buildPropertiesForNode(*i);
        }
    }
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::clearProperties (void)
{
    // Disconnect any slots from previous properties
    disconnect(this, SIGNAL(doReadAllProperties()), 0, 0);
    disconnect(this, SIGNAL(doKeyMenu(QString, const QPointF &)), 0, 0);
        
    // Disconnect all children
    QList<QWidget*> widgetList = findChildren<QWidget *>("prop");
    for (auto i = widgetList.begin(); i != widgetList.end(); ++i) {
        (**i).blockSignals(true);
    }
    
    clear();
    _properties->clear();
    
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::onCommand (QString command, bool recache)
{
    if (recache) clearProperties();
    
    emit doUndoBlock();
    emit doCommand(command);
    
    if (recache) buildProperties(_node);
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::onKeyMenu(QString property, const QPointF &global_pos)
{

    // Context Menu
    QMenu *context_menu = new QMenu();
    
    QAction *set_key_action = new QAction(this);
    set_key_action->setVisible(true);
    set_key_action->setText("Add Key");
    set_key_action->setData(property);
    set_key_action->setVisible(true);

    connect(set_key_action,     SIGNAL(triggered()),
            this,               SLOT(onSetKey()));
                                
    context_menu->addAction(set_key_action);

    context_menu->addSeparator();

    QAction *clear_keys_action = new QAction(this);
    clear_keys_action->setVisible(true);
    clear_keys_action->setText("Clear All Keys");
    clear_keys_action->setData(property);
    clear_keys_action->setVisible(true);

    connect(clear_keys_action,   SIGNAL(triggered()),
            this,               SLOT(onClearAllKeys()));
                                
    context_menu->addAction(clear_keys_action);
        
    context_menu->exec(global_pos.toPoint());
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::onSetKey (void)
{
    QAction *action = qobject_cast<QAction*>(sender());
    QString object_type = action->data().toString();
    
    emit doCommand("SetKeyframe \"" + object_type + "\"");
}

void EdLevelPropertiesWindow::onClearAllKeys (void)
{
    QAction *action = qobject_cast<QAction*>(sender());
    QString object_type = action->data().toString();

    emit doCommand("ClearAllKeyframes \"" + object_type + "\"");
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::onSelectionChanged (const std::list<std::shared_ptr<PlugNode>> &s)
{
    clearProperties();
    
    if (s.size() != 1) {
        _node = NULL;
        return;
    }
                
    _node = s.front();
    buildProperties(s.front());
}

void EdLevelPropertiesWindow::onRefreshProperties (void)
{
    LOG_MESSAGE << "EdLevelPropertiesWindow::onRefreshProperties";

    emit doReadAllProperties();
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::onAddComponent (ComponentBase* component)
{
    if (_node.get() == component->owner()) {
        clearProperties();
        buildProperties(_node);
    }
}

void EdLevelPropertiesWindow::onRemoveComponent (ComponentBase* component)
{
    if (_node.get() == component->owner()) {
        clearProperties();
        buildProperties(_node);
    }
}

//==============================================================================
//==============================================================================

void EdLevelPropertiesWindow::cut (void)
{
    QListWidgetItem *item = currentItem();
    if (!item)
        return;
    
    QWidget *widget = itemWidget(item);
    if (!widget)
        return;
        
    EdLevelPropertyBase *property = qobject_cast<EdLevelPropertyBase*>(widget);
    if (!property)
        return;
    
    std::string value = property->getValueOfField();
    QApplication::clipboard()->setText(value.c_str());
    
    property->setValueOfField("");
}

void EdLevelPropertiesWindow::copy (void)
{
    QListWidgetItem *item = currentItem();
    if (!item)
        return;
    
    QWidget *widget = itemWidget(item);
    if (!widget)
        return;
        
    EdLevelPropertyBase *property = qobject_cast<EdLevelPropertyBase*>(widget);
    if (!property)
        return;
    
    std::string value = property->getValueOfField();
    QApplication::clipboard()->setText(value.c_str());
}

void EdLevelPropertiesWindow::paste (void)
{
    QListWidgetItem *item = currentItem();
    if (!item)
        return;
    
    QWidget *widget = itemWidget(item);
    if (!widget)
        return;
        
    EdLevelPropertyBase *property = qobject_cast<EdLevelPropertyBase*>(widget);
    if (!property)
        return;
    
    std::string value = QApplication::clipboard()->text().toUtf8().data();
    property->setValueOfField(value);
}

//==============================================================================
//==============================================================================


#include "moc_EdLevelPropertiesWindow.cpp"


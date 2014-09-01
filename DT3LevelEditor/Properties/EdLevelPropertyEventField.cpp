//==============================================================================
///	
///	File: EdLevelPropertyEventField.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyEventField.hpp"
#include "EdLevelPropertiesWindow.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>

// Engine includes
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"

//==============================================================================
//==============================================================================

EdLevelPropertyEventField::EdLevelPropertyEventField (EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data)
{
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = data;
    _node = node;
        
    buildStatusButtons();
    


    _value = new QPushButton(MoreStrings::captialize_and_format(_data->title()).c_str(), this);
    _value->setEnabled(true);
    
	connect(	_value,         SIGNAL(clicked()),
				this,           SLOT(doClicked())	);
    
	connect(	this,           SIGNAL(doCommand(QString, bool)),
				parent,         SLOT(onCommand(QString, bool))	);
    
    
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->addWidget(keyButton(),0,0);
    layout->addWidget(hasInputButton(),0,1);
    layout->addWidget(hasOutputButton(),0,2);
    layout->addWidget(_value,0,4);

    setLayout(layout);
    
    setMinimumHeight(15+2);
    
    doReadParams();
}

EdLevelPropertyEventField::~EdLevelPropertyEventField	(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelPropertyEventField::doReadParams(void)
{
    blockSignals(true);
    
    keyButton()->disconnect();
    
    // Has Key
    if (_data->event()) {
        keyButton()->setIcon(QIcon(":/images/key.png"));
                
        connect(    keyButton(),        SIGNAL(pressed()),
                    this,       SLOT(doKeyframePressed())    );
                    
    } else {
        keyButton()->setIcon(QIcon(":/images/blank.png"));
    }
    
    // Has Input
    if (_data->event() && _data->event()->has_incoming_connection()) {
        hasInputButton()->setIcon(QIcon(":/images/has_input.png"));
    } else {
        hasInputButton()->setIcon(QIcon(":/images/blank.png"));
    }

    // Has Output
    if (_data->event() && _data->event()->has_outgoing_connection()) {
        hasOutputButton()->setIcon(QIcon(":/images/has_output.png"));
    } else {
        hasOutputButton()->setIcon(QIcon(":/images/blank.png"));
    }

    blockSignals(false);
}

void EdLevelPropertyEventField::doClicked(void)
{
    emit doCommand(QString("TriggerEvent \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\"", _data->flags() & DATA_FLUSH_UI);
}

//==============================================================================
//==============================================================================

void EdLevelPropertyEventField::doKeyframePressed (void)
{
    emit doKeyMenu( QString(_node->full_name().c_str()) + "." + _data->title().c_str(), keyButton()->mapToGlobal(QPoint(0,0)));
}

//==============================================================================
//==============================================================================

std::string EdLevelPropertyEventField::getValueOfField (void)
{
    return "";
}
    
void EdLevelPropertyEventField::setValueOfField (const std::string &value)
{

}

//==============================================================================
//==============================================================================

#include "moc_EdLevelPropertyEventField.cpp"


//==============================================================================
///	
///	File: EdLevelPropertyTitleField.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyTitleField.hpp"
#include "EdLevelPropertiesWindow.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>

// Engine includes
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"

//==============================================================================
//==============================================================================

EdLevelPropertyTitleField::EdLevelPropertyTitleField (EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data)
{
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = data;
    _node = node;
    
    _value = new EdLevelLineEdit(this);
    _value->setObjectName("prop");

	connect(	_value,         SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
    
	connect(	this,           SIGNAL(doCommand(QString, bool)),
				parent,         SLOT(onCommand(QString, bool))	);

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0,10,0,5);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->addWidget(_value,0,0);

    setLayout(layout);
    
    setMinimumHeight(15+2+10+5);
    
    doReadParams();
}

EdLevelPropertyTitleField::~EdLevelPropertyTitleField	(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelPropertyTitleField::doReadParams(void)
{
    blockSignals(true);
	TextBufferStream stream;
	_data->value(stream);
    _value->setText( stream.buffer().c_str() );
    blockSignals(false);
}

void EdLevelPropertyTitleField::doWriteParams(void)
{
	TextBufferStream stream(parseParam(_value->text().toUtf8().data()));

    TextBufferStream oldstream;
	_data->value(oldstream);
    
    // Only if value changed
    if (stream.buffer() != oldstream.buffer()) {
        emit doCommand(QString("SetName \"") + _node->full_name().c_str() + "\" \"" + stream.buffer().c_str() + "\"", _data->flags() & DATA_FLUSH_UI);
    }
}

//==============================================================================
//==============================================================================

std::string EdLevelPropertyTitleField::getValueOfField (void)
{
	TextBufferStream stream;
	_data->value(stream);
    return stream.buffer();
}
    
void EdLevelPropertyTitleField::setValueOfField (const std::string &value)
{
	TextBufferStream stream(value);
	_data->set_value(stream);

    emit doCommand(QString("SetName \"") + _node->full_name().c_str() + "\" \"" + stream.buffer().c_str() + "\"", _data->flags() & DATA_FLUSH_UI);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelPropertyTitleField.cpp"


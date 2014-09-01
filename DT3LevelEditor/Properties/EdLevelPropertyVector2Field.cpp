//==============================================================================
///	
///	File: EdLevelPropertyVector2Field.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyVector2Field.hpp"
#include "EdLevelPropertiesWindow.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>

// Engine includes
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"

//==============================================================================
//==============================================================================

EdLevelPropertyVector2Field::EdLevelPropertyVector2Field (EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data)
{
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = data;
    _node = node;

    buildStatusButtons();

    QLabel *name = new QLabel(this);
    name->setMaximumWidth(130);
    name->setMinimumWidth(130);
    name->setText(MoreStrings::captialize_and_format(_data->title()).c_str());

    _value_x = new EdLevelLineEdit(this);
    _value_y = new EdLevelLineEdit(this);
    
    _value_x->setObjectName("prop");
    _value_y->setObjectName("prop");

	connect(	_value_x,       SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_y,       SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);

	connect(	this,           SIGNAL(doCommand(QString, bool)),
				parent,         SLOT(onCommand(QString, bool))	);

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->addWidget(keyButton(),0,0);
    layout->addWidget(hasInputButton(),0,1);
    layout->addWidget(hasOutputButton(),0,2);
    layout->addWidget(name,0,3);
    layout->addWidget(_value_x,0,4);
    layout->addWidget(_value_y,0,5);

    setLayout(layout);
    
    setMinimumHeight(15+2);

    doReadParams();
}

EdLevelPropertyVector2Field::~EdLevelPropertyVector2Field	(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelPropertyVector2Field::doReadParams(void)
{
    blockSignals(true);

    keyButton()->disconnect();

    // Has Key
    if (_data->plug()) {
        keyButton()->setIcon(QIcon(":/images/key.png"));
                
        connect(    keyButton(),        SIGNAL(pressed()),
                    this,       SLOT(doKeyframePressed())    );
                    
    } else {
        keyButton()->setIcon(QIcon(":/images/blank.png"));
    }

    // Has Input
    if (_data->plug() && _data->plug()->has_incoming_connection()) {
        hasInputButton()->setIcon(QIcon(":/images/has_input.png"));
    } else {
        hasInputButton()->setIcon(QIcon(":/images/blank.png"));
    }

    // Has Output
    if (_data->plug() && _data->plug()->has_outgoing_connection()) {
        hasOutputButton()->setIcon(QIcon(":/images/has_output.png"));
    } else {
        hasOutputButton()->setIcon(QIcon(":/images/blank.png"));
    }

	TextBufferStream stream;
	_data->value(stream);
    
    Vector2 val;
    stream >> val;
    
    _value_x->setText( MoreStrings::cast_to_string(val.x).c_str() );
    _value_y->setText( MoreStrings::cast_to_string(val.y).c_str() );
    blockSignals(false);
}

void EdLevelPropertyVector2Field::doWriteParams(void)
{
    Vector2 val;
    val.x = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_x->text().toUtf8().data()));
    val.y = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_y->text().toUtf8().data()));

	TextBufferStream stream;
    stream << val;

    TextBufferStream oldstream;
	_data->value(oldstream);
    
    // Only if value changed
    if (stream.buffer() != oldstream.buffer()) {
        emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" (" + stream.buffer().c_str() + ")", _data->flags() & DATA_FLUSH_UI);
    }
}

//==============================================================================
//==============================================================================

void EdLevelPropertyVector2Field::doKeyframePressed (void)
{
    emit doKeyMenu( QString(_node->full_name().c_str()) + "." + _data->title().c_str(), keyButton()->mapToGlobal(QPoint(0,0)));
}

//==============================================================================
//==============================================================================

std::string EdLevelPropertyVector2Field::getValueOfField (void)
{
	TextBufferStream stream;
	_data->value(stream);
    return stream.buffer();
}
    
void EdLevelPropertyVector2Field::setValueOfField (const std::string &value)
{
	TextBufferStream stream(value);
	_data->set_value(stream);

    emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" (" + stream.buffer().c_str() + ")", _data->flags() & DATA_FLUSH_UI);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelPropertyVector2Field.cpp"


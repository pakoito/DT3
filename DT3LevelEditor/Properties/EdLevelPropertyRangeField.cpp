//==============================================================================
///	
///	File: EdLevelPropertyRangeField.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyRangeField.hpp"
#include "EdLevelPropertiesWindow.hpp"
#include "EdLevelLineEdit.hpp"

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

const int EdLevelPropertyRangeField::RANGE = 10000;

//==============================================================================
//==============================================================================

EdLevelPropertyRangeField::EdLevelPropertyRangeField (EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data)
{
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = data;
    _node = node;
   
    buildStatusButtons();

    QLabel *name = new QLabel(this);
    name->setMaximumWidth(130);
    name->setMinimumWidth(130);
    name->setText(MoreStrings::captialize_and_format(_data->title()).c_str());

    _value = new QSlider(Qt::Horizontal, this);
    _value->setMinimum(0);
    _value->setMaximum(RANGE);
    _value->setObjectName("prop");

    _value2 = new EdLevelLineEdit(this);
    _value2->setMaximumWidth(60);
    _value2->setObjectName("prop");

	connect(	_value2,        SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams2())	);
    
	connect(	_value,         SIGNAL(valueChanged(int)),
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
    layout->addWidget(_value,0,4);
    layout->addWidget(_value2,0,5);

    setLayout(layout);
    
    setMinimumHeight(15+2);
    
    doReadParams();
}

EdLevelPropertyRangeField::~EdLevelPropertyRangeField	(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelPropertyRangeField::doReadParams(void)
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
    
    DTfloat v = MoreStrings::cast_from_string<DTfloat>(stream.buffer());
    _value->setValue( RANGE * (v-_data->range_min()) / (_data->range_max()-_data->range_min()) );
    _value2->setText( stream.buffer().c_str() );

    blockSignals(false);
}

void EdLevelPropertyRangeField::doWriteParams(void)
{
    DTfloat value = (_value->value() / RANGE) * (_data->range_max()-_data->range_min()) + _data->range_min();

    std::string value_string = MoreStrings::cast_to_string(value);

    TextBufferStream oldstream;
	_data->value(oldstream);
    
    // Only if value changed
    if (value_string != oldstream.buffer()) {
        emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" \"" + value_string.c_str() + "\"", _data->flags() & DATA_FLUSH_UI);
    }
}

void EdLevelPropertyRangeField::doWriteParams2(void)
{
	TextBufferStream stream(parseParam(_value2->text().toUtf8().data()));
    
    TextBufferStream oldstream;
	_data->value(oldstream);
    
    // Only if value changed
    if (stream.buffer() != oldstream.buffer()) {
        emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" \"" + stream.buffer().c_str() + "\"", _data->flags() & DATA_FLUSH_UI);
    }
}

//==============================================================================
//==============================================================================

void EdLevelPropertyRangeField::doKeyframePressed (void)
{
    emit doKeyMenu( QString(_node->full_name().c_str()) + "." + _data->title().c_str(), keyButton()->mapToGlobal(QPoint(0,0)));
}

//==============================================================================
//==============================================================================

std::string EdLevelPropertyRangeField::getValueOfField (void)
{
	TextBufferStream stream;
	_data->value(stream);
    return stream.buffer();
}
    
void EdLevelPropertyRangeField::setValueOfField (const std::string &value)
{
	TextBufferStream stream(value);
	_data->set_value(stream);

    emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" \"" + stream.buffer().c_str() + "\"", _data->flags() & DATA_FLUSH_UI);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelPropertyRangeField.cpp"


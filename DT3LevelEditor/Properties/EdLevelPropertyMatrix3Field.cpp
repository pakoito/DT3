//==============================================================================
///	
///	File: EdLevelPropertyMatrix3Field.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyMatrix3Field.hpp"
#include "EdLevelPropertiesWindow.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>

// Engine includes
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"

//==============================================================================
//==============================================================================

EdLevelPropertyMatrix3Field::EdLevelPropertyMatrix3Field (EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data)
{
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = data;
    _node = node;

    buildStatusButtons();

    QLabel *name = new QLabel(this);
    name->setMaximumWidth(130);
    name->setMinimumWidth(130);
    name->setText(MoreStrings::captialize_and_format(_data->title()).c_str());

    _value_11 = new EdLevelLineEdit(this);
    _value_12 = new EdLevelLineEdit(this);
    _value_13 = new EdLevelLineEdit(this);

    _value_21 = new EdLevelLineEdit(this);
    _value_22 = new EdLevelLineEdit(this);
    _value_23 = new EdLevelLineEdit(this);

    _value_31 = new EdLevelLineEdit(this);
    _value_32 = new EdLevelLineEdit(this);
    _value_33 = new EdLevelLineEdit(this);
 
    _value_11->setObjectName("prop");
    _value_12->setObjectName("prop");
    _value_13->setObjectName("prop");

    _value_21->setObjectName("prop");
    _value_22->setObjectName("prop");
    _value_23->setObjectName("prop");

    _value_31->setObjectName("prop");
    _value_32->setObjectName("prop");
    _value_33->setObjectName("prop");


	connect(	_value_11,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_12,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_13,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_21,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_22,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_23,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_31,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_32,      SIGNAL(editingFinished()),
				this,           SLOT(doWriteParams())	);
	connect(	_value_33,      SIGNAL(editingFinished()),
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
    
    layout->addWidget(_value_11,0,4);
    layout->addWidget(_value_12,0,5);
    layout->addWidget(_value_13,0,6);

    layout->addWidget(_value_21,1,4);
    layout->addWidget(_value_22,1,5);
    layout->addWidget(_value_23,1,6);

    layout->addWidget(_value_31,2,4);
    layout->addWidget(_value_32,2,5);
    layout->addWidget(_value_33,2,6);

    setLayout(layout);
    
    setMinimumHeight( (15+2) *3);

    doReadParams();
}

EdLevelPropertyMatrix3Field::~EdLevelPropertyMatrix3Field(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelPropertyMatrix3Field::doReadParams(void)
{
    blockSignals(true);

    keyButton()->disconnect();

    // Has Key
    if (_data->plug()) {
        keyButton()->setIcon(QIcon(":/images/key.png"));
                
        connect(    keyButton(),    SIGNAL(pressed()),
                    this,           SLOT(doKeyframePressed())    );
                    
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
    
    Matrix3 val;
    stream >> val;
    
    _value_11->setText( MoreStrings::cast_to_string(val._m11).c_str() );
    _value_12->setText( MoreStrings::cast_to_string(val._m12).c_str() );
    _value_13->setText( MoreStrings::cast_to_string(val._m13).c_str() );
    
    _value_21->setText( MoreStrings::cast_to_string(val._m21).c_str() );
    _value_22->setText( MoreStrings::cast_to_string(val._m22).c_str() );
    _value_23->setText( MoreStrings::cast_to_string(val._m23).c_str() );
    
    _value_31->setText( MoreStrings::cast_to_string(val._m31).c_str() );
    _value_32->setText( MoreStrings::cast_to_string(val._m32).c_str() );
    _value_33->setText( MoreStrings::cast_to_string(val._m33).c_str() );
    blockSignals(false);
}

void EdLevelPropertyMatrix3Field::doWriteParams(void)
{
    Matrix3 val;
    val._m11 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_11->text().toUtf8().data()));
    val._m12 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_12->text().toUtf8().data()));
    val._m13 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_13->text().toUtf8().data()));

    val._m21 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_21->text().toUtf8().data()));
    val._m22 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_22->text().toUtf8().data()));
    val._m23 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_23->text().toUtf8().data()));
    
    val._m31 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_31->text().toUtf8().data()));
    val._m32 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_32->text().toUtf8().data()));
    val._m33 = MoreStrings::cast_from_string<DTfloat>(parseParam(_value_33->text().toUtf8().data()));

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

void EdLevelPropertyMatrix3Field::doKeyframePressed (void)
{
    emit doKeyMenu( QString(_node->full_name().c_str()) + "." + _data->title().c_str(), keyButton()->mapToGlobal(QPoint(0,0)));
}

//==============================================================================
//==============================================================================

std::string EdLevelPropertyMatrix3Field::getValueOfField (void)
{
	TextBufferStream stream;
	_data->value(stream);
    return stream.buffer();
}
    
void EdLevelPropertyMatrix3Field::setValueOfField (const std::string &value)
{
	TextBufferStream stream(value);
	_data->set_value(stream);

    emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" (" + stream.buffer().c_str() + ")", _data->flags() & DATA_FLUSH_UI);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelPropertyMatrix3Field.cpp"


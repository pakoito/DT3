//==============================================================================
///	
///	File: EdSpritePropertyBoolField.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpritePropertyBoolField.hpp"
#include "EdSpriteProperties.hpp"
#include <QtCore/QFile>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>

#include "ArchiveData.hpp"
#include "StringCast.hpp"
#include "PlugNode.hpp"

//==============================================================================
//==============================================================================

EdSpritePropertyBoolField::EdSpritePropertyBoolField (QListWidget *parent, EdSpriteMainWindow *main_window, ArchiveData *data)
{
    _main_window = main_window;
    
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = RETAIN(data);
        
    QLabel *name = new QLabel(this);
    name->setMaximumWidth(130);
    name->setMinimumWidth(130);
    name->setText(_data->getTitle().captializeAndFormat().cStr());

    _value = new QCheckBox(this);
    _value->setObjectName("prop");

	connect(	_value,         SIGNAL(stateChanged(int)),
				this,           SLOT(doWriteParams())	);
    
	connect(	this,           SIGNAL(doCommand(QString, bool)),
				parent,         SLOT(onCommand(QString, bool))	);
    
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->addWidget(name,0,0);
    layout->addWidget(_value,0,1);

    setLayout(layout);
    
    setMinimumHeight(15+2);
    
    doReadParams();
}

EdSpritePropertyBoolField::~EdSpritePropertyBoolField	(void)
{
    RELEASE(_data);
}

//==============================================================================
//==============================================================================

void EdSpritePropertyBoolField::doReadParams(void)
{
    _value->blockSignals(true);
    
	TextBufferStream stream;
	_data->getValue(stream);
    _value->setCheckState( castFromString<DTboolean>(stream.getBuffer()) ? Qt::Checked : Qt::Unchecked );
    //_value->setText( stream.getBuffer().cStr() );
    _value->blockSignals(false);
}

void EdSpritePropertyBoolField::doWriteParams(void)
{
    String value;
    
    if (_value->checkState() == Qt::Checked)    value = "1";
    else                                        value = "0";
    
	TextBufferStream stream(value);
    TextBufferStream stream_prev;
    _data->getValue(stream_prev);
    
    if (stream.getBuffer() != stream_prev.getBuffer()) {
        _main_window->pushUndo();

        _data->setValue(stream);

        _main_window->keySelectedJoint();
        _main_window->fireSpriteChanged();
    }

}

//==============================================================================
//==============================================================================

#include "moc_EdSpritePropertyBoolField.cpp"


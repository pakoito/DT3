//==============================================================================
///	
///	File: EdSpritePropertyBitfieldField.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpritePropertyBitfieldField.hpp"
#include "EdSpriteProperties.hpp"
#include "EdSpriteBitfield.hpp"
#include <QtCore/QFile>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>

#include "ArchiveData.hpp"
#include "StringCast.hpp"
#include "PlugNode.hpp"

//==============================================================================
//==============================================================================

EdSpritePropertyBitfieldField::EdSpritePropertyBitfieldField (QListWidget *parent, EdSpriteMainWindow *main_window, ArchiveData *data)
{
    _main_window = main_window;
    
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = RETAIN(data);
        
    QLabel *name = new QLabel(this);
    name->setMaximumWidth(130);
    name->setMinimumWidth(130);
    name->setText(_data->getTitle().captializeAndFormat().cStr());

    _value = new EdSpriteBitfield(this);
    _value->setObjectName("prop");

	connect(	_value,         SIGNAL(stateChanged(DTushort)),
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

EdSpritePropertyBitfieldField::~EdSpritePropertyBitfieldField	(void)
{
    RELEASE(_data);
}

//==============================================================================
//==============================================================================

void EdSpritePropertyBitfieldField::doReadParams(void)
{
    _value->blockSignals(true);
    
	TextBufferStream stream;
	_data->getValue(stream);
    _value->setBitfield(castFromString<DTushort>(stream.getBuffer()));
    _value->blockSignals(false);
}

void EdSpritePropertyBitfieldField::doWriteParams(void)
{
    String value = castToString(_value->getBitfield());

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

#include "moc_EdSpritePropertyBitfieldField.cpp"


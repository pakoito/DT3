//==============================================================================
///	
///	File: EdLevelPropertyColorField.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyColorField.hpp"
#include "EdLevelColorSwatch.hpp"
#include "EdLevelPropertiesWindow.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGridLayout>

// Engine includes
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"

//==============================================================================
//==============================================================================

EdLevelPropertyColorField::EdLevelPropertyColorField (EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data)
{
    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    _data = data;
    _node = node;
    
    buildStatusButtons();

    QLabel *name = new QLabel(this);
    name->setMaximumWidth(130);
    name->setMinimumWidth(130);
    name->setText(MoreStrings::captialize_and_format(_data->title()).c_str());
    
    
    QLabel *r = new QLabel(this);
    r->setText("R");

    QLabel *g = new QLabel(this);
    g->setText("G");

    QLabel *b = new QLabel(this);
    b->setText("B");

    QLabel *a = new QLabel(this);
    a->setText("A");


    _r = new QSlider(Qt::Horizontal, this);
    _r->setMinimum(0);
    _r->setMaximum(255);
    _r->setObjectName("prop");

    _r2 = new EdLevelLineEdit(this);
    _r2->setMaximumWidth(60);

    connect(    _r,         SIGNAL(valueChanged(int)), 
                this,       SLOT(doColorChange(int)));
    connect(	_r2,        SIGNAL(editingFinished()),
				this,       SLOT(doWriteParams2())	);


    _g = new QSlider(Qt::Horizontal, this);
    _g->setMinimum(0);
    _g->setMaximum(255);
    _g->setObjectName("prop");

    _g2 = new EdLevelLineEdit(this);
    _g2->setMaximumWidth(60);
    
    connect(    _g,         SIGNAL(valueChanged(int)), 
                this,       SLOT(doColorChange(int)));
    connect(	_g2,        SIGNAL(editingFinished()),
				this,       SLOT(doWriteParams2())	);

    _b = new QSlider(Qt::Horizontal, this);
    _b->setMinimum(0);
    _b->setMaximum(255);
    _b->setObjectName("prop");

    _b2 = new EdLevelLineEdit(this);
    _b2->setMaximumWidth(60);
    
    connect(    _b,         SIGNAL(valueChanged(int)), 
                this,       SLOT(doColorChange(int)));
    connect(	_b2,        SIGNAL(editingFinished()),
				this,       SLOT(doWriteParams2())	);

    _a = new QSlider(Qt::Horizontal, this); 
    _a->setMinimum(0);
    _a->setMaximum(255);
    _a->setObjectName("prop");

    _a2 = new EdLevelLineEdit(this);
    _a2->setMaximumWidth(60);
    
    connect(    _a,         SIGNAL(valueChanged(int)), 
                this,       SLOT(doColorChange(int)));
    connect(	_a2,        SIGNAL(editingFinished()),
				this,       SLOT(doWriteParams2())	);

    _fill = new EdLevelColorSwatch();
    _fill->setMaximumWidth(50);
    _fill->setMinimumWidth(50);
    _fill->setMaximumHeight(40);
    _fill->setMinimumHeight(40);

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
    layout->addWidget(_fill,1,3,2,1, Qt::AlignHCenter | Qt::AlignVCenter);
    
    layout->addWidget(r,0,4);
    layout->addWidget(_r,0,5);
    layout->addWidget(_r2,0,6);

    layout->addWidget(g,1,4);
    layout->addWidget(_g,1,5);
    layout->addWidget(_g2,1,6);
    
    layout->addWidget(b,2,4);
    layout->addWidget(_b,2,5);
    layout->addWidget(_b2,2,6);
    
    layout->addWidget(a,3,4);
    layout->addWidget(_a,3,5);
    layout->addWidget(_a2,3,6);

    setLayout(layout);
    
    setMinimumHeight( (15+2) *4);

    doReadParams();
}

EdLevelPropertyColorField::~EdLevelPropertyColorField	(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelPropertyColorField::doColorChange (int)
{
    _fill->setBrush(QBrush(QColor(_r->value(), _g->value(), _b->value(), _a->value() )));
    _fill->update();
    
    doWriteParams();
}

//==============================================================================
//==============================================================================

void EdLevelPropertyColorField::doReadParams(void)
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
    
    Color4f val;
    stream >> val;
    
    Color4b val_b(val);
    
    _r->setValue(val_b.r_as_byte());
    _g->setValue(val_b.g_as_byte());
    _b->setValue(val_b.b_as_byte());
    _a->setValue(val_b.a_as_byte());
    
    _r2->setText( MoreStrings::cast_to_string(val.r_as_float()).c_str() );
    _g2->setText( MoreStrings::cast_to_string(val.g_as_float()).c_str() );
    _b2->setText( MoreStrings::cast_to_string(val.b_as_float()).c_str() );
    _a2->setText( MoreStrings::cast_to_string(val.a_as_float()).c_str() );

    doColorChange(0);
    blockSignals(false);
}

void EdLevelPropertyColorField::doWriteParams(void)
{
    Color4f val(Color4b( (DTubyte) _r->value(), (DTubyte) _g->value(), (DTubyte) _b->value(), (DTubyte) _a->value()));

	TextBufferStream stream;
    stream << val;

    TextBufferStream oldstream;
	_data->value(oldstream);
    
    // Only if value changed
    if (stream.buffer() != oldstream.buffer()) {
        emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" (" + stream.buffer().c_str() + ")", _data->flags() & DATA_FLUSH_UI);
    }
}

void EdLevelPropertyColorField::doWriteParams2(void)
{
    Color4f val;
    val.set_r( MoreStrings::cast_from_string<DTfloat>(_r2->text().toUtf8().data()) );
    val.set_g( MoreStrings::cast_from_string<DTfloat>(_g2->text().toUtf8().data()) );
    val.set_b( MoreStrings::cast_from_string<DTfloat>(_b2->text().toUtf8().data()) );
    val.set_a( MoreStrings::cast_from_string<DTfloat>(_a2->text().toUtf8().data()) );

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

void EdLevelPropertyColorField::doKeyframePressed (void)
{
    emit doKeyMenu( QString(_node->full_name().c_str()) + "." + _data->title().c_str(), keyButton()->mapToGlobal(QPoint(0,0)));
}

//==============================================================================
//==============================================================================

std::string EdLevelPropertyColorField::getValueOfField (void)
{
	TextBufferStream stream;
	_data->value(stream);
    return stream.buffer();
}
    
void EdLevelPropertyColorField::setValueOfField (const std::string &value)
{
	TextBufferStream stream(value);
	_data->set_value(stream);
    
    emit doCommand(QString("SetProp \"") + _node->full_name().c_str() + "." + _data->title().c_str() + "\" (" + stream.buffer().c_str() + ")", _data->flags() & DATA_FLUSH_UI);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelPropertyColorField.cpp"


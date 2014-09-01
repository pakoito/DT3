//==============================================================================
///	
///	File: EdLevelSoundBus.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelSoundBus.hpp"
#include "EdLevelLineEdit.hpp"
#include "EdLevelSoundVUMeter.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QDial>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>

// Engine includes
#include "DT3Core/Scripting/ScriptingSoundBusOut.hpp"
#include "DT3Core/Scripting/ScriptingSoundBusIn.hpp"


//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelSoundBus::EdLevelSoundBus(QWidget *parent, std::shared_ptr<ScriptingSoundBusOut> bus)
    :   QWidget   (parent)
{    
    _bus_in = NULL;
    _bus_out = bus;
    
    setMaximumWidth(100);
    setMinimumWidth(100);    

    _name = new EdLevelLineEdit(this);
    _name->setText("Foobar");
    
    _name->setMinimumHeight(16);
    _name->setMaximumHeight(16);
    _name->setMaximumWidth(96);
    _name->setMinimumWidth(96);

    _gain_left = new QDial(this);
    _gain_left->setNotchesVisible(1);
	_gain_left->setMinimum(0);
	_gain_left->setMaximum(100);
    
    _gain_left->setMinimumWidth(40);
    _gain_left->setMinimumHeight(40);
    _gain_left->setMaximumWidth(40);
    _gain_left->setMaximumHeight(40);

    _gain_left_num = new EdLevelLineEdit(this);
    _gain_left_num->setText("");


    _gain_right = new QDial(this);
    _gain_right->setNotchesVisible(1);
	_gain_right->setMinimum(0);
	_gain_right->setMaximum(100);
    
    _gain_right->setMinimumWidth(40);
    _gain_right->setMinimumHeight(40);
    _gain_right->setMaximumWidth(40);
    _gain_right->setMaximumHeight(40);
    
    _gain_right_num = new EdLevelLineEdit(this);
    _gain_right_num->setText("");
    
    QObject::connect(   _gain_left,     SIGNAL(valueChanged(int)),
                        this,           SLOT(gainKnobsChanged ()));
    QObject::connect(   _gain_left_num, SIGNAL(textChanged(QString)),
                        this,           SLOT(gainKnobsChanged()));

    QObject::connect(   _gain_right,    SIGNAL(valueChanged(int)),
                        this,           SLOT(gainKnobsChanged ()));
    QObject::connect(   _gain_right_num,SIGNAL(textChanged(QString)),
                        this,           SLOT(gainKnobsChanged()));

    QObject::connect(   _name,          SIGNAL(textChanged(QString)),
                        this,           SLOT(nameChanged()));

    
    _gain_left->setValue(100 * _bus_out->gain_left());
    _gain_right->setValue(100 * _bus_out->gain_right());
    
    
    _list = new QListWidget(this);
    //list->setStyleSheet(style_sheet);
    _list->setFont(QFont("Arial", 10));
    
    QObject::connect(   _list,          SIGNAL(itemClicked(QListWidgetItem *)),
                        this,           SLOT(listChanged (QListWidgetItem *)));

    
    _vu_meter = new EdLevelSoundVUMeter(this);


    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(2,2,2,2);
    layout->setSpacing(4);
    layout->addWidget(_name, 0, 0, 1, 2, Qt::AlignCenter );

    layout->addWidget(_gain_left, 1, 0, Qt::AlignCenter );
    layout->addWidget(_gain_right, 1, 1, Qt::AlignCenter );

    layout->addWidget(_gain_left_num, 2, 0, Qt::AlignCenter );
    layout->addWidget(_gain_right_num, 2, 1, Qt::AlignCenter );

    layout->setRowStretch(3,1);
    layout->addWidget(_list, 3, 0, 1, 2 );
    layout->addWidget(_vu_meter, 4, 0, 1, 2, Qt::AlignCenter );
    setLayout(layout);

}

//==============================================================================
//==============================================================================

void EdLevelSoundBus::nameChanged ()
{
    emit doUndoBlock();

    if (_bus_in) {
        emit doCommand(QString("SetProp ") + _bus_in->full_name().c_str() + "._name " + _name->text());
    }
}

void EdLevelSoundBus::gainKnobsChanged ()
{
    emit doUndoBlock();

    if (sender() == _gain_left) {
        _gain_left_num->setText(QString::number(_gain_left->value()) );
        emit doCommand(QString("SetProp ") + _bus_out->full_name().c_str() + ".Gain_Left " + QString::number(_gain_left->value() / 100.0F));
    }
    
    if (sender() == _gain_right) {
        _gain_right_num->setText(QString::number(_gain_right->value()) );
        emit doCommand(QString("SetProp ") + _bus_out->full_name().c_str() + ".Gain_Right " + QString::number(_gain_right->value() / 100.0F));
    }

    if (sender() == _gain_left_num) {
        _gain_left->setValue(_gain_left_num->text().toInt());
        emit doCommand(QString("SetProp ") + _bus_out->full_name().c_str() + ".Gain_Left " + QString::number(_gain_left->value() / 100.0F));
    }
    
    if (sender() == _gain_right_num) {
        _gain_right->setValue(_gain_right_num->text().toInt());    
        emit doCommand(QString("SetProp ") + _bus_out->full_name().c_str() + ".Gain_Right " + QString::number(_gain_right->value() / 100.0F));
    }
}

void EdLevelSoundBus::listChanged (QListWidgetItem * item)
{
    emit doCommand(QString("Select \"") + item->text() + "\"");
}

//==============================================================================
//==============================================================================

void EdLevelSoundBus::resizeEvent (QResizeEvent *event)
{

}

void EdLevelSoundBus::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	draw(&painter);
}

void EdLevelSoundBus::draw(QPainter *painter)
{    
	painter->setRenderHint(QPainter::Antialiasing, false);
    
    painter->setPen(QPen(QColor(40,40,40,255),1));
	painter->setBrush(QBrush(QColor(100,100,100,255)));
	painter->drawRect(rect());
}

//==============================================================================
//==============================================================================

void EdLevelSoundBus::updateBus (void)
{
    _vu_meter->setLevels(_bus_out->level_left(), _bus_out->level_right());
}

void EdLevelSoundBus::updateProperties (void)
{
    if (_bus_in) {
        _name->setText(_bus_in->name().c_str());
    } else {
        _name->setText("NoInputs");
    }
        
    _gain_left->setValue(_bus_out->gain_left() * 100);
    _gain_right->setValue(_bus_out->gain_right() * 100);
}

void EdLevelSoundBus::updateList (void)
{
    _list->clear();
    
    std::shared_ptr<PlugNode> node = _bus_out;
    
    // Scan incoming nodes and add them to the list
    while (node) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(node->name().c_str());
        _list->insertItem(0,item);
        
        if (node->isa(ScriptingSoundBusIn::kind())) {
            _bus_in = checked_static_cast<ScriptingSoundBusIn>(node);
            break;
        }

        PlugBase *plug = node->plug_by_name("Sound_Packet_In");
        if (!plug)
            break;
            
        if (!plug->has_incoming_connection())
            break;
            
        node = checked_static_cast<PlugNode>(plug->incoming_connection()->owner()->shared_from_this());
    
    }
    
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelSoundBus.cpp"


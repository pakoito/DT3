//==============================================================================
///	
///	File: EdLevelSoundVUMeter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelSoundVUMeter.hpp"
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtCore/QFile>
#include <QtWidgets/QDial>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtWidgets/QGroupBox>

// Engine includes
#include "DT3Core/Scripting/ScriptingSoundBusIn.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelSoundVUMeter::EdLevelSoundVUMeter(QWidget *parent)
    :   QWidget   (parent)
{    

    setMaximumWidth(80);
    setMinimumWidth(80);

    setMaximumHeight(220);
    setMinimumHeight(220);

    setLevels(1.0F,1.0F);
}

//==============================================================================
//==============================================================================

void EdLevelSoundVUMeter::resizeEvent (QResizeEvent *event)
{

}

void EdLevelSoundVUMeter::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	draw(&painter);
}

void EdLevelSoundVUMeter::draw(QPainter *painter)
{    
	painter->setRenderHint(QPainter::Antialiasing, false);
    
    painter->setPen(QPen(QColor(40,40,40,255),1));
	painter->setBrush(QBrush(QColor(80,80,80,255)));
	painter->drawRect(rect());
    
    const int NUM_SEPARATORS = 20;
    const int SPACING = 2;
    const int WIDTH = 20;
    const int HEIGHT = 8;
    
    for (DTuint i = 0; i < NUM_SEPARATORS; ++i) {
        int ypos = 200 - i * (SPACING + HEIGHT);
        
        if (_left > (i/static_cast<DTfloat>(NUM_SEPARATORS-1)) ) {
            if (i < 15)         painter->setBrush(QBrush(QColor(0,230,0,255)));
            else if (i < 18)    painter->setBrush(QBrush(QColor(230,230,0,255)));
            else                painter->setBrush(QBrush(QColor(240,0,0,255)));
        } else {
            painter->setBrush(QBrush(QColor(20,20,20,255)));
        }
        
        painter->drawRect(QRect(10,ypos, WIDTH, HEIGHT));
        
        
        if (_right > (i/static_cast<DTfloat>(NUM_SEPARATORS-1)) ) {
            if (i < 15)         painter->setBrush(QBrush(QColor(0,230,0,255)));
            else if (i < 18)    painter->setBrush(QBrush(QColor(230,230,0,255)));
            else                painter->setBrush(QBrush(QColor(240,0,0,255)));
        } else {
            painter->setBrush(QBrush(QColor(20,20,20,255)));
        }
       
        
        painter->drawRect(QRect(50,ypos, WIDTH, HEIGHT));

    }
    
    
}

//==============================================================================
//==============================================================================

void EdLevelSoundVUMeter::setLevels (float left, float right)
{
    _left = left;
    _right = right;
    
    update();
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelSoundVUMeter.cpp"


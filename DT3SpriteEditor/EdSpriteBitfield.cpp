//==============================================================================
///	
///	File: EdSpriteBitfield.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteBitfield.hpp"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include <cmath>

//==============================================================================
//==============================================================================

EdSpriteBitfield::EdSpriteBitfield(QWidget *parent)
    :   QWidget         (parent),
        _bitfield       (0),
        _font           ("Arial", 7)
{

}

//==============================================================================
//==============================================================================

void EdSpriteBitfield::paintEvent (QPaintEvent *event)
{
	QPainter painter(this);
    draw(&painter);
}

void EdSpriteBitfield::draw(QPainter *painter)
{

    QRect r = rect();
    DTint w = r.width() / 16;
    
    for (DTint i = 0; i < 16; ++i) {
        DTboolean bit = _bitfield & (1 << i);
        
        QRect bitrect(r.left() + i*w, r.top()+2, w, r.height()-1-4);

        // Rect
        if (bit) {
            painter->setPen(QPen(QColor(200,200,200,255),0));
            painter->setBrush(QBrush(QColor(150,150,150,255)));
        } else {
            painter->setPen(QPen(QColor(200,200,200,255),0));
            painter->setBrush(QBrush(QColor(50,50,50,255)));
        }
        
        painter->drawRect(bitrect);
        
        // Text
        QString n;
        n.setNum(i);
    
        if (bit) {
            painter->setPen(QPen(QColor(50,50,50,255),0));
        } else {
            painter->setPen(QPen(QColor(200,200,200,255),0));
        }

        painter->setFont(_font);
        painter->drawText(bitrect, Qt::AlignHCenter | Qt::AlignVCenter, n);
    }
}

//==============================================================================
//==============================================================================

void EdSpriteBitfield::mousePressEvent (QMouseEvent *event)
{

}

void EdSpriteBitfield::mouseMoveEvent (QMouseEvent *event)
{

}

void EdSpriteBitfield::mouseReleaseEvent (QMouseEvent *event)
{
    QRect r = rect();
    DTint w = r.width() / 16;

    for (DTint i = 0; i < 16; ++i) {        
        QRect bitrect(r.left() + i*w, r.top()+2, w, r.height()-1-4);

        if (bitrect.contains(event->pos())) {
            
            // Flip the bit
            _bitfield ^= (1 << i);
            
            emit stateChanged(_bitfield);
            update();
            return;
        }
    }
}

//==============================================================================
//==============================================================================

#include "moc_EdSpriteBitfield.cpp"


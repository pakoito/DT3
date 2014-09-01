//==============================================================================
///	
///	File: EdLevelScriptPlugConnection.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptPlugConnection.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

const float EdLevelScriptPlugConnection::SHADOW_OFFSET_X = 5.0F;
const float EdLevelScriptPlugConnection::SHADOW_OFFSET_Y = 5.0F;
const float EdLevelScriptPlugConnection::BEZIER_SIZE = 50.0F;
const float EdLevelScriptPlugConnection::PAD = 3.0F;
const float EdLevelScriptPlugConnection::OFFSET = 5.0F;

//==============================================================================
//==============================================================================

EdLevelScriptPlugConnection::EdLevelScriptPlugConnection	(   EdLevelScriptNodeStandard *node1, PlugBase *plug1,
                                                                EdLevelScriptNodeStandard *node2, PlugBase *plug2)
    :   EdLevelScriptConnection(node1, node2),
        _font("Arial", 9),
        _fm(_font)
{
    _plug1 = plug1;
    _plug2 = plug2;
}

//==============================================================================
//==============================================================================

std::string EdLevelScriptPlugConnection::getValue (void) const
{
    std::string value = _plug1->value_as_string();
    
    if (value.size() > 50)
        value = value.substr(0,50) + "...";
        
    return value;
}

//==============================================================================
//==============================================================================

QRectF EdLevelScriptPlugConnection::getTextRect (void) const
{
    if (_node1 && _plug1) {
        // Draw value
        std::string type = _plug1->plug_type();
        std::string value = getValue();
        
        QPoint halfway = (_p1_local + _p2_local) / 2;
        
        float type_width = _fm.width(type.c_str());
        float value_width = _fm.width(value.c_str());
        float max_width = std::max(type_width, value_width);
        
        float height = _fm.height();
        
        return QRectF(halfway.x() + -max_width/2 - PAD, halfway.y() - height - PAD, max_width + 2*PAD, 2 * height + 2*PAD);
    } else {
        QPoint halfway = (_p1_local + _p2_local) / 2;
        return QRectF(halfway.x(), halfway.y(), 0,0);
    }
}

//==============================================================================
//==============================================================================

QRectF EdLevelScriptPlugConnection::boundingRect (void) const
{
    DTboolean show_values = MoreStrings::cast_from_string<DTboolean>( Globals::global("ED_SHOW_VALUES") );
    if (show_values) {
        QRectF old_bounds = EdLevelScriptConnection::boundingRect();
        QRectF text_bounds = getTextRect();
        
        text_bounds.adjust(0, 0, SHADOW_OFFSET_X, SHADOW_OFFSET_Y);
        
        return text_bounds.united(old_bounds);
    } else {
        QRectF old_bounds = EdLevelScriptConnection::boundingRect();
        return old_bounds;
    }
}

//==============================================================================
//==============================================================================

void EdLevelScriptPlugConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	
    painter->setPen(QPen(QColor(0,0,0,64),3));
	painter->setBrush(Qt::NoBrush);

	QPainterPath path0;
	path0.moveTo(	_p1_local.x() + SHADOW_OFFSET_X + OFFSET,                   _p1_local.y() + SHADOW_OFFSET_Y);
	path0.cubicTo (	_p1_local.x() + SHADOW_OFFSET_X + OFFSET + BEZIER_SIZE,     _p1_local.y() + SHADOW_OFFSET_Y, 
					_p2_local.x() + SHADOW_OFFSET_X - OFFSET - BEZIER_SIZE,     _p2_local.y() + SHADOW_OFFSET_Y, 
					_p2_local.x() + SHADOW_OFFSET_X - OFFSET,                   _p2_local.y() + SHADOW_OFFSET_Y );
	painter->drawPath(path0);

    if (_plug1->is_single_output()) {
    
        if (isSelected())
            painter->setPen(QPen(QColor(170,100,100,255),3));
        else
            painter->setPen(QPen(QColor(220,150,150,255),3));

    } else {
    
        if (isSelected())
            painter->setPen(QPen(QColor(100,170,100,255),3));
        else
            painter->setPen(QPen(QColor(150,220,150,255),3));
            
    }

	painter->setBrush(Qt::NoBrush);

	QPainterPath path1;
	path1.moveTo(	_p1_local.x() + OFFSET,                  _p1_local.y());
	path1.cubicTo (	_p1_local.x() + OFFSET + BEZIER_SIZE,    _p1_local.y(), 
					_p2_local.x() - OFFSET - BEZIER_SIZE,    _p2_local.y(), 
					_p2_local.x() - OFFSET,                  _p2_local.y() );
	painter->drawPath(path1);
    
    DTboolean show_values = MoreStrings::cast_from_string<DTboolean>( Globals::global("ED_SHOW_VALUES") );
    if (_node1 && _plug1 && show_values) {
        // Draw value
        std::string type = _plug1->plug_type();
        std::string value = getValue();        
            
        float height = _fm.height(); 
        
        QRectF text_rect = getTextRect();
        QRectF shadow_rect = text_rect.translated(SHADOW_OFFSET_X, SHADOW_OFFSET_Y);
            
        // Drop shadow
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(QColor(0,0,0,64)));
        painter->drawRoundedRect(shadow_rect.toRect(), 5, 5);

        // Draw box
        painter->setPen(QPen(QColor(40,40,40,255),1));
        painter->setBrush(QBrush(QColor(181,181,255,255)));
        painter->drawRoundedRect(text_rect.toRect(), 5, 5);
        
        painter->setPen(QPen(QColor(40,40,40,255),1));
        painter->setFont(_font);
        painter->drawText( text_rect.translated(0.0F, -height/2).toRect(), Qt::AlignHCenter | Qt::AlignVCenter, type.c_str());
        painter->drawText( text_rect.translated(0.0F, +height/2).toRect(), Qt::AlignHCenter | Qt::AlignVCenter, value.c_str());
    }

}

//==============================================================================
//==============================================================================

void EdLevelScriptPlugConnection::readjustBounds()
{
    if (!_node1 || !_node2)
        return;

    prepareGeometryChange();

    QPointF location1 = _node1->getPlugLocation(_plug1, false);
    QPointF location2 = _node2->getPlugLocation(_plug2, true);
        
    _scene_location1 = _node1->mapToScene(location1).toPoint();
    _scene_location2 = _node2->mapToScene(location2).toPoint();
    
	float x_min = std::min(_scene_location1.x(), _scene_location2.x());
	float y_min = std::min(_scene_location1.y(), _scene_location2.y());

	setPos(x_min, y_min);

	_p1_local = mapFromScene( QPointF(_scene_location1.x(), _scene_location1.y()) ).toPoint();
	_p2_local = mapFromScene( QPointF(_scene_location2.x(), _scene_location2.y()) ).toPoint();
}

//==============================================================================
//==============================================================================

//#include "moc_EdLevelScriptPlugConnection.cpp"


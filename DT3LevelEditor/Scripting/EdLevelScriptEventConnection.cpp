//==============================================================================
///	
///	File: EdLevelScriptEventConnection.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptEventConnection.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes

//==============================================================================
//==============================================================================

const float EdLevelScriptEventConnection::SHADOW_OFFSET_X = 5.0F;
const float EdLevelScriptEventConnection::SHADOW_OFFSET_Y = 5.0F;
const float EdLevelScriptEventConnection::BEZIER_SIZE = 50.0F;
const float EdLevelScriptEventConnection::OFFSET = 5.0F;

//==============================================================================
//==============================================================================

EdLevelScriptEventConnection::EdLevelScriptEventConnection	(   EdLevelScriptNodeStandard *node1, Event *event1,
                                                                EdLevelScriptNodeStandard *node2, Event *event2)
    :   EdLevelScriptConnection(node1, node2)
{
    _event1 = event1;
    _event2 = event2;
}

//==============================================================================
//==============================================================================

void EdLevelScriptEventConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

	if (isSelected())
		painter->setPen(QPen(QColor(100,100,170),3));
	else
		painter->setPen(QPen(QColor(150,150,220),3));

	painter->setBrush(Qt::NoBrush);

	QPainterPath path1;
	path1.moveTo(	_p1_local.x() + OFFSET, _p1_local.y());
	path1.cubicTo (	_p1_local.x() + OFFSET + BEZIER_SIZE, _p1_local.y(), 
					_p2_local.x() - OFFSET - BEZIER_SIZE, _p2_local.y(), 
					_p2_local.x() - OFFSET, _p2_local.y() );
	painter->drawPath(path1);
}

//==============================================================================
//==============================================================================

void EdLevelScriptEventConnection::readjustBounds()
{
    prepareGeometryChange();
    
    QPointF location1 = _node1->getEventLocation(_event1, false);
    QPointF location2 = _node2->getEventLocation(_event2, true);
        
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

//#include "moc_EdLevelScriptEventConnection.cpp"


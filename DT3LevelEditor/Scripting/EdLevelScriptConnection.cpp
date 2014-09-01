//==============================================================================
///	
///	File: EdLevelScriptConnection.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptConnection.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes

//==============================================================================
//==============================================================================

const float EdLevelScriptConnection::SHADOW_OFFSET_X = 5.0F;
const float EdLevelScriptConnection::SHADOW_OFFSET_Y = 5.0F;
const float EdLevelScriptConnection::BEZIER_SIZE = 50.0F;
const float EdLevelScriptConnection::OFFSET = 5.0F;

//==============================================================================
//==============================================================================

EdLevelScriptConnection::EdLevelScriptConnection(   EdLevelScriptNodeStandard *node1,
                                                    EdLevelScriptNodeStandard *node2)
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
    
    _node1 = node1;
    _node2 = node2;
    
    _node1->addConnection(this);
    _node2->addConnection(this);
}

EdLevelScriptConnection::~EdLevelScriptConnection()
{
    if (_node1) _node1->removeConnection(this);
    if (_node2) _node2->removeConnection(this);
}

//==============================================================================
//==============================================================================

QRectF EdLevelScriptConnection::boundingRect(void) const
{
	float x_min = std::min(_p1_local.x(), _p2_local.x());
	float x_max = std::max(_p1_local.x(), _p2_local.x());
	float y_min = std::min(_p1_local.y(), _p2_local.y());
	float y_max = std::max(_p1_local.y(), _p2_local.y());

	return QRectF(  -BEZIER_SIZE, 
                    -BEZIER_SIZE, 
                    x_max-x_min+SHADOW_OFFSET_X+2.0F*BEZIER_SIZE, 
                    y_max-y_min+SHADOW_OFFSET_Y+2.0F*BEZIER_SIZE);
}

//==============================================================================
//==============================================================================

QPainterPath EdLevelScriptConnection::shape(void) const
{
	QPainterPath path;
	QPainterPathStroker stroke;

	stroke.setWidth	(8.0F);

	path.moveTo		(   _p1_local.x() + OFFSET, _p1_local.y());
	path.cubicTo	(	_p1_local.x() + OFFSET + BEZIER_SIZE, _p1_local.y(), 
						_p2_local.x() - OFFSET - BEZIER_SIZE, _p2_local.y(), 
						_p2_local.x() - OFFSET, _p2_local.y() );

	return stroke.createStroke(path);
}

//==============================================================================
//==============================================================================

void EdLevelScriptConnection::removeLink (EdLevelScriptNodeStandard *node)
{
    if (_node1 == node) _node1 = NULL;
    if (_node2 == node) _node2 = NULL;
}

//==============================================================================
//==============================================================================

//#include "moc_EdLevelScriptPlugConnection.cpp"


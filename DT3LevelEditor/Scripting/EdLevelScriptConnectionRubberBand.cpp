//==============================================================================
///	
///	File: EdLevelScriptConnectionRubberBand.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptConnectionRubberBand.hpp"
#include "EdLevelMainWindow.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes

//==============================================================================
//==============================================================================

const float EdLevelScriptConnectionRubberBand::SHADOW_OFFSET_X = 5.0F;
const float EdLevelScriptConnectionRubberBand::SHADOW_OFFSET_Y = 5.0F;
const float EdLevelScriptConnectionRubberBand::BEZIER_SIZE = 50.0F;
const float EdLevelScriptConnectionRubberBand::OFFSET = 5.0F;

//==============================================================================
//==============================================================================

EdLevelScriptConnectionRubberBand::EdLevelScriptConnectionRubberBand(void)
{
	
}

//==============================================================================
//==============================================================================

void EdLevelScriptConnectionRubberBand::paintEvent (QPaintEvent *event)
{
	QPainter painter(this);
        
	painter.setRenderHint(QPainter::Antialiasing, true);
	
    painter.setPen(QPen(QColor(50,255,50,128),3,Qt::DotLine));
	painter.setBrush(Qt::NoBrush);

    if (_reverse_spline) {
        QPainterPath path1;
        path1.moveTo(	_p0_local.x() + OFFSET, _p0_local.y());
        path1.cubicTo (	_p0_local.x() + OFFSET - BEZIER_SIZE, _p0_local.y(), 
                        _p1_local.x() - OFFSET + BEZIER_SIZE, _p1_local.y(), 
                        _p1_local.x() - OFFSET, _p1_local.y() );
        painter.drawPath(path1);
    } else {
        QPainterPath path1;
        path1.moveTo(	_p0_local.x() + OFFSET, _p0_local.y());
        path1.cubicTo (	_p0_local.x() + OFFSET + BEZIER_SIZE, _p0_local.y(), 
                        _p1_local.x() - OFFSET - BEZIER_SIZE, _p1_local.y(), 
                        _p1_local.x() - OFFSET, _p1_local.y() );
        painter.drawPath(path1);
    }
    
}

void EdLevelScriptConnectionRubberBand::setEndpoints (const QPoint &p0, const QPoint &p1, bool reverse_spline)
{
    _p0 = p0;
    _p1 = p1;
    _reverse_spline = reverse_spline;
    
	float x_min = std::min(_p0.x(), _p1.x());
	float x_max = std::max(_p0.x(), _p1.x());
	float y_min = std::min(_p0.y(), _p1.y());
	float y_max = std::max(_p0.y(), _p1.y());

	resize(x_max-x_min+2.0F*BEZIER_SIZE, y_max-y_min+2.0F*BEZIER_SIZE);
	move(x_min-BEZIER_SIZE, y_min-BEZIER_SIZE);

	_p0_local = mapFromParent(_p0);
	_p1_local = mapFromParent(_p1);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelScriptConnectionRubberBand.cpp"


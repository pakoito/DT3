//==============================================================================
///	
///	File: EdLevelScriptSelectionRubberBand.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptSelectionRubberBand.hpp"
#include "EdLevelMainWindow.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes

//==============================================================================
//==============================================================================

EdLevelScriptSelectionRubberBand::EdLevelScriptSelectionRubberBand(void)
{
	
}

//==============================================================================
//==============================================================================

void EdLevelScriptSelectionRubberBand::paintEvent (QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	
    painter.setPen(QPen(QColor(53,120,255,255),3));
	painter.setBrush(QBrush(QColor(0,0,0,50)));

	painter.drawRoundedRect(_p0_local.x(),_p0_local.y(), _p1_local.x(),_p1_local.y(), 5, 5);
}

void EdLevelScriptSelectionRubberBand::setEndpoints (const QPoint &p0, const QPoint &p1)
{
	float x_min = std::min(p0.x(), p1.x());
	float x_max = std::max(p0.x(), p1.x());
	float y_min = std::min(p0.y(), p1.y());
	float y_max = std::max(p0.y(), p1.y());

	resize(x_max-x_min, y_max-y_min);
	move(x_min, y_min);

	_p0_local = mapFromParent(QPoint(x_min,y_min));
	_p1_local = mapFromParent(QPoint(x_max,y_max));
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelScriptSelectionRubberBand.cpp"


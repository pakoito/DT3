//==============================================================================
///	
///	File: EdLevelColorSwatch.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelColorSwatch.hpp"
#include "EdLevelMainWindow.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes
#include <cmath>

//==============================================================================
//==============================================================================

EdLevelColorSwatch::EdLevelColorSwatch(void)
{
	_brush = QBrush(QColor(0,0,0,255));
}

//==============================================================================
//==============================================================================

void EdLevelColorSwatch::paintEvent (QPaintEvent *event)
{
	QPainter painter(this);
    painter.setPen(Qt::NoPen);
	painter.setBrush(_brush);
	painter.drawRect(rect());

}

//==============================================================================
//==============================================================================

#include "moc_EdLevelColorSwatch.cpp"


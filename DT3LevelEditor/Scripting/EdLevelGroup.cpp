//==============================================================================
///	
///	File: EdLevelGroup.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelGroup.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes
#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

const float EdLevelGroup::SHADOW_OFFSET_X = 5.0F;
const float EdLevelGroup::SHADOW_OFFSET_Y = 5.0F;

//==============================================================================
//==============================================================================

EdLevelGroup::EdLevelGroup(std::shared_ptr<Group> group)
    :   _title_font		("Arial", 15)
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	//setFlag(QGraphicsItem::ItemIsMovable);
    
    _group = group;
}

EdLevelGroup::~EdLevelGroup(void)
{

}

void EdLevelGroup::setBoundingRect (const QRectF &rect)
{
    prepareGeometryChange();

    setPos(rect.x(), rect.y());
    
    _bounding_rect = rect;
    _bounding_rect.translate(-_bounding_rect.x(), -_bounding_rect.y());
}

//QPainterPath EdLevelGroup::shape (void) const
//{
//    return QPainterPath();
//}

//==============================================================================
//==============================================================================

void EdLevelGroup::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{        
	painter->setRenderHint(QPainter::Antialiasing, true);
	
    const DTint TITLE_HEIGHT = 20;
    
    Color4f c = _group->group_color();
    
	if (isSelected()) {
		painter->setPen(QPen(QColor(53,120,255,255),3,Qt::SolidLine));
	} else {
        painter->setPen(QPen(QColor(50,50,50,64),3,Qt::SolidLine));
	}

	painter->setClipping (true);

	painter->setClipRect(QRectF(0,TITLE_HEIGHT,_bounding_rect.width(), _bounding_rect.height() - TITLE_HEIGHT));
	painter->setBrush(QBrush(QColor(    MoreMath::max(0,c.r_as_byte()-60),
                                        MoreMath::max(0,c.g_as_byte()-60),
                                        MoreMath::max(0,c.b_as_byte()-60),
                                        64)));
	painter->drawRoundedRect(_bounding_rect, 5, 5);

	painter->setClipRect(QRectF(0,0,_bounding_rect.width(), TITLE_HEIGHT));
	painter->setBrush(QBrush(QColor(    MoreMath::max(0,c.r_as_byte()-30),
                                        MoreMath::max(0,c.g_as_byte()-30),
                                        MoreMath::max(0,c.b_as_byte()-30),
                                        64)));
	painter->drawRoundedRect(_bounding_rect, 5, 5);

	painter->setPen(QPen(QColor(40,40,40,255),1));
	painter->setFont ( _title_font);
	painter->drawText( QRectF(10,0,_bounding_rect.width(), TITLE_HEIGHT), Qt::AlignLeft | Qt::AlignVCenter, _group->name().c_str() );

	painter->setClipping (false);

}

//==============================================================================
//==============================================================================

bool EdLevelGroup::checkClick (const QPointF &scene_pos, const QPointF &global_pos)
{
    if ( _bounding_rect.contains(mapFromScene(scene_pos)) ) 
        return true;

    return false;
}

bool EdLevelGroup::handleClick (const QPointF &scene_pos, const QPointF &global_pos)
{
    return false;
}

//==============================================================================
//==============================================================================

//#include "moc_EdLevelScriptPlugConnection.cpp"


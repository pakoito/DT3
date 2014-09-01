//==============================================================================
///	
///	File: EdLevelScriptNodeStandard.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptNodeStandard.hpp"
#include "EdLevelScriptConnection.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Objects/PlaceableObject.hpp"
#include DT3_HAL_INCLUDE_PATH
#include <map>
#include <set>

//==============================================================================
//==============================================================================

const float EdLevelScriptNodeStandard::TITLE_HEIGHT = 12.0F;
const float EdLevelScriptNodeStandard::ITEM_HEIGHT = 12.0F;
const float EdLevelScriptNodeStandard::ITEM_PAD = 7.0F;
const float EdLevelScriptNodeStandard::WIDTH = 150.0F;
const float EdLevelScriptNodeStandard::CONNECTOR_RADIUS = 4.0F;

const float EdLevelScriptNodeStandard::SHADOW_OFFSET_X = 5.0F;
const float EdLevelScriptNodeStandard::SHADOW_OFFSET_Y = 5.0F;

const int EdLevelScriptNodeStandard::ICON_SIZE = 10;
const int EdLevelScriptNodeStandard::ICON_PAD = 5;

//==============================================================================
//==============================================================================

namespace DT3 {
    extern std::map<std::string,std::set<std::string>>&	component_map	(void);
}

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelScriptNodeStandard::EdLevelScriptNodeStandard(std::shared_ptr<WorldNode> node)
	:	_width			(WIDTH),
		_height			(TITLE_HEIGHT),
		_input_width	(0.0F),
		_output_width	(0.0F),
		_title_font		("Arial", 9),
		_item_font		("Arial", 9)
{
    _node = node;
    
    //
    // Configure Item
    //
    
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    
    //setCacheMode(QGraphicsItem::ItemCoordinateCache);
                
    // Set defaults
    Vector3 pos = _node->node_position();
    setPos(pos.x, pos.y);
		
	layoutNode();
}

EdLevelScriptNodeStandard::~EdLevelScriptNodeStandard  (void)
{
    // Update connections
    FOR_EACH (i,_connections) {
        (**i).removeLink(this);
    }
}

//==============================================================================
//==============================================================================

bool EdLevelScriptNodeStandard::getPlugAtLocation (QPointF pos, QPoint &actual_pos, bool &is_input, PlugBase *&plug)
{
    const int DIST = 8;
    
    FOR_EACH (p_iter,_plugs) {
        if ( (pos - p_iter->_input_location).manhattanLength() < DIST) {
            actual_pos = p_iter->_input_location;
            plug = p_iter->_plug;
            is_input = true;
            return true;
        }

        if ( (pos - p_iter->_output_location).manhattanLength() < DIST) {
            actual_pos = p_iter->_output_location;
            plug = p_iter->_plug;
            is_input = false;
            return true;
        }
    }
    
    plug = NULL;
    return false;
}

bool EdLevelScriptNodeStandard::getEventAtLocation (QPointF pos, QPoint &actual_pos, bool &is_input, Event *&event)
{
    const int DIST = 8;
        
    FOR_EACH (e_iter,_events) {
        if ( (pos - e_iter->_input_location).manhattanLength() < DIST) {
            actual_pos = e_iter->_input_location;
            event = e_iter->_event;
            is_input = true;
            return true;
        }

        if ( (pos - e_iter->_output_location).manhattanLength() < DIST) {
            actual_pos = e_iter->_output_location;
            event = e_iter->_event;
            is_input = false;
            return true;
        }
    }

    event = NULL;
    return false;
}

//==============================================================================
//==============================================================================

QPoint EdLevelScriptNodeStandard::getPlugLocation (PlugBase *plug, bool as_input)
{
    FOR_EACH (p_iter,_plugs) {
    
        if (p_iter->_plug == plug) {
            if (as_input)   return p_iter->_input_location;
            else            return p_iter->_output_location;
        }
    }
    
    if (as_input)
        return QPoint(0.0F,TITLE_HEIGHT*0.5F);
    else
        return QPoint(_width,TITLE_HEIGHT*0.5F);
}

QPoint EdLevelScriptNodeStandard::getEventLocation (Event *event, bool as_input)
{
    FOR_EACH (e_iter,_events) {
    
        if (e_iter->_event == event) {
            if (as_input)   return e_iter->_input_location;
            else            return e_iter->_output_location;
        }
    }
    
    return QPoint();
}

//==============================================================================
//==============================================================================

QRectF EdLevelScriptNodeStandard::getNodeActionRect (void)
{
    return QRectF(_width-ICON_SIZE-ICON_PAD, (TITLE_HEIGHT-ICON_SIZE)/2, ICON_SIZE, ICON_SIZE);
}

QRectF EdLevelScriptNodeStandard::getNodeShrinkRect (void)
{
    return QRectF(_width-2*ICON_SIZE-2*ICON_PAD, (TITLE_HEIGHT-ICON_SIZE)/2, ICON_SIZE, ICON_SIZE);
}

QRectF EdLevelScriptNodeStandard::getNodeEditorRect (void)
{
    return QRectF(ICON_SIZE+ICON_PAD*2, (TITLE_HEIGHT-ICON_SIZE)/2, ICON_SIZE, ICON_SIZE);
}

QRectF EdLevelScriptNodeStandard::getTitleActionRect (const QComponentTitle& title)
{
    return QRectF(_width-ICON_SIZE-ICON_PAD, title._position * ITEM_HEIGHT + (TITLE_HEIGHT-ICON_SIZE)/2 + TITLE_HEIGHT, ICON_SIZE, ICON_SIZE);
}

//==============================================================================
//==============================================================================

QVariant EdLevelScriptNodeStandard::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {        
        layoutConnections();
    }
    return QGraphicsItem::itemChange(change, value);
}

//==============================================================================
//==============================================================================

void EdLevelScriptNodeStandard::addConnection (EdLevelScriptConnection* connection)
{
    auto i = std::find(_connections.begin(), _connections.end(), connection);
    if (i == _connections.end()) {
        _connections.push_back(connection);
    }
}

void EdLevelScriptNodeStandard::removeConnection (EdLevelScriptConnection* connection)
{
    auto i = std::find(_connections.begin(), _connections.end(), connection);
    if (i != _connections.end()) {
        _connections.erase(i);
    }
}

//==============================================================================
//==============================================================================

bool EdLevelScriptNodeStandard::checkClick (const QPointF &scene_pos, const QPointF &global_pos)
{
    if ( getNodeActionRect().contains(mapFromScene(scene_pos)) ) 
        return true;

    if ( getNodeShrinkRect().contains(mapFromScene(scene_pos)) ) 
        return true;

    if ( getNodeEditorRect().contains(mapFromScene(scene_pos)) )
        return true;
    
	for (int i = 0; i < _titles.size(); ++i) {
        if ( getTitleActionRect(_titles[i]).contains(mapFromScene(scene_pos)) )
            return true;
	}

    return false;
}

bool EdLevelScriptNodeStandard::handleClick (const QPointF &scene_pos, const QPointF &global_pos)
{
    if ( getNodeActionRect().contains(mapFromScene(scene_pos)) ) {
        emit doNodeContextMenu(_node, global_pos);
        return true;
    }

    if ( getNodeShrinkRect().contains(mapFromScene(scene_pos)) ) {
        _node->set_node_collapsed( !_node->node_collapsed() );
        
        layoutNode();
        
        return true;
    }

    if ( getNodeEditorRect().contains(mapFromScene(scene_pos)) ) {

        HAL::launch_editor(FilePath(_node->file()));
        return true;
    }
    
	for (int i = 0; i < _titles.size(); ++i) {
        if ( getTitleActionRect(_titles[i]).contains(mapFromScene(scene_pos)) ) {
            emit doComponentContextMenu(_titles[i]._component, global_pos);
            return true;
        }
	}

    return false;
}

//==============================================================================
//==============================================================================

void EdLevelScriptNodeStandard::addPlugsAndEvents(int &input_pos, int &output_pos, std::shared_ptr<PlugNode> node)
{
	QFontMetrics fm(_item_font);
    
    //
    // Add component title
    //
    
    if (node->isa(ComponentBase::kind())) {
        std::shared_ptr<ComponentBase> component = checked_static_cast<ComponentBase>(node);
    
        int pos = std::max(input_pos,output_pos);
        
        QComponentTitle title;
        title._position = pos++;
        title._title = component->name().c_str();
        title._component = component;
                
        input_pos = output_pos = pos;
        
        _titles.push_front(title);
    }
    
    //
    // Do plugs and events on object
    //
    
    std::list<PlugBase*> plugs;
    node->all_plugs (plugs);

    std::list<Event*> events;
    node->all_events (events);

	// Plugs first
    FOR_EACH (p,plugs) {
        if ( (*p)->is_no_draw())
            continue;
    
        QInputOutputPlug plug;

        float title_width = (float) fm.width( MoreStrings::captialize_and_format( (*p)->name() ).c_str());
    
		if ((*p)->is_input())	{
            plug._plug = (*p);
            plug._input_position = input_pos++;	            
            _input_width = std::max(_input_width, title_width);	
        }

		if ((*p)->is_output())	{
            plug._plug = (*p);
            plug._output_position = output_pos++;	
            _output_width = std::max(_output_width, title_width);	
        }

        _plugs.push_front(plug);
	}

	input_pos = output_pos = std::max(input_pos,output_pos);

	// Events second
    FOR_EACH (e,events) {
        if ( (*e)->is_no_draw())
            continue;

        QInputOutputEvent event;

        float title_width = (float) fm.width( MoreStrings::captialize_and_format( (*e)->name() ).c_str());
    
		if ( (*e)->is_input())	{
            event._event = (*e);
            event._input_position = input_pos++;	            
            _input_width = std::max(_input_width, title_width);	
        }
        
		if ( (*e)->is_output())	{
            event._event = (*e);
            event._output_position = output_pos++;	
            _output_width = std::max(_output_width, title_width);	
        }

        _events.push_front(event);
	}
    
    input_pos = output_pos = std::max(input_pos,output_pos);
    
}

//==============================================================================
//==============================================================================

void EdLevelScriptNodeStandard::layoutNode (void)
{
    _plugs.clear();
    _events.clear();
    _titles.clear();

    if (_node->node_collapsed()) {

        QFontMetrics fm(_item_font);

        _title_width = fm.width( _node->name().c_str() );
        _input_width = 0.0F;
        _output_width = 0.0F;
    
        _title_width = _title_width + 4 * ICON_SIZE + 4 * ICON_PAD;
        _height = TITLE_HEIGHT;
        
    } else {

        int input_pos = 0;
        int output_pos = 0;

        QFontMetrics fm(_item_font);

        _title_width = fm.width( _node->name().c_str() );
        _input_width = 0.0F;
        _output_width = 0.0F;
        
        // Do base plugs
        addPlugsAndEvents(input_pos, output_pos, _node);
        
        // Do components if necessary
        std::shared_ptr<ObjectBase> base_object = checked_cast<ObjectBase>(_node);
        if (base_object) {
            std::list<std::shared_ptr<ComponentBase>> c = base_object->all_components();
            
            FOR_EACH (i,c) {
                _title_width = std::max(_title_width, (float) fm.width( (**i).name().c_str() ) );
                addPlugsAndEvents(input_pos, output_pos, *i);
            }
        }
        
        _title_width = _title_width + 4 * ICON_SIZE + 4 * ICON_PAD;


        // Resize the node
        _width = std::max(_title_width, _input_width + _output_width) + 4.0F * ITEM_PAD;
        _height = TITLE_HEIGHT + std::max(input_pos,output_pos) * ITEM_HEIGHT;
        
        // Plugs first
        for (int i = 0; i < _plugs.size(); ++i) {
            if (_plugs[i]._plug->is_input())
                _plugs[i]._input_location = QPoint(0.0F, _plugs[i]._input_position * ITEM_HEIGHT + TITLE_HEIGHT * 1.5F);            

            if (_plugs[i]._plug->is_output())
                _plugs[i]._output_location = QPoint(_width, _plugs[i]._output_position * ITEM_HEIGHT + TITLE_HEIGHT * 1.5F);
        }

        // Events second
        for (int i = 0; i < _events.size(); ++i) {
            if (_events[i]._event->is_input())
                _events[i]._input_location = QPoint(0.0F, _events[i]._input_position * ITEM_HEIGHT + TITLE_HEIGHT * 1.5F);
            
            if (_events[i]._event->is_output())
                _events[i]._output_location = QPoint(_width, _events[i]._output_position * ITEM_HEIGHT + TITLE_HEIGHT * 1.5F);
        }
        
    }

    layoutConnections();
	prepareGeometryChange();
}

//==============================================================================
//==============================================================================

void EdLevelScriptNodeStandard::layoutConnections()
{
    // Update connections
    FOR_EACH (i,_connections) {
        (**i).readjustBounds();
    }
}

//==============================================================================
//==============================================================================

QRectF EdLevelScriptNodeStandard::boundingRect(void) const
{
	return QRectF(-CONNECTOR_RADIUS, -CONNECTOR_RADIUS, _width + SHADOW_OFFSET_X + CONNECTOR_RADIUS, _height + SHADOW_OFFSET_Y + CONNECTOR_RADIUS);
}

//==============================================================================
//==============================================================================

void EdLevelScriptNodeStandard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);

	//
	// Title
	//

	// Drop shadow
	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(QColor(0,0,0,64)));
	painter->drawRoundedRect(0+SHADOW_OFFSET_X, 0+SHADOW_OFFSET_Y, _width, _height, 5, 5);

	painter->setClipping (true);

	//
	// Draw Title
	//

    Color4f c = _node->node_color();

	painter->setClipRect(QRectF(0,0,_width, TITLE_HEIGHT));

	if (isSelected()) {
		painter->setPen(QPen(QColor(53,120,255,255),4));
		painter->setBrush(QBrush(QColor(MoreMath::max(0,c.r_as_byte()-60),MoreMath::max(0,c.g_as_byte()-60),MoreMath::max(0,c.b_as_byte()-60),255)));
	} else {
		painter->setPen(QPen(QColor(40,40,40,255),1));
		painter->setBrush(QBrush(QColor(MoreMath::max(0,c.r_as_byte()-30),MoreMath::max(0,c.g_as_byte()-30),MoreMath::max(0,c.b_as_byte()-30),255)));
	}
	painter->drawRoundedRect(0, 0, _width, _height, 5, 5);

	painter->setPen(QPen(QColor(40,40,40,255),1));
	painter->setFont ( _title_font);
	painter->drawText( QRectF(0,0,_width, TITLE_HEIGHT), Qt::AlignHCenter | Qt::AlignVCenter, _node->name().c_str() );
    
    painter->drawImage(getNodeActionRect(), QImage(":/images/gear.png"));
    
    if (_node->node_collapsed())
        painter->drawImage(getNodeShrinkRect(), QImage(":/images/grow.png"));
    else
        painter->drawImage(getNodeShrinkRect(), QImage(":/images/shrink.png"));

    painter->drawImage(getNodeEditorRect(), QImage(":/images/source.png"));
    
    if (_node->isa(PlaceableObject::kind())) {
        painter->drawImage(QRectF(ICON_PAD,(TITLE_HEIGHT-ICON_SIZE)/2,ICON_SIZE,ICON_SIZE), QImage(":/images/hierobj.png"));
    } else {
        painter->drawImage(QRectF(ICON_PAD,(TITLE_HEIGHT-ICON_SIZE)/2,ICON_SIZE,ICON_SIZE), QImage(":/images/hiercalc.png"));
    }

	//
	// Draw Node
	//

	painter->setClipRect(QRectF(0,TITLE_HEIGHT,_width, _height-TITLE_HEIGHT));
    

	if (isSelected()) {
		painter->setPen(QPen(QColor(53,120,255,255),4));
		painter->setBrush(QBrush(QColor(MoreMath::max(0,c.r_as_byte()-30),MoreMath::max(0,c.g_as_byte()-30),MoreMath::max(0,c.b_as_byte()-30),255)));
	} else {
		painter->setPen(QPen(QColor(40,40,40,255),1));
		painter->setBrush(QBrush(QColor(c.r_as_byte(),c.g_as_byte(),c.b_as_byte(),255)));
	}
	painter->drawRoundedRect(0, 0, _width, _height, 5, 5);

	// Separator line
    float center = (_input_width + (_width - _output_width)) * 0.5F;
	painter->setPen(QPen(QColor(130,130,130,255),0,Qt::DotLine));
	painter->drawLine(center, 0, center, _height);

	//
	// Draw items
	//
	painter->setPen(QPen(QColor(40,40,40,255),1));
	painter->setClipping (false);
	painter->setFont ( _item_font);

	painter->setBrush(QBrush(QColor(150,220,150,255)));

	// Plugs first
	for (int i = 0; i < _plugs.size(); ++i) {
		if (_plugs[i]._input_position >= 0) {
			painter->drawText(	QRectF(	ITEM_PAD,
										_plugs[i]._input_position * ITEM_HEIGHT + TITLE_HEIGHT,
										_input_width, 
										ITEM_HEIGHT), 
								Qt::AlignLeft | Qt::AlignVCenter, MoreStrings::captialize_and_format(_plugs[i]._plug->name()).c_str());

			painter->drawEllipse( QPointF(_plugs[i]._input_location), CONNECTOR_RADIUS, CONNECTOR_RADIUS );

		}

		if (_plugs[i]._output_position >= 0) {
			painter->drawText(	QRectF(	_width - ITEM_PAD - _output_width,
										_plugs[i]._output_position * ITEM_HEIGHT + TITLE_HEIGHT,
										_output_width, 
										ITEM_HEIGHT), 
								Qt::AlignRight | Qt::AlignVCenter, MoreStrings::captialize_and_format(_plugs[i]._plug->name()).c_str());

			painter->drawEllipse( QPointF(_plugs[i]._output_location), CONNECTOR_RADIUS, CONNECTOR_RADIUS );
		}
	}

	painter->setBrush(QBrush(QColor(150,150,220,255)));

	// Events second
	for (int i = 0; i < _events.size(); ++i) {
		if (_events[i]._input_position >= 0) {
			painter->drawText(	QRectF(	ITEM_PAD,
										_events[i]._input_position * ITEM_HEIGHT + TITLE_HEIGHT,
										_input_width, 
										ITEM_HEIGHT), 
								Qt::AlignLeft | Qt::AlignVCenter, MoreStrings::captialize_and_format(_events[i]._event->name()).c_str());

			
			painter->drawRect ( QRectF(0.0F - CONNECTOR_RADIUS, _events[i]._input_location.y() - CONNECTOR_RADIUS, CONNECTOR_RADIUS * 2.0F, CONNECTOR_RADIUS * 2.0F ));
		}

		if (_events[i]._output_position >= 0) {
			painter->drawText(	QRectF(	_width - ITEM_PAD - _output_width,
										_events[i]._output_position * ITEM_HEIGHT + TITLE_HEIGHT,
										_output_width, 
										ITEM_HEIGHT), 
								Qt::AlignRight | Qt::AlignVCenter, MoreStrings::captialize_and_format(_events[i]._event->name()).c_str());

			painter->drawRect ( QRectF(_width - CONNECTOR_RADIUS, _events[i]._output_location.y() - CONNECTOR_RADIUS, CONNECTOR_RADIUS * 2.0F, CONNECTOR_RADIUS * 2.0F ));
		}		
	}
    
    // Titles third
	painter->setFont ( _title_font);

	for (int i = 0; i < _titles.size(); ++i) {
        painter->setPen(Qt::NoPen);
        
        if (isSelected()) {
            painter->setBrush(QBrush(QColor(MoreMath::max(0,c.r_as_byte()-60),MoreMath::max(0,c.g_as_byte()-60),MoreMath::max(0,c.b_as_byte()-60),255)));
        } else {
            painter->setBrush(QBrush(QColor(MoreMath::max(0,c.r_as_byte()-30),MoreMath::max(0,c.g_as_byte()-30),MoreMath::max(0,c.b_as_byte()-30),255)));
        }
        
        QRectF bounds = QRectF(  0, 
                                _titles[i]._position * ITEM_HEIGHT + TITLE_HEIGHT,
                                _width, 
                                TITLE_HEIGHT);
        bounds.adjust (2, 2, -2, -2);

        painter->drawRect ( bounds);

        painter->setPen(QPen(QColor(40,40,40,255),1));

        painter->drawText(  bounds,
                            Qt::AlignHCenter | Qt::AlignVCenter, _titles[i]._title);

        painter->drawImage(getTitleActionRect(_titles[i]), QImage(":/images/gear.png"));

	}

}

//==============================================================================
//==============================================================================

#include "moc_EdLevelScriptNodeStandard.cpp"


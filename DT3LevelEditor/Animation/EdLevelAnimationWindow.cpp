//==============================================================================
///	
///	File: EdLevelAnimationWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelAnimationWindow.hpp"
#include "EdLevelDocument.hpp"

// Qt include
#include <QtWidgets/qdrawutil.h>
#include <QtWidgets/QStyle>
#include <QtCore/QFile>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

// Engine includes
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Scripting/ScriptingKeyframesRoot.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelAnimationWindow::EdLevelAnimationWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   QWidget   (parent),
        _font("Arial", 9),
        _font_bold("Arial", 9, QFont::Bold),
        _fm(_font),
        _pixels_per_second(100),
        _time   (0.0F),
        _scroll (0),
        _root   (NULL),
        _thumb_image                (":/images/thumb.png"),
        _keyframe_image             (":/images/keyframe.png"),
        _keyframe_image_selected    (":/images/keyframe_selected.png")
{
    _document = document;
    _toolbar = toolbar;
    
    //
    // Actions and toolbar
    //
    
    _anim_play = new QAction(tr("&Play"), this);
    _anim_play->setIcon(QIcon(":/images/play.png"));
    _anim_play->setStatusTip(tr("Play"));
    connect(_anim_play, SIGNAL(triggered()), this, SLOT(onScriptPlay()));

    _anim_stop = new QAction(tr("&Play"), this);
    _anim_stop->setIcon(QIcon(":/images/stop.png"));
    _anim_stop->setStatusTip(tr("Play"));
    connect(_anim_stop, SIGNAL(triggered()), this, SLOT(onScriptStop()));

    toolbar->addAction(_anim_play);
    toolbar->addAction(_anim_stop);

    // 
    // Set up window
    //

    _horz_scrollbar = new QScrollBar(Qt::Horizontal, this);
    _horz_scrollbar->setSingleStep(20);
    
    _vert_scrollbar = new QScrollBar(Qt::Vertical, this);
    _vert_scrollbar->setSingleStep(10);

    connect(    _horz_scrollbar,    SIGNAL(valueChanged(int)),
                this,               SLOT(onScrollTime(int))   );
    connect(    _vert_scrollbar,    SIGNAL(valueChanged(int)),
                this,               SLOT(onScroll(int))   );
   
    _time_min = new EdLevelLineEdit(this);
    _time_min->setText("-1");

    _time_max = new EdLevelLineEdit(this);
    _time_max->setText("30");
    
	connect(	_time_min,      SIGNAL(editingFinished()),
				this,           SLOT(onChangeTimeRange())	);
	connect(	_time_max,      SIGNAL(editingFinished()),
				this,           SLOT(onChangeTimeRange())	);
    
    _scroll_width = _vert_scrollbar->sizeHint().width();
    _scroll_height = _horz_scrollbar->sizeHint().height();

    setFocusPolicy(Qt::ClickFocus);
}

EdLevelAnimationWindow::~EdLevelAnimationWindow	(void)
{

}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::onAnimPlay (void)
{
    //_anim_timer.start(200, this);
}

void EdLevelAnimationWindow::onAnimStop (void)
{
    //_anim_timer.stop();
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::scanRoot (const std::shared_ptr<ScriptingKeyframesRoot> &root, std::vector<PlugEventCache> &plug_event_cache)
{
    _root = root;
    
    // Get all of the plugs for the current node
    std::list<PlugBase*> plugs;
    root->all_plugs(plugs);

    FOR_EACH (j,plugs) {
        // We only care about outputs
        if ( !(**j).is_output() )
            continue;
    
        // Make sure it's hooked up
        if ( !(**j).has_outgoing_connection() )
            continue;

        // Check each outgoing connection
        const std::vector<PlugBase*> &connections = (**j).outgoing_connections();
        for (int k = 0; k < connections.size(); ++k) {
            
            // Get connected keyframes
            std::shared_ptr<ScriptingKeyframes> keyframes = checked_cast<ScriptingKeyframes>( checked_cast<ScriptingKeyframes>(connections[k]->owner()->shared_from_this()) );
            if (!keyframes)
                continue;
                
            // Get Keyframes output
            PlugBase *keyframes_out_plug = keyframes->plug_by_name("Out");
            if (keyframes_out_plug) {
                
                // Make sure output is connected
                if (!keyframes_out_plug->has_outgoing_connection())
                    continue;
                  
                // Get the FIRST output connection
                PlugBase* next_input_plug = keyframes_out_plug->outgoing_connections()[0];

                // Record the nodes
                PlugEventCache animated_node;
                animated_node._node = checked_static_cast<PlugNode>(next_input_plug->owner()->shared_from_this());
                animated_node._plug = next_input_plug;
                animated_node._keyframes = keyframes;
                
                plug_event_cache.push_back(animated_node);
            }
            
            // Get Keyframes output
            Event *keyframes_out_event = keyframes->event_by_name("Out");
            if (keyframes_out_event) {
                
                // Make sure output is connected
                if (!keyframes_out_event->has_outgoing_connection())
                    continue;
                  
                // Get the FIRST output connection
                Event* next_input_event = keyframes_out_event->outgoing_connections()[0];

                // Record the nodes
                PlugEventCache animated_node;
                animated_node._node = checked_static_cast<PlugNode>(next_input_event->owner()->shared_from_this());
                animated_node._event = next_input_event;
                animated_node._keyframes = keyframes;
                
                plug_event_cache.push_back(animated_node);
            }
            
            
        }
        
    }
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::layoutItems (std::vector<PlugEventCache> &plug_event_cache)
{
    std::shared_ptr<PlugNode> last_node;

    int ypos = 0;
    
    for (int i = 0; i < plug_event_cache.size(); ++i) {
        if (plug_event_cache[i]._node != last_node) {
            last_node = plug_event_cache[i]._node;

            PlugEventCache c;
            //c._node = last_node;
            c._title = last_node->full_name();
            c._ypos = ypos;
            plug_event_cache.insert (plug_event_cache.begin() + i, c);

            ypos += NODE_ITEM_HEIGHT;
            ++i;
        } 
                        
        if (plug_event_cache[i]._plug)
            plug_event_cache[i]._title = MoreStrings::captialize_and_format(plug_event_cache[i]._plug->name());
        else
            plug_event_cache[i]._title = MoreStrings::captialize_and_format(plug_event_cache[i]._event->name());
        plug_event_cache[i]._ypos = ypos;
        
        ypos += NODE_ITEM_HEIGHT;
    }
    
    _nodes_height = ypos;
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::scanRelevantNodes (const std::list<std::shared_ptr<PlugNode>> &selection_list)
{
    _root.reset();
    
    //
    // Build a giant list of all of the relevant nodes breadth-first
    //
    
    std::list<std::shared_ptr<PlugNode>> visited_nodes = selection_list;
    std::vector<PlugEventCache> plug_event_cache;
      
    FOR_EACH (i,visited_nodes) {
    
        // Check if node is a root
        if ((*i)->isa(ScriptingKeyframesRoot::kind())) {
            std::shared_ptr<ScriptingKeyframesRoot> root = checked_static_cast<ScriptingKeyframesRoot>(*i);
            scanRoot (root, plug_event_cache);
            layoutItems (plug_event_cache);
            
            if (_plug_event_cache != plug_event_cache) {
                _plug_event_cache = plug_event_cache;
            }   
                
            return;
        }
        
        // Special case components too
        if ((*i)->isa(ObjectBase::kind())) {
            std::shared_ptr<ObjectBase> node_base = checked_static_cast<ObjectBase>(*i);
            
            for (int i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
                std::shared_ptr<ComponentBase> component_base = node_base->component_by_type ( (ComponentBase::ComponentType) i);
                
                if (component_base) {
                    // Check if we already visited the node
                    auto j = std::find(visited_nodes.begin(), visited_nodes.end(), component_base);
                    if (j != visited_nodes.end())
                        continue;
                        
                    visited_nodes.push_back(component_base);
                    
                }

            }
        } 

        
        // Get all of the plugs for the current node
        std::list<PlugBase*> plugs;
        (**i).all_plugs(plugs);

        FOR_EACH (j, plugs) {
        
            // We only care about inputs
            if ( !(**j).is_input() )
                continue;
            
            // Make sure it's hooked up
            if ( !(**j).has_incoming_connection() )
                continue;
            
            // Get the node
            std::shared_ptr<PlugNode> node = checked_static_cast<PlugNode>((**j).incoming_connection()->owner()->shared_from_this());
            if (!node)
                continue;
                                
            // Check if we already visited the node
            auto k = std::find(visited_nodes.begin(), visited_nodes.end(), node);
            if (k == visited_nodes.end())
                visited_nodes.push_back(node);
        }

        // Get all of the events for the current node
        std::list<Event*> events;
        (**i).all_events(events);

        FOR_EACH (k, events) {
        
            // We only care about inputs
            if ( !(**k).is_input() )
                continue;
            
            // Make sure it's hooked up
            if ( !(**k).has_incoming_connection() )
                continue;
                
            // Get the nodes
            const std::vector<Event*>& events = (*k)->incoming_connections();

            FOR_EACH (i,  events) {
                std::shared_ptr<PlugNode> node = checked_static_cast<PlugNode>((*i)->owner()->shared_from_this());
            
                auto k = std::find(visited_nodes.begin(), visited_nodes.end(), node);
                if (k == visited_nodes.end())
                    visited_nodes.push_back(node);
            }
            
        }
    
    }

}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::itemRect(PlugEventCache &n, QRect &tr, QRect &r) const
{

    if (n._node) {
        const int INDENT = 15;
            
        tr = QRect(INDENT, n._ypos, NODE_ITEM_WIDTH-INDENT, NODE_ITEM_HEIGHT);
        r = QRect(0, n._ypos, NODE_ITEM_WIDTH, NODE_ITEM_HEIGHT);

    } else {
        const int INDENT = 5;

        tr = QRect(INDENT, n._ypos, NODE_ITEM_WIDTH-INDENT, NODE_ITEM_HEIGHT);
        r = QRect(0, n._ypos, NODE_ITEM_WIDTH, NODE_ITEM_HEIGHT);
    }
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::thumbRect (QRect &r) const
{
    if (_root) {
        DTfloat t = _root->time();
        int xpos = timeToPosition(t);

        r = QRect(xpos - _thumb_image.width()/2,0, _thumb_image.width(),_thumb_image.height());
    } else {
        r = QRect(_thumb_image.width()/2,0, _thumb_image.width(),_thumb_image.height());
    }
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::selectionRect (QRect &r) const
{
    QPoint selection_start = _start_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT + _scroll);
    QPoint selection_end = _end_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT + _scroll);
    
    DTfloat xmin = selection_start.x();
    DTfloat xmax = selection_end.x();
    if (xmin > xmax)    std::swap(xmin,xmax);
    
    DTfloat ymin = selection_start.y();
    DTfloat ymax = selection_end.y();
    if (ymin > ymax)    std::swap(ymin,ymax);

    r = QRect(  xmin,
                ymin, 
                xmax - xmin,
                ymax - ymin);

}

void EdLevelAnimationWindow::keyRect (PlugEventCache &p, int k, QRect &rk) const
{
    QRect tr, r;
    itemRect(p, tr, r);

    DTfloat kf_time = p._keyframes->key_time(k);
    int xpos = timeToPosition(kf_time);

    rk = QRect( xpos - _keyframe_image.width()/2,
                r.center().y() - _keyframe_image.height()/2, 
                _keyframe_image.width(),
                _keyframe_image.height()    );
}

//==============================================================================
//==============================================================================

int EdLevelAnimationWindow::timeToPosition (DTfloat seconds) const
{
    return _pixels_per_second * seconds;
}

DTfloat EdLevelAnimationWindow::positionToTime (int position) const
{
    return static_cast<DTfloat>(position) / static_cast<DTfloat>(_pixels_per_second);
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::onSelectionChanged (const std::list<std::shared_ptr<PlugNode>> &selection_list)
{
    blockSignals(true);

    scanRelevantNodes(selection_list);
    
    blockSignals(false);
    
    update();
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::resizeEvent (QResizeEvent *event)
{
    _title = QRect(0,0,NODE_ITEM_WIDTH, TITLE_HEIGHT);
    _timeline = QRect(NODE_ITEM_WIDTH,0,rect().width() - NODE_ITEM_WIDTH - _scroll_width, TITLE_HEIGHT);
    _nodes = QRect(0,TITLE_HEIGHT, NODE_ITEM_WIDTH, rect().height() - TITLE_HEIGHT);
    _controls = QRect(NODE_ITEM_WIDTH, TITLE_HEIGHT, rect().width() - NODE_ITEM_WIDTH - _scroll_width, rect().height() - TITLE_HEIGHT - _scroll_height);
    
    // Place scrollbars
    _horz_scrollbar->setGeometry(NODE_ITEM_WIDTH, rect().height() - _scroll_height, rect().width() - NODE_ITEM_WIDTH - _scroll_width, _scroll_height);
    _vert_scrollbar->setGeometry(rect().width() - _scroll_width, TITLE_HEIGHT, _scroll_width, rect().height() - TITLE_HEIGHT - _scroll_height);
    
    // Scale scrollbars
    int page_step = rect().height() - TITLE_HEIGHT - _scroll_height;
    _vert_scrollbar->setPageStep(page_step);
    _vert_scrollbar->setRange(0,_nodes_height - page_step);
        
    // Scale fields
    _time_min->setGeometry(NODE_ITEM_WIDTH, rect().height() - _scroll_height - TIME_FIELD_HEIGHT, TIME_FIELD_WIDTH, TIME_FIELD_HEIGHT);
    _time_max->setGeometry(rect().width() - _scroll_width - TIME_FIELD_WIDTH, rect().height() - _scroll_height - TIME_FIELD_HEIGHT, TIME_FIELD_WIDTH, TIME_FIELD_HEIGHT);
    
    onChangeTimeRange();
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::onScrollTime(int time)
{
    _time = positionToTime(time);
    update();
}

void EdLevelAnimationWindow::onScroll(int scroll)
{
    _scroll = scroll;
    update();
}

void EdLevelAnimationWindow::onChangeTimeRange()
{
    _time_min_sec = _time_min->text().toFloat();
    _time_max_sec = _time_max->text().toFloat();
    
    if (_time < _time_min_sec)   _time = _time_min_sec;
    if (_time > _time_max_sec)   _time = _time_max_sec;
    
    int page_step = _controls.width();
    _horz_scrollbar->setPageStep(page_step);
        
    _horz_scrollbar->setRange( timeToPosition(_time_min_sec), timeToPosition(_time_max_sec) - page_step);

    update();
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::mouseDoubleClickEvent   (QMouseEvent *event)
{
    event->accept();
}

void EdLevelAnimationWindow::mousePressEvent(QMouseEvent *event)
{
    emit doUndoBlock();

	_start_point = _last_point = _end_point = event->pos();
    
    // Translate into timeline space
    QPoint local_pos = _end_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT + _scroll);

    // Check click in thumb
    if (_timeline.contains(event->pos())) {
        
        setMode(MODE_DRAGGING_THUMB);
        
        if (_root) {
            DTfloat t = positionToTime(local_pos.x());
            
            emit doCommand(QString("SetProp \"") + _root->full_name().c_str() + ".Time_Out\" " + MoreStrings::cast_to_string(t).c_str());
        }
        update();

    } else if (_controls.contains(_end_point)) {
        
        if ( (event->buttons() & Qt::LeftButton) && ( (event->buttons() & Qt::MidButton) || (event->modifiers() & Qt::ALT)) ) {
            setMode(MODE_ZOOMING);
            
        } else {
        
            // Check all keyframes to see if one is clicked
            for (int i = 0; i < _plug_event_cache.size(); ++i) {
                if (!_plug_event_cache[i]._node || !_plug_event_cache[i]._keyframes)
                    continue;
                                
                for (int k = 0; k < getNumKeys(_plug_event_cache[i]); ++k) {
                    QRect rk;
                    keyRect (_plug_event_cache[i], k, rk);

                    if (rk.contains(local_pos)) {
                        int id = _plug_event_cache[i]._keyframes->key_id(k);
                 
                        // Check select new keyframe        
                        if (!isSelected(_plug_event_cache[i], id)) {
                            clearSelected();
                            setSelected(_plug_event_cache[i], id, true);
                        }
                        
                        setMode(MODE_DRAGGING);
                        update();
                        return;
                    }
                }
                                    
            }

            setMode(MODE_DRAG_SELECTING);
        }
        
    } else {
        setMode(MODE_NONE);
    }

}

void EdLevelAnimationWindow::mouseMoveEvent(QMouseEvent *event)
{		
	_end_point = event->pos();
	//QPoint dist = _end_point - _start_point;
	QPoint delta = _end_point - _last_point;

    // Translate into timeline space
    QPoint local_pos = _end_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT);

    switch (getMode()) {
        case MODE_DRAGGING_THUMB:
            if (_root) {
                DTfloat t = positionToTime(local_pos.x());
                
                emit doCommand(QString("SetProp \"") + _root->full_name().c_str() + ".Time_Out\" " + MoreStrings::cast_to_string(t).c_str());
            }
            update();
            break;

		case MODE_DRAG_SELECTING:
            update();   // Just displaying selection rect
            break;
            
		case MODE_DRAGGING:
            
            // Check all keyframes to see if one is clicked
            for (int i = 0; i < _plug_event_cache.size(); ++i) {
                if (!_plug_event_cache[i]._node || !_plug_event_cache[i]._keyframes)
                    continue;
                                  
                DTfloat dt = positionToTime(delta.x());
                int num_keys = getNumKeys(_plug_event_cache[i]);
                            
                // Since order can flip when keys are set, we start with the farther ahead points first
                // depending on direction of dt
                
                if (dt < 0.0F) {
                    for (int k = 0; k < num_keys; ++k) {
                        int id = _plug_event_cache[i]._keyframes->key_id(k);
                        
                        if (isSelected(_plug_event_cache[i], id)) {

                            DTfloat current_time = _plug_event_cache[i]._keyframes->key_time(k);
                            
                            std::string cmd = "SetKeyframeTimeByID " + _plug_event_cache[i]._keyframes->full_name() + " " + MoreStrings::cast_to_string(id) + " " + MoreStrings::cast_to_string(current_time + dt);
                            emit doCommand(cmd.c_str());
                        }

                    }
                } else {
                    for (int k = num_keys-1; k >= 0; --k) {
                        int id = _plug_event_cache[i]._keyframes->key_id(k);
                        
                        if (isSelected(_plug_event_cache[i], id)) {

                            DTfloat current_time = _plug_event_cache[i]._keyframes->key_time(k);
                            
                            std::string cmd = "SetKeyframeTimeByID " + _plug_event_cache[i]._keyframes->full_name() + " " + MoreStrings::cast_to_string(id) + " " + MoreStrings::cast_to_string(current_time + dt);
                            emit doCommand(cmd.c_str());
                        }

                    }
                }
                
            }
            update();
            break;
            
        case MODE_ZOOMING: {
                DTfloat center_time_before = (_horz_scrollbar->value() + _controls.width() / 2) / static_cast<DTfloat>(_pixels_per_second);

                DTfloat zoom = -delta.y();
                _pixels_per_second += zoom; 
                
                if (_pixels_per_second < 5)         _pixels_per_second = 5;
                else if (_pixels_per_second > 500)  _pixels_per_second = 500;
                
                DTfloat center_time_after= (_horz_scrollbar->value() + _controls.width() / 2) / static_cast<DTfloat>(_pixels_per_second);

                _horz_scrollbar->setValue(_horz_scrollbar->value() - (center_time_after - center_time_before) * _pixels_per_second);
                
                onChangeTimeRange();
            } break;
            
        default:
            break;

    };

    _last_point = _end_point;
}

void EdLevelAnimationWindow::mouseReleaseEvent(QMouseEvent *event)
{
	_end_point = event->pos();

    // Translate into timeline space
    QPoint local_pos = _end_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT);

    switch (getMode()) {
        case MODE_DRAGGING_THUMB:    
            if (_root) {
                DTfloat t = positionToTime(local_pos.x());
                
                emit doCommand(QString("SetProp \"") + _root->full_name().c_str() + ".Time_Out\" " + MoreStrings::cast_to_string(t).c_str());
            }
            update();
            break;
                    
        case MODE_DRAG_SELECTING: {
            QRect rs;
            selectionRect(rs);
        
            for (int i = 0; i < _plug_event_cache.size(); ++i) {
                if (!_plug_event_cache[i]._node || !_plug_event_cache[i]._keyframes)
                    continue;
            
                // Append selection
                if (event->modifiers() & Qt::SHIFT) {

                    for (int k = 0; k < getNumKeys(_plug_event_cache[i]); ++k) {
                        QRect rk;
                        keyRect (_plug_event_cache[i], k, rk);

                        if (rs.intersects(rk)) {
                        
                            int id = _plug_event_cache[i]._keyframes->key_id(k);
                            
                            // Invert keyframes        
                            if (isSelected(_plug_event_cache[i], id))
                                setSelected(_plug_event_cache[i], id, false);
                            else
                                setSelected(_plug_event_cache[i], id, true);
                        }
                    }
                    
                // New selection
                } else {
                
                    clearSelected (_plug_event_cache[i]);
                
                    for (int k = 0; k < getNumKeys(_plug_event_cache[i]); ++k) {
                        QRect rk;
                        keyRect (_plug_event_cache[i], k, rk);

                        if (rs.contains(rk)) {
                            int id = _plug_event_cache[i]._keyframes->key_id(k);
                            setSelected(_plug_event_cache[i], id, true);
                        }
                    }
                }
                
                
            }
            
            update();
            } break;
            
		case MODE_DRAGGING:
            update();
            break;
        
        default:
            break;

    }
    
    setMode(MODE_NONE);
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::wheelEvent (QWheelEvent *event)
{
    int move = -event->delta();

    if (event->orientation() == Qt::Horizontal) {
        _horz_scrollbar->setValue(_horz_scrollbar->value() + move);
    } else {
        _vert_scrollbar->setValue(_vert_scrollbar->value() + move);
    }

    update();

    event->accept();
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::keyPressEvent (QKeyEvent *event)
{
    emit doUndoBlock();

    int key = event->key();

    if (event->matches(QKeySequence::Delete) || key == 0x1000003) {

        for (int i = 0; i < _plug_event_cache.size(); ++i) {
            if (!_plug_event_cache[i]._node || !_plug_event_cache[i]._keyframes)
                continue;
                
            std::list<int> &selected = _plug_event_cache[i]._selected_ids;
        
            FOR_EACH (j ,selected) {
                std::string cmd = "ClearKeyframeByID " + _plug_event_cache[i]._keyframes->full_name() + " " + MoreStrings::cast_to_string(*j);
                emit doCommand(cmd.c_str());
            }

        }
        
        clearSelected();
        update();
    }
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::setSelected (PlugEventCache &p, int id, bool selected)
{
    if (selected) {
    
        auto i = std::find(p._selected_ids.begin(), p._selected_ids.end(), id);
        if (i == p._selected_ids.end())
            p._selected_ids.push_back(id);
            
    } else {

        auto i = std::find(p._selected_ids.begin(), p._selected_ids.end(), id);
        if (i != p._selected_ids.end())
            p._selected_ids.erase(i);
    
    }
}

void EdLevelAnimationWindow::clearSelected (PlugEventCache &p)
{
    p._selected_ids.clear();
}

void EdLevelAnimationWindow::clearSelected (void)
{
    for (int i = 0; i < _plug_event_cache.size(); ++i)
        clearSelected(_plug_event_cache[i]);
}

bool EdLevelAnimationWindow::isSelected (PlugEventCache &p, int id)
{
    auto i = std::find(p._selected_ids.begin(), p._selected_ids.end(), id);
    if (i == p._selected_ids.end())
        return false;
        
    return true;
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::onRefreshAnimation(void)
{
    scanRelevantNodes( _document->selection() );
    update();
}

//==============================================================================
//==============================================================================

void EdLevelAnimationWindow::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	draw(&painter);
}

void EdLevelAnimationWindow::draw(QPainter *painter)
{    
	painter->setRenderHint(QPainter::Antialiasing, false);
    
	painter->setClipRect(rect());
		
    painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(QColor(100,100,100,255)));
	painter->drawRect(rect());

	painter->setBrush(QBrush(QColor(95,95,95,255)));
	painter->drawRect(_nodes);
	painter->drawRect(_title);
	painter->drawRect(_timeline);
    
    //
    // Draw Node Names
    //
    
    painter->resetTransform ();
    painter->setClipRect ( _nodes );
    painter->translate (0, TITLE_HEIGHT - _scroll);
    
    for (int i = 0; i < _plug_event_cache.size(); ++i) {
        QRect tr, r;
        itemRect(_plug_event_cache[i], tr, r);
        
        if (_plug_event_cache[i]._node) {
            painter->setPen(QPen(QColor(100,100,100,255),0));
            painter->setBrush(QBrush(QColor(130,130,130,255)));
            painter->drawRect(r);

            painter->setPen(QPen(QColor(40,40,40,255),1));
            painter->setFont(_font);
            painter->drawText(tr, Qt::AlignLeft | Qt::AlignVCenter, _plug_event_cache[i]._title.c_str() );
            
        } else {
            painter->setPen(QPen(QColor(40,40,40,255),1));
            painter->setFont(_font_bold);
            painter->drawText(tr, Qt::AlignLeft | Qt::AlignVCenter, _plug_event_cache[i]._title.c_str() );
        
        }

    }
    
    //
    // Draw Node Keyframes
    //

    painter->resetTransform ();
    painter->setClipRect ( _controls );
    painter->translate (NODE_ITEM_WIDTH - timeToPosition(_time), TITLE_HEIGHT - _scroll);
    
    for (int i = 0; i < _plug_event_cache.size(); ++i) {
        if (!_plug_event_cache[i]._node || !_plug_event_cache[i]._keyframes)
            continue;
    
        QRect tr, r;
        itemRect(_plug_event_cache[i], tr, r);
        
        // Build a new Rect based on this one
        r.setLeft( timeToPosition(_time_min_sec) );
        r.setWidth( timeToPosition(_time_max_sec - _time_min_sec) );
        
        painter->setPen(QPen(QColor(100,100,100,255),0));
        painter->setBrush(QBrush(QColor(130,130,130,255)));
        painter->drawRect(r);            

        // Draw guide line
        painter->setPen(QPen(QColor(100,100,100,255),1));
        painter->drawLine( timeToPosition(_time_min_sec), r.center().y(), timeToPosition(_time_max_sec - _time_min_sec), r.center().y());
        
        if (getNumKeys(_plug_event_cache[i]) > 0) {
            // Draw regions between keyframes
            for (int k = 0; k < getNumKeys(_plug_event_cache[i])-1; ++k) {
                QRect r1,r2;
                keyRect (_plug_event_cache[i], k, r1);
                keyRect (_plug_event_cache[i], k+1, r2);
                
                QRect r = r1.united(r2);
                r.adjust(_keyframe_image.width()/2, 0, -_keyframe_image.width()/2, 0);
                
                painter->setPen(Qt::NoPen);
                painter->setBrush(QBrush(QColor(0,0,0,30)));
                painter->drawRect(r);            
            }
            
            // Draw keyframes        
            for (int k = 0; k < getNumKeys(_plug_event_cache[i]); ++k) {
                QRect r;
                keyRect (_plug_event_cache[i], k, r);
                
                int id = _plug_event_cache[i]._keyframes->key_id(k);
                
                if (isSelected (_plug_event_cache[i], id))
                    painter->drawImage(r, _keyframe_image_selected);
                else
                    painter->drawImage(r, _keyframe_image);
            }
        }
        
    }
    
    // Selection
    if (getMode() == MODE_DRAG_SELECTING) {
        QRect rs;
        selectionRect(rs);
    
        painter->setPen(QPen(QColor(53,120,255,255),2));
        painter->setBrush(QBrush(QColor(0,0,0,50)));

        painter->drawRoundedRect(rs, 5, 5);
    }



    //
    // Draw Seconds
    //
    
    painter->resetTransform ();
    painter->setClipRect ( _timeline.united(_controls) );
    painter->translate (NODE_ITEM_WIDTH - timeToPosition(_time), 0);
    
    painter->setFont(_font);
    
    // Milliseconds
    for (int t = _time_min_sec*1000; t <= _time_max_sec*1000; t += 100) {
        
        int xpos = timeToPosition(t/1000.0F);
    
        // Full second
        if (t % 1000 == 0) {
            
            if (t == 0) painter->setPen(QPen(QColor(75,75,75,255),3,Qt::SolidLine));
            else        painter->setPen(QPen(QColor(75,75,75,255),1,Qt::SolidLine));
            
            painter->drawLine( QPoint(xpos, TITLE_HEIGHT) , QPoint(xpos, rect().height() - _scroll_height) );
            
            QRect tr(xpos - _pixels_per_second/2,0,_pixels_per_second,TITLE_HEIGHT);
            
            painter->setPen(QPen(QColor(25,25,25,255),1,Qt::SolidLine));
            
            std::stringstream ss;
            ss << (t/1000) << " s";
            painter->drawText(tr, Qt::AlignHCenter | Qt::AlignBottom, ss.str().c_str());
        } else {
            painter->setPen(QPen(QColor(90,90,90,255),1,Qt::DotLine));
            painter->drawLine( QPoint(xpos, TITLE_HEIGHT) , QPoint(xpos, rect().height() - _scroll_height) );
        }
    
    
    }

    //
    // Draw Current Time
    //

    if (_root) {
        painter->resetTransform ();
        painter->setClipRect ( _timeline.united(_controls) );
        painter->translate (NODE_ITEM_WIDTH - timeToPosition(_time), 0);
        
        DTfloat t = _root->time();
        int xpos = timeToPosition(t);

        QRect thr;
        thumbRect(thr);
        
        painter->drawImage(thr, _thumb_image);
        
        painter->setPen(QPen(QColor(0,255,0,255),1,Qt::SolidLine));
        painter->drawLine( QPoint(xpos, TITLE_HEIGHT) , QPoint(xpos, rect().height() - _scroll_height) );
        
        QRect tr(xpos - _pixels_per_second/2,0,_pixels_per_second,TITLE_HEIGHT);
                
        std::stringstream ss;
        ss << t << " s";
        painter->drawText(tr, Qt::AlignHCenter | Qt::AlignTop, ss.str().c_str());
    }

    //
    // Draw Separators
    //
    
    painter->resetTransform ();
    painter->setClipRect (rect());

    painter->setPen(QPen(QColor(75,75,75,255),2));
    painter->setBrush(QBrush(QColor(100,100,100,255)));

    painter->drawLine( QPoint(0, TITLE_HEIGHT), QPoint(rect().width(), TITLE_HEIGHT) );
    painter->drawLine( QPoint(NODE_ITEM_WIDTH, 0), QPoint(NODE_ITEM_WIDTH, rect().height()) );

}

//==============================================================================
//==============================================================================

#include "moc_EdLevelAnimationWindow.cpp"


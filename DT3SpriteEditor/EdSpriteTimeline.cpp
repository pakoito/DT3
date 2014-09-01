//==============================================================================
///	
///	File: EdSpriteTimeline.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteTimeline.hpp"
#include "EdSpriteLineEdit.hpp"
#include "EdSpriteMainWindow.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"
#include "KeyedSpriteAnimation.hpp"
#include "Algorithm.hpp"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QScrollBar>
#include <QtGui/QPainter>

using namespace DT2;

//==============================================================================
//==============================================================================

EdSpriteTimeline::EdSpriteTimeline(EdSpriteMainWindow *main_window)
	:	QWidget                     (main_window),
        _font                       ("Arial", 9),
        _font_bold                  ("Arial", 9, QFont::Bold),
        _fm                         (_font),
        _pixels_per_second          (100),
        _time                       (0.0F),
        _scroll                     (0),
        _thumb_image                (":/images/thumb.png"),
        _keyframe_image             (":/images/keyframe.png"),
        _keyframe_image_selected    (":/images/keyframe_selected.png")
{
    _main_window = main_window;
    
    setMinimumHeight(150);

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
   
    _time_min = new EdSpriteLineEdit(this);
    _time_min->setText("-1");

    _time_max = new EdSpriteLineEdit(this);
    _time_max->setText("30");
    
	connect(	_time_min,      SIGNAL(editingFinished()),
				this,           SLOT(onChangeTimeRange())	);
	connect(	_time_max,      SIGNAL(editingFinished()),
				this,           SLOT(onChangeTimeRange())	);
    
    _scroll_width = _vert_scrollbar->sizeHint().width();
    _scroll_height = _horz_scrollbar->sizeHint().height();

    setFocusPolicy(Qt::ClickFocus);
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::itemRect(Track &t, QRect &tr, QRect &r) const
{
    const int INDENT = 5;

    tr = QRect(INDENT, t._ypos, NODE_ITEM_WIDTH-INDENT, NODE_ITEM_HEIGHT);
    r = QRect(0, t._ypos, NODE_ITEM_WIDTH, NODE_ITEM_HEIGHT);
}

void EdSpriteTimeline::keyRect (Track &t, DTuint k, QRect &rk) const
{
    QRect tr, r;
    itemRect(t, tr, r);

    DTfloat kf_time = t._track->getKeyframes()[k].getTime();
    DTint xpos = timeToPosition(kf_time);

    rk = QRect( xpos - _keyframe_image.width()/2,
                r.center().y() - _keyframe_image.height()/2, 
                _keyframe_image.width(),
                _keyframe_image.height()    );
}

void EdSpriteTimeline::eventRect (DTuint k, QRect &rk) const
{
    KeyedSpriteAnimation *animation = _main_window->getSelectedAnimation();
    KeyedSpriteAnimationEventTrack& event_track = animation->getEventTrack();
    const Array<KeyedSpriteAnimationEvent>& keyframes = event_track.getKeyframes();

    DTfloat kf_time = keyframes[k].getTime();
    DTint xpos = timeToPosition(kf_time);

    rk = QRect( xpos - 5,
                TITLE_HEIGHT, 
                10,
                rect().height() - _scroll_height - TITLE_HEIGHT   );
}

void EdSpriteTimeline::thumbRect (QRect &r) const
{
    DTfloat t = _main_window->getCurrentTime();
    int xpos = timeToPosition(t);

    r = QRect(xpos - _thumb_image.width()/2,0, _thumb_image.width(),_thumb_image.height());
}

void EdSpriteTimeline::selectionRect (QRect &r) const
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

//==============================================================================
//==============================================================================

void EdSpriteTimeline::clearKeySelection (void)
{
    clearSelected();
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::setSelected (Track &t, DTuint id, DTboolean selected)
{
    if (selected) {
    
        auto i = t._selected_ids.find(id);
        if (i == t._selected_ids.end())
            t._selected_ids.pushBack(id);
            
    } else {

        auto i = t._selected_ids.find(id);
        if (i != t._selected_ids.end())
            t._selected_ids.erase(i);
    
    }
}

void EdSpriteTimeline::clearSelected (Track &t)
{
    t._selected_ids.clear();
}

void EdSpriteTimeline::clearSelected (void)
{
    for (int i = 0; i < _tracks.size(); ++i)
        clearSelected(_tracks[i]);
}

DTboolean EdSpriteTimeline::isSelected (Track &t, DTuint id)
{
    if (t._selected_ids.find(id) == t._selected_ids.end())
        return false;
        
    return true;
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::setSelectedEvent (DTuint id, DTboolean selected)
{
    if (selected) {
    
        auto i = _selected_event_ids.find(id);
        if (i == _selected_event_ids.end())
            _selected_event_ids.pushBack(id);
            
    } else {

        auto i = _selected_event_ids.find(id);
        if (i != _selected_event_ids.end())
            _selected_event_ids.erase(i);
    
    }
}

DTboolean EdSpriteTimeline::isSelectedEvent (DTuint id)
{
    if (_selected_event_ids.find(id) == _selected_event_ids.end())
        return false;
        
    return true;
}

void EdSpriteTimeline::clearSelectedEvent (void)
{
    _selected_event_ids.clear();
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::onDuplicate(void)
{
    for (int i = 0; i < _tracks.size(); ++i) {
        
        // Get all of the keyframes for the track
        const Array<KeyedSpriteAnimationKeyframe>& keyframes = _tracks[i]._track->getKeyframes();
        DTuint num_keyframes = (DTuint) keyframes.size();
        
        List<DTint> new_selected_ids;

        for (int j = 0; j < num_keyframes; ++j) {
        
            // if keyframe is selected
            if (isSelected (_tracks[i], keyframes[j].getKeyID())) {
                
                int new_index = _tracks[i]._track->duplicate(j);
                new_selected_ids.pushBack(keyframes[new_index].getKeyID());
            }
        
        }    
        
        _tracks[i]._selected_ids = new_selected_ids;
        _tracks[i]._track->updateKeyframes();

        update();
    }
}

//==============================================================================
//==============================================================================

DTint EdSpriteTimeline::timeToPosition (DTfloat seconds) const
{
    return _pixels_per_second * seconds;
}

DTfloat EdSpriteTimeline::positionToTime (DTint position) const
{
    return (DTfloat) position / (DTfloat) _pixels_per_second;
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::resizeEvent (QResizeEvent *event)
{
    _title = QRect(0,0,NODE_ITEM_WIDTH, TITLE_HEIGHT);
    _timeline = QRect(NODE_ITEM_WIDTH,0,rect().width() - NODE_ITEM_WIDTH - _scroll_width, TITLE_HEIGHT);
    _nodes = QRect(0,TITLE_HEIGHT, NODE_ITEM_WIDTH, rect().height() - TITLE_HEIGHT);
    _controls = QRect(NODE_ITEM_WIDTH, TITLE_HEIGHT, rect().width() - NODE_ITEM_WIDTH - _scroll_width, rect().height() - TITLE_HEIGHT - _scroll_height);
        
    // Place scrollbars
    _horz_scrollbar->setGeometry(NODE_ITEM_WIDTH, rect().height() - _scroll_height, rect().width() - NODE_ITEM_WIDTH - _scroll_width, _scroll_height);
    _vert_scrollbar->setGeometry(rect().width() - _scroll_width, TITLE_HEIGHT, _scroll_width, rect().height() - TITLE_HEIGHT - _scroll_height);
    
    // Scale scrollbars
    DTfloat content_height = _tracks.size() * NODE_ITEM_HEIGHT;
    DTint page_step = rect().height() - TITLE_HEIGHT - _scroll_height;
    _vert_scrollbar->setPageStep(page_step);
    _vert_scrollbar->setRange(0,content_height - page_step);
    
    // Scale fields
    _time_min->setGeometry(NODE_ITEM_WIDTH, rect().height() - _scroll_height - TIME_FIELD_HEIGHT, TIME_FIELD_WIDTH, TIME_FIELD_HEIGHT);
    _time_max->setGeometry(rect().width() - _scroll_width - TIME_FIELD_WIDTH, rect().height() - _scroll_height - TIME_FIELD_HEIGHT, TIME_FIELD_WIDTH, TIME_FIELD_HEIGHT);
    
    onChangeTimeRange();
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::onScrollTime(int time)
{
    _time = positionToTime(time);
    update();
}

void EdSpriteTimeline::onScroll(int scroll)
{
    _scroll = scroll;
    update();
}

void EdSpriteTimeline::onChangeTimeRange()
{
    _time_min_sec = _time_min->text().toFloat();
    _time_max_sec = _time_max->text().toFloat();
    
    if (_time < _time_min_sec)   _time = _time_min_sec;
    if (_time > _time_max_sec)   _time = _time_max_sec;
    
    DTint page_step = _controls.width();
    _horz_scrollbar->setPageStep(page_step);
        
    _horz_scrollbar->setRange( timeToPosition(_time_min_sec), timeToPosition(_time_max_sec) - page_step);

    update();
}

//==============================================================================
//==============================================================================

DTint EdSpriteTimeline::compare(const Track &a, const Track &b)
{
    if (a._track_name < b._track_name)  return -1;
    if (a._track_name > b._track_name)  return 1;
    return 0;
}

void EdSpriteTimeline::onSpriteChanged (void)
{
    _tracks.clear();
    
    KeyedSpriteResource *sprite = _main_window->getSprite();
    if (!sprite)
        return;

    Array<KeyedSpriteAnimationPoseJoint*> &joints = sprite->getJoints();
    
    KeyedSpriteAnimation *animation = _main_window->getSelectedAnimation();
    if (!animation)
        return;

    DTfloat ypos = 0.0F;

    for (DTuint i = 0; i < joints.size(); ++i) {
        Track t;
        t._track_name = joints[i]->getName();
        t._ypos = 0.0F;
        t._track = animation->getTrackByName(t._track_name);
        t._selected = _main_window->isPartSelected(joints[i]);

        _tracks.pushBack(t);
    }
    
    Algorithm::bubblesort(_tracks, &EdSpriteTimeline::compare);
    
    for (DTuint i = 0; i < _tracks.size(); ++i) {
        Track &t = _tracks[i];
        t._ypos = ypos;

        ypos += NODE_ITEM_HEIGHT;
    }

    // Scale scrollbars
    DTfloat content_height = _tracks.size() * NODE_ITEM_HEIGHT;
    DTint page_step = rect().height() - TITLE_HEIGHT - _scroll_height;
    _vert_scrollbar->setPageStep(page_step);
    _vert_scrollbar->setRange(0,content_height - page_step);
    
    update();
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::mouseDoubleClickEvent   (QMouseEvent *event)
{
    event->accept();
}

void EdSpriteTimeline::mousePressEvent(QMouseEvent *event)
{
    _main_window->pushUndo();

	_start_point = _last_point = _end_point = event->pos();
    
    // Translate into timeline space
    QPoint local_pos = _end_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT + _scroll);

    // Check click in thumb
    if (_timeline.contains(event->pos())) {
        
        setMode(MODE_DRAGGING_THUMB);
        
        float grid = _main_window->getGrid();
        float time = positionToTime(local_pos.x());
    
        // Snap to grid
        if (grid > 0.0F)
            time = std::floor(time / grid + 0.5F) * grid;

        _main_window->setCurrentTime(time);

    } else if (_controls.contains(_end_point)) {
        
        if ( (event->buttons() & Qt::LeftButton) && ( (event->buttons() & Qt::MidButton) || (event->modifiers() & Qt::ALT)) ) {
            setMode(MODE_ZOOMING);
            
        } else {
        
            //
            // Try selecting keyframes
            //
            
            for (int i = 0; i < _tracks.size(); ++i) {
            
                if (!_tracks[i]._track)
                    continue;
                            
                DTuint num_keys = (DTuint) _tracks[i]._track->getKeyframes().size();
                for (DTint k = 0; k < num_keys; ++k) {
                    QRect rk;
                    keyRect (_tracks[i], k, rk);
                    
                    if (rk.contains(local_pos)) {
                        DTint id = _tracks[i]._track->getKeyID(k);
                 
                        // Check select new keyframe        
                        if (!isSelected(_tracks[i], id)) {
                            clearSelected();
                            setSelected(_tracks[i], id, true);
                        }
                        
                        setMode(MODE_DRAGGING);
                        update();
                        
                        return;
                    }
                }
                
                
            }
            
            //
            // Try selecting events
            //
            
            KeyedSpriteAnimation *animation = _main_window->getSelectedAnimation();
            if (animation) {
                KeyedSpriteAnimationEventTrack& event_track = animation->getEventTrack();
                const Array<KeyedSpriteAnimationEvent>& keyframes = event_track.getKeyframes();
                
                for (int k = 0; k < keyframes.size(); ++k) {
                    QRect rk;
                    eventRect(k, rk);
                
                    if (rk.contains(local_pos)) {
                        DTint id = event_track.getKeyID(k);
                 
                        // Check select new keyframe        
                        if (!isSelectedEvent(id)) {
                            clearSelectedEvent();
                            setSelectedEvent(id, true);
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

void EdSpriteTimeline::mouseMoveEvent(QMouseEvent *event)
{		
	_end_point = event->pos();
	//QPoint dist = _end_point - _start_point;
	QPoint delta = _end_point - _last_point;

    // Translate into timeline space
    QPoint local_pos = _end_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT);

    switch (getMode()) {
        case MODE_DRAGGING_THUMB: {
                float grid = _main_window->getGrid();
                float time = positionToTime(local_pos.x());
            
                // Snap to grid
                if (grid > 0.0F)
                    time = std::floor(time / grid + 0.5F) * grid;
        
                _main_window->setCurrentTime(time);
            } break;

		case MODE_DRAG_SELECTING: {
                update();   // Just displaying selection rect
            } break;
            
		case MODE_DRAGGING: {

                DTfloat dt = positionToTime(delta.x());

                //
                // Try dragging keyframes
                //
        
                for (int i = 0; i < _tracks.size(); ++i) {
                
                    if (!_tracks[i]._track)
                        continue;
                                
                    DTuint num_keys = (DTuint) _tracks[i]._track->getKeyframes().size();
                    
                    for (DTint k = 0; k < num_keys; ++k) {
                        DTint id = _tracks[i]._track->getKeyID(k);
                        
                        if (isSelected(_tracks[i], id)) {
                            DTfloat current_time = _tracks[i]._track->getKeyTime(k);
                            _tracks[i]._track->setKeyTime(k,current_time+dt);
                        }

                    }
                    
                    _tracks[i]._track->updateKeyframes();
                    
                }
            
                //
                // Try dragging events
                //

                KeyedSpriteAnimation *animation = _main_window->getSelectedAnimation();
                if (animation) {
                    KeyedSpriteAnimationEventTrack& event_track = animation->getEventTrack();
                    const Array<KeyedSpriteAnimationEvent>& keyframes = event_track.getKeyframes();
                
                    for (int k = 0; k < keyframes.size(); ++k) {
                        DTint id = event_track.getKeyID(k);
                        
                        if (isSelectedEvent(id)) {
                            DTfloat current_time = event_track.getKeyTime(k);
                            event_track.setKeyTime(k,current_time+dt);
                        }
                    }
                }

                update();
            } break;
            
        case MODE_ZOOMING: {
                DTfloat center_time_before = (_horz_scrollbar->value() + _controls.width() / 2) / (DTfloat) _pixels_per_second;

                DTfloat zoom = -delta.y();
                _pixels_per_second += zoom; 
                
                if (_pixels_per_second < 5)         _pixels_per_second = 5;
                else if (_pixels_per_second > 500)  _pixels_per_second = 500;
                
                DTfloat center_time_after= (_horz_scrollbar->value() + _controls.width() / 2) / (DTfloat) _pixels_per_second;

                _horz_scrollbar->setValue(_horz_scrollbar->value() - (center_time_after - center_time_before) * _pixels_per_second);
                
                onChangeTimeRange();
            } break;
            
        default:
            break;

    };

    _last_point = _end_point;
}

void EdSpriteTimeline::mouseReleaseEvent(QMouseEvent *event)
{
	_end_point = event->pos();

    // Translate into timeline space
    QPoint local_pos = _end_point + QPoint(timeToPosition(_time) - NODE_ITEM_WIDTH, -TITLE_HEIGHT);

    switch (getMode()) {
        case MODE_DRAGGING_THUMB: {  
                float grid = _main_window->getGrid();
                float time = positionToTime(local_pos.x());
            
                // Snap to grid
                if (grid > 0.0F)
                    time = std::floor(time / grid + 0.5F) * grid;
        
                _main_window->setCurrentTime(time);
            } break;
                    
        case MODE_DRAG_SELECTING: {
                QRect rs;
                selectionRect(rs);
            
                //
                // Selecting Keyframes
                //
                        
                for (int i = 0; i < _tracks.size(); ++i) {
                
                    if (!_tracks[i]._track)
                        continue;
                
                    DTuint num_keys = (DTuint) _tracks[i]._track->getKeyframes().size();

                    // Append selection
                    if (event->modifiers() & Qt::SHIFT) {


                        for (DTint k = 0; k < num_keys; ++k) {
                            QRect rk;
                            keyRect (_tracks[i], k, rk);

                            if (rs.intersects(rk)) {
                            
                                DTint id = _tracks[i]._track->getKeyID(k);
                                
                                // Invert keyframes        
                                if (isSelected(_tracks[i], id))
                                    setSelected(_tracks[i], id, false);
                                else
                                    setSelected(_tracks[i], id, true);
                                
                            }
                        }
                        
                    // New selection
                    } else {
                    
                        clearSelected (_tracks[i]);
                    
                        for (DTint k = 0; k < num_keys; ++k) {
                            QRect rk;
                            keyRect (_tracks[i], k, rk);

                            if (rs.contains(rk)) {
                                DTint id = _tracks[i]._track->getKeyID(k);
                                setSelected(_tracks[i], id, true);
                                
                           }
                        }
                    }
                    
                }
            
                //
                // Selecting Events
                //
            
                KeyedSpriteAnimation *animation = _main_window->getSelectedAnimation();
                if (animation) {

                    KeyedSpriteAnimationEventTrack& event_track = animation->getEventTrack();
                    const Array<KeyedSpriteAnimationEvent>& keyframes = event_track.getKeyframes();

                    DTuint num_keys = (DTuint) keyframes.size();

                    // Append selection
                    if (event->modifiers() & Qt::SHIFT) {

                        for (DTint k = 0; k < num_keys; ++k) {
                            QRect rk;
                            eventRect (k, rk);

                            if (rs.intersects(rk)) {
                            
                                DTint id = event_track.getKeyID(k);
                                
                                // Invert keyframes        
                                if (isSelectedEvent(id))
                                    setSelectedEvent(id, false);
                                else
                                    setSelectedEvent(id, true);
                                
                            }
                        }
                        
                    // New selection
                    } else {
                    
                        clearSelectedEvent();
                    
                        for (DTint k = 0; k < num_keys; ++k) {
                            QRect rk;
                            eventRect (k, rk);

                            if (rs.intersects(rk)) {
                                DTint id = event_track.getKeyID(k);
                                setSelectedEvent(id, true);
                                
                           }
                        }
                    }
                    
                }
                
                update();
            } break;
            
		case MODE_DRAGGING: {
                update();
            } break;
        
        default:
            break;

    }
    
    setMode(MODE_NONE);
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::wheelEvent (QWheelEvent *event)
{
    int move = -event->delta();

    if (event->orientation() == Qt::Horizontal) {
    
        if (event->modifiers() & Qt::ShiftModifier)
            _main_window->setCurrentTime(_main_window->getCurrentTime() + (DTfloat) move/(DTfloat) _pixels_per_second * 0.5F);
        else
            _horz_scrollbar->setValue(_horz_scrollbar->value() + move);
            
    } else {
        _vert_scrollbar->setValue(_vert_scrollbar->value() + move);
    }

    update();

    event->accept();
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::keyPressEvent (QKeyEvent *event)
{
    int key = event->key();

    if (event->matches(QKeySequence::Delete) || key == 0x1000003) {

        //
        // Delete keys
        //

        for (int i = 0; i < _tracks.size(); ++i) {
        
            if (!_tracks[i]._track)
                continue;
                        
            for (DTint k = 0; k < _tracks[i]._track->getKeyframes().size(); ++k) {
                DTint id = _tracks[i]._track->getKeyID(k);
                
                if (isSelected(_tracks[i], id)) {
                    _tracks[i]._track->eraseKey(k);
                }
                
            }

            _tracks[i]._track->updateKeyframes();
            clearSelected(_tracks[i]);
        }
        
        //
        // Delete events
        //
        
        KeyedSpriteAnimation *animation = _main_window->getSelectedAnimation();
        if (animation) {
            KeyedSpriteAnimationEventTrack& event_track = animation->getEventTrack();
            const Array<KeyedSpriteAnimationEvent>& keyframes = event_track.getKeyframes();
        
            for (int k = 0; k < keyframes.size(); ++k) {
                DTint id = event_track.getKeyID(k);
                            
                if (isSelectedEvent(id)) {
                    event_track.eraseKey(k);
                }
            }
            
            clearSelectedEvent();
        }
        
        update();
    }
}

//==============================================================================
//==============================================================================

void EdSpriteTimeline::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	draw(&painter);
}

void EdSpriteTimeline::draw(QPainter *painter)
{

    KeyedSpriteAnimation *animation = _main_window->getSelectedAnimation();

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
    
    for (int i = 0; i < _tracks.size(); ++i) {
    
        DTboolean selected = _tracks[i]._selected;
    
        QRect tr, r;
        itemRect(_tracks[i], tr, r);
        
        if (selected) {
            painter->setPen(QPen(QColor(100,100,100,255),0));
            painter->setBrush(QBrush(QColor(53,120,255,255)));
        } else {
            painter->setPen(QPen(QColor(100,100,100,255),0));
            painter->setBrush(QBrush(QColor(130,130,130,255)));
        }
        
        painter->drawRect(r);

        painter->setPen(QPen(QColor(40,40,40,255),1));
        painter->setFont(_font);
        painter->drawText(tr, Qt::AlignLeft | Qt::AlignVCenter, _tracks[i]._track_name.cStr() );
    }
    
    //
    // Draw Node Keyframes
    //

    painter->resetTransform ();
    painter->setClipRect ( _controls );
    painter->translate (NODE_ITEM_WIDTH - timeToPosition(_time), TITLE_HEIGHT - _scroll);
    
    for (int i = 0; i < _tracks.size(); ++i) {
    
        if (!_tracks[i]._track)
            continue;
    
        QRect tr, r;
        itemRect(_tracks[i], tr, r);
        
        // Build a new Rect based on this one
        r.setLeft( timeToPosition(_time_min_sec) );
        r.setWidth( timeToPosition(_time_max_sec - _time_min_sec) );
        
        painter->setPen(QPen(QColor(100,100,100,255),0));
        painter->setBrush(QBrush(QColor(130,130,130,255)));
        painter->drawRect(r);            

        // Draw guide line
        painter->setPen(QPen(QColor(100,100,100,255),1));
        painter->drawLine( timeToPosition(_time_min_sec), r.center().y(), timeToPosition(_time_max_sec - _time_min_sec), r.center().y());
        
        DTuint num_keys = (DTuint) _tracks[i]._track->getKeyframes().size();
        if (num_keys > 0) {
        
            // Draw regions between keyframes
            for (DTint k = 0; k < num_keys-1; ++k) {
                QRect r1,r2;
                keyRect (_tracks[i], k, r1);
                keyRect (_tracks[i], k+1, r2);
                
                QRect r = r1.united(r2);
                r.adjust(_keyframe_image.width()/2, 0, -_keyframe_image.width()/2, 0);
                
                painter->setPen(Qt::NoPen);
                painter->setBrush(QBrush(QColor(0,0,0,30)));
                painter->drawRect(r);            
            }
            
            // Draw keyframes        
            for (DTint k = 0; k < num_keys; ++k) {
                QRect r;
                keyRect (_tracks[i], k, r);
                
                DTint id = _tracks[i]._track->getKeyID(k);
                
                if (isSelected (_tracks[i], id))
                    painter->drawImage(r, _keyframe_image_selected);
                else
                    painter->drawImage(r, _keyframe_image);
            }
        }
        
    }
    
    
    //
    // Selection
    //

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
    
    
    int grid = _main_window->getGrid() * 1000;
    if (grid < 100) grid = 100;
    // Draw grid
    
    
    // Grid
    for (int t = _time_min_sec*1000; t <= _time_max_sec*1000; t += grid) {
        
        int xpos = timeToPosition(t/1000.0F);
    
        painter->setPen(QPen(QColor(90,90,90,255),1,Qt::DotLine));
        painter->drawLine( QPoint(xpos, TITLE_HEIGHT) , QPoint(xpos, rect().height() - _scroll_height) );    
    }

    // Full seconds
    for (int t = _time_min_sec*1000; t <= _time_max_sec*1000; t += 1000) {
        
        int xpos = timeToPosition(t/1000.0F);
    
        if (t == 0) painter->setPen(QPen(QColor(75,75,75,255),3,Qt::SolidLine));
        else        painter->setPen(QPen(QColor(75,75,75,255),1,Qt::SolidLine));
        
        painter->drawLine( QPoint(xpos, TITLE_HEIGHT) , QPoint(xpos, rect().height() - _scroll_height) );
        
        QRect tr(xpos - _pixels_per_second/2,0,_pixels_per_second,TITLE_HEIGHT);
        
        painter->setPen(QPen(QColor(25,25,25,255),1,Qt::SolidLine));
        
        std::stringstream ss;
        ss << (t/1000) << " s";
        painter->drawText(tr, Qt::AlignHCenter | Qt::AlignBottom, ss.str().c_str());
    }
    
    //
    // Draw Event Track
    //
    
    painter->resetTransform ();
    painter->setClipRect ( _timeline.united(_controls) );
    painter->translate (NODE_ITEM_WIDTH - timeToPosition(_time), 0);
    
    painter->setFont(_font);

    if (animation) {
        KeyedSpriteAnimationEventTrack& event_track = animation->getEventTrack();
        const Array<KeyedSpriteAnimationEvent>& keyframes = event_track.getKeyframes();
        
        for (DTint i = 0; i < keyframes.size(); ++i) {
            const KeyedSpriteAnimationEvent &event = keyframes[i];
            
            int xpos = timeToPosition(event.getTime());
            
            if (isSelectedEvent(event.getKeyID())) {
                painter->setPen(QPen(QColor(255,0,0,255),3,Qt::SolidLine));
            } else {
                painter->setPen(QPen(QColor(255,0,0,128),2,Qt::SolidLine));
            }
            
            painter->drawLine( QPoint(xpos, TITLE_HEIGHT) , QPoint(xpos, rect().height() - _scroll_height) );
        }
    }
    

    //
    // Draw Current Time
    //

    painter->resetTransform ();
    painter->setClipRect ( _timeline.united(_controls) );
    painter->translate (NODE_ITEM_WIDTH - timeToPosition(_time), 0);
    
    DTfloat t = _main_window->getCurrentTime();
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

#include "moc_EdSpriteTimeline.cpp"


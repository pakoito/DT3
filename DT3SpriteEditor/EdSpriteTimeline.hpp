#ifndef EDSPRITETIMELINE
#define EDSPRITETIMELINE
//==============================================================================
///	
///	File: EdSpriteTimeline.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <QtWidgets/QWidget>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include "KeyedSpriteAnimationKeyframe.hpp"
#include "KeyedSpriteAnimationTrack.hpp"

using namespace DT2;

//==============================================================================
/// Forward declarations
//==============================================================================

class EdSpriteMainWindow;
class QScrollBar;
class EdSpriteLineEdit;

//==============================================================================
/// Class
//==============================================================================

class EdSpriteTimeline : public QWidget
{
    Q_OBJECT

	public:
                                    EdSpriteTimeline				(EdSpriteMainWindow *main_window);
    
        void                        clearKeySelection               (void);
                                    		
	protected:
		void						draw							(QPainter *painter);
		void						paintEvent						(QPaintEvent *event);
        
        void                        mouseDoubleClickEvent           (QMouseEvent *event);
		void						mousePressEvent					(QMouseEvent *event);
		void						mouseMoveEvent					(QMouseEvent *event);
		void						mouseReleaseEvent				(QMouseEvent *event);

        void                        keyPressEvent                   (QKeyEvent *event);
        void                        wheelEvent                      (QWheelEvent *event);

        void                        resizeEvent                     (QResizeEvent *event);

    public slots:   
        void                        onScrollTime                    (int time);
        void                        onScroll                        (int scroll);
        void                        onChangeTimeRange               (void);
        
        void                        onSpriteChanged                 (void);
        
        void                        onDuplicate                     (void);

    private:
        static const DTint TITLE_HEIGHT = 20;
        static const DTint NODE_ITEM_WIDTH = 200;
        static const DTint NODE_ITEM_HEIGHT = 20;
        static const DTint TIME_FIELD_WIDTH = 75;
        static const DTint TIME_FIELD_HEIGHT = 15;

        struct Track {
            String                      _track_name;
            DTfloat                     _ypos;
            KeyedSpriteAnimationTrack   *_track;
            
            DTboolean                   _selected;

            List<DTint>                 _selected_ids;
        };
        Array<Track>                _tracks;
    
        List<DTint>                 _selected_event_ids;

    
        static DTint                compare                         (const Track &a, const Track &b);

        void                        itemRect                        (Track &t, QRect &tr, QRect &r) const;
        void                        keyRect                         (Track &t, DTuint k, QRect &rk) const;
        void                        eventRect                       (DTuint k, QRect &rk) const;
        void                        thumbRect                       (QRect &r) const;
        void                        selectionRect                   (QRect &r) const;
        
        DTint                       timeToPosition                  (DTfloat seconds) const;
        DTfloat                     positionToTime                  (DTint position) const;
        
        void                        setSelected                     (Track &t, DTuint id, DTboolean selected);
        void                        clearSelected                   (Track &t);
        DTboolean                   isSelected                      (Track &t, DTuint id);
        void                        clearSelected                   (void);

        void                        setSelectedEvent                (DTuint id, DTboolean selected);
        DTboolean                   isSelectedEvent                 (DTuint id);
        void                        clearSelectedEvent              (void);


        EdSpriteMainWindow          *_main_window;

        // Rectangles
        QRect                       _nodes;
        QRect                       _controls;
        QRect                       _title;
        QRect                       _timeline;
        
        DTint                       _scroll_width;
        DTint                       _scroll_height;
        DTint                       _nodes_height;

        QImage                      _thumb_image;
        QImage                      _keyframe_image;
        QImage                      _keyframe_image_selected;
        
        DTint                       _pixels_per_second;
        
        QScrollBar                  *_horz_scrollbar;
        QScrollBar                  *_vert_scrollbar;
        
        EdSpriteLineEdit             *_time_min;
        EdSpriteLineEdit             *_time_max;
        
        QFont                       _font;
        QFont                       _font_bold;
        QFontMetrics                _fm;

        DTfloat                     _time;      // Horizontal scroll
        DTint                       _scroll;    // Vertical scroll
                
        DTint                       _time_min_sec;
        DTint                       _time_max_sec;
        
        
        
        // Interactions
		enum Mode {
			MODE_NONE,
			MODE_DRAGGING_THUMB,           // Delta and no item
            MODE_DRAG_SELECTING,
            MODE_DRAGGING,
            MODE_ZOOMING
        } _mode;
        
		void                        setMode         (Mode mode)     {   _mode = mode;   }
        Mode                        getMode         (void)          {   return _mode;   }
        
		QPoint						_start_point;
		QPoint						_last_point;
		QPoint						_end_point;

};

//==============================================================================
//==============================================================================

#endif

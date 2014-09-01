#ifndef EDLEVELANIMATIONWINDOW
#define EDLEVELANIMATIONWINDOW
//==============================================================================
///	
///	File: EdLevelAnimationWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtWidgets/QWidget>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QAction>

// Engine includes
#include "DT3Core/Scripting/ScriptingKeyframes.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class ScriptingKeyframes;
    class ScriptingKeyframesRoot;
    class PlugNode;
    class PlugBase;
    class Event;
}

class EdLevelDocument;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelAnimationWindow : public QWidget
{
    Q_OBJECT

	public:
                                    EdLevelAnimationWindow	(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);
        virtual                     ~EdLevelAnimationWindow	(void);

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
        void                        onSelectionChanged              (const std::list<std::shared_ptr<PlugNode>> &selection_list);
        void                        onScrollTime                    (int time);
        void                        onScroll                        (int scroll);
        
        void                        onChangeTimeRange               (void);
        void                        onRefreshAnimation              (void);

        void                        onAnimPlay                      (void);
        void                        onAnimStop                      (void);

    signals:
        void                        doCommand                      (QString command);
        void                        doUndoBlock                     (void);
        void                        doSelectionChanged              (const std::list<std::shared_ptr<PlugNode>> &selection_list);

    private:
        static const int TITLE_HEIGHT = 20;
        static const int NODE_ITEM_WIDTH = 200;
        static const int NODE_ITEM_HEIGHT = 20;
        static const int TIME_FIELD_WIDTH = 75;
        static const int TIME_FIELD_HEIGHT = 15;

        struct PlugEventCache {
            PlugEventCache (void)    
                :   _plug       (NULL),
                    _event      (NULL),
                    _ypos       (0)
            {}

            PlugEventCache (const PlugEventCache &rhs)
            {
                _node = rhs._node;
                _plug = rhs._plug;
                _event = rhs._event;
                _keyframes = rhs._keyframes;
                
                _title = rhs._title;
                _ypos = rhs._ypos;
            }
                                                
            PlugEventCache& operator = (const PlugEventCache &rhs)
            {
                _node = rhs._node;
                _plug = rhs._plug;
                _event = rhs._event;
                _keyframes = rhs._keyframes;
                
                _title = rhs._title;
                _ypos = rhs._ypos;

                return *this;
            }

            ~PlugEventCache (void)    
            {

            }
            
            bool operator == (const PlugEventCache &rhs) const  {   return _plug == rhs._plug;  }
            bool operator != (const PlugEventCache &rhs) const  {   return _plug != rhs._plug;  }
                    
            std::shared_ptr<PlugNode>           _node;
            
            PlugBase                            *_plug;
            Event                               *_event;
            
            std::shared_ptr<ScriptingKeyframes> _keyframes;
            
            std::string                         _title;
            int                                 _ypos;
            
            std::list<int>                    _selected_ids;
        };
        
        
        int                         timeToPosition                  (float seconds) const;
        float                       positionToTime                  (int position) const;
        
        void                        itemRect                        (PlugEventCache &n, QRect &tr, QRect &r) const;
        void                        thumbRect                       (QRect &r) const;
        void                        selectionRect                   (QRect &r) const;
        void                        keyRect                         (PlugEventCache &p, int k, QRect &r) const;

        void                        layoutItems                     (std::vector<PlugEventCache> &plug_event_cache);
        void                        scanRoot                        (const std::shared_ptr<ScriptingKeyframesRoot> &root, std::vector<PlugEventCache> &plug_event_cache);
        void                        scanRelevantNodes               (const std::list<std::shared_ptr<PlugNode>> &selection_list);

        // Keyframes access
        int                         getNumKeys                      (PlugEventCache &p) const                       {   return (int) p._keyframes->num_keys();  }
        float                       getKeyTime                      (PlugEventCache &p, int k) const                {   return p._keyframes->key_time(k);       }
        void                        setKeyTime                      (PlugEventCache &p, int k, float time) const    {   p._keyframes->set_key_time(k,time);     }

        void                        setSelected                     (PlugEventCache &p, int id, bool selected);
        void                        clearSelected                   (PlugEventCache &p);
        bool                        isSelected                      (PlugEventCache &p, int id);

        void                        clearSelected                   (void);

        std::vector<PlugEventCache> _plug_event_cache;
        
        EdLevelDocument             *_document;
        QToolBar                    *_toolbar;
        
		QAction                     *_anim_play;
		QAction                     *_anim_stop;

        std::shared_ptr<ScriptingKeyframesRoot> _root;

        QFont                       _font;
        QFont                       _font_bold;
        QFontMetrics                _fm;
        
        // Rectangles
        QRect                       _nodes;
        QRect                       _controls;
        QRect                       _title;
        QRect                       _timeline;
        
        QScrollBar                  *_horz_scrollbar;
        QScrollBar                  *_vert_scrollbar;
        
        EdLevelLineEdit             *_time_min;
        EdLevelLineEdit             *_time_max;
        
        int                       _scroll_width;
        int                       _scroll_height;
        int                       _nodes_height;
        
        int                       _pixels_per_second;
        
        float                     _time;      // Horizontal scroll
        int                       _scroll;    // Vertical scroll
        
        int                       _time_min_sec;
        int                       _time_max_sec;
        
        QImage                      _thumb_image;
        QImage                      _keyframe_image;
        QImage                      _keyframe_image_selected;
        
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

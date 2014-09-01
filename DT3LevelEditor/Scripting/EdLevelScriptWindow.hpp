#ifndef EDLEVELSCRIPTWINDOW
#define EDLEVELSCRIPTWINDOW
//==============================================================================
///	
///	File: EdLevelScriptWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QToolBar>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QTimer>
#include <QtWidgets/QMenu>

// Engine includes

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlugNode;
    class WorldNode;
    class PlugBase;
    class Event;
    class ComponentBase;
    class Group;
}

class EdLevelDocument;
class EdLevelGroup;
class EdLevelScriptConnectionRubberBand;
class EdLevelScriptSelectionRubberBand;
class EdLevelScriptNodeStandard;
class EdLevelScriptPlugConnection;
class EdLevelScriptEventConnection;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelScriptWindow : public QGraphicsView
{
    Q_OBJECT

	public:
										EdLevelScriptWindow	(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);

        void                            dragEnterEvent (QDragEnterEvent *event);
        void                            dragMoveEvent (QDragMoveEvent *event);
        void                            dropEvent (QDropEvent *event);
        

	protected:
        void                            mouseDoubleClickEvent   (QMouseEvent *event);
		void							mousePressEvent		(QMouseEvent *event);
		void							mouseMoveEvent		(QMouseEvent *event);
		void							mouseReleaseEvent	(QMouseEvent *event);
        
        void                            keyPressEvent       (QKeyEvent *event);

		void							drawBackground		(QPainter *painter, const QRectF &rect);

	private:
        static const int AUTO_SCROLL_TIMER;
        static const int AUTO_SCROLL_MARIGN;

        EdLevelDocument                 *_document;
        QToolBar                        *_toolbar;
        
		QAction *						_script_align;
		QAction *						_script_values;
		QAction *						_script_add_to_library;
		QAction *						_script_color;
        
		QGraphicsScene					_scene;
        
        QTimer                          _auto_scroll_timer;
        int                             _auto_scroll_x;
        int                             _auto_scroll_y;

        void                            syncConnections     (std::shared_ptr<PlugNode> node);
        void                            adjustGroupBounds   (std::shared_ptr<Group> group);

		void							readjustSceneRect   (void);

		enum Mode {
			MODE_NONE,
			MODE_CLICKING,              // Left mouse button down
			MODE_DRAGGING,              // Delta and no item
			MODE_DRAG_SELECTING,        // Delta and selected item
			MODE_CONNECTING,
			MODE_PANNING,
			MODE_ZOOMING
		} _mode;

		void                                setMode         (Mode mode);
        Mode                                getMode         (void)          {   return _mode;   }

		QPoint								_start_point;
		QPoint								_last_point;
		QPoint								_end_point;

		QPointF								_start_point_scene;
		QPointF								_last_point_scene;
		QPointF								_end_point_scene;

		float								_scale;
		unsigned int						_z;

        // Rubber band widgets
		EdLevelScriptConnectionRubberBand	*_connection_rubber_band;
		EdLevelScriptSelectionRubberBand	*_selection_rubber_band;
        
        // Connection info while connecting
        EdLevelScriptNodeStandard           *_connection_start_node;
        QPoint                              _connection_start_location;
        bool                                _connection_start_location_is_input;
        PlugBase                            *_connection_start_plug;
        Event                               *_connection_start_event;
        
             
        // Internal representation of scene
        struct NodeCache {
            NodeCache(void)                             {   _scene_object = NULL; }
            NodeCache(std::shared_ptr<WorldNode> node)  {   _node = node; _scene_object = NULL; }
            
            bool   operator == (const NodeCache& rhs) const	{	return _node == rhs._node;		}

            std::shared_ptr<WorldNode>      _node;
            EdLevelScriptNodeStandard       *_scene_object;
        };

        struct GroupCache {
            GroupCache(void)                            {   _scene_object = NULL; }
            GroupCache(std::shared_ptr<Group> group)    {   _group = group; _scene_object = NULL; }
            
            bool   operator == (const GroupCache& rhs) const	{	return _group == rhs._group;    }

            std::shared_ptr<Group>          _group;
            EdLevelGroup                    *_scene_object;
        };
    
        struct ConnectionPlugCache {
            ConnectionPlugCache(void)   
                {   _outgoing = NULL; _incoming = NULL; _scene_object = NULL; }
            ConnectionPlugCache(PlugBase *outgoing, PlugBase *incoming)   
                {   _outgoing = outgoing; _incoming = incoming; _scene_object = NULL; }
            
            bool   operator == (const ConnectionPlugCache& rhs) const	{	return (_outgoing == rhs._outgoing) && (_incoming == rhs._incoming);		}

            PlugBase                        *_outgoing;
            PlugBase                        *_incoming;
            EdLevelScriptPlugConnection     *_scene_object;
        };

        struct ConnectionEventCache {
            ConnectionEventCache(void)  
                {   _outgoing = NULL; _incoming = NULL; _scene_object = NULL; }
            ConnectionEventCache(Event *outgoing, Event *incoming)   
                {   _outgoing = outgoing; _incoming = incoming; _scene_object = NULL; }

            bool   operator == (const ConnectionEventCache& rhs) const	{	return (_outgoing == rhs._outgoing) && (_incoming == rhs._incoming);		}

            Event                           *_outgoing;
            Event                           *_incoming;
            EdLevelScriptEventConnection    *_scene_object;
       };
        
        std::list<NodeCache>                _node_cache;
        std::list<GroupCache>               _group_cache;
        std::list<ConnectionPlugCache>      _plug_cache;
        std::list<ConnectionEventCache>     _event_cache;
        
        EdLevelScriptNodeStandard*      getSceneObjectForNode       (std::shared_ptr<PlugNode> node);
        EdLevelGroup*                   getSceneObjectForGroup      (std::shared_ptr<Group> group);
        
        
    public slots:
        void                            onAddNode                   (WorldNode *node);
        void                            onRemoveNode                (WorldNode *node);
        void                            onReparentNode              (WorldNode *node, WorldNode *old_parent, WorldNode *new_parent);

        void                            onAddGroup                  (Group *group);
        void                            onRemoveGroup               (Group *group);

        void                            onAddComponent              (ComponentBase *node);
        void                            onRemoveComponent           (ComponentBase *node);

        void                            onConnectPlug               (PlugBase *outgoing, PlugBase *incoming);
        void                            onDisconnectPlug            (PlugBase *outgoing, PlugBase *incoming);
        void                            onConnectEvent              (Event *outgoing, Event *incoming);
        void                            onDisconnectEvent           (Event *outgoing, Event *incoming);

        void                            selectionChanged            (void);
        
        void                            onRefreshScript             (void);
        void                            onSelectionChanged          (const std::list<std::shared_ptr<PlugNode>> &selection_list);
        
        void                            onNodeContextMenu           (std::shared_ptr<WorldNode> node, const QPointF &global_pos);
        void                            onComponentContextMenu      (std::shared_ptr<ComponentBase> component, const QPointF &global_pos);
        
        void                            onAutoScroll                (void);
        
        void                            onScriptAlign               (void);
        void                            onScriptToggleValues        (void);
        bool                            onScriptAddToLibrary        (void);
        void                            onScriptColor               (void);

    signals:
        void                            doCommand(QString command);
        void                            doUndoBlock                 (void);
        void                            doRefreshScript();
        void                            doSelectionChanged          (const std::list<std::shared_ptr<PlugNode>> &selection_list);
        
        void                            doNodeContextMenu           (std::shared_ptr<WorldNode> node, const QPointF &global_pos);
        void                            doComponentContextMenu      (std::shared_ptr<ComponentBase> component, const QPointF &global_pos);

};

//==============================================================================
//==============================================================================

#endif

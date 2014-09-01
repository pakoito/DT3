#ifndef EDLEVELSCRIPTNODESTANDARD
#define EDLEVELSCRIPTNODESTANDARD
//==============================================================================
///	
///	File: EdLevelScriptNodeStandard.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QGraphicsItem>
#include <QtCore/QObject>
#include <QtGui/QFont>
#include <QtGui/QMouseEvent>

// Engine includes
#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <deque>

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlugNode;
    class WorldNode;
    class PlugBase;
    class Event;
    class ComponentBase;
}
class EdLevelMainWindow;
class EdLevelScriptConnection;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

struct QInputOutputPlug {
    QInputOutputPlug (void) {   _input_position = _output_position = -1;    _plug = NULL;   }
    
	int                             _input_position;
	int                             _output_position;
    
    QPoint                          _input_location;
    QPoint                          _output_location;
    
    PlugBase                        *_plug;
};

struct QInputOutputEvent {
    QInputOutputEvent (void) {   _input_position = _output_position = -1;   _event = NULL;   }
    
	int                             _input_position;
	int                             _output_position;

    QPoint                          _input_location;
    QPoint                          _output_location;
    
    Event                           *_event;
};

struct QComponentTitle {    
	int                             _position;
    QString                         _title;

    std::shared_ptr<ComponentBase>  _component;
};

//==============================================================================
/// Class
//==============================================================================

class EdLevelScriptNodeStandard : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    
	public:
        enum { Type = UserType + 1 };
        int type() const {  return Type;    }

    public:
									EdLevelScriptNodeStandard	(std::shared_ptr<WorldNode> node);
                                    ~EdLevelScriptNodeStandard  (void);


	public:
        std::shared_ptr<WorldNode>  getNode             (void)      {   return _node;   }
        
        bool                        getPlugAtLocation   (QPointF pos, QPoint &actual_pos, bool &is_input, PlugBase *&plug);
        bool                        getEventAtLocation  (QPointF pos, QPoint &actual_pos, bool &is_input, Event *&event);
        
        QPoint                      getPlugLocation     (PlugBase *plug, bool as_input);
        QPoint                      getEventLocation    (Event *event, bool as_input);

        QVariant                    itemChange      (GraphicsItemChange change, const QVariant &value);
        
		QRectF						boundingRect	(void) const;
		void						paint			(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                        addConnection   (EdLevelScriptConnection* connection);
        void                        removeConnection(EdLevelScriptConnection* connection);
        
        bool                        checkClick      (const QPointF &scene_pos, const QPointF &global_pos);
        bool                        handleClick     (const QPointF &scene_pos, const QPointF &global_pos);
        
        QRectF                      getNodeActionRect   (void);
        QRectF                      getNodeShrinkRect   (void);
        QRectF                      getNodeEditorRect   (void);
        QRectF                      getTitleActionRect  (const QComponentTitle& title);

		void						layoutNode          (void);
        void                        layoutConnections   (void);
        
	private:
        static const float TITLE_HEIGHT;
        static const float ITEM_HEIGHT;
        static const float ITEM_PAD;
        static const float WIDTH;
        static const float CONNECTOR_RADIUS;

        static const float SHADOW_OFFSET_X;
        static const float SHADOW_OFFSET_Y;

        static const int ICON_SIZE;
        static const int ICON_PAD;

        void                        addPlugsAndEvents(int &input_pos, int &output_pos, std::shared_ptr<PlugNode> node);
        
        std::shared_ptr<WorldNode>  _node;
      
        std::list<EdLevelScriptConnection*> _connections;
		std::deque<QInputOutputPlug>        _plugs;
		std::deque<QInputOutputEvent>       _events;
        std::deque<QComponentTitle>         _titles;

		float						_width;
		float						_height;

		float						_input_width;
		float						_output_width;
		float						_title_width;

		QFont						_title_font;
		QFont						_item_font;
                        
    signals:
        void                        doNodeContextMenu           (std::shared_ptr<WorldNode> node, const QPointF &global_pos);
        void                        doComponentContextMenu      (std::shared_ptr<ComponentBase> component, const QPointF &global_pos);

};

//==============================================================================
//==============================================================================

#endif

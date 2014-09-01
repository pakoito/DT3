#ifndef EDLEVELGROUP
#define EDLEVELGROUP
//==============================================================================
///	
///	File: EdLevelGroup.hpp
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
#include <QtGui/QFont>

// Engine includes

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class Group;
}

class EdLevelScriptNodeStandard;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelGroup : public QGraphicsItem
{
    //Q_OBJECT
    //Q_INTERFACES(QGraphicsItem)
    
	public:
        enum { Type = UserType + 4 };
        int type() const {  return Type;    }

	public:
                                    EdLevelGroup    (void)  {}
                                    EdLevelGroup    (std::shared_ptr<Group> group);
                                    ~EdLevelGroup   (void);


	public:
        std::shared_ptr<Group>      getGroup        (void)                  {   return _group;   }

        void                        setBoundingRect (const QRectF &rect);
		QRectF						boundingRect	(void) const            {   return _bounding_rect;  }
        //QPainterPath				shape			(void) const;

		virtual void				paint			(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
        bool                        checkClick      (const QPointF &scene_pos, const QPointF &global_pos);
        bool                        handleClick     (const QPointF &scene_pos, const QPointF &global_pos);

	protected:
		QFont						_title_font;
        QRectF                      _bounding_rect;
        std::shared_ptr<Group>      _group;

    private:
        static const float SHADOW_OFFSET_X;
        static const float SHADOW_OFFSET_Y;

};

//==============================================================================
//==============================================================================

#endif

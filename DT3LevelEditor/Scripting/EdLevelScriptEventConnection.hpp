#ifndef EDLEVELSCRIPTEVENTCONNECTION
#define EDLEVELSCRIPTEVENTCONNECTION
//==============================================================================
///	
///	File: EdLevelScriptEventConnection.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptConnection.hpp"

// Qt include
#include <QtGui/QFont>

// Engine includes
#include "DT3Core/Types/Node/Event.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

class EdLevelMainWindow;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelScriptEventConnection : public EdLevelScriptConnection
{
    //Q_OBJECT
    
	public:
        enum { Type = UserType + 3 };
        int type() const {  return Type;    }
        
	public:
                                    EdLevelScriptEventConnection    (void)  {}
									EdLevelScriptEventConnection	(   EdLevelScriptNodeStandard *node1, Event *event1,
                                                                        EdLevelScriptNodeStandard *node2, Event *event2);
                                                                
                                    ~EdLevelScriptEventConnection()  {}

        void                        disconnect      (void)          {   _event1 = _event2 = NULL; }

        Event*                      event1          (void) const    {   return _event1;  }
        Event*                      event2          (void) const    {   return _event2;  }

	public:
        virtual void                readjustBounds  (void);

		virtual void				paint			(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        Event                       *_event1;
        Event                       *_event2;
    
    private:
        static const float SHADOW_OFFSET_X;
        static const float SHADOW_OFFSET_Y;
        static const float BEZIER_SIZE;
        static const float OFFSET;


};

//==============================================================================
//==============================================================================

#endif

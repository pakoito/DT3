#ifndef EDLEVELSCRIPTCONNECTION
#define EDLEVELSCRIPTCONNECTION
//==============================================================================
///	
///	File: EdLevelScriptConnection.hpp
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

class EdLevelScriptNodeStandard;

//==============================================================================
/// Class
//==============================================================================

class EdLevelScriptConnection : public QGraphicsItem
{
    //Q_OBJECT

	public:
                                    EdLevelScriptConnection (void)  {}
									EdLevelScriptConnection	(   EdLevelScriptNodeStandard *node1,
                                                                EdLevelScriptNodeStandard *node2);
                                                                
                                    ~EdLevelScriptConnection();


	public:
		QRectF						boundingRect	(void) const;
		QPainterPath				shape			(void) const;

        virtual void                readjustBounds  (void) = 0;
        void                        removeLink      (EdLevelScriptNodeStandard *node);
        

	protected:
        EdLevelScriptNodeStandard   *_node1;
        EdLevelScriptNodeStandard   *_node2;
        
        QPoint                      _scene_location1;
        QPoint                      _scene_location2;
        
        QPoint                      _p1_local;
        QPoint                      _p2_local;
    
    private:
        static const float SHADOW_OFFSET_X;
        static const float SHADOW_OFFSET_Y;
        static const float BEZIER_SIZE;
        static const float OFFSET;

};

//==============================================================================
//==============================================================================

#endif

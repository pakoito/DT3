#ifndef EDSPRITEBITFIELD
#define EDSPRITEBITFIELD
//==============================================================================
///	
///	File: EdSpriteBitfield.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <QtWidgets/QGraphicsView>
#include <QtGui/QBrush>
#include "Types.hpp"

using namespace DT2;

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class EdSpriteBitfield : public QWidget
{
    Q_OBJECT

	public:
                            EdSpriteBitfield		(QWidget *parent = NULL);

	public:
		void                paintEvent				(QPaintEvent *event);
        void                draw                    (QPainter *painter);

        void                setBitfield             (DTushort bitfield) {   _bitfield = bitfield;   }
        DTushort            getBitfield             (void) const        {   return _bitfield;       }

		void                mousePressEvent         (QMouseEvent *event);
		void                mouseMoveEvent          (QMouseEvent *event);
		void                mouseReleaseEvent       (QMouseEvent *event);

	private:
        QFont               _font;
        DTushort            _bitfield;
    
    signals:
        void                stateChanged            (DTushort bitfield);
};

//==============================================================================
//==============================================================================

#endif

#ifndef EDLEVELCOLORSWATCH
#define EDLEVELCOLORSWATCH
//==============================================================================
///	
///	File: EdLevelColorSwatch.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QGraphicsView>
#include <QtGui/QBrush>

// Engine includes


//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelColorSwatch : public QWidget
{
    Q_OBJECT

	public:
                            EdLevelColorSwatch		(void);

	public:
        void                setBrush                (QBrush brush)          {   _brush = brush; }
		void                paintEvent				(QPaintEvent *event);

	private:
		QBrush              _brush;
};

//==============================================================================
//==============================================================================

#endif

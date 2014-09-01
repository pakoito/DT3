#ifndef EDLEVELSCRIPTSELECTIONRUBBERBAND
#define EDLEVELSCRIPTSELECTIONRUBBERBAND
//==============================================================================
///	
///	File: EdLevelScriptSelectionRubberBand.hpp
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

// Engine includes


//==============================================================================
/// Forward declarations
//==============================================================================

class EdLevelMainWindow;

//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelScriptSelectionRubberBand : public QWidget
{
    Q_OBJECT

	public:
										EdLevelScriptSelectionRubberBand		(void);


	public:
		void							paintEvent					(QPaintEvent *event);
		void							setEndpoints				(const QPoint &p0, const QPoint &p1);

	private:
		QPoint		_p0_local;
		QPoint		_p1_local;
};

//==============================================================================
//==============================================================================

#endif

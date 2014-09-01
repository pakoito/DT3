#ifndef EDLEVELSCRIPTCONNECTIONRUBBERBAND
#define EDLEVELSCRIPTCONNECTIONRUBBERBAND
//==============================================================================
///	
///	File: EdLevelScriptConnectionRubberBand.hpp
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

//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelScriptConnectionRubberBand : public QWidget
{
    Q_OBJECT

	public:
										EdLevelScriptConnectionRubberBand		(void);


	public:
		void							paintEvent					(QPaintEvent *event);
		void							setEndpoints				(const QPoint &p0, const QPoint &p1, bool reverse_spline);

	private:
        static const float SHADOW_OFFSET_X;
        static const float SHADOW_OFFSET_Y;
        static const float BEZIER_SIZE;
        static const float OFFSET;

        QPoint      _p0;
        QPoint      _p1;
        bool        _reverse_spline;
        
		QPoint		_p0_local;
		QPoint		_p1_local;
};

//==============================================================================
//==============================================================================

#endif

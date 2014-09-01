#ifndef EDLEVELSOUNDVUMETER
#define EDLEVELSOUNDVUMETER
//==============================================================================
///	
///	File: EdLevelSoundVUMeter.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>

// Engine includes

//==============================================================================
/// Forward declarations
//==============================================================================

class EdLevelDocument;
class QDial;
class EdLevelLineEdit;

//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelSoundVUMeter : public QWidget
{
    Q_OBJECT

	public:
                                    EdLevelSoundVUMeter     (QWidget *parent);

		void						draw                    (QPainter *painter);
		void						paintEvent              (QPaintEvent *event);
        void                        resizeEvent             (QResizeEvent *event);

        void                        setLevels               (float left, float right);
        
    private:
        float                       _left;
        float                       _right;
};

//==============================================================================
//==============================================================================

#endif

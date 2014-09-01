#ifndef EDLEVELPERFORMANCEWINDOW
#define EDLEVELPERFORMANCEWINDOW
//==============================================================================
///	
///	File: EdLevelPerformanceWindow.hpp
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
#include <QtWidgets/QScrollBar>
#include <QtGui/QWheelEvent>
#include <QtCore/QTimer>

// Engine includes
#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class ProfilerCategory;
}

class EdLevelDocument;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelPerformanceWindow : public QWidget
{
    Q_OBJECT

	public:
                                    EdLevelPerformanceWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);

        void                        resizeEvent             (QResizeEvent *event);

    protected:
		void						draw					(QPainter *painter);
		void						paintEvent				(QPaintEvent *event);

        void                        wheelEvent              (QWheelEvent *event);

    private:
        static const DTint NUM_SAMPLES;
        static const DTfloat ENTRY_HEIGHT;
        static const DTfloat TITLE_WIDTH;
        static const DTfloat TITLE_HEIGHT;
        static const DTfloat LABEL_HEIGHT;

        QFont                       _font;
        QFont                       _font_small;

        struct Data {
            ProfilerCategory        *_category;
            std::list<DTfloat>      _values;
        };
    
        std::list<Data>             _data;
    
        QScrollBar                  *_vert_scrollbar;
        DTint                       _scroll_width;

        QTimer                      _sample_timer;

        EdLevelDocument             *_document;
        QToolBar                    *_toolbar;
    
    public slots:
        void                        onSample                (void);
        void                        onScroll                (int scroll);

};

//==============================================================================
//==============================================================================

#endif

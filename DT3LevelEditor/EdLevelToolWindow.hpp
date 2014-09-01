#ifndef EDLEVELTOOLWINDOW
#define EDLEVELTOOLWINDOW
//==============================================================================
///	
///	File: EdLevelToolWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelDocument.hpp"

// Qt include
#include <QtOpenGL/QGLWidget>

// Engine includes


//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class EdLevelToolWindow : public QGLWidget
{
    Q_OBJECT

	public:
										EdLevelToolWindow       (QWidget *parent, EdLevelDocument *document);

    public slots:
        void                            onCommand               (QString command);

    signals:
        void                            doCommand              (QString command);
};

//==============================================================================
//==============================================================================

#endif

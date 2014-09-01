#ifndef EDSPRITEANIMATIONPROPERTIES
#define EDSPRITEANIMATIONPROPERTIES
//==============================================================================
///	
///	File: EdSpriteAnimationProperties.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QToolBar>

#include "EdSpriteMainWindow.hpp"
#include "ArchivePropertyReaderWriter.hpp"

using namespace DT2;

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class EdSpriteAnimationProperties : public QListWidget
{
    Q_OBJECT

	public:
										EdSpriteAnimationProperties		(EdSpriteMainWindow *main_window);

    public slots:   
        void                            onSpriteChanged             (void);
        void                            onSelectionChanged          (void);
        void                            onAnimationsChanged         (void);

	signals:
		void                            doReadAllProperties         (void);

	private:
        EdSpriteMainWindow              *_main_window;
        ArchivePropertyReaderWriter     _properties;

        void                            buildProperties             (void);
        void                            clearProperties             (void);
        
};

//==============================================================================
//==============================================================================

#endif

#ifndef EDSPRITEANIMATIONS
#define EDSPRITEANIMATIONS
//==============================================================================
///	
///	File: EdSpriteAnimations.hpp
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

class EdSpriteAnimations : public QListWidget
{
    Q_OBJECT

	public:
										EdSpriteAnimations          (EdSpriteMainWindow *main_window);

        void                            buildList                   (void);        
        void                            clearList                   (void);        

    public slots:   
        void                            onSpriteChanged             (void);
        void                            onSelectionChanged          (void);
        void                            onAnimationsChanged         (void);

        void                            onItemChanged               ( QListWidgetItem * item );
        void                            onItemClicked               ( QListWidgetItem * item );

	signals:
		void                            doReadAllProperties         (void);

	private:
        EdSpriteMainWindow              *_main_window;

};

//==============================================================================
//==============================================================================

#endif

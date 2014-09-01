#ifndef EDSPRITEPROPERTYDEFAULTFIELD
#define EDSPRITEPROPERTYDEFAULTFIELD
//==============================================================================
///	
///	File: EdSpritePropertyDefaultField.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpritePropertyBase.hpp"
#include "EdSpriteMainWindow.hpp"
#include "EdSpriteLineEdit.hpp"
#include "ArchiveData.hpp"
#include <QtWidgets/QToolButton>
#include <QtWidgets/QListWidget>

//==============================================================================
/// Forward declarations
//==============================================================================

class EdSpriteProperties;

//==============================================================================
//==============================================================================

using namespace DT2;

//==============================================================================
//==============================================================================

class EdSpritePropertyDefaultField : public EdSpritePropertyBase
{
    Q_OBJECT

	public:
										EdSpritePropertyDefaultField    (QListWidget *parent, EdSpriteMainWindow *main_window, ArchiveData *data);
										~EdSpritePropertyDefaultField	(void);
                                        
    private:
        EdSpriteMainWindow              *_main_window;
        ArchiveData                     *_data;
        EdSpriteLineEdit                *_value;
                
    public slots:
        void                            doReadParams                    (void);
        void                            doWriteParams                   (void);
};

//==============================================================================
//==============================================================================

#endif

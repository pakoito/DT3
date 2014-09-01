#ifndef EDSPRITEPROPERTYBITMASKFIELD
#define EDSPRITEPROPERTYBITMASKFIELD
//==============================================================================
///	
///	File: EdSpritePropertyBitfieldField.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpritePropertyBase.hpp"
#include "EdSpriteMainWindow.hpp"
#include "ArchiveData.hpp"
#include "EdSpriteBitfield.hpp"
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

class EdSpritePropertyBitfieldField : public EdSpritePropertyBase
{
    Q_OBJECT

	public:
										EdSpritePropertyBitfieldField    (QListWidget *parent, EdSpriteMainWindow *main_window, ArchiveData *data);
										~EdSpritePropertyBitfieldField   (void);
                                        
    private:
        EdSpriteMainWindow              *_main_window;
        ArchiveData                     *_data;
        EdSpriteBitfield                *_value;

    public slots:
        void                            doReadParams                    (void);
        void                            doWriteParams                   (void);
};

//==============================================================================
//==============================================================================

#endif

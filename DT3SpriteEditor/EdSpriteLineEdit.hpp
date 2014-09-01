#ifndef EDSPRITELINEEDIT
#define EDSPRITELINEEDIT
//==============================================================================
///	
///	File: EdSpriteLineEdit.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <QtWidgets/QLineEdit>
#include <QtGui/QKeyEvent>

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class EdSpriteLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    EdSpriteLineEdit (QWidget *parent);
    
    virtual void        keyPressEvent (QKeyEvent *event);
};

//==============================================================================
//==============================================================================

#endif

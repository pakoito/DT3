//==============================================================================
///	
///	File: EdSpriteLineEdit.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteLineEdit.hpp"

//==============================================================================
//==============================================================================

EdSpriteLineEdit::EdSpriteLineEdit (QWidget *parent)
    :   QLineEdit(parent)
{

}

void EdSpriteLineEdit::keyPressEvent (QKeyEvent *event)
{
    if (event->modifiers() & (Qt::ControlModifier | Qt::MetaModifier) &&
        event->key() == Qt::Key_Z) {
        
        event->ignore();
    } else {
        QLineEdit::keyPressEvent(event);
    }
}

//==============================================================================
//==============================================================================

#include "moc_EdSpriteLineEdit.cpp"


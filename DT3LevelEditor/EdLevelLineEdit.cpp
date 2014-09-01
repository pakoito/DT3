//==============================================================================
///	
///	File: EdLevelLineEdit.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include

// Qt include
#include <QtGui/QKeyEvent>

// Engine includes
#include "EdLevelLineEdit.hpp"

//==============================================================================
//==============================================================================

EdLevelLineEdit::EdLevelLineEdit (QWidget *parent)
    :   QLineEdit(parent)
{

}

void EdLevelLineEdit::keyPressEvent (QKeyEvent *event)
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

#include "moc_EdLevelLineEdit.cpp"


//==============================================================================
///	
///	File: EdLevelToolWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelToolWindow.hpp"

// Qt include
#include <QtWidgets/QPushButton>

// Engine includes

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelToolWindow::EdLevelToolWindow(QWidget *parent, EdLevelDocument *document)
    :   QGLWidget       (parent)
{

}

//==============================================================================
//==============================================================================

void EdLevelToolWindow::onCommand (QString command)
{
    emit doCommand(command);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelToolWindow.cpp"


//==============================================================================
///	
///	File: EdLevelAppMain.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelAppMain.hpp"
#include "EdLevelMainWindow.hpp"

// Qt include
#include <QtWidgets/QApplication>
#include <QtCore/QFile>

// Engine includes
#include "DT3Core/Entry/GameMainThread.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/Configure.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Application.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Devices/DeviceNetwork.hpp"
#include "DT3Core/Devices/DeviceInput.hpp"
#include "DT3Core/Devices/DeviceMusic.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"

#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID || DT3_OS == DT3_MACOSX
    #include "DT3OpenGL/DT3OpenGL.hpp"
#else
    #include "DT3DX11/DT3GLDX11.hpp"
#endif

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

int main (int argc, char *argv[])
{
    GameMainThread::initialize_engine();

    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication::setStyle("fusion");
    
    // Set style sheet
    QFile file(":/LevelEditor.qss");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style_sheet = file.readAll();
    file.close();
     
    QApplication app(argc, argv);
    app.setStyleSheet(style_sheet);
        
    EdLevelMainWindow *window = new EdLevelMainWindow();
    window->show();
        
	QObject::connect(	&app,       SIGNAL(focusChanged(QWidget*,QWidget*)),
                        window,     SLOT(onAppFocusChanged(QWidget*,QWidget*))	);

    
    app.exec();
        
    GameMainThread::destroy_engine();
}

//==============================================================================
//==============================================================================


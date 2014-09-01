//==============================================================================
///	
///	File: EdSpriteAppMain.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteAppMain.hpp"
#include "EdSpriteMainWindow.hpp"

#include <QtWidgets/QApplication>
#include <QtCore/QFile>
#include <QtWidgets/QLabel>

#include "System.hpp"

#include "Application.hpp"
#include "Configure.hpp"

#include "DeviceGraphics.hpp"
#include "DeviceAudio.hpp"

#include "DeviceInput.hpp"
#include "ResourceManager.hpp"
#include "ConsoleStream.hpp"
#include "Profiler.hpp"
#include "DeviceNetwork.hpp"
#include "DeviceReplication.hpp"
#include "FileManager.hpp"

#include "MaterialResource.hpp"
#include "CheckedCast.hpp"
#include "Factory.hpp"

using namespace DT2;

//==============================================================================
//==============================================================================

int main (int argc, char *argv[])
{
	StaticInitializer::initialize();

	// Import a config file for the engine
	Configure::importConfig(FilePath("{config_engine.txt}"));
    
	//
	// Create Managers
	//

	LOG_MESSAGE << "Creating Input Manager...";
	DeviceInput		*input_manager = checkedCast<DeviceInput *>(Factory::createObject("{CLASS_INPUT_MANAGER}"));
	System::setInputManager(input_manager);
	RELEASE(input_manager);

	LOG_MESSAGE << "Creating Resource Manager...";
	ResourceManager		*resource_manager = checkedCast<ResourceManager *>(Factory::createObject("{CLASS_RESOURCE_MANAGER}"));	
	System::setResourceManager(resource_manager);
	RELEASE(resource_manager);

	LOG_MESSAGE << "Creating Network Manager...";
	DeviceNetwork		*network_manager = checkedCast<DeviceNetwork *>(Factory::createObject("{CLASS_NETWORK_MANAGER}"));	     
	System::setNetworkManager(network_manager);
	RELEASE(network_manager);

	LOG_MESSAGE << "Creating Replication Manager...";
	DeviceReplication	*replication_manager = checkedCast<DeviceReplication *>(Factory::createObject("{CLASS_REPLICATION_MANAGER}"));	     
	System::setReplicationManager(replication_manager);
	RELEASE(replication_manager);

	LOG_MESSAGE << "Creating Renderer...";
	DeviceGraphics		*renderer = checkedCast<DeviceGraphics *>(Factory::createObject("{CLASS_RENDERER}"));
	System::setRenderer(renderer);
	RELEASE(renderer);

	LOG_MESSAGE << "Creating Audio Device...";
	DeviceAudio		*audio_device = checkedCast<DeviceAudio *>(Factory::createObject("{CLASS_AUDIO_DEVICE}"));
	System::setAudioRenderer(audio_device);
	RELEASE(audio_device);
		
	//
	// Application object
	//
	
	// Build the application class
	Application *application = checkedCast<Application *>(Factory::createObject("{CLASS_APPLICATION}"));
	System::setApplication(application);
	RELEASE(application);


    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication::setStyle("fusion");
    
    // Set style sheet
    QFile file(":/PropertyList.qss");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style_sheet = file.readAll();
    file.close();
     
    QApplication app(argc, argv);
    app.setStyleSheet(style_sheet);

    EdSpriteMainWindow *window = new EdSpriteMainWindow();
    window->show();
    
	QObject::connect(	&app,       SIGNAL(focusChanged(QWidget*,QWidget*)),
                        window,     SLOT(onAppFocusChanged(QWidget*,QWidget*))	);

    
    app.exec();
        
	//
	// Destroy Managers
	//
	
	LOG_MESSAGE << "Cleaning up Application...";
	System::setApplication(NULL);

	LOG_MESSAGE << "Cleaning up Audio Device...";
   	System::setAudioRenderer(NULL);
	
	LOG_MESSAGE << "Cleaning up Renderer...";
	System::setRenderer(NULL);

	LOG_MESSAGE << "Cleaning up Replication Manager...";
	System::setReplicationManager(NULL);

	LOG_MESSAGE << "Cleaning up Network Manager...";
	System::setNetworkManager(NULL);	
		
	LOG_MESSAGE << "Cleaning up Resource Manager...";
    System::setResourceManager(NULL);
		
	LOG_MESSAGE << "Cleaning up Input Manager...";
    System::setInputManager(NULL);
    
	StaticInitializer::destroy();
}

//==============================================================================
//==============================================================================


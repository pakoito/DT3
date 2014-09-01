//==============================================================================
///	
///	File: EdLevelMainWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelDocument.hpp"
#include "EdLevelMainWindow.hpp"
#include "EdLevelWorldWindow.hpp"
#include "EdLevelScriptWindow.hpp"
#include "EdLevelAnimationWindow.hpp"
#include "EdLevelSoundWindow.hpp"
#include "EdLevelConsoleWindow.hpp"
#include "EdLevelResourcesWindow.hpp"
#include "EdLevelLibraryWindow.hpp"
#include "EdLevelPerformanceWindow.hpp"
#include "EdLevelPropertiesWindow.hpp"
#include "EdLevelHierarchyWindow.hpp"
#include "EdLevelLaunchDialog.hpp"
#include "EdLevelWorldLayout.hpp"
#include "EdLevelMeshBuilder.hpp"

// Qt include
#include <QtGui/QtGui>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QSlider>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenuBar>
#include <QtOpenGL/QGLFormat>
#include <QtCore/QFile>

// Engine includes
#include "DT3Core/Entry/GameMainThread.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/Console.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveTextWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include DT3_HAL_INCLUDE_PATH
#include <limits>
#include <set>

//==============================================================================
//==============================================================================

namespace DT3 {
    extern std::map<std::string,std::shared_ptr<CreatorBase>>&	world_map       (void);
    
    extern std::map<std::string,std::set<std::string>>&         placeable_map	(void);
    extern std::map<std::string,std::set<std::string>>&         component_map	(void);
    extern std::map<std::string,std::set<std::string>>&         script_map      (void);
}

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelMainWindow::EdLevelMainWindow(void)
{
    setAttribute(Qt::WA_TranslucentBackground, false);

    createActions(); 
    
    // Register error handlers
    AssertImpl::set_callback_assert_msg(make_callback(this, &EdLevelMainWindow::showAssert));
    ErrorImpl::set_callback_error_msg(make_callback(this, &EdLevelMainWindow::showError));
    ErrorImpl::set_callback_warning_msg(make_callback(this, &EdLevelMainWindow::showWarning));
    
    // Register callbacks for engine events
    SystemCallbacks::add_node_cb().add(make_callback(this, &EdLevelMainWindow::addNodeCB));
    SystemCallbacks::remove_node_cb().add(make_callback(this, &EdLevelMainWindow::removeNodeCB));
    SystemCallbacks::reparent_node_cb().add(make_callback(this, &EdLevelMainWindow::reparentNodeCB));

    SystemCallbacks::add_group_cb().add(make_callback(this, &EdLevelMainWindow::addGroupCB));
    SystemCallbacks::remove_group_cb().add(make_callback(this, &EdLevelMainWindow::removeGroupCB));

    SystemCallbacks::add_component_cb().add(make_callback(this, &EdLevelMainWindow::addComponentCB));
    SystemCallbacks::remove_component_cb().add(make_callback(this, &EdLevelMainWindow::removeComponentCB));

    SystemCallbacks::connect_plug_cb().add(make_callback(this, &EdLevelMainWindow::connectPlugCB));
    SystemCallbacks::disconnect_plug_cb().add(make_callback(this, &EdLevelMainWindow::disconnectPlugCB));
    SystemCallbacks::connect_event_cb().add(make_callback(this, &EdLevelMainWindow::connectEventCB));
    SystemCallbacks::disconnect_event_cb().add(make_callback(this, &EdLevelMainWindow::disconnectEventCB));
    
    
    // Create a new document
    _document = new EdLevelDocument();


    // Set editor callbacks

    // Turn on Anti aliasing
    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(4);
    QGLFormat::setDefaultFormat(glf);
    
    // Dock config
    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

    //
	// World Window
    //
    
    QWidget *world_section = new QWidget(this);
    
    _world_toolbar = new QToolBar(world_section);
	_world_toolbar->setIconSize(QSize(16,16));
    
	_world_widget = new EdLevelWorldWindow(world_section, _world_toolbar, _document);
    //_world_widget->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));

    EdLevelWorldLayout *layout1 = new EdLevelWorldLayout;
    layout1->setContentsMargins(0,0,0,0);
    //layout1->setHorizontalSpacing(1);
    //layout1->setVerticalSpacing(1);
    layout1->addWidget(_world_toolbar);
    layout1->addWidget(_world_widget);
    
    world_section->setLayout(layout1);
    
    _world_widget->onChangeResolution(0);
    
    //
    // Views Window
    //
    
    //QWidget *views_section = new QWidget(this);
    

    // Script window
    QWidget *script_section = new QWidget(this);
    
    _script_toolbar = new QToolBar(script_section);
	_script_toolbar->setIconSize(QSize(16,16));
   
    _script_widget = new EdLevelScriptWindow(script_section, _script_toolbar, _document);

    QGridLayout *layout3 = new QGridLayout;
    layout3->setContentsMargins(0,0,0,0);
    layout3->setHorizontalSpacing(1);
    layout3->setVerticalSpacing(1);
    layout3->addWidget(_script_toolbar);
    layout3->addWidget(_script_widget);

    script_section->setLayout(layout3);
    
    QDockWidget *scriptDock = new QDockWidget(tr("Script"), this);
    scriptDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    scriptDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    scriptDock->setWidget(script_section);
    addDockWidget(Qt::BottomDockWidgetArea, scriptDock);

    
    
    // Animation Window
    QWidget *animation_section = new QWidget(this);
        
    _animation_toolbar = new QToolBar(animation_section);
	_animation_toolbar->setIconSize(QSize(16,16));
    
	_animation_widget = new EdLevelAnimationWindow(animation_section, _animation_toolbar, _document);

    QGridLayout *layout4 = new QGridLayout;
    layout4->setContentsMargins(0,0,0,0);
    layout4->setHorizontalSpacing(1);
    layout4->setVerticalSpacing(1);
    layout4->addWidget(_animation_toolbar);
    layout4->addWidget(_animation_widget);

    animation_section->setLayout(layout4);

    QDockWidget *animationDock = new QDockWidget(tr("Animation"), this);
    animationDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    animationDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    animationDock->setWidget(animation_section);
    addDockWidget(Qt::BottomDockWidgetArea, animationDock);


    // Sound Window
    QWidget *sound_section = new QWidget(this);
        
    _sound_toolbar = new QToolBar(sound_section);
	_sound_toolbar->setIconSize(QSize(16,16));
    
	_sound_widget = new EdLevelSoundWindow(sound_section, _sound_toolbar, _document);

    QGridLayout *layout8 = new QGridLayout;
    layout8->setContentsMargins(0,0,0,0);
    layout8->setHorizontalSpacing(1);
    layout8->setVerticalSpacing(1);
    layout8->addWidget(_sound_toolbar);
    layout8->addWidget(_sound_widget);

    sound_section->setLayout(layout8);

    QDockWidget *soundDock = new QDockWidget(tr("Sound"), this);
    soundDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    soundDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    soundDock->setWidget(sound_section);
    addDockWidget(Qt::BottomDockWidgetArea, soundDock);


    /*// Sound FX Window
    QWidget *sound_fx_section = new QWidget(this);
        
    _sound_fx_toolbar = new QToolBar(sound_fx_section);
	_sound_fx_toolbar->setIconSize(QSize(16,16));
    
	_sound_fx_widget = new EdLevelSoundFXWindow(sound_fx_section, _sound_fx_toolbar, _document);

    QGridLayout *layout9 = new QGridLayout;
    layout9->setContentsMargins(0,0,0,0);
    layout9->setHorizontalSpacing(1);
    layout9->setVerticalSpacing(1);
    layout9->addWidget(_sound_fx_toolbar);
    layout9->addWidget(_sound_fx_widget);

    sound_fx_section->setLayout(layout9);

    QDockWidget *soundfxDock = new QDockWidget(tr("Sound FX"), this);
    soundfxDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    soundfxDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    soundfxDock->setWidget(sound_fx_section);
    addDockWidget(Qt::BottomDockWidgetArea, soundfxDock);*/


    // Console Window
    QWidget *console_section = new QWidget(this);

    _console_toolbar = new QToolBar(console_section);
	_console_toolbar->setIconSize(QSize(16,16));
    //_console_toolbar->addAction(_script_align);

    _console_widget = new EdLevelConsoleWindow(console_section, _console_toolbar, _document);
    
    QGridLayout *layout5 = new QGridLayout;
    layout5->setContentsMargins(0,0,0,0);
    layout5->setHorizontalSpacing(1);
    layout5->setVerticalSpacing(1);
    layout5->addWidget(_console_toolbar);
    layout5->addWidget(_console_widget);

    console_section->setLayout(layout5);

    QDockWidget *consoleDock = new QDockWidget(tr("Console"), this);
    consoleDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    consoleDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    consoleDock->setWidget(console_section);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);



    // Performance Window
    QWidget *performance_section = new QWidget(this);
    
    _performance_toolbar = new QToolBar(performance_section);
	_performance_toolbar->setIconSize(QSize(16,16));
    //_performance_toolbar->addAction(_performance_widget);

    _performance_widget = new EdLevelPerformanceWindow(performance_section, _performance_toolbar, _document);
    
    QGridLayout *layout7 = new QGridLayout;
    layout7->setContentsMargins(0,0,0,0);
    layout7->setHorizontalSpacing(1);
    layout7->setVerticalSpacing(1);
    layout7->addWidget(_performance_toolbar);
    layout7->addWidget(_performance_widget);

    performance_section->setLayout(layout7);

    QDockWidget *performanceDock = new QDockWidget(tr("Performance"), this);
    performanceDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    performanceDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    performanceDock->setWidget(performance_section);
    addDockWidget(Qt::BottomDockWidgetArea, performanceDock);

//    tabifyDockWidget ( scriptDock, animationDock);
//    tabifyDockWidget ( animationDock, soundDock);
//    tabifyDockWidget ( soundDock, soundfxDock);
//    tabifyDockWidget ( soundfxDock, consoleDock);
//    tabifyDockWidget ( consoleDock, performanceDock);

    tabifyDockWidget ( scriptDock, animationDock);
    tabifyDockWidget ( animationDock, soundDock);
    tabifyDockWidget ( soundDock, consoleDock);
    tabifyDockWidget ( consoleDock, performanceDock);

    scriptDock->show();
    scriptDock->raise();


//    QTabWidget *tabWidget = new QTabWidget(this);
//
//    tabWidget->addTab(script_section, QString("Script"));
//    tabWidget->addTab(animation_section, QString("Animation"));
//    tabWidget->addTab(sound_section, QString("Sound"));
//    tabWidget->addTab(sound_fx_section, QString("SoundFX"));
//    tabWidget->addTab(console_section, QString("Console"));
//    //tabWidget->addTab(resources_section, QString("Resources"));
//    //tabWidget->addTab(library_section, QString("Library"));
//    tabWidget->addTab(performance_section, QString("Performance"));
//    
//    QGridLayout *layout2 = new QGridLayout;
//    layout2->setContentsMargins(0,0,0,0);
//    layout2->setHorizontalSpacing(1);
//    layout2->setVerticalSpacing(1);
//    layout2->addWidget(tabWidget);
//    
//    views_section->setLayout(layout2);

    
    //
    // Hierarchy Window
    //
    QWidget *hierarchy_section = new QWidget(this);
    
    _hierarchy_toolbar = new QToolBar(hierarchy_section);
	_hierarchy_toolbar->setIconSize(QSize(16,16));
    
    _hierarchy_widget = new EdLevelHierarchyWindow(hierarchy_section, _hierarchy_toolbar, _document);

    QGridLayout *layout11 = new QGridLayout;
    layout11->setContentsMargins(0,0,0,0);
    layout11->setHorizontalSpacing(1);
    layout11->setVerticalSpacing(1);
    layout11->addWidget(_hierarchy_toolbar);
    layout11->addWidget(_hierarchy_widget);

    hierarchy_section->setLayout(layout11);
    
    QDockWidget *hierarchyDock = new QDockWidget(tr("Hierarchy"), this);
    hierarchyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    hierarchyDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    hierarchyDock->setWidget(hierarchy_section);
    addDockWidget(Qt::RightDockWidgetArea, hierarchyDock);

    //
    // Properties
    //
    
    _properties_widget = new EdLevelPropertiesWindow(this, NULL, _document);

    // Properties Dock
    QDockWidget *propertiesDock = new QDockWidget(tr("Properties"), this);
    propertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    propertiesDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    propertiesDock->setWidget(_properties_widget);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDock);
    
    //
    // Library
    //
    
    _library_widget = new EdLevelLibraryWindow(this, NULL, _document);
    
    // Library Dock
    QDockWidget *libraryDock = new QDockWidget(tr("Library"), this);
    libraryDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    libraryDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    libraryDock->setWidget(_library_widget);
    addDockWidget(Qt::RightDockWidgetArea, libraryDock);
        
    
    //
    // Resources
    //
    
    // Resources Window
    QWidget *resources_section = new QWidget(this);
    
    _resources_toolbar = new QToolBar(resources_section);
	_resources_toolbar->setIconSize(QSize(16,16));

    _resources_widget = new EdLevelResourcesWindow(resources_section, _resources_toolbar, _document,_world_widget);
    
    QGridLayout *layout6 = new QGridLayout;
    layout6->setContentsMargins(0,0,0,0);
    layout6->setHorizontalSpacing(1);
    layout6->setVerticalSpacing(1);
    layout6->addWidget(_resources_toolbar);
    layout6->addWidget(_resources_widget);

    resources_section->setLayout(layout6);
    
    QDockWidget *resourcesDock = new QDockWidget(tr("Resources"), this);

    resourcesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    resourcesDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    resourcesDock->setWidget(resources_section);
    addDockWidget(Qt::RightDockWidgetArea, resourcesDock);



    //
    // Tabify docks
    //
    
    tabifyDockWidget ( resourcesDock, libraryDock);
    tabifyDockWidget ( libraryDock, propertiesDock);

    

//    QSplitter *mainSplitter = new QSplitter(Qt::Vertical);
//    mainSplitter->addWidget(world_section);
//    mainSplitter->addWidget(views_section);
    
    /*QSplitter *propertiesSplitter = new QSplitter(Qt::Horizontal);
    propertiesSplitter->addWidget(mainSplitter);
    propertiesSplitter->addWidget(propertiesDock);
    propertiesSplitter->setStretchFactor(1, 1);*/

    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

	setCentralWidget(world_section);
    setWindowTitle(tr("Level Editor"));

    readSettings();

    //setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentLevelFile("");
    
    QWidget *widgets1[] = { this, _hierarchy_widget, _world_widget, _script_widget, _animation_widget, _sound_widget, /*_sound_fx_widget,*/ _console_widget, _resources_widget, _library_widget, _performance_widget, _properties_widget };
    
    for (DTint i = 0; i < ARRAY_SIZE(widgets1); ++i) {
        for (DTint j = 0; j < ARRAY_SIZE(widgets1); ++j) {
            connect(	widgets1[i],     SIGNAL(doRefreshScript()),
                        widgets1[j],     SLOT(onRefreshScript()) );

            connect(	widgets1[i],     SIGNAL(doRefreshWorld()),
                        widgets1[j],     SLOT(onRefreshWorld())	);
                        
            connect(	widgets1[i],     SIGNAL(doRefreshHierarchy()),
                        widgets1[j],     SLOT(onRefreshHierarchy())	);

            connect(	widgets1[i],     SIGNAL(doRefreshProperties()),
                        widgets1[j],     SLOT(onRefreshProperties()) );

            connect(	widgets1[i],     SIGNAL(doRefreshLibrary()),
                        widgets1[j],     SLOT(onRefreshLibrary()) );

            connect(	widgets1[i],     SIGNAL(doRefreshResources()),
                        widgets1[j],     SLOT(onRefreshResources())	);

            connect(	widgets1[i],     SIGNAL(doRefreshAnimation()),
                        widgets1[j],     SLOT(onRefreshAnimation())	);

            connect(	widgets1[i],     SIGNAL(doRefreshConsole()),
                        widgets1[j],     SLOT(onRefreshConsole()) );

            connect(	widgets1[i],     SIGNAL(doRefreshPerformance()),
                        widgets1[j],     SLOT(onRefreshPerformance()) );

            connect(	widgets1[i],     SIGNAL(doRefreshSound()),
                        widgets1[j],     SLOT(onRefreshSound()) );

            connect(	widgets1[i],     SIGNAL(doRefreshSoundFX()),
                        widgets1[j],     SLOT(onRefreshSoundFX()) );

            connect(	widgets1[i],     SIGNAL(doSelectionChanged(const std::list<std::shared_ptr<PlugNode>> &)),
                        widgets1[j],     SLOT(onSelectionChanged(const std::list<std::shared_ptr<PlugNode>> &))	);
        }
    }
    
    QWidget *widgets2[] = { _hierarchy_widget, _world_widget, _script_widget, _animation_widget, _sound_widget, /*_sound_fx_widget,*/ _console_widget, _resources_widget, _library_widget, _performance_widget, _properties_widget };
    
    for (DTint i = 0; i < ARRAY_SIZE(widgets2); ++i) {
        connect(	widgets2[i],        SIGNAL(doCommand(QString)),
                    this,               SLOT(onCommand(QString))	);

        connect(	widgets2[i],        SIGNAL(doUndoBlock()),
                    this,               SLOT(onUndoBlock())	);
                    
        // Engine events
        connect(    this,               SIGNAL(doAddNode(WorldNode*)),
                    widgets2[i],        SLOT(onAddNode(WorldNode*))  );

        connect(    this,               SIGNAL(doRemoveNode(WorldNode*)),
                    widgets2[i],        SLOT(onRemoveNode(WorldNode*))  );

        connect(    this,               SIGNAL(doReparentNode(WorldNode*, WorldNode*, WorldNode*)),
                    widgets2[i],        SLOT(onReparentNode(WorldNode*, WorldNode*, WorldNode*))  );


        connect(    this,               SIGNAL(doAddGroup(Group*)),
                    widgets2[i],        SLOT(onAddGroup(Group*))  );

        connect(    this,               SIGNAL(doRemoveGroup(Group*)),
                    widgets2[i],        SLOT(onRemoveGroup(Group*))  );


        connect(    this,               SIGNAL(doAddComponent(ComponentBase*)),
                    widgets2[i],        SLOT(onAddComponent(ComponentBase*))  );

        connect(    this,               SIGNAL(doRemoveComponent(ComponentBase*)),
                    widgets2[i],        SLOT(onRemoveComponent(ComponentBase*))  );

        connect(    this,               SIGNAL(doConnectPlug(PlugBase*, PlugBase*)),
                    widgets2[i],        SLOT(onConnectPlug(PlugBase*, PlugBase*))  );

        connect(    this,               SIGNAL(doDisconnectPlug(PlugBase*, PlugBase*)),
                    widgets2[i],        SLOT(onDisconnectPlug(PlugBase*, PlugBase*))  );
                    
        connect(    this,               SIGNAL(doConnectEvent(Event*, Event*)),
                    widgets2[i],        SLOT(onConnectEvent(Event*, Event*))  );

        connect(    this,               SIGNAL(doDisconnectEvent(Event*, Event*)),
                    widgets2[i],        SLOT(onDisconnectEvent(Event*, Event*))  );

    }
                
    // Context menus
    connect(    _script_widget,         SIGNAL(doNodeContextMenu(std::shared_ptr<WorldNode>, const QPointF &)),
                this,                   SLOT(onNodeContextMenu(std::shared_ptr<WorldNode>, const QPointF &))  );

    connect(    _script_widget,         SIGNAL(doComponentContextMenu(std::shared_ptr<ComponentBase>, const QPointF &)),
                this,                   SLOT(onComponentContextMenu(std::shared_ptr<ComponentBase>, const QPointF &))  );
                

    /*// Script Tools
    connect(    this,                   SIGNAL(doScriptAlign(void)),
                _script_widget,         SLOT(onScriptAlign(void))  );
    connect(    this,                   SIGNAL(doScriptToggleValues(void)),
                _script_widget,         SLOT(onScriptToggleValues(void))  );
    connect(    this,                   SIGNAL(doScriptAddToLibrary(void)),
                _script_widget,         SLOT(onScriptAddToLibrary(void))  );*/
      
    // Menu updating
    connect(    _file_menu,             SIGNAL(aboutToShow(void)),
                this,                   SLOT(onUpdateMenus(void))  );
    connect(    _edit_menu,             SIGNAL(aboutToShow(void)),
                this,                   SLOT(onUpdateMenus(void))  );
    connect(    _object_menu,           SIGNAL(aboutToShow(void)),
                this,                   SLOT(onUpdateMenus(void))  );
    connect(    _component_menu,        SIGNAL(aboutToShow(void)),
                this,                   SLOT(onUpdateMenus(void))  );
    connect(    _script_menu,           SIGNAL(aboutToShow(void)),
                this,                   SLOT(onUpdateMenus(void))  );
    connect(    _level_menu,            SIGNAL(aboutToShow(void)),
                this,                   SLOT(onUpdateMenus(void))  );
    connect(    _help_menu,             SIGNAL(aboutToShow(void)),
                this,                   SLOT(onUpdateMenus(void))  );
                
    // Set the default selection
    std::list<std::shared_ptr<PlugNode>> empty_selection;
    doSelectionChanged(empty_selection);
    
    
    _timer.start(200, this);
    _autosave_timer.start(60*1000*1, this); // 1 minute
        
    emit doRefreshWorld();
    
    GameMainThread::show_engine(1,1);
}

EdLevelMainWindow::~EdLevelMainWindow(void)
{
    GameMainThread::hide_engine();

    delete _document;
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::setSaveIcon (DTboolean icon_state)
{
    _save_level_action->setEnabled(icon_state);
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timer.timerId()) {
    
//        DTfloat STEP_SIZE = 200.0F / 1000.0F; // 200 msec
    
//        if (System::input_manager())          System::input_manager()->tick(STEP_SIZE);
//		if (System::network_manager())		System::network_manager()->tick(STEP_SIZE);
//		if (System::getReplicationManager())	System::getReplicationManager()->tick(STEP_SIZE);
//		if (System::getAudioRenderer())         System::getAudioRenderer()->tick(STEP_SIZE);
//		if (System::getMusicRenderer())         System::getMusicRenderer()->tick(STEP_SIZE);
//		if (System::getRenderer())				System::getRenderer()->tick(STEP_SIZE);
        
    } else if (event->timerId() == _autosave_timer.timerId()) {
        
        autosave();
        
    } else {
        QWidget::timerEvent(event);
    }

}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::closeEvent(QCloseEvent *event)
{
    if (isOkToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onUpdateMenus()
{
    if (_undo_queue.size() > 0) {
        _undo_action->setEnabled(true);
    } else {
        _undo_action->setEnabled(false);
    }
    
    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection();
    if (selection.size() > 0) {
        _export_selection_action->setEnabled(true);
    } else {
        _export_selection_action->setEnabled(false);
    }

}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onOpen()
{

    if (isOkToContinue()) {
        std::string levels_path = FilePath("{ED_LEVELS_PATH}").full_path() + FilePath::path_separator() + "*";

        QString filename = QFileDialog::getOpenFileName(this,
                                   tr("Open Level File"), 
								   levels_path.c_str(),
                                   tr("Level (*.lvl)"));
								   		
		if (!filename.isEmpty()) {
            loadLevel(filename);
            _save_level_action->setEnabled(false);
		}
    }
}

bool EdLevelMainWindow::onSaveLevel()
{
    if (_current_level_file.isEmpty()) {
        return onSaveLevelAs();
    } else {
        bool saved = saveLevel(_current_level_file);
        _save_level_action->setEnabled(!saved);
        return saved;
    }
}

bool EdLevelMainWindow::onSaveLevelAs()
{
	QFileDialog fileDialog(this, "Choose level file to save");
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setNameFilter("Level files (*.lvl)");
	fileDialog.setDefaultSuffix("lvl");
	int result = fileDialog.exec();

	if (!result)
		return false;
		
    QString filename = fileDialog.selectedFiles().first();							   
    if (filename.isEmpty())
        return false;

    return saveLevel(filename);
}

bool EdLevelMainWindow::onExportSelection()
{
    QFileDialog fileDialog(this, "Choose object file to save");
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("Object files (*.lobj)");
    fileDialog.setDefaultSuffix("lobj");
    int result = fileDialog.exec();

    if (!result)
        return false;
        
    QString filename = fileDialog.selectedFiles().first();							   
    if (filename.isEmpty())
        return false;
        
    onCommand( QString("Export \"") + filename + "\"" );     
    
    //statusBar()->showMessage(tr("Objects exported"), 2000);
    
    
    return true;
}

void EdLevelMainWindow::onImportSelection()
{
    QString filename = QFileDialog::getOpenFileName(this,
                               tr("Open Object File"), 
                               FilePath("{ED_LEVELS_PATH}").full_path().c_str(),
                               tr("Object file (*.lobj)"));

    if (!filename.isEmpty()) {
        onCommand( QString("Import \"") + filename + "\"" );        
        
        //statusBar()->showMessage(tr("Objects imported"), 2000);
    }
}

void EdLevelMainWindow::onRunScript()
{
    QString filename = QFileDialog::getOpenFileName(this,
                               tr("Run Script"), 
                               FilePath("{ED_LEVELS_PATH}").full_path().c_str(),
                               tr("Text file (*.txt)"));

    if (!filename.isEmpty()) {
        onCommand( QString("RunScript \"") + filename + "\"" );        
        
        //statusBar()->showMessage(tr("Script Run"), 2000);
    }
}

void EdLevelMainWindow::onAbout()
{
    /*QMessageBox::On_About(this, tr("On_About Level Editor"),
            tr("<h2>Level Editor 1.1</h2>"
               "<p>Copyright &copy; 2008 Software Inc."
               "<p>Level Editor is a small application that "
               "demonstrates QAction, QEdLevelMainWindow, QMenuBar, "
               "QStatusBar, QTableWidget, QToolBar, and many other "
               "Qt classes."));*/
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onOpenRecent()	
{

}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onCreateObject()
{
    onUndoBlock();

    QAction *action = qobject_cast<QAction*>(sender());
    QString object_type = action->data().toString();
    
    onCommand("Add \"" + object_type + "\"");
}

void EdLevelMainWindow::onCreateComponent()
{
    onUndoBlock();

    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection ();
    if (selection.size() == 1) {
        QAction *action = qobject_cast<QAction*>(sender());
        QString component_type = action->data().toString();
        
        onCommand(QString("AddComponent \"") + selection.front()->name().c_str() + "\" \"" + component_type + "\"");
    }
    
}

void EdLevelMainWindow::onClearComponent (void)
{
    onUndoBlock();

    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection ();
    if (selection.size() == 1) {
        QAction *action = qobject_cast<QAction*>(sender());
        ComponentBase *component = (ComponentBase *) action->data().value<void*>();
        
        onCommand(QString("RemoveComponent \"") + selection.front()->name().c_str() + "\" \"" + component->name().c_str() + "\"");
    }

}

void EdLevelMainWindow::onCreateScripts()
{
    onUndoBlock();

    QAction *action = qobject_cast<QAction*>(sender());
    QString object_type = action->data().toString();
   
    onCommand("add \"" + object_type + "\"");
}

void EdLevelMainWindow::onCreateWorld()
{
    //onUndoBlock();

    QAction *action = qobject_cast<QAction*>(sender());
    QString object_type = action->data().toString();
    
    std::shared_ptr<World> world = checked_cast<World>(Factory::create_world(object_type.toUtf8().data()));
    if (world) {
        _document->setWorld(world);
        setCurrentLevelFile("");    
        _undo_queue.clear();
    }
    
    updateWidgets(CommandResult::UPDATE_ALL);
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::updateWidgets (DTuint h)
{
    if (h & CommandResult::UPDATE_SCRIPT)       emit doRefreshScript();
    if (h & CommandResult::UPDATE_WORLD)        emit doRefreshWorld();
    if (h & CommandResult::UPDATE_HIERARCHY)    emit doRefreshHierarchy();
    if (h & CommandResult::UPDATE_PROPERTIES)   emit doRefreshProperties();
    if (h & CommandResult::UPDATE_LIBRARY)      emit doRefreshLibrary();
    if (h & CommandResult::UPDATE_RESOURCES)    emit doRefreshResources();
    if (h & CommandResult::UPDATE_ANIMATION)    emit doRefreshAnimation();
    if (h & CommandResult::UPDATE_CONSOLE)      emit doRefreshConsole();
    if (h & CommandResult::UPDATE_PERFORMANCE)  emit doRefreshPerformance();
    if (h & CommandResult::UPDATE_SOUND)        emit doRefreshSound();
}

//==============================================================================
//==============================================================================

bool EdLevelMainWindow::onCommand(QString command)
{
    // Set up the context object for the command
    CommandContext ctx;
    ctx.set_world(_document->world());
    ctx.set_selection(_document->selection());
    
    // Figure out rect of script view
    QPointF topLeft = _script_widget->mapToScene (0, 0);
    QPointF bottomRight = _script_widget->mapToScene (_script_widget->width(), _script_widget->height());
    ctx.set_selection_rectangle( DT3::Rectangle(topLeft.x(), bottomRight.x(), topLeft.y(), bottomRight.y()) );
    
    CommandResult result = Console::do_command(ctx, CommandParams(command.toUtf8().data()) );
    
    // Add to undo queue. If we can't undo, clear the queue.
    if (result.undo_commands().size() > 0) {
    
        // Copy commands over
        const std::list<CommandParams>& commands = result.undo_commands();
        for (auto i = commands.rbegin(); i != commands.rend(); ++i) {
            _undo_queue.push_front(*i);
        }
            
        // Update Menu
        _undo_action->setEnabled(true);

    } else {
        _undo_queue.clear();
        
        // Update Menu
        _undo_action->setEnabled(false);
    }
    
    _console_widget->append (command + "      // " + QString(result.message().c_str()));
    
    // Change selection
    if (_document->selection() != ctx.selection())
        emit doSelectionChanged(ctx.selection());
    
    // Change world
    if (_document->world() != ctx.world())
        _document->setWorld(ctx.world());
    
    // Refresh Everything
    DTuint h = result.update_hint();
    updateWidgets(h);
    
    // Reenable save
    _save_level_action->setEnabled(true);
    
    return result.result();
}

void EdLevelMainWindow::onUndoBlock()
{
    if (_undo_queue.size() > 0 && _undo_queue.front().count() == 0)
        return;

    // Push an empty command to signify a break
    _undo_queue.push_front(CommandParams());
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onUndo()
{
    _console_widget->append ("// Undo Begins");

    // Set up the context object for the command
    CommandContext ctx;
    ctx.set_world(_document->world());
    ctx.set_selection(_document->selection());
    
    DTuint h = 0;
    
    // Eat blocks
    while (_undo_queue.size() > 0 && _undo_queue.front().count() == 0) {
        _undo_queue.pop_front();
    }

    // Pop until an empty command is found
    while (_undo_queue.size() > 0) {
    
        if (_undo_queue.front().count() == 0) {
            break;
        }
    
        CommandParams   &params = _undo_queue.front();
        CommandResult   result = Console::do_command(ctx, params );
        
        h |= result.update_hint();
            
        _console_widget->append (QString(params.original_string().c_str()) + "      // " + QString(result.message().c_str()));
        
        _undo_queue.pop_front();
    }
    
    // Eat blocks
    while (_undo_queue.size() > 0 && _undo_queue.front().count() == 0) {
        _undo_queue.pop_front();
    }
        
    updateWidgets(h);

    if (_undo_queue.size() == 0) {
        // Update Menu
        _undo_action->setEnabled(false);
    }

    _console_widget->append ("// Undo Ends");
}

void EdLevelMainWindow::onCut()
{
    onUndoBlock();
    if (QMetaObject::invokeMethod( QApplication::focusWidget(), "cut", Qt::DirectConnection))
        return;

    onCommand("Cut");
}

void EdLevelMainWindow::onCopy()
{
    onUndoBlock();
    if (QMetaObject::invokeMethod( QApplication::focusWidget(), "copy", Qt::DirectConnection))
        return;

    onCommand("Copy");
}

void EdLevelMainWindow::onPaste()
{
    onUndoBlock();
    if (QMetaObject::invokeMethod( QApplication::focusWidget(), "paste", Qt::DirectConnection))
        return;

    onCommand("Paste");
}

void EdLevelMainWindow::onClear()
{
    onUndoBlock();
        
    if (QMetaObject::invokeMethod( QApplication::focusWidget(), "clear", Qt::DirectConnection))
        return;

    onCommand("Clear");
}

void EdLevelMainWindow::onSelectAll()
{
    onUndoBlock();
    onCommand("SelectAll");
}

void EdLevelMainWindow::onSelectNone()
{
    onUndoBlock();
    onCommand("SelectNone");
}

void EdLevelMainWindow::onDuplicate()
{
    onUndoBlock();
    onCommand("Duplicate");
}

void EdLevelMainWindow::onDuplicateHierarchy()
{
    onUndoBlock();
    onCommand("DuplicateHierarchy");
}

void EdLevelMainWindow::onGroup()
{
    onUndoBlock();
    onCommand("Group \"New Group\"");
}

void EdLevelMainWindow::onUngroup()
{
    onUndoBlock();
    onCommand("Ungroup");
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onSelectionChanged(const std::list<std::shared_ptr<PlugNode>> &selection_list)
{
    _document->setSelection(selection_list);
    
    // Update menus
    if (selection_list.size() == 1 && selection_list.front()->isa(ObjectBase::kind()) ) {
        _component_menu->setDisabled(false);
    } else {
        _component_menu->setDisabled(true);
    }
    
    
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onDocumentModified()
{

}

void EdLevelMainWindow::onUpdateStatusBar()
{
    setWindowModified(true);
    onOpenRecent();
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onNodeContextMenu(std::shared_ptr<WorldNode> node, const QPointF &global_pos)
{

    // Context Menu
    QMenu *node_context_menu = new QMenu();
    QMenu *components = node_context_menu->addMenu("Add Component");
	
	std::map<std::string,std::set<std::string>> &component_types = component_map();
	
	FOR_EACH (i,component_types) {
		QMenu *menu = components->addMenu(i->first.c_str());
        
        std::set<std::string> &components = i->second;

		FOR_EACH (j, components) {
            QAction *add_component_action = new QAction(this);
            add_component_action->setVisible(true);
            add_component_action->setText(j->c_str());
            add_component_action->setData(j->c_str());
            add_component_action->setVisible(true);

            connect(add_component_action,   SIGNAL(triggered()),
                    this,                   SLOT(onCreateComponent()));
                                        
            menu->addAction(add_component_action);
        }	
	
	}
            
    node_context_menu->exec(global_pos.toPoint());
}

void EdLevelMainWindow::onComponentContextMenu(std::shared_ptr<ComponentBase> component, const QPointF &global_pos)
{
    // Component contextual menu
    QAction *clear_action = new QAction(tr("Clear"), this);
    clear_action->setStatusTip(tr("Clear the component"));
    clear_action->setData(qVariantFromValue( (void*) component.get()));
    connect(clear_action, SIGNAL(triggered()), this, SLOT(onClearComponent()));

    QMenu *component_context_menu = new QMenu();
    component_context_menu->addAction(clear_action);
    
    component_context_menu->exec(global_pos.toPoint());
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::createActions()
{
    _new_action = new QAction(tr("&New"), this);
    _new_action->setIcon(QIcon(":/images/new.png"));
    _new_action->setShortcut(QKeySequence::New);
    _new_action->setStatusTip(tr("Create a new Level file"));

    _open_action = new QAction(tr("&Open..."), this);
    _open_action->setIcon(QIcon(":/images/open.png"));
    _open_action->setShortcut(QKeySequence::Open);
    _open_action->setStatusTip(tr("Open an existing Level file"));
    connect(_open_action, SIGNAL(triggered()), this, SLOT(onOpen()));

    _save_level_action = new QAction(tr("&Save Level"), this);
    _save_level_action->setIcon(QIcon(":/images/save.png"));
    _save_level_action->setShortcut(QKeySequence::Save);
    _save_level_action->setStatusTip(tr("Save the Level to disk"));
    connect(_save_level_action, SIGNAL(triggered()), this, SLOT(onSaveLevel()));

    _save_level_as_action = new QAction(tr("Save Level &As..."), this);
    _save_level_as_action->setStatusTip(tr("Save a copy of the Level to disk"));
    connect(_save_level_as_action, SIGNAL(triggered()), this, SLOT(onSaveLevelAs()));

    _export_selection_action = new QAction(tr("Export Selection..."), this);
    _export_selection_action->setStatusTip(tr("Export the selection to disk"));
    connect(_export_selection_action, SIGNAL(triggered()), this, SLOT(onExportSelection()));

    _import_selection_action = new QAction(tr("Import Selection..."), this);
    _import_selection_action->setStatusTip(tr("Import the selection from disk"));
    connect(_import_selection_action, SIGNAL(triggered()), this, SLOT(onImportSelection()));
    
    _run_script_action = new QAction(tr("Run Script..."), this);
    _run_script_action->setStatusTip(tr("Run script file"));
    connect(_run_script_action, SIGNAL(triggered()), this, SLOT(onRunScript()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        _recent_file_actions[i] = new QAction(this);
        _recent_file_actions[i]->setVisible(false);
        connect(_recent_file_actions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    _exit_action = new QAction(tr("E&xit"), this);
    _exit_action->setShortcut(tr("Ctrl+Q"));
    _exit_action->setStatusTip(tr("Exit the application"));
    connect(_exit_action, SIGNAL(triggered()), this, SLOT(close()));
	
	//
	// Edit menu
	// 
    
    _undo_action = new QAction(tr("&Undo"), this);
    _undo_action->setShortcut(QKeySequence::Undo);
    _undo_action->setStatusTip(tr("Undo the last operation"));
    connect(_undo_action, SIGNAL(triggered()), this, SLOT(onUndo()));

    _cut_action = new QAction(tr("Cu&t"), this);
    _cut_action->setIcon(QIcon(":/images/cut.png"));
    _cut_action->setShortcut(QKeySequence::Cut);
    _cut_action->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(_cut_action, SIGNAL(triggered()), this, SLOT(onCut()));

    _copy_action = new QAction(tr("&Copy"), this);
    _copy_action->setIcon(QIcon(":/images/copy.png"));
    _copy_action->setShortcut(QKeySequence::Copy);
    _copy_action->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(_copy_action, SIGNAL(triggered()), this, SLOT(onCopy()));

    _paste_action = new QAction(tr("&Paste"), this);
    _paste_action->setIcon(QIcon(":/images/paste.png"));
    _paste_action->setShortcut(QKeySequence::Paste);
    _paste_action->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(_paste_action, SIGNAL(triggered()), this, SLOT(onPaste()));

    _clear_action = new QAction(tr("&Clear"), this);
    //_clear_action->setShortcut(Qt::CTRL + QKeySequence::Delete);
    _clear_action->setStatusTip(tr("Clear the current selection's contents"));
    connect(_clear_action, SIGNAL(triggered()),
            this, SLOT(onClear()));

    _select_all_action = new QAction(tr("Select All"), this);
	_select_all_action->setShortcut(QKeySequence::SelectAll);
	_select_all_action->setStatusTip(tr("Select all parts"));
    connect(_select_all_action, SIGNAL(triggered()),
            this, SLOT(onSelectAll()));

    _select_none_action = new QAction(tr("Select None"), this);
	_select_none_action->setStatusTip(tr("Deselect all parts"));
    connect(_select_none_action, SIGNAL(triggered()),
            this, SLOT(onSelectNone()));
            
    _duplicate_action = new QAction(tr("Duplicate"), this);
	_duplicate_action->setShortcut(tr("Ctrl+d"));
	_duplicate_action->setStatusTip(tr("Duplicate Selection"));
    connect(_duplicate_action, SIGNAL(triggered()),
            this, SLOT(onDuplicate()));

    _duplicate_hierarchy_action = new QAction(tr("Duplicate Hierarchy"), this);
	_duplicate_hierarchy_action->setShortcut(tr("Ctrl+Shift+d"));
	_duplicate_hierarchy_action->setStatusTip(tr("Duplicate Selection and all children"));
    connect(_duplicate_hierarchy_action, SIGNAL(triggered()),
            this, SLOT(onDuplicateHierarchy()));
    
    _group_action = new QAction(tr("Group"), this);
	_group_action->setShortcut(tr("Ctrl+g"));
	_group_action->setStatusTip(tr("Group Selection"));
    connect(_group_action, SIGNAL(triggered()),
            this, SLOT(onGroup()));
            
    _ungroup_action = new QAction(tr("Ungroup"), this);
	_ungroup_action->setShortcut(tr("Ctrl+Shift+g"));
	_ungroup_action->setStatusTip(tr("Ungroup Selection"));
    connect(_ungroup_action, SIGNAL(triggered()),
            this, SLOT(onUngroup()));
    
            

			
	//
	// About item
	//

    _about_action = new QAction(tr("&About"), this);
    _about_action->setStatusTip(tr("Show the application's About box"));
    connect(_about_action, SIGNAL(triggered()), this, SLOT(onAbout()));
    
    //
    // Level
    //
    
    _play_level_action = new QAction(tr("Play"), this);
    _play_level_action->setIcon(QIcon(":/images/play.png"));
    //_play_level_action->setShortcut(QKeySequence::New);
    _play_level_action->setStatusTip(tr("Play Level"));
    connect(_play_level_action, SIGNAL(triggered()), this, SLOT(onPlayLevel()));
    
    _play_options_action = new QAction(tr("Play Options"), this);
    _play_options_action->setIcon(QIcon(":/images/settings.png"));
    //_play_options_action->setShortcut(QKeySequence::New);
    _play_options_action->setStatusTip(tr("Stop Level"));
    connect(_play_options_action, SIGNAL(triggered()), this, SLOT(onPlayOptions()));

    //
    // Tools
    //
    
    _mesh_builder_action = new QAction(tr("Mesh Builder"), this);
    _mesh_builder_action->setIcon(QIcon(":/images/mesh_builder.png"));
    _mesh_builder_action->setStatusTip(tr("Mesh Builder"));
    connect(_mesh_builder_action, SIGNAL(triggered()), this, SLOT(onMeshBuilder()));

    _package_builder_action = new QAction(tr("Package Builder"), this);
    _package_builder_action->setIcon(QIcon(":/images/package_builder.png"));
    _package_builder_action->setStatusTip(tr("Package Builder"));
    connect(_package_builder_action, SIGNAL(triggered()), this, SLOT(onPackageBuilder()));
}

void EdLevelMainWindow::createMenus()
{
    _file_menu = menuBar()->addMenu(tr("&File"));
    
    //_file_menu->addAction(_new_action);
    QMenu *new_menu = _file_menu->addMenu("New");

	std::map<std::string,std::shared_ptr<CreatorBase>> &world_types = world_map();
	
	FOR_EACH (k,world_types) {		
        QAction *add_world_action = new QAction(this);
        add_world_action->setVisible(true);
        add_world_action->setText(k->first.c_str());
        add_world_action->setData(k->first.c_str());
        add_world_action->setVisible(true);

        connect(add_world_action, SIGNAL(triggered()),
                this, SLOT(onCreateWorld()));
                    
        new_menu->addAction(add_world_action);
	}
    
    
    _file_menu->addAction(_open_action);
    _file_menu->addSeparator();
    _file_menu->addAction(_save_level_action);
    _file_menu->addAction(_save_level_as_action);
    _file_menu->addSeparator();
    _file_menu->addAction(_export_selection_action);
    _file_menu->addAction(_import_selection_action);
    _file_menu->addSeparator();
    _file_menu->addAction(_run_script_action);
    _file_menu->addSeparator();
    _separator_action = _file_menu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        _file_menu->addAction(_recent_file_actions[i]);
    _file_menu->addSeparator();
    _file_menu->addAction(_exit_action);

    _edit_menu = menuBar()->addMenu(tr("&Edit"));
    _edit_menu->addAction(_undo_action);
	_edit_menu->addSeparator();
    _edit_menu->addAction(_cut_action);
    _edit_menu->addAction(_copy_action);
    _edit_menu->addAction(_paste_action);
    _edit_menu->addAction(_clear_action);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_select_all_action);
	_edit_menu->addAction(_select_none_action);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_duplicate_action);
	_edit_menu->addAction(_duplicate_hierarchy_action);
	_edit_menu->addSeparator();
    _edit_menu->addAction(_group_action);
	_edit_menu->addAction(_ungroup_action);

    // Object Menu
    _object_menu = menuBar()->addMenu(tr("Objects"));
	
	std::map<std::string,std::set<std::string>> &placeable_types = placeable_map();
	
	FOR_EACH (i,placeable_types) {
		QMenu *menu = _object_menu->addMenu(i->first.c_str());
		
        std::set<std::string> &placeable = i->second;

		FOR_EACH (j,placeable) {
            QAction *add_object_action = new QAction(this);
            add_object_action->setVisible(true);
            add_object_action->setText(j->c_str());
            add_object_action->setData(j->c_str());
            add_object_action->setVisible(true);

            connect(add_object_action,      SIGNAL(triggered()),
                    this,                   SLOT(onCreateObject()));
                                        
            menu->addAction(add_object_action);
        }	
	
	}
    
    // Component Menu
    _component_menu = menuBar()->addMenu(tr("Components"));
	
	std::map<std::string,std::set<std::string>> &component_types = component_map();
	
	FOR_EACH (i,component_types) {
        QMenu *menu = _component_menu->addMenu(i->first.c_str());
		
        std::set<std::string> &components = i->second;

		FOR_EACH (j,components) {
            QAction *add_component_action = new QAction(this);
            add_component_action->setVisible(true);
            add_component_action->setText(j->c_str());
            add_component_action->setData(j->c_str());
            add_component_action->setVisible(true);

            connect(add_component_action,SIGNAL(triggered()),
                    this,                   SLOT(onCreateComponent()));
                                        
            menu->addAction(add_component_action);
        }	
	
	}
    
    // Script Menu
    _script_menu = menuBar()->addMenu(tr("Scripts"));
	
	std::map<std::string,std::set<std::string>> &script_types = script_map();
	
	FOR_EACH (i,script_types) {
		QMenu *menu = _script_menu->addMenu(i->first.c_str());
		
        std::set<std::string> &scripts = i->second;

		FOR_EACH (j,scripts) {
            QAction *add_script_action = new QAction(this);
            add_script_action->setVisible(true);
            add_script_action->setText(j->c_str());
            add_script_action->setData(j->c_str());
            add_script_action->setVisible(true);

            connect(add_script_action,   SIGNAL(triggered()),
                    this, SLOT(onCreateScripts()));
                                      
            menu->addAction(add_script_action);
        }	
	
	}
    
    // Level Menu
    _level_menu = menuBar()->addMenu(tr("Level"));
    _level_menu->addAction(_play_level_action);
    _level_menu->addAction(_play_options_action);
    
    // Tools Menu
    _tools_menu = menuBar()->addMenu(tr("Tools"));
    _tools_menu->addAction(_mesh_builder_action);
    _tools_menu->addAction(_package_builder_action);

    menuBar()->addSeparator();

    _help_menu = menuBar()->addMenu(tr("&Help"));
    _help_menu->addAction(_about_action);
}

void EdLevelMainWindow::createContextMenu()
{
    this->addAction(_cut_action);
    this->addAction(_copy_action);
    this->addAction(_paste_action);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EdLevelMainWindow::createToolBars()
{
    _file_toolbar = addToolBar(tr("&File"));
	_file_toolbar->setIconSize(QSize(16,16));
    _file_toolbar->addAction(_new_action);
    _file_toolbar->addAction(_open_action);
    _file_toolbar->addAction(_save_level_action);

    _edit_toolbar = addToolBar(tr("&Edit"));
	_edit_toolbar->setIconSize(QSize(16,16));
    _edit_toolbar->addAction(_cut_action);
    _edit_toolbar->addAction(_copy_action);
    _edit_toolbar->addAction(_paste_action);
    
    _level_toolbar = addToolBar(tr("&Level"));
	_level_toolbar->setIconSize(QSize(16,16));
    _level_toolbar->addAction(_play_level_action);
    _level_toolbar->addAction(_play_options_action);
    
    _tools_toolbar = addToolBar(tr("&Tools"));
    _tools_toolbar->addAction(_mesh_builder_action);
    _tools_toolbar->addAction(_package_builder_action);

}

void EdLevelMainWindow::createStatusBar()
{
    onOpenRecent();
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::readSettings()
{
    QSettings settings("Smells Like Donkey Software Inc.", "Level Editor");

    restoreGeometry(settings.value("geometry").toByteArray());

    _recent_files = settings.value("recentfiles").toStringList();
    updateRecentFileActions();
}

void EdLevelMainWindow::writeSettings()
{
    QSettings settings("Smells Like Donkey Software Inc.", "Level Editor");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentfiles", _recent_files);
}

//==============================================================================
//==============================================================================

bool EdLevelMainWindow::isOkToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Level Editor"),
                        tr("The document has been modified.\n"
                           "Do you want to On_Save_Level your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return onSaveLevel();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void EdLevelMainWindow::loadObject(std::shared_ptr<BaseClass> obj)
{
    // Wait for a world object and store it.
    if (obj && obj->isa(World::kind())) {
        std::shared_ptr<World> world = checked_static_cast<World>(obj);
        _document->setWorld(world);
    }
}

bool EdLevelMainWindow::loadLevel(const QString &filename)
{
    // Load the world using the specialized "world loading" routines so the document pointer can be set immediately
    
    // Set up a callback to filter the objects. We need to wait for a world object (probably the first one)
    // so that it can be set in the document immediately. Otherwise the system callbacks won't work.
    std::shared_ptr<Callback<std::shared_ptr<BaseClass>>> cb = make_callback(this, &EdLevelMainWindow::loadObject);
    
	std::shared_ptr<BaseClass> obj = Factory::create_object_from_stream(FilePath(filename.toUtf8().data()), NULL, cb);
    
    std::shared_ptr<World> world = _document->world();
    
    if (world) {
        world->set_streamable(true);
        _undo_queue.clear();
        
    } else {
    
        QMessageBox::warning(this, tr("Level Editor"),
            tr("Error loading file. Perhaps you need a new build of the editor?"),
            QMessageBox::Cancel);
    
    }
    
    updateWidgets(CommandResult::UPDATE_ALL);

    setCurrentLevelFile(filename);

    return true;
}

bool EdLevelMainWindow::saveLevel(const QString &filename)
{
    FilePath temp_file("{APPDIR}/temp.lvl");
    
    std::shared_ptr<ArchiveTextWriter> writer = ArchiveTextWriter::create();
	writer->open(FilePath(temp_file));
	ArchiveObjectQueue::queue_out_tree(writer, _document->world() );
	writer->close();
    
    HAL::delete_file(FilePath(filename.toUtf8().data()));
    HAL::move_file(temp_file,FilePath(filename.toUtf8().data()));

    setCurrentLevelFile(filename);
    
    //statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void EdLevelMainWindow::autosave(void)
{
    std::shared_ptr<ArchiveTextWriter> writer = ArchiveTextWriter::create();
    writer->open(FilePath("{APPDIR}/autosave.lvl"));
    ArchiveObjectQueue::queue_out_tree(writer, _document->world() );
    writer->close();
}

void EdLevelMainWindow::setCurrentLevelFile(const QString &filename)
{
    _current_level_file = filename;

    setWindowTitle(tr("%1 (%2)")
        .arg( strippedName(_current_level_file.isEmpty() ? "Untitled" : _current_level_file) )
        .arg( _document->world()->class_id_child() )
    );
}

void EdLevelMainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(_recent_files);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < _recent_files.count()) {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(_recent_files[j]));
            _recent_file_actions[j]->setText(text);
            _recent_file_actions[j]->setData(_recent_files[j]);
            _recent_file_actions[j]->setVisible(true);
        } else {
            _recent_file_actions[j]->setVisible(false);
        }
    }
    _separator_action->setVisible(!_recent_files.isEmpty());
}

QString EdLevelMainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString EdLevelMainWindow::strippedDir(const QString &fullFileName)
{
    return QFileInfo(fullFileName).dir().path();
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onPlayLevel(void)
{
    autosave();

    // Level info
    std::string path = Globals::substitute_global("{ED_GAME_PATH}");
    std::string level_path = Globals::substitute_global("{APPDIR}/autosave.lvl");
    
    // Resolution info
    DTboolean resolution_enabled = MoreStrings::cast_from_string<DTboolean>(Globals::global("ED_RESOLUTION_ENABLED"));
    std::string resolution_width = Globals::global("ED_RESOLUTION_WIDTH");
    std::string resolution_height = Globals::global("ED_RESOLUTION_HEIGHT");
    
    std::vector<std::string> args;
    args.push_back("-loadlevel");
    args.push_back(level_path);
    
    if (resolution_enabled) {
        args.push_back("-resolution");
        args.push_back(resolution_width + "x" + resolution_height);
    }
    
    HAL::run_on_command_line(path, args);
}

void EdLevelMainWindow::onPlayOptions(void)
{
    EdLevelLaunchDialog dialog;
    dialog.exec();
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onMeshBuilder (void)
{
    EdLevelMeshBuilder dialog;
    dialog.exec();
}

void EdLevelMainWindow::onPackageBuilder (void)
{

}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::onAppFocusChanged(QWidget* old, QWidget* now)
{    
    if(old == NULL) {
        //Focus regained..This is foreground app now
        FileManager::scan_files();
        
        SystemCallbacks::reload_resources_cb().fire();
            
        //emit doRefreshResources();
        //emit doRefreshLibrary();
        
        update();
        
    } else if(now == NULL) {
        //Focus lost. This is background app now.
    }

}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::addNodeCB (World *world, WorldNode *node)
{
    if (_document->world().get() != world)
        return;
    
    emit doAddNode(node);
}

void EdLevelMainWindow::removeNodeCB (World *world, WorldNode *node)
{
    if (_document->world().get() != world)
        return;

    emit doRemoveNode(node);
}

void EdLevelMainWindow::reparentNodeCB (World *world, WorldNode *node, WorldNode *old_parent, WorldNode *new_parent)
{
    if (_document->world().get() != world)
        return;

    emit doReparentNode(node,old_parent,new_parent);
}

void EdLevelMainWindow::addGroupCB (World *world,Group *group)
{
    if (_document->world().get() != world)
        return;
    
    emit doAddGroup(group);
}

void EdLevelMainWindow::removeGroupCB (World *world, Group *group)
{
    if (_document->world().get() != world)
        return;

    emit doRemoveGroup(group);
}

void EdLevelMainWindow::addComponentCB (World *world, ComponentBase *component)
{
    if (_document->world().get() != world)
        return;

    emit doAddComponent(component);
}

void EdLevelMainWindow::removeComponentCB (World *world, ComponentBase *component)
{
    if (_document->world().get() != world)
        return;

    emit doRemoveComponent(component);
}

void EdLevelMainWindow::connectPlugCB (PlugBase *outgoing, PlugBase *incoming)
{
    emit doConnectPlug (outgoing, incoming);
}

void EdLevelMainWindow::disconnectPlugCB (PlugBase *outgoing, PlugBase *incoming)
{
    emit doDisconnectPlug (outgoing, incoming);
}

void EdLevelMainWindow::connectEventCB (Event *outgoing, Event *incoming)
{
    emit doConnectEvent (outgoing, incoming);
}

void EdLevelMainWindow::disconnectEventCB (Event *outgoing, Event *incoming)
{
    emit doDisconnectEvent (outgoing, incoming);
}

//==============================================================================
//==============================================================================

void EdLevelMainWindow::showAssert (const DTcharacter* file, const DTcharacter* func, const DTint line)
{
    static std::list<ErrorEntry> errors;
    
    ErrorEntry error_entry(file,func,line);
    if (std::find(errors.begin(), errors.end(), error_entry) == errors.end()) {

        int r = QMessageBox::critical(this, tr("Assertion"),
                    QString("Assertion in file ") + file + QString(" in function ") + func + QString(" on line ") + QString::number(line),
                    QMessageBox::Abort | QMessageBox::Ignore | QMessageBox::Ok);

        if (r == QMessageBox::Abort) {    
            exit(1);        
        } else if (r == QMessageBox::Ignore) {
            errors.push_back(error_entry);
        } else if (r == QMessageBox::Ok) {
            // Do nothing
        }
        
    }
}

void EdLevelMainWindow::showError (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter* msg)
{
    static std::list<ErrorEntry> errors;
    
    ErrorEntry error_entry(file,func,line);
    if (std::find(errors.begin(), errors.end(), error_entry) == errors.end()) {

        int r = QMessageBox::critical(this, tr("Error"),
                    QString("Error ") + msg + QString(" in file ") + file + QString(" in function ") + func + QString(" on line ") + QString::number(line),
                    QMessageBox::Abort | QMessageBox::Ignore | QMessageBox::Ok);

        if (r == QMessageBox::Abort) {    
            exit(1);        
        } else if (r == QMessageBox::Ignore) {
            errors.push_back(error_entry);
        } else if (r == QMessageBox::Ok) {
            // Do nothing
        }
        
    }
}

void EdLevelMainWindow::showWarning (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter* msg)
{
    static std::list<ErrorEntry> errors;
    
    ErrorEntry error_entry(file,func,line);
    if (std::find(errors.begin(), errors.end(), error_entry) == errors.end()) {

        int r = QMessageBox::warning(this, tr("Warning"),
                    QString("Warning ") + msg + QString(" in file ") + file + QString(" in function ") + func + QString(" on line ") + QString::number(line),
                    QMessageBox::Abort | QMessageBox::Ignore | QMessageBox::Ok);

        if (r == QMessageBox::Abort) {    
            exit(1);        
        } else if (r == QMessageBox::Ignore) {
            errors.push_back(error_entry);
        } else if (r == QMessageBox::Ok) {
            // Do nothing
        }
    }
}

//==============================================================================
//==============================================================================


#include "moc_EdLevelMainWindow.cpp"


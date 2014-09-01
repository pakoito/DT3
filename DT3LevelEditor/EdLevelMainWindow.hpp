#ifndef EDLEVELMAINWINDOW
#define EDLEVELMAINWINDOW
//==============================================================================
///	
///	File: EdLevelMainWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTextEdit>
#include <QtCore/QTimer>
#include <QtCore/QProcess>

// Engine includes
#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/CommandParams.hpp"
#include <list>

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlugNode;
    class ComponentBase;
    class World;
    class WorldNode;
    class Group;
    class BaseClass;
    class PlugBase;
    class Event;
}

class QAction;
class QLabel;
class QGLWidget;
class QListView;
class QComboBox;
class EdLevelWorldWindow;
class EdLevelScriptWindow;
class EdLevelAnimationWindow;
class EdLevelSoundWindow;
class EdLevelSoundFXWindow;
class EdLevelConsoleWindow;
class EdLevelResourcesWindow;
class EdLevelLibraryWindow;
class EdLevelPerformanceWindow;
class EdLevelPropertiesWindow;
class EdLevelHierarchyWindow;
class EdLevelDocument;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelMainWindow : public QMainWindow
{
    Q_OBJECT

	public:
		EdLevelMainWindow(void);
		~EdLevelMainWindow(void);

        void                                    timerEvent          (QTimerEvent *event);
        void                                    setSaveIcon         (DTboolean icon_state);
    
	protected:
		void									closeEvent(QCloseEvent *event);

	private:
		// Create GUI
		void 									createActions();
		void 									createMenus();
		void 									createContextMenu();
		void 									createToolBars();
		void 									createStatusBar();

		// Read and Write prefs
		void 									readSettings();
		void 									writeSettings();

		// Loading and saving data
		bool 									isOkToContinue();
    
        void                                    loadObject(std::shared_ptr<BaseClass> obj);
		bool 									loadLevel(const QString &filename);
    
		bool 									saveLevel(const QString &filename);
        void                                    autosave(void);

		// File management
		void 									setCurrentLevelFile(const QString &filename);
		void 									updateRecentFileActions();
		QString 								strippedName(const QString &fullFileName);
		QString 								strippedDir(const QString &fullFileName);
    
        void                                    updateWidgets (DTuint h);
    
		// GUI Widgets
		EdLevelWorldWindow						*_world_widget;
		EdLevelScriptWindow						*_script_widget;
        EdLevelAnimationWindow                  *_animation_widget;
        EdLevelSoundWindow                      *_sound_widget;
		EdLevelConsoleWindow					*_console_widget;
		EdLevelResourcesWindow                  *_resources_widget;
		EdLevelLibraryWindow                    *_library_widget;
		EdLevelPerformanceWindow				*_performance_widget;
        EdLevelPropertiesWindow                 *_properties_widget;
        EdLevelHierarchyWindow                  *_hierarchy_widget;
    			
		QString									_current_level_file;
		QStringList								_recent_files;
		
        QBasicTimer                             _timer;
        QBasicTimer                             _autosave_timer;

		static const DTint MaxRecentFiles = 5;
		QAction *								_recent_file_actions[MaxRecentFiles];
		QAction *								_separator_action;

		QMenu *									_file_menu;
		QMenu *									_edit_menu;
		QMenu *									_object_menu;
		QMenu *									_component_menu;
		QMenu *									_script_menu;
		QMenu *									_level_menu;
		QMenu *									_tools_menu;
		QMenu *									_help_menu;

		QToolBar *								_file_toolbar;
		QToolBar *								_edit_toolbar;
		QToolBar *								_level_toolbar;
		QToolBar *								_tools_toolbar;

        QToolBar *								_world_toolbar;
		QToolBar *								_script_toolbar;
		QToolBar *								_animation_toolbar;
		QToolBar *								_sound_toolbar;
		QToolBar *								_sound_fx_toolbar;
		QToolBar *								_console_toolbar;
		QToolBar *								_resources_toolbar;
		QToolBar *								_library_toolbar;
		QToolBar *								_hierarchy_toolbar;
		QToolBar *								_performance_toolbar;
    
		QAction *								_new_action;
		QAction *								_open_action;
		QAction *								_save_level_action;
		QAction *								_save_level_as_action;
        QAction *                               _export_selection_action;
        QAction *                               _import_selection_action;
        QAction *                               _run_script_action;
		QAction *								_exit_action;
		
		QAction *								_undo_action;
		QAction *								_cut_action;
		QAction *								_copy_action;
		QAction *								_paste_action;
		QAction *								_clear_action;
		QAction *								_select_all_action;
		QAction *								_select_none_action;
		QAction *								_duplicate_action;
		QAction *								_duplicate_hierarchy_action;
		QAction *								_group_action;
		QAction *								_ungroup_action;
		
		QAction *								_about_action;

		QAction *								_play_level_action;
        QAction *                               _play_options_action;

        QAction *                               _mesh_builder_action;
        QAction *                               _package_builder_action;
    
        // Undo Queue
        std::list<CommandParams>                _undo_queue;
        
        // Document
        EdLevelDocument                         *_document;
        
        // Error handlers
        struct ErrorEntry {
            ErrorEntry (const DTcharacter* file, const DTcharacter* func, const DTint line)
                :   _file(file),
                    _func(func),
                    _line(line)
            {}
            
            DTboolean operator == (const ErrorEntry &rhs) {
                return ( (_file == rhs._file) && (_func == rhs._func) && (_line == rhs._line) );
            }
                        
            const DTcharacter* _file;
            const DTcharacter* _func;
            const DTint _line;
        };
                
        void                                    showAssert  (const DTcharacter* file, const DTcharacter* func, const DTint line);
        void                                    showError   (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter*);
        void                                    showWarning (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter*);
        
	//==========================================================================
    // Callbacks
	//==========================================================================
        
        void                                    addNodeCB               (World *world, WorldNode *node);
        void                                    removeNodeCB            (World *world, WorldNode *node);
        void                                    reparentNodeCB          (World *world, WorldNode *node, WorldNode *old_parent, WorldNode *new_parent);

        void                                    addGroupCB              (World *world, Group *group);
        void                                    removeGroupCB           (World *world, Group *group);

        void                                    addComponentCB          (World *world, ComponentBase *component);
        void                                    removeComponentCB       (World *world, ComponentBase *component);

        void                                    connectPlugCB           (PlugBase *outgoing, PlugBase *incoming);
        void                                    disconnectPlugCB        (PlugBase *outgoing, PlugBase *incoming);
        void                                    connectEventCB          (Event *outgoing, Event *incoming);
        void                                    disconnectEventCB       (Event *outgoing, Event *incoming);
        
    //==========================================================================
	/// Qt Signals and slots
	//==========================================================================
	private slots:
		void									onOpen();
		bool									onSaveLevel();
		bool									onSaveLevelAs();
		bool									onExportSelection();
		void									onImportSelection();
		void									onRunScript();
		void									onAbout();
		void									onOpenRecent();	
        
        void                                    onUndo();
        void                                    onCut();
        void                                    onCopy();
        void                                    onPaste();
        void                                    onClear();
        void                                    onSelectAll();
        void                                    onSelectNone();
        void                                    onDuplicate();
        void                                    onDuplicateHierarchy();
        void                                    onGroup();
        void                                    onUngroup();

		void 									onDocumentModified();
		void									onUpdateStatusBar();
        void                                    onUpdateMenus();

		void									onCreateObject();
		void									onCreateComponent();
        void                                    onClearComponent (void);

		void									onCreateScripts();
		void									onCreateWorld();
        
        bool                                    onCommand(QString command);
        void                                    onUndoBlock();

        void                                    onSelectionChanged(const std::list<std::shared_ptr<PlugNode>> &selection_list);
                
        void                                    onNodeContextMenu(std::shared_ptr<WorldNode> node, const QPointF &global_pos);
        void                                    onComponentContextMenu(std::shared_ptr<ComponentBase> component, const QPointF &global_pos);
        
        void                                    onPlayLevel                 (void);
        void                                    onPlayOptions               (void);

        void                                    onMeshBuilder               (void);
        void                                    onPackageBuilder            (void);
    
        void                                    onAppFocusChanged           (QWidget* old, QWidget* now);
        
    signals:
        void                                    doAddNode(WorldNode *node);
        void                                    doRemoveNode(WorldNode *node);
        void                                    doReparentNode(WorldNode *node, WorldNode *old_parent, WorldNode *new_parent);
    
        void                                    doAddGroup(Group *group);
        void                                    doRemoveGroup(Group *group);

        void                                    doAddComponent(ComponentBase *node);
        void                                    doRemoveComponent(ComponentBase *node);

        void                                    doConnectPlug (PlugBase *outgoing, PlugBase *incoming);
        void                                    doDisconnectPlug (PlugBase *outgoing, PlugBase *incoming);
        void                                    doConnectEvent (Event *outgoing, Event *incoming);
        void                                    doDisconnectEvent (Event *outgoing, Event *incoming);

        void                                    doRefreshScript();
        void                                    doRefreshWorld();
        void                                    doRefreshHierarchy();
        void                                    doRefreshProperties();
        void                                    doRefreshLibrary();
        void                                    doRefreshResources();
        void                                    doRefreshAnimation();
        void                                    doRefreshConsole();
        void                                    doRefreshPerformance();
        void                                    doRefreshSound();
        void                                    doRefreshSoundFX();

        void                                    doSelectionChanged(const std::list<std::shared_ptr<PlugNode>> &selection_list);
};

//==============================================================================
//==============================================================================

#endif

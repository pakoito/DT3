#ifndef EDSPRITEMAINWINDOW
#define EDSPRITEMAINWINDOW
//==============================================================================
///	
///	File: EdSpriteMainWindow.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QComboBox>
#include <QtCore/QTimer>

#include "DT3Sprites/Types/KeyedSpriteResource.hpp"
#include "DT3Sprites/Types/KeyedSpriteAnimationResource.hpp"
#include "DT3Sprites/Types/KeyedSpriteAnimationTrack.hpp"

using namespace DT2;

//==============================================================================
/// Forward declarations
//==============================================================================

class QAction;
class QLabel;

class EdSpriteEditor;
class EdSpriteUVEditor;
class EdSpriteTimeline;
class EdSpriteProperties;
class EdSpriteAnimationProperties;
class EdSpriteJointProperties;
class EdSpriteAnimations;
class EdSpriteJointList;

//==============================================================================
/// Class
//==============================================================================

class EdSpriteMainWindow : public QMainWindow
{
    Q_OBJECT

	public:
		EdSpriteMainWindow();
		~EdSpriteMainWindow();

		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteResource*					getSprite				(void)  {   return _sprite;             }

		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteAnimationResource*			getAnimationResource    (void)  {   return _animation;          }

		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteAnimation*                   getSelectedAnimation    (void)  {   return _selected_animation; }
        
		/// Description
		/// \param param description
		/// \return description
		void                                    setSelectedAnimation    (KeyedSpriteAnimation* selected)    {   _selected_animation = selected; 
                                                                                                                fireAnimationsChanged();
                                                                                                                setCurrentTime(_current_time);
                                                                                                            }
        

        
        /// Description
        /// \param param description
        /// \return description
        void                                    setCurrentTime          (DTfloat time);

        /// Description
        /// \param param description
        /// \return description
        DTfloat                                 getCurrentTime          (void) const    {   return _current_time;   }

        /// Description
        /// \param param description
        /// \return description
        DTfloat                                 getGrid                 (void) const;

        
        /// Description
        /// \param param description
        /// \return description
        const List<KeyedSpriteAnimationPoseJoint*>&  getPartSelection   (void);
        
        /// Description
        /// \param param description
        /// \return description
        void                                    setPartSelection        (const List<KeyedSpriteAnimationPoseJoint*> &selection);

        /// Description
        /// \param param description
        /// \return description
        void                                    addPartSelection        (KeyedSpriteAnimationPoseJoint* selection);

        /// Description
        /// \param param description
        /// \return description
        void                                    removePartSelection     (KeyedSpriteAnimationPoseJoint* selection);

        /// Description
        /// \param param description
        /// \return description
        void                                    clearPartSelection      (void);

        /// Description
        /// \param param description
        /// \return description
        bool                                    isPartSelected          (KeyedSpriteAnimationPoseJoint* selection);


        /// Description
        /// \param param description
        /// \return description
        void                                    keyJoint                (KeyedSpriteAnimationPoseJoint* joint);

        /// Description
        /// \param param description
        /// \return description
        void                                    keySelectedJoint        (void);

        /// Description
        /// \param param description
        /// \return description
        void                                    keyAllJoints            (void);


        /// Description
        /// \param param description
        /// \return description
        void                                    fireSpriteChanged       (void);
        
        /// Description
        /// \param param description
        /// \return description
        void                                    fireAnimationsChanged   (void);
    
    
        /// Description
        /// \param param description
        /// \return description
        void                                    timerEvent              (QTimerEvent *event);

        /// Description
        /// \param param description
        /// \return description
        void                                    pushUndo(void);

	protected:
		void									closeEvent              (QCloseEvent *event);

	private:
        static const float TIMER_INTERVAL;

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
		bool 									saveSprite(const QString &fileName);
		bool 									saveAnimation(const QString &fileName);

        void                                    autosave                (void);

		// File management
		void 									setCurrentSpriteFile(const QString &fileName);
		void 									setCurrentAnimationFile(const QString &fileName);
		QString 								strippedName(const QString &fullFileName);
		QString 								strippedDir(const QString &fullFileName);
        
        struct UndoQueueEntry {
            SmartPtr<KeyedSpriteResource>           sprite;
            SmartPtr<KeyedSpriteAnimationResource>  animation;
        };
        List<UndoQueueEntry>                    _undo_queue;


		// GUI Widgets
		EdSpriteEditor							*_editor_widget;
        EdSpriteUVEditor                        *_texcoord_widget;
		EdSpriteTimeline						*_timeline_widget;
        EdSpriteAnimations                      *_animations_widget;
        
        EdSpriteProperties                      *_properties_widget;
        EdSpriteAnimationProperties             *_animation_properties_widget;
        EdSpriteJointProperties                 *_joint_properties_widget;
    
        QToolBar                                *_joint_list_toolbar;
        EdSpriteJointList                       *_joint_list_widget;
    
        KeyedSpriteResource                     *_sprite;
        KeyedSpriteResource                     *_sprite_default_pose;
        KeyedSpriteAnimationResource            *_animation;
    
        
        KeyedSpriteAnimation                    *_selected_animation;
        List<KeyedSpriteAnimationPoseJoint*>    _selection;
			
		// This is the "Data" for the window
		QString									_current_sprite_file;
		QString									_current_animation_file;
        QString                                 _current_directory;
        
        DTfloat                                 _current_time;


        void                                    fireEvents (const String &event_id, const String &event_parameter);

        
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
        void                                    showError   (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter* msg);
        void                                    showWarning (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter* msg);


		QAction *								_separator_action;

		QMenu *									_file_menu;
		QMenu *									_edit_menu;
		QMenu *									_select_sub_menu;
		QMenu *									_help_menu;

		QToolBar *								_file_toolbar;
		QToolBar *								_edit_toolbar;
		QToolBar *								_playback_toolbar;
    
        QComboBox *                             _grid_selection;

		QAction *								_new_action;
		QAction *								_open_sprite_action;
		QAction *								_open_animation_action;
		QAction *								_save_sprite_action;
		QAction *								_save_sprite_as_action;
		QAction *								_save_animation_action;
		QAction *								_save_animation_as_action;
		QAction *								_exit_action;
		
		QAction *								_undo_action;
		QAction *								_cut_action;
		QAction *								_copy_action;
		QAction *								_paste_action;
		QAction *								_delete_action;
		QAction *								_select_all_action;
		QAction *								_deselect_all_action;
		
		QAction *								_duplicate_action;
		QAction *								_insert_part_action;
		QAction *								_move_to_front_action;
		QAction *								_move_to_back_action;
		
		QAction *								_insert_animation_action;
		QAction *								_delete_animation_action;
		
		QAction *								_duplicate_keys_action;
		QAction *								_next_keyframe;
		QAction *								_prev_keyframe;
		QAction *								_set_keyframe;
		QAction *								_set_all_keyframes;

		QAction *								_set_event;

		QAction *								_parent_action;
		QAction *								_unparent_action;

		QAction *								_reset_grid;

		QAction *								_transfer_animations;
		
		QAction *								_about_action;

		QAction *								_play_action;
		QAction *								_stop_action;

        QTimer                                  _animation_timer;
        QBasicTimer                             _autosave_timer;


	//==========================================================================
	/// Qt Signals and slots
	//==========================================================================
	private slots:
		void									onNew();
		void									onOpenSprite();
		void									onOpenAnimation();
		bool									onSaveSprite();
		bool									onSaveSpriteAs();
		bool									onSaveAnimation();
		bool									onSaveAnimationAs();
		void									onAbout();

		void									onPlay();
		void									onStop();
    
        void                                    onUndo();
        void                                    onCut();
        void                                    onCopy();
        void                                    onPaste();
		
		void									onDelete();
		void									onParent();
		void									onUnparent();
		void									onSelectAll();
		void									onDeselectAll();
		void									onDuplicate();
		void									onMoveToFront();
		void									onMoveToBack();
		void									onInsertPart();
		void									onInsertAnimation();
		void									onDeleteAnimation();
		
		void									onDuplicateKeys();
		void									onNextKeyframe();
		void									onPrevKeyframe();
		void									onSetKeyframe();
		void									onSetAllKeyframes();
        void                                    onSetEvent();

		void									onResetGrid();
        void                                    onTransferAnimations();

		void 									onUpdateStatusBar();
		void 									onDocumentModified();

		void 									onTickAnimation();
    
        void                                    onChangeGrid(int);

	signals:
		void									spriteChanged					(void);
		void									selectionChanged                (void);
		void									propertiesChanged               (void);
		void									animationsChanged               (void);
        


};

//==============================================================================
//==============================================================================

#endif

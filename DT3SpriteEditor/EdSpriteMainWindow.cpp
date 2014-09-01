//==============================================================================
///	
///	File: EdSpriteMainWindow.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <QtOpenGL/QGLWidget>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QCloseEvent>
#include <QtCore/QSettings>

#include <limits>

#include "EdSpriteMainWindow.hpp"
#include "EdSpriteEditor.hpp"
#include "EdSpriteUVEditor.hpp"
#include "EdSpriteTimeline.hpp"
#include "EdSpriteProperties.hpp"
#include "EdSpriteAnimationProperties.hpp"
#include "EdSpriteJointProperties.hpp"
#include "EdSpriteAnimations.hpp"
#include "EdSpriteJointList.hpp"
#include "EdSpriteAnimationTransferDialog.hpp"
#include "EdSpriteNewEventDialog.hpp"

#include "Factory.hpp"
#include "Error.hpp"
#include "System.hpp"
#include "DeviceAudio.hpp"
#include "ConsoleStream.hpp"

#include "KeyedSpriteResource.hpp"
#include "KeyedSpriteAnimation.hpp"
#include "KeyedSpriteAnimationResource.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"
#include "KeyedSpriteAnimationTrack.hpp"
#include "KeyedSpriteAnimationPose.hpp"

#include "CheckedCast.hpp"

#include "ArchiveTextWriter.hpp"
#include "ArchiveBinaryReader.hpp"
#include "ArchiveObjectQueue.hpp"

using namespace DT2;

//==============================================================================
//==============================================================================

const float EdSpriteMainWindow::TIMER_INTERVAL = 1.0F/15.0F;

//==============================================================================
//==============================================================================

EdSpriteMainWindow::EdSpriteMainWindow()
    :   _current_time           (0.0F),
        _selected_animation     (NULL)
{

    // Register error handlers
    AssertImpl::setCallbackAssertMsg(makeCallback(this, &EdSpriteMainWindow::showAssert));
    ErrorImpl::setCallbackErrorMsg(makeCallback(this, &EdSpriteMainWindow::showError));
    ErrorImpl::setCallbackWarningMsg(makeCallback(this, &EdSpriteMainWindow::showWarning));

	// Default objects
	_sprite = KeyedSpriteResource::create();
	_animation = KeyedSpriteAnimationResource::create();
    

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();
		
	// Editor Window
	_editor_widget = new EdSpriteEditor(this);
	_texcoord_widget = new EdSpriteUVEditor(this,_editor_widget);

    // Animations window
    _animations_widget = new EdSpriteAnimations(this);
    
    QDockWidget *animaitonsDock = new QDockWidget(tr("Animations"), this);
    animaitonsDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    animaitonsDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    animaitonsDock->setWidget(_animations_widget);
    addDockWidget(Qt::BottomDockWidgetArea, animaitonsDock);


	// Timeline Window
	_timeline_widget = new EdSpriteTimeline(this);
    
    QDockWidget *timelineDock = new QDockWidget(tr("Timeline"), this);
    timelineDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    timelineDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    timelineDock->setWidget(_timeline_widget);
    addDockWidget(Qt::BottomDockWidgetArea, timelineDock);



    
	// Properties Window
	_properties_widget = new EdSpriteProperties(this);
    _animation_properties_widget = new EdSpriteAnimationProperties(this);
    _joint_properties_widget = new EdSpriteJointProperties(this);
    
    // Joint List
    QWidget *joint_list_section = new QWidget(this);
    _joint_list_toolbar = new QToolBar(joint_list_section);
    _joint_list_toolbar->setIconSize(QSize(16,16));
    _joint_list_widget = new EdSpriteJointList(joint_list_section, _joint_list_toolbar, this);
    
    QGridLayout *layout_joint_list = new QGridLayout;
    layout_joint_list->setContentsMargins(0,0,0,0);
    layout_joint_list->setHorizontalSpacing(1);
    layout_joint_list->setVerticalSpacing(1);
    layout_joint_list->addWidget(_joint_list_toolbar);
    layout_joint_list->addWidget(_joint_list_widget);
    joint_list_section->setLayout(layout_joint_list);

    
    QDockWidget *propertiesDock = new QDockWidget(tr("Sprite"), this);
    propertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertiesDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    propertiesDock->setWidget(_properties_widget);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDock);

    QDockWidget *animationPropertiesDock = new QDockWidget(tr("Animation"), this);
    animationPropertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    animationPropertiesDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    animationPropertiesDock->setWidget(_animation_properties_widget);
    addDockWidget(Qt::RightDockWidgetArea, animationPropertiesDock);

    QDockWidget *jointPropertiesDock = new QDockWidget(tr("Joint"), this);
    jointPropertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    jointPropertiesDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    jointPropertiesDock->setWidget(_joint_properties_widget);
    addDockWidget(Qt::RightDockWidgetArea, jointPropertiesDock);

    QDockWidget *jointListDock = new QDockWidget(tr("Joint List"), this);
    jointListDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    jointListDock->setFeatures ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    jointListDock->setWidget(joint_list_section);
    addDockWidget(Qt::RightDockWidgetArea, jointListDock);

    tabifyDockWidget ( propertiesDock, animationPropertiesDock);
    tabifyDockWidget ( animationPropertiesDock, jointPropertiesDock);

    // Tabs
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(_editor_widget, QString("Editor"));
    tabWidget->addTab(_texcoord_widget, QString("Texcoords"));

	setCentralWidget(tabWidget);
    
    setWindowTitle(tr("Sprite Editor"));

    readSettings();

    //setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentSpriteFile("");
    setCurrentAnimationFile("");
	
	//
	// Connect Signals from custom views
	//
	
	//
	// Main Window
	//
    
    connect(	this,                           SIGNAL(spriteChanged()),
				_properties_widget,             SLOT(onSpriteChanged())	);

    connect(	this,                           SIGNAL(spriteChanged()),
				_editor_widget,                 SLOT(onSpriteChanged())	);
    
    connect(	this,                           SIGNAL(spriteChanged()),
				_texcoord_widget,               SLOT(onSpriteChanged())	);
    
    connect(	this,                           SIGNAL(spriteChanged()),
				_timeline_widget,               SLOT(onSpriteChanged())	);

    
    connect(	this,                           SIGNAL(spriteChanged()),
				_animation_properties_widget,   SLOT(onSpriteChanged())	);
    
    connect(	this,                           SIGNAL(spriteChanged()),
				_joint_properties_widget,       SLOT(onSpriteChanged())	);

    connect(	this,                           SIGNAL(spriteChanged()),
				_joint_list_widget,             SLOT(onSpriteChanged())	);
    
    connect(	this,                           SIGNAL(spriteChanged()),
				_animations_widget,             SLOT(onSpriteChanged())	);
    


    connect(	this,                           SIGNAL(selectionChanged()),
				_properties_widget,             SLOT(onSelectionChanged())	);

    connect(	this,                           SIGNAL(selectionChanged()),
				_editor_widget,                 SLOT(onSelectionChanged())	);
    
    connect(	this,                           SIGNAL(selectionChanged()),
				_texcoord_widget,               SLOT(onSelectionChanged())	);
    
    connect(	this,                           SIGNAL(selectionChanged()),
				_timeline_widget,               SLOT(onSelectionChanged())	);
    
    connect(	this,                           SIGNAL(selectionChanged()),
				_animation_properties_widget,   SLOT(onSelectionChanged())	);
    
    connect(	this,                           SIGNAL(selectionChanged()),
				_joint_properties_widget,       SLOT(onSelectionChanged())	);
                
    connect(	this,                           SIGNAL(selectionChanged()),
				_joint_list_widget,             SLOT(onSelectionChanged())	);
                


    connect(	this,                           SIGNAL(animationsChanged()),
				_animations_widget,             SLOT(onAnimationsChanged())	);
                
    connect(	this,                           SIGNAL(animationsChanged()),
				_animation_properties_widget,   SLOT(onAnimationsChanged())	);
                
                
	//
	// Animation Timer
	//
	
	connect(	&_animation_timer,			SIGNAL(timeout()), 
				this,						SLOT(onTickAnimation())	);
    
    _autosave_timer.start(60*1000*1, this); // 1 minute

    emit animationsChanged();
    emit selectionChanged();
    emit spriteChanged();

}

EdSpriteMainWindow::~EdSpriteMainWindow()
{
	RELEASE(_sprite);
	RELEASE(_animation);
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::pushUndo(void)
{
    KeyedSpriteResource             *sprite = KeyedSpriteResource::create();
    KeyedSpriteAnimationResource    *animation = KeyedSpriteAnimationResource::create();
    
    sprite->copy(*_sprite);
    animation->copy(*_animation);

    UndoQueueEntry e;
    e.sprite = makeSmartPtr(sprite);
    e.animation = makeSmartPtr(animation);
    
    _undo_queue.pushBack(e);
    
    // Limit size
    while (_undo_queue.size() > 100) {
        _undo_queue.popFront();
    }
    
    LOG_MESSAGE << "Undo Queue size: " << _undo_queue.size();
    
    _undo_action->setEnabled(true);
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _autosave_timer.timerId()) {
        autosave();
    } else {
        QWidget::timerEvent(event);
    }

}

void EdSpriteMainWindow::autosave(void)
{
    if (_sprite) {
        ArchiveTextWriter writer1;
        writer1.setIgnoreStreamableFlag(true);
        writer1.open(FilePath("{APPDIR}/autosave.kspr"));
        ArchiveObjectQueue::queueOutTree(&writer1, _sprite);
        writer1.close();
    }

    if (_animation) {
        ArchiveTextWriter writer2;
        writer2.setIgnoreStreamableFlag(true);
        writer2.open(FilePath("{APPDIR}/autosave.kani"));
        ArchiveObjectQueue::queueOutTree(&writer2, _animation);
        writer2.close();
    }
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::fireEvents (const String &event_id, const String &event_parameter)
{
    if (!_animation_timer.isActive())
        return;

    if (event_id == "PlaySound" && System::getAudioRenderer()) {
        System::getAudioRenderer()->playQuick(FilePath(event_parameter), NULL);
    }
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::setCurrentTime (DTfloat time)
{
    if (!_selected_animation)
        return;

    _current_time = time;
    
    // Build a pose
    KeyedSpriteAnimationPose pose;
    pose.update(time, _selected_animation, makeCallback(this, &EdSpriteMainWindow::fireEvents));
    
    Array<KeyedSpriteAnimationPoseJoint*> &joints = _sprite->getJoints();

	// Calculate every sub joint
	for (DTuint i = 0; i < joints.size(); ++i) {
    
        // Check mask to see if the joint should be updated
        if (_selected_animation->getGroups() & joints[i]->getGroups()) {
    
            KeyedSpriteAnimationPoseJoint *joint = joints[i];
            
            // Lookup and calculate transform in the pose
            DTuint name_hash = joint->getNameHash();
            
            pose.getJointState(name_hash, *joint);
            
        }
	}

	emit spriteChanged();
	emit animationsChanged();
}

//==============================================================================
//==============================================================================

DTfloat EdSpriteMainWindow::getGrid (void) const
{
    bool ok;
    float grid = _grid_selection->currentText().toFloat(&ok);
    
    if (!ok)    return 0.0F;
    else        return grid;
}

//==============================================================================
//==============================================================================

const List<KeyedSpriteAnimationPoseJoint*>& EdSpriteMainWindow::getPartSelection (void)
{
	return _selection;
}

void EdSpriteMainWindow::setPartSelection (const List<KeyedSpriteAnimationPoseJoint*> &selection)
{
	_selection = selection;
    emit selectionChanged();
}

void EdSpriteMainWindow::addPartSelection (KeyedSpriteAnimationPoseJoint* selection)
{
	_selection.pushBack(selection);
    emit selectionChanged();
}

void EdSpriteMainWindow::removePartSelection (KeyedSpriteAnimationPoseJoint* selection)
{
	auto i = _selection.find(selection);
	if (i != _selection.end())
		_selection.erase(i);
    emit selectionChanged();
}

void EdSpriteMainWindow::clearPartSelection (void)
{
	_selection.clear();
    emit selectionChanged();
}

bool EdSpriteMainWindow::isPartSelected (KeyedSpriteAnimationPoseJoint* selection)
{
	auto i = _selection.find(selection);
	return (i == _selection.end()) ? false : true;
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::closeEvent(QCloseEvent *event)
{
    if (isOkToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void EdSpriteMainWindow::onNew()
{
    if (isOkToContinue()) {
		setCurrentSpriteFile("");
		setCurrentAnimationFile("");
    }
}

void EdSpriteMainWindow::onOpenSprite()
{
    if (isOkToContinue()) {
        String sprites_path = FilePath("{ED_SPRITES_PATH}").getPath() + FilePath::getPathSeparator() + "*";

        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), 
								   sprites_path.cStr(),
                                   tr("Sprite files (*.kspr)"));
								   
		_current_directory = strippedDir(fileName);
		
        if (!fileName.isEmpty()) {
            QByteArray filenameascii = fileName.toUtf8();
                 
            BaseClass *sprite = Factory::createObjectFromStream(FilePath(filenameascii.constData()));
            if (sprite && sprite->isA(KeyedSpriteResource::kind())) {
                RELEASE(_sprite);
                _sprite = checkedStaticCast<KeyedSpriteResource*>(sprite);
                
                setCurrentSpriteFile(fileName);
            } else {
                RELEASE(sprite);
            }

            clearPartSelection();
            emit selectionChanged();
            emit spriteChanged();
        }
    }
}

void EdSpriteMainWindow::onOpenAnimation()
{
    if (isOkToContinue()) {
        String sprites_path = FilePath("{ED_SPRITES_PATH}").getPath() + FilePath::getPathSeparator() + "*";

        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Animation File"), 
								   sprites_path.cStr(),
                                   tr("Animation files (*.kani)"));
								   
		_current_directory = strippedDir(fileName);
		
        if (!fileName.isEmpty()) {
            QByteArray filenameascii = fileName.toUtf8();
            
            BaseClass *animation = Factory::createObjectFromStream(FilePath(filenameascii.constData()));
            if (animation && animation->isA(KeyedSpriteAnimationResource::kind())) {
                RELEASE(_animation);
                _animation = checkedStaticCast<KeyedSpriteAnimationResource*>(animation);
                
                setCurrentAnimationFile(fileName);
                
                // Set selection to first animation
                if (_animation->getAnimations().size() > 0)
                    _selected_animation = _animation->getAnimations().front();

            } else {
                RELEASE(animation);
            }
            

            clearPartSelection();
            
            emit animationsChanged();
            emit selectionChanged();
            emit spriteChanged();
        }
    }
}

bool EdSpriteMainWindow::onSaveSprite()
{
    if (_current_sprite_file.isEmpty()) {
        return onSaveSpriteAs();
    } else {
        return saveSprite(_current_sprite_file);
    }
}

bool EdSpriteMainWindow::onSaveSpriteAs()
{
	QFileDialog fileDialog(this, "Choose sprite file to save");
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setNameFilter("Sprite files (*.kspr)");
	fileDialog.setDefaultSuffix("kspr");
	int result = fileDialog.exec();

	if (!result)
		return false;
		
    QString fileName = fileDialog.selectedFiles().first();							   
    if (fileName.isEmpty())
        return false;

    return saveSprite(fileName);
}

bool EdSpriteMainWindow::onSaveAnimation()
{
    if (_current_animation_file.isEmpty()) {
        return onSaveAnimationAs();
    } else {
        return saveAnimation(_current_animation_file);
    }
}

bool EdSpriteMainWindow::onSaveAnimationAs()
{
	QFileDialog fileDialog(this, "Choose animation file to save");
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setNameFilter("Sprite files (*.kani)");
	fileDialog.setDefaultSuffix("kani");
	int result = fileDialog.exec();

	if (!result)
		return false;

    QString fileName = fileDialog.selectedFiles().first();							   
    if (fileName.isEmpty())
        return false;

    return saveAnimation(fileName);
}

void EdSpriteMainWindow::onAbout()
{
    /*QMessageBox::On_About(this, tr("On_About Sprite Editor"),
            tr("<h2>Sprite Editor 1.1</h2>"
               "<p>Copyright &copy; 2008 Software Inc."
               "<p>Sprite Editor is a small application that "
               "demonstrates QAction, QEdSpriteMainWindow, QMenuBar, "
               "QStatusBar, QTableWidget, QToolBar, and many other "
               "Qt classes."));*/
}

void EdSpriteMainWindow::onUndo()
{
    _timeline_widget->clearKeySelection();
    clearPartSelection();
    
    // Get index of selected animation
    DTsize selected_animation = _animation->getAnimations().indexOf(_selected_animation);

    // Pop Undo
    if (_undo_queue.size() > 0) {
        UndoQueueEntry &e = _undo_queue.back();
        
        _animation->copy(*e.animation.get());
        _sprite->copy(*e.sprite.get());

        _undo_queue.popBack();
    }
    
    if (_undo_queue.size() == 0) {
        _undo_action->setEnabled(false);
    }
    
    // Reselect animation
    if (selected_animation >= 0) {
        _selected_animation = _animation->getAnimations().get(selected_animation);
    }
    
    emit spriteChanged();
    emit animationsChanged();
}

void EdSpriteMainWindow::onCut()
{
    if (QMetaObject::invokeMethod( QApplication::focusWidget(), "cut", Qt::DirectConnection))
        return;
}

void EdSpriteMainWindow::onCopy()
{
    if (QMetaObject::invokeMethod( QApplication::focusWidget(), "copy", Qt::DirectConnection))
        return;
}

void EdSpriteMainWindow::onPaste()
{
    if (QMetaObject::invokeMethod( QApplication::focusWidget(), "paste", Qt::DirectConnection))
        return;
}

void EdSpriteMainWindow::onDelete()
{
	for (auto i = _selection.begin(); i != _selection.end(); ++i) {
		_sprite->deleteJoint(*i);
	}
    
	clearPartSelection();

	emit spriteChanged();	
}

void EdSpriteMainWindow::onParent()
{
	if (_selection.size() < 2)
		return;
		
	auto i = _selection.begin();
	++i;	// Skip the first item
	
	for (; i != _selection.end(); ++i) {
		_sprite->reparentJoint(*i, _selection.front());
	}
	
	emit spriteChanged();
}

void EdSpriteMainWindow::onUnparent()
{
	for (auto i = _selection.begin(); i != _selection.end(); ++i) {
		_sprite->reparentJoint(*i, NULL);
	}
	
	emit spriteChanged();
}

void EdSpriteMainWindow::onSelectAll()
{
	clearPartSelection();
	
	Array<KeyedSpriteAnimationPoseJoint*> &parts = _sprite->getJoints();
	for (int i = 0; i < parts.size(); ++i) {
		addPartSelection(parts[i]);
	}

	emit spriteChanged();
}

void EdSpriteMainWindow::onDeselectAll()
{
	clearPartSelection();
    emit spriteChanged();
}

void EdSpriteMainWindow::onDuplicate()
{
	List<KeyedSpriteAnimationPoseJoint*> selection = _selection;
	clearPartSelection();

	for (auto i : selection) {
		KeyedSpriteAnimationPoseJoint *part = i->clone();
		_sprite->addJoint(part);
		RELEASE(part);
	}

	//_animation.Set_Part_Keyframes_Size(_sprite->Get_Sprite().Get_Parts().Size());

	emit spriteChanged();
}

void EdSpriteMainWindow::onMoveToFront()
{
	for (auto i : _selection) {
		_sprite->moveToFront(i);
        keyJoint(i);
	}

	emit spriteChanged();
}

void EdSpriteMainWindow::onMoveToBack()
{
	for (auto i : _selection) {
		_sprite->moveToBack(i);
        keyJoint(i);
	}

	emit spriteChanged();
}

void EdSpriteMainWindow::onInsertPart() 
{
	clearPartSelection();

	KeyedSpriteAnimationPoseJoint *part = KeyedSpriteAnimationPoseJoint::create();
	_sprite->addJoint(part);
	RELEASE(part);
	
	emit spriteChanged();
}

void EdSpriteMainWindow::onInsertAnimation() 
{
	KeyedSpriteAnimation *animation = KeyedSpriteAnimation::create();
	animation->setName("New Animation");
	_animation->addAnimation(animation);
    
    _selected_animation = animation;
    
	RELEASE(animation);

    emit animationsChanged();
	emit spriteChanged();
}

void EdSpriteMainWindow::onDeleteAnimation() 
{
	_animation->deleteAnimation(_selected_animation);
	
    // Set selection to first animation
    if (_animation->getAnimations().size() > 0)
        _selected_animation = _animation->getAnimations().front();
    else
        _selected_animation = NULL;

    emit animationsChanged();
	emit spriteChanged();
}

void EdSpriteMainWindow::onDuplicateKeys()
{
    _timeline_widget->onDuplicate();
}

void EdSpriteMainWindow::onNextKeyframe()
{
    if (!_selected_animation)
        return;

	DTfloat time = std::numeric_limits<DTfloat>::infinity();

    Array<KeyedSpriteAnimationPoseJoint*> &joints = _sprite->getJoints();
    
    FOR_EACH(i, joints) {
    
        KeyedSpriteAnimationTrack *track = _selected_animation->getTrackByName( (**i).getName() );
        if (!track)
            continue;

        const Array<KeyedSpriteAnimationKeyframe> &keyframes = track->getKeyframes();
        
        for (DTuint j = 0; j < keyframes.size(); ++j) {
			DTfloat test_time = keyframes[j].getTime();
			
			if (test_time > _current_time && test_time < time)
				time = test_time;
        }
    
    }
    
    if (time == std::numeric_limits<DTfloat>::infinity())
		time = _selected_animation->getStopTime();
		
	setCurrentTime(time);
}

void EdSpriteMainWindow::onPrevKeyframe()
{
    if (!_selected_animation)
        return;

	DTfloat time = -std::numeric_limits<DTfloat>::infinity();

    Array<KeyedSpriteAnimationPoseJoint*> &joints = _sprite->getJoints();
    
    FOR_EACH(i, joints) {
    
        KeyedSpriteAnimationTrack *track = _selected_animation->getTrackByName( (**i).getName() );
        if (!track)
            continue;
        
        const Array<KeyedSpriteAnimationKeyframe> &keyframes = track->getKeyframes();
        
        for (DTuint j = 0; j < keyframes.size(); ++j) {
			DTfloat test_time = keyframes[j].getTime();
			
			if (test_time < _current_time && test_time > time)
				time = test_time;
        }
    
    }
    
    if (time == -std::numeric_limits<DTfloat>::infinity())
		time = _selected_animation->getStartTime();
		
	setCurrentTime(time);
}

void EdSpriteMainWindow::onSetKeyframe()
{
    keySelectedJoint();
}

void EdSpriteMainWindow::onSetAllKeyframes()
{
    keyAllJoints();
}

void EdSpriteMainWindow::onSetEvent()
{
    if (!_selected_animation)
        return;

    EdSpriteNewEventDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {

        KeyedSpriteAnimationEvent event;
        event.setTime(_current_time);
        event.setEventID(dialog.getEventID());
        event.setEventParameter(dialog.getEventParameters());

        if (!_animation || !_selected_animation)
            return;
            
        _selected_animation->getEventTrack().addKeyframe(event);
        
        emit spriteChanged();
        
    }
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::onResetGrid()
{
    const List<KeyedSpriteAnimationPoseJoint*>& selection = getPartSelection();
    
    FOR_EACH(i, selection) {
        (**i).resetGrid();
        keyJoint(*i);
    }
    
	emit spriteChanged();
}

void EdSpriteMainWindow::onTransferAnimations()
{
    EdSpriteAnimationTransferDialog dialog(this);
    dialog.exec();
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::keyJoint (KeyedSpriteAnimationPoseJoint* joint)
{
    if (!joint || !_animation || !_selected_animation)
        return;
        
    KeyedSpriteAnimationTrack *track = _selected_animation->getTrackByName(joint->getName());
    if (!track) {
        track = KeyedSpriteAnimationTrack::create();
        track->setName(joint->getName());
        
        _selected_animation->addTrack(track);
        track->release();   // Retained on the previous line
    }
    
    // Add the keyframe
    KeyedSpriteAnimationKeyframe kf = joint->getAsKey();
    kf.setTime(getCurrentTime());
    
    track->addKeyframe(kf);
    
	emit spriteChanged();
}

void EdSpriteMainWindow::keySelectedJoint (void)
{
    const List<KeyedSpriteAnimationPoseJoint*>& selection = getPartSelection();
    
    FOR_EACH(i, selection) {
        keyJoint(*i);
    }
    
	emit spriteChanged();    
}

void EdSpriteMainWindow::keyAllJoints (void)
{
    Array<KeyedSpriteAnimationPoseJoint*> &joints = _sprite->getJoints();
    
    FOR_EACH(i, joints) {
        keyJoint(*i);
    }

	emit spriteChanged();    
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::onPlay()
{
	_animation_timer.setSingleShot(false);
	_animation_timer.start(TIMER_INTERVAL * 1000);
    if (System::getAudioRenderer())
        System::getAudioRenderer()->stopAll();
}

void EdSpriteMainWindow::onStop()
{
	_animation_timer.stop();
    if (System::getAudioRenderer())
        System::getAudioRenderer()->stopAll();
}

void EdSpriteMainWindow::onTickAnimation()
{
    if (!_selected_animation) {
        onStop();
        return;
    }
    
    if (System::getAudioRenderer())
        System::getAudioRenderer()->tick(TIMER_INTERVAL);

	_current_time += TIMER_INTERVAL * _selected_animation->getSpeed();
    
	while (_current_time > _selected_animation->getStopTime())
		_current_time -= _selected_animation->getStopTime() - _selected_animation->getStartTime();
	
	setCurrentTime(_current_time);
    
	emit spriteChanged();
	emit animationsChanged();
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::onDocumentModified()
{

}

void EdSpriteMainWindow::onUpdateStatusBar()
{
    setWindowModified(true);
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::onChangeGrid(int)
{
	emit spriteChanged();
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::createActions()
{
    _new_action = new QAction(tr("&New"), this);
    _new_action->setIcon(QIcon(":/images/new.png"));
    _new_action->setShortcut(QKeySequence::New);
    _new_action->setStatusTip(tr("Create a new Sprite file"));
    connect(_new_action, SIGNAL(triggered()), this, SLOT(onNew()));

    _open_sprite_action = new QAction(tr("&Open Sprite..."), this);
    _open_sprite_action->setIcon(QIcon(":/images/open_sprite.png"));
    _open_sprite_action->setShortcut(tr("Ctrl+Shift+O"));
    _open_sprite_action->setStatusTip(tr("Open an existing Sprite file"));
    connect(_open_sprite_action, SIGNAL(triggered()), this, SLOT(onOpenSprite()));

    _open_animation_action = new QAction(tr("&Open Animation..."), this);
    _open_animation_action->setIcon(QIcon(":/images/open_animation.png"));
    _open_animation_action->setShortcut(tr("Ctrl+O"));
    _open_animation_action->setStatusTip(tr("Open an existing Animation file"));
    connect(_open_animation_action, SIGNAL(triggered()), this, SLOT(onOpenAnimation()));

    _save_sprite_action = new QAction(tr("&Save Sprite"), this);
    _save_sprite_action->setIcon(QIcon(":/images/save_sprite.png"));
    _save_sprite_action->setShortcut(tr("Ctrl+Shift+S"));
    _save_sprite_action->setStatusTip(tr("Save the Sprite to disk"));
    connect(_save_sprite_action, SIGNAL(triggered()), this, SLOT(onSaveSprite()));

    _save_sprite_as_action = new QAction(tr("Save Sprite &As..."), this);
    _save_sprite_as_action->setStatusTip(tr("Save a copy of the Sprite to disk"));
    connect(_save_sprite_as_action, SIGNAL(triggered()), this, SLOT(onSaveSpriteAs()));

    _save_animation_action = new QAction(tr("&Save Animation"), this);
    _save_animation_action->setIcon(QIcon(":/images/save_animation.png"));
    _save_animation_action->setShortcut(tr("Ctrl+S"));
    _save_animation_action->setStatusTip(tr("Save the Animation to disk"));
    connect(_save_animation_action, SIGNAL(triggered()), this, SLOT(onSaveAnimation()));

    _save_animation_as_action = new QAction(tr("Save Animation &As..."), this);
    _save_animation_as_action->setStatusTip(tr("Save a copy of the Animation to disk"));
    connect(_save_animation_as_action, SIGNAL(triggered()), this, SLOT(onSaveAnimationAs()));

    _exit_action = new QAction(tr("E&xit"), this);
    _exit_action->setShortcut(tr("Ctrl+Q"));
    _exit_action->setStatusTip(tr("Exit the application"));
    connect(_exit_action, SIGNAL(triggered()), this, SLOT(close()));
	
	//
	// Edit menu
	// 

    _undo_action = new QAction(tr("Undo"), this);
    _undo_action->setIcon(QIcon(":/images/undo.png"));
    _undo_action->setShortcut(QKeySequence::Undo);
    _undo_action->setStatusTip(tr("Undo the last action"));
    _undo_action->setEnabled(false);
    connect(_undo_action, SIGNAL(triggered()), this, SLOT(onUndo()));

    _cut_action = new QAction(tr("Cu&t"), this);
    _cut_action->setIcon(QIcon(":/images/cut.png"));
    _cut_action->setShortcut(QKeySequence::Cut);
    _cut_action->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    _cut_action->setEnabled(false);
    connect(_cut_action, SIGNAL(triggered()), this, SLOT(onCut()));

    _copy_action = new QAction(tr("&Copy"), this);
    _copy_action->setIcon(QIcon(":/images/copy.png"));
    _copy_action->setShortcut(QKeySequence::Copy);
    _copy_action->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    _copy_action->setEnabled(false);
    connect(_copy_action, SIGNAL(triggered()), this, SLOT(onCopy()));

    _paste_action = new QAction(tr("&Paste"), this);
    _paste_action->setIcon(QIcon(":/images/paste.png"));
    _paste_action->setShortcut(QKeySequence::Paste);
    _paste_action->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    _paste_action->setEnabled(false);
    connect(_paste_action, SIGNAL(triggered()),
            this, SLOT(onPaste()));

    _delete_action = new QAction(tr("&Delete"), this);
    _delete_action->setStatusTip(tr("Delete the current selection's contents"));
    connect(_delete_action, SIGNAL(triggered()),
            this, SLOT(onDelete()));

    _parent_action = new QAction(tr("Parent"), this);
    _parent_action->setStatusTip(tr("Parent to the first selected item"));
    connect(_parent_action, SIGNAL(triggered()),
            this, SLOT(onParent()));

    _unparent_action = new QAction(tr("Unparent"), this);
    _unparent_action->setStatusTip(tr("Unparent the part"));
    connect(_unparent_action, SIGNAL(triggered()),
            this, SLOT(onUnparent()));

    _select_all_action = new QAction(tr("Select All"), this);
	_select_all_action->setShortcut(QKeySequence::SelectAll);
	_select_all_action->setStatusTip(tr("Select all parts"));
    connect(_select_all_action, SIGNAL(triggered()),
            this, SLOT(onSelectAll()));

    _deselect_all_action = new QAction(tr("Deselect All"), this);
	_deselect_all_action->setStatusTip(tr("Deselect all parts"));
    connect(_deselect_all_action, SIGNAL(triggered()),
            this, SLOT(onDeselectAll()));

    _duplicate_action = new QAction(tr("Duplicate"), this);
	_duplicate_action->setShortcut(QKeySequence("Ctrl+D"));
    _duplicate_action->setStatusTip(tr("Duplicate selected parts"));
    connect(_duplicate_action, SIGNAL(triggered()),
            this, SLOT(onDuplicate()));

    _move_to_front_action = new QAction(tr("Move To Front"), this);
    _move_to_front_action->setStatusTip(tr("Move Part to Front"));
    connect(_move_to_front_action, SIGNAL(triggered()),
            this, SLOT(onMoveToFront()));

    _move_to_back_action = new QAction(tr("Move To Back"), this);
    _move_to_back_action->setStatusTip(tr("Move Part to Back"));
    connect(_move_to_back_action, SIGNAL(triggered()),
            this, SLOT(onMoveToBack()));

    _insert_part_action = new QAction(tr("Insert Part"), this);
	_insert_part_action->setShortcut(QKeySequence("Ctrl+I"));
    _insert_part_action->setStatusTip(tr("Inserts a new part"));
    connect(_insert_part_action, SIGNAL(triggered()),
            this, SLOT(onInsertPart()));

    _insert_animation_action = new QAction(tr("Insert Animation"), this);
	_insert_animation_action->setShortcut(QKeySequence("Ctrl+Shift+I"));
    _insert_animation_action->setStatusTip(tr("Inserts a new animation"));
    connect(_insert_animation_action, SIGNAL(triggered()),
            this, SLOT(onInsertAnimation()));

    _delete_animation_action = new QAction(tr("Delete Animation"), this);
    _delete_animation_action->setStatusTip(tr("Delete the current Animation"));
    connect(_delete_animation_action, SIGNAL(triggered()),
            this, SLOT(onDeleteAnimation()));
			
			
	//
	// About item
	//

    _about_action = new QAction(tr("&About"), this);
    _about_action->setStatusTip(tr("Show the application's About box"));
    connect(_about_action, SIGNAL(triggered()), this, SLOT(onAbout()));


	//
	// Playback item
	//

    _play_action = new QAction(tr("&Play"), this);
    _play_action->setIcon(QIcon(":/images/play.png"));
    _play_action->setStatusTip(tr("Play animations"));
    connect(_play_action, SIGNAL(triggered()), this, SLOT(onPlay()));

    _stop_action = new QAction(tr("&Stop"), this);
    _stop_action->setIcon(QIcon(":/images/stop.png"));
    _stop_action->setStatusTip(tr("Stop animations"));
    connect(_stop_action, SIGNAL(triggered()), this, SLOT(onStop()));


	//
	// Keyframe navigation item
	//

	_duplicate_keys_action = new QAction(tr("Duplicate Keys"), this);
    _duplicate_keys_action->setStatusTip(tr("Duplicates selected keys"));
    connect(_duplicate_keys_action, SIGNAL(triggered()),
            this, SLOT(onDuplicateKeys()));

	_next_keyframe = new QAction(tr("Next Keyframe"), this);
	_next_keyframe->setShortcut(QKeySequence("Ctrl+]"));
    _next_keyframe->setStatusTip(tr("Moves current time to next keyframe"));
    connect(_next_keyframe, SIGNAL(triggered()),
            this, SLOT(onNextKeyframe()));

	_prev_keyframe = new QAction(tr("Prev Keyframe"), this);
	_prev_keyframe->setShortcut(QKeySequence("Ctrl+["));
    _prev_keyframe->setStatusTip(tr("Moves current time to previous keyframe"));
    connect(_prev_keyframe, SIGNAL(triggered()),
            this, SLOT(onPrevKeyframe()));

	_set_keyframe = new QAction(tr("Set Keyframe"), this);
	_set_keyframe->setShortcut(QKeySequence("Ctrl+K"));
    _set_keyframe->setStatusTip(tr("Sets keyframe"));
    connect(_set_keyframe, SIGNAL(triggered()),
            this, SLOT(onSetKeyframe()));

	_set_all_keyframes = new QAction(tr("Set All Keyframes"), this);
	_set_all_keyframes->setShortcut(QKeySequence("Ctrl+Shift+K"));
    _set_all_keyframes->setStatusTip(tr("Sets keyframes for all tracks"));
    connect(_set_all_keyframes, SIGNAL(triggered()),
            this, SLOT(onSetAllKeyframes()));


	_set_event = new QAction(tr("Set Event..."), this);
	_set_event->setShortcut(QKeySequence("Ctrl+E"));
    _set_event->setStatusTip(tr("Sets event"));
    connect(_set_event, SIGNAL(triggered()),
            this, SLOT(onSetEvent()));


    //
    // Grid Warp
    //
    
	_reset_grid = new QAction(tr("Reset Grid"), this);
    _reset_grid->setStatusTip(tr("Resets the grid warp"));
    connect(_reset_grid, SIGNAL(triggered()),
            this, SLOT(onResetGrid()));
    
    //
    // Animation Transfer tool
    //
    
    _transfer_animations = new QAction(tr("Transfer Animations..."), this);
    _transfer_animations->setStatusTip(tr("Tool to help in transferring animations"));
    connect(_transfer_animations, SIGNAL(triggered()),
            this, SLOT(onTransferAnimations()));
}

void EdSpriteMainWindow::createMenus()
{
    _file_menu = menuBar()->addMenu(tr("&File"));
    _file_menu->addAction(_new_action);
    _file_menu->addAction(_open_sprite_action);
    _file_menu->addAction(_open_animation_action);
    _file_menu->addSeparator();
    _file_menu->addAction(_save_sprite_action);
    _file_menu->addAction(_save_sprite_as_action);
    _file_menu->addSeparator();
    _file_menu->addAction(_save_animation_action);
    _file_menu->addAction(_save_animation_as_action);
    _file_menu->addSeparator();
    _file_menu->addAction(_exit_action);

    _edit_menu = menuBar()->addMenu(tr("&Edit"));
    _edit_menu->addAction(_undo_action);
	_edit_menu->addSeparator();
    _edit_menu->addAction(_cut_action);
    _edit_menu->addAction(_copy_action);
    _edit_menu->addAction(_paste_action);
    _edit_menu->addAction(_delete_action);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_select_all_action);
	_edit_menu->addAction(_deselect_all_action);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_duplicate_action);
	_edit_menu->addAction(_insert_part_action);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_insert_animation_action);
	_edit_menu->addAction(_delete_animation_action);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_move_to_front_action);
	_edit_menu->addAction(_move_to_back_action);
	_edit_menu->addAction(_parent_action);
    _edit_menu->addAction(_unparent_action);
	_edit_menu->addSeparator();
    _edit_menu->addAction(_duplicate_keys_action);
	_edit_menu->addAction(_next_keyframe);
	_edit_menu->addAction(_prev_keyframe);
	_edit_menu->addAction(_set_keyframe);
	_edit_menu->addAction(_set_all_keyframes);
	_edit_menu->addAction(_set_event);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_reset_grid);
	_edit_menu->addSeparator();
	_edit_menu->addAction(_transfer_animations);

    menuBar()->addSeparator();

    _help_menu = menuBar()->addMenu(tr("&Help"));
    _help_menu->addAction(_about_action);
}

void EdSpriteMainWindow::createContextMenu()
{
    this->addAction(_cut_action);
    this->addAction(_copy_action);
    this->addAction(_paste_action);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void EdSpriteMainWindow::createToolBars()
{
    _file_toolbar = addToolBar(tr("&File"));
	_file_toolbar->setIconSize(QSize(16,16));
    _file_toolbar->addAction(_new_action);
    _file_toolbar->addAction(_open_sprite_action);
    _file_toolbar->addAction(_open_animation_action);
    _file_toolbar->addAction(_save_sprite_action);
    _file_toolbar->addAction(_save_animation_action);

    _edit_toolbar = addToolBar(tr("&Edit"));
	_edit_toolbar->setIconSize(QSize(16,16));
    _edit_toolbar->addAction(_cut_action);
    _edit_toolbar->addAction(_copy_action);
    _edit_toolbar->addAction(_paste_action);
	
	_playback_toolbar = addToolBar(tr("&Playback"));
	_playback_toolbar->setIconSize(QSize(16,16));
    _playback_toolbar->addAction(_play_action);
    _playback_toolbar->addAction(_stop_action);
    
    //
    // Grid selection
    //
    
    QWidget *spacer3 = new QWidget(_playback_toolbar);
    spacer3->setMinimumWidth(10);
    _playback_toolbar->addWidget(spacer3);

    QLabel *grid_selection_label = new QLabel("Time Grid:",_playback_toolbar);
    grid_selection_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _playback_toolbar->addWidget(grid_selection_label);

    _grid_selection = new QComboBox(_playback_toolbar);
    _grid_selection->setMinimumWidth(75);

    _grid_selection->setEditable(true);
    
    _grid_selection->addItem("None");
    _grid_selection->addItem("0.1");
    _grid_selection->addItem("0.2");
    _grid_selection->addItem("0.5");
    _grid_selection->addItem("1.0");

	connect(	_grid_selection,  SIGNAL(activated(int)),
				this,               SLOT(onChangeGrid(int))	);
    
	connect(	_grid_selection,  SIGNAL(activated(int)),
				this,               SLOT(onChangeGrid(int))	);

    _grid_selection->setCurrentIndex(1);

    _playback_toolbar->addWidget(_grid_selection);

}

void EdSpriteMainWindow::createStatusBar()
{
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::readSettings()
{
    QSettings settings("Smells Like Donkey, Inc.", "Sprite Editor");

    restoreGeometry(settings.value("geometry").toByteArray());
}

void EdSpriteMainWindow::writeSettings()
{
    QSettings settings("Smells Like Donkey, Inc.", "Sprite Editor");

    settings.setValue("geometry", saveGeometry());
}

//==============================================================================
//==============================================================================

bool EdSpriteMainWindow::isOkToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Sprite Editor"),
                        tr("The document has been modified.\n"
                           "Do you want to On_Save_Sprite your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return onSaveSprite();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool EdSpriteMainWindow::saveSprite(const QString &fileName)
{
	ArchiveTextWriter writer;
	writer.setIgnoreStreamableFlag(true);
	writer.open(FilePath(fileName.toUtf8().data()));
	ArchiveObjectQueue::queueOutTree(&writer, _sprite);
	writer.close();

    setCurrentSpriteFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool EdSpriteMainWindow::saveAnimation(const QString &fileName)
{
	ArchiveTextWriter writer;
	writer.setIgnoreStreamableFlag(true);
	writer.open(FilePath(fileName.toUtf8().data()));
	ArchiveObjectQueue::queueOutTree(&writer, _animation);
	writer.close();

    setCurrentAnimationFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void EdSpriteMainWindow::setCurrentSpriteFile(const QString &fileName)
{
    _current_sprite_file = fileName;

    setWindowTitle(tr("%1,%2[*] - %3").arg(strippedName(_current_sprite_file.isEmpty() ? "Untitled" : _current_sprite_file))
									.arg(strippedName(_current_animation_file.isEmpty() ? "Untitled" : _current_animation_file))
                                   .arg(tr("Sprite Editor")));
}

void EdSpriteMainWindow::setCurrentAnimationFile(const QString &fileName)
{
    _current_animation_file = fileName;
    setWindowModified(false);

    setWindowTitle(tr("%1,%2[*] - %3").arg(strippedName(_current_sprite_file.isEmpty() ? "Untitled" : _current_sprite_file))
									.arg(strippedName(_current_animation_file.isEmpty() ? "Untitled" : _current_animation_file))
                                   .arg(tr("Sprite Editor")));
}

QString EdSpriteMainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString EdSpriteMainWindow::strippedDir(const QString &fullFileName)
{
    return QFileInfo(fullFileName).dir().path();
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::fireSpriteChanged (void)
{
    emit spriteChanged();
}

void EdSpriteMainWindow::fireAnimationsChanged (void)
{
    emit animationsChanged();
}

//==============================================================================
//==============================================================================

void EdSpriteMainWindow::showAssert (const DTcharacter* file, const DTcharacter* func, const DTint line)
{
    static List<ErrorEntry> errors;
    
    ErrorEntry error_entry(file,func,line);
    if (errors.find(error_entry) == errors.end()) {

        int r = QMessageBox::critical(this, tr("Assertion"),
                    QString("Assertion in file ") + file + QString(" in function ") + func + QString(" on line ") + QString::number(line),
                    QMessageBox::Abort | QMessageBox::Ignore | QMessageBox::Ok);

        if (r == QMessageBox::Abort) {    
            exit(1);        
        } else if (r == QMessageBox::Ignore) {
            errors.pushBack(error_entry);
        } else if (r == QMessageBox::Ok) {
            // Do nothing
        }
        
    }
}

void EdSpriteMainWindow::showError (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter* msg)
{
    static List<ErrorEntry> errors;
    
    ErrorEntry error_entry(file,func,line);
    if (errors.find(error_entry) == errors.end()) {

        int r = QMessageBox::critical(this, tr("Error"),
                    QString("Error ") + msg + QString(" in file ") + file + QString(" in function ") + func + QString(" on line ") + QString::number(line),
                    QMessageBox::Abort | QMessageBox::Ignore | QMessageBox::Ok);

        if (r == QMessageBox::Abort) {    
            exit(1);        
        } else if (r == QMessageBox::Ignore) {
            errors.pushBack(error_entry);
        } else if (r == QMessageBox::Ok) {
            // Do nothing
        }
        
    }
}

void EdSpriteMainWindow::showWarning (const DTcharacter* file, const DTcharacter* func, const DTint line, const DTcharacter* msg)
{
    static List<ErrorEntry> errors;
    
    ErrorEntry error_entry(file,func,line);
    if (errors.find(error_entry) == errors.end()) {

        int r = QMessageBox::warning(this, tr("Warning"),
                    QString("Warning ") + msg + QString(" in file ") + file + QString(" in function ") + func + QString(" on line ") + QString::number(line),
                    QMessageBox::Abort | QMessageBox::Ignore | QMessageBox::Ok);

        if (r == QMessageBox::Abort) {    
            exit(1);        
        } else if (r == QMessageBox::Ignore) {
            errors.pushBack(error_entry);
        } else if (r == QMessageBox::Ok) {
            // Do nothing
        }
    }
}

//==============================================================================
//==============================================================================

#include "moc_EdSpriteMainWindow.cpp"


//==============================================================================
///	
///	File: EdSpriteAnimationTransferDialog.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteAnimationTransferDialog.hpp"
#include "EdSpriteMainWindow.hpp"
#include "KeyedSpriteAnimationResource.hpp"
#include "KeyedSpriteAnimation.hpp"

#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>

using namespace DT2;

//==============================================================================
//==============================================================================

EdSpriteAnimationTransferDialog::EdSpriteAnimationTransferDialog(EdSpriteMainWindow *main_window)
    :   QDialog             (NULL)
{
    setWindowTitle("Animation Transfer Tool");

    _main_window = main_window;

    //
    // Top of Window
    //
    
    _from_animation = new QComboBox(this);
    _to_animation = new QComboBox(this);


    QGridLayout *layout_top = new QGridLayout;
    layout_top->setContentsMargins(5,5,5,5);
    
    layout_top->setHorizontalSpacing(5);
    layout_top->setVerticalSpacing(5);
    
    layout_top->addWidget(new QLabel("From Animation:", this), 0, 0);
    layout_top->addWidget(_from_animation, 0, 1);
    
    layout_top->addWidget(new QLabel("To Animation", this), 1, 0);
    layout_top->addWidget(_to_animation, 1, 1);
    
    if (_main_window->getSprite() && _main_window->getAnimationResource()) {

        KeyedSpriteAnimationResource* animation_resource = _main_window->getAnimationResource();
        List<KeyedSpriteAnimation*>& animations = animation_resource->getAnimations();
        Array<KeyedSpriteAnimationPoseJoint*>& joints = _main_window->getSprite()->getJoints();

        // Add default items to "to_animation"
        _to_animation->addItem("New Animation");
        
        if (_to_animation->count() > 0)
            _to_animation->insertSeparator(_to_animation->count());

        FOR_EACH(i,animations) {
            _from_animation->addItem( (**i).getName().cStr() );
            _to_animation->addItem( (**i).getName().cStr() );
        }
        
        // Choose current animation if there is one
        if (_from_animation) {
            DTint i = _from_animation->findText(_main_window->getSelectedAnimation()->getName().cStr());
            _from_animation->setCurrentIndex(i);
        }


        //
        // Middle of Window
        //
        
        _scroll_area = new QTableWidget;
        _scroll_area->setMinimumWidth(370);
        _scroll_area->setMinimumHeight(500);
        
        _scroll_area->setShowGrid(false);
        _scroll_area->verticalHeader()->setVisible(false);
        //_scroll_area->horizontalHeader()->setVisible(false);
        _scroll_area->setSelectionMode(QAbstractItemView::NoSelection);
    
    
        
        _scroll_area->setColumnCount(5);
        _scroll_area->setRowCount((DTint) joints.size());
        
        _scroll_area->setColumnWidth(0, 150);
        _scroll_area->setColumnWidth(1, 150);
        _scroll_area->setColumnWidth(2, 20);
        _scroll_area->setColumnWidth(3, 16);
        _scroll_area->setColumnWidth(4, 16);
        
        _scroll_area->setHorizontalHeaderItem(0, new QTableWidgetItem("From Track"));
        _scroll_area->setHorizontalHeaderItem(1, new QTableWidgetItem("To Track"));
        _scroll_area->setHorizontalHeaderItem(2, new QTableWidgetItem(" "));
        _scroll_area->setHorizontalHeaderItem(3, new QTableWidgetItem("H"));
        _scroll_area->setHorizontalHeaderItem(4, new QTableWidgetItem("V"));
        
        _track_info.resize(joints.size());

        for (DTint i = 0; i < joints.size(); ++i) {
            _scroll_area->setRowHeight(i, 16);

            // Label
            _scroll_area->setCellWidget(i, 0, new QLabel( joints[i]->getName().cStr(), this ));
        
            // Joint list
            QComboBox *joint_list = new QComboBox(this);
            
            for (DTint j = 0; j < joints.size(); ++j) {
                joint_list->addItem( joints[j]->getName().cStr() );
            }
            
            // Auto select reflected joint
            String selected = joints[i]->getName();
            if (selected.find("Left") != String::NPOS)          selected = selected.findAndReplace("Left", "Right");
            else if (selected.find("Right") != String::NPOS)    selected = selected.findAndReplace("Right", "Left");
            joint_list->setCurrentText(selected.cStr());
            
            
            _scroll_area->setCellWidget(i, 1, joint_list);
            
            // Flip Horz
            QCheckBox *flip_horz = new QCheckBox(this);
            flip_horz->setCheckState(Qt::Checked);
            _scroll_area->setCellWidget(i, 3, flip_horz);

            // Flip Vert
            QCheckBox *flip_vert = new QCheckBox(this);
            _scroll_area->setCellWidget(i, 4, flip_vert);
            
            
            // Remember controls
            _track_info[i]._from_track = joints[i]->getName();
            _track_info[i]._to_track = joint_list;
            _track_info[i]._flip_horz = flip_horz;
            _track_info[i]._flip_vert = flip_vert;
        }
        
    }
    

    //
    // Bottom of Window
    //
    
    QPushButton *cancel = new QPushButton("Cancel", this);
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(onCancel()));

    QPushButton *transfer = new QPushButton("Transfer", this);
    connect(transfer, SIGNAL(clicked()),
            this, SLOT(onTransfer()));
    
    QGridLayout *layout_bottom = new QGridLayout;
    layout_bottom->setContentsMargins(5,5,5,5);
    
    layout_bottom->setHorizontalSpacing(5);
    layout_bottom->setVerticalSpacing(5);

    layout_bottom->addWidget(cancel, 0, 0);
    layout_bottom->addWidget(transfer, 0, 1);
    
    //
    // All Window
    //

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(5,5,5,5);
    
    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(5);
    
    layout->addLayout(layout_top, 0, 0);
    layout->addWidget(_scroll_area, 1, 0);
    layout->addLayout(layout_bottom, 2, 0);

    setLayout(layout);
    
    
}

EdSpriteAnimationTransferDialog::~EdSpriteAnimationTransferDialog (void)
{

}


//==============================================================================
//==============================================================================

void EdSpriteAnimationTransferDialog::onCancel (void)
{
    done(QDialog::Rejected);
}

void EdSpriteAnimationTransferDialog::onTransfer (void)
{
    _main_window->pushUndo();

    if (_main_window->getSprite() && _main_window->getAnimationResource()) {

        KeyedSpriteAnimationResource* animation_resource = _main_window->getAnimationResource();
        Array<KeyedSpriteAnimationPoseJoint*>& joints = _main_window->getSprite()->getJoints();

        // Get Source Animation
        KeyedSpriteAnimation *from_animation = animation_resource->getAnimationByName(_from_animation->currentText().toUtf8().data());

        // Get destination animation
        KeyedSpriteAnimation *to_animation = NULL;
        if (_to_animation->currentIndex() == 0) {
            to_animation = KeyedSpriteAnimation::create();
            to_animation->setName("New Animation");
            
            // Copy params from source animation
            to_animation->setStartTime(from_animation->getStartTime());
            to_animation->setStopTime(from_animation->getStopTime());
            to_animation->setSpeed(from_animation->getSpeed());
            to_animation->setGroups(from_animation->getGroups());
            
            animation_resource->addAnimation(to_animation);
        } else {
            to_animation = animation_resource->getAnimationByName(_to_animation->currentText().toUtf8().data());
            RETAIN(to_animation);
        }
        Assert(to_animation);

        // Transfer all tracks
        for (DTint i = 0; i < joints.size(); ++i) {
            String from_track_name = _track_info[i]._from_track;
            String to_track_name = _track_info[i]._to_track->currentText().toUtf8().data();
            
            // Get the track fron the source track
            KeyedSpriteAnimationTrack *from_track = from_animation->getTrackByName(from_track_name);
            if (!from_track)
                continue;
            
            // Add the track to the destination animation
            KeyedSpriteAnimationTrack *to_track = KeyedSpriteAnimationTrack::create();
            to_track->setName(to_track_name);
            
            // Remove any existing teack and re add
            to_animation->removeTrack(to_animation->getTrackByName(to_track_name));
            to_animation->addTrack(to_track);
            
            // Copy keys
            const Array<KeyedSpriteAnimationKeyframe>& keys = from_track->getKeyframes();
            for (DTint j = 0; j < keys.size(); ++j) {
                KeyedSpriteAnimationKeyframe key = keys[j];
            
                // Manipulate keys
                if (_track_info[i]._flip_horz->checkState() == Qt::Checked)
                    key.flipHorz();

                if (_track_info[i]._flip_vert->checkState() == Qt::Checked)
                    key.flipVert();
                
                // Copy texture coords from joint for new track
                KeyedSpriteAnimationPoseJoint *to_joint = _main_window->getSprite()->getJointByName(to_track_name);
                
                key.setPlusU(to_joint->getPlusU());
                key.setPlusV(to_joint->getPlusV());
                key.setMinusU(to_joint->getMinusU());
                key.setMinusV(to_joint->getMinusV());
                
                to_track->addKeyframe(key);
            }
            
            to_track->updateKeyframes();

            RELEASE(to_track);
        }

        RELEASE(to_animation);

    }
    
    _main_window->fireAnimationsChanged();
    _main_window->fireSpriteChanged();
    
    done(QDialog::Accepted);
}

//==============================================================================
//==============================================================================

#include "moc_EdSpriteAnimationTransferDialog.cpp"


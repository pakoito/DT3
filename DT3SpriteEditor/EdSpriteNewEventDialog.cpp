//==============================================================================
///	
///	File: EdSpriteNewEventDialog.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteNewEventDialog.hpp"
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

EdSpriteNewEventDialog::EdSpriteNewEventDialog(EdSpriteMainWindow *main_window)
    :   QDialog             (NULL)
{
    setWindowTitle("New Event");

    _main_window = main_window;

    //
    // Top of Window
    //
    
    _event_id = new QComboBox(this);
    _event_id->setMinimumWidth(300);
    _event_id->addItem("PlaySound");
    _event_id->addItem("SpawnParticles");
    _event_id->setEditable(true);
    
    _event_parameters = new EdSpriteLineEdit(this);
    _event_parameters->setMinimumWidth(300);

    QGridLayout *layout_top = new QGridLayout;
    layout_top->setContentsMargins(5,5,5,5);
    
    layout_top->setHorizontalSpacing(5);
    layout_top->setVerticalSpacing(5);
    
    layout_top->addWidget(new QLabel("Event ID:", this), 0, 0);
    layout_top->addWidget(_event_id, 0, 1);
    
    layout_top->addWidget(new QLabel("Event Parameter:", this), 1, 0);
    layout_top->addWidget(_event_parameters, 1, 1);
    
    //
    // Bottom of Window
    //
    
    QPushButton *cancel = new QPushButton("Cancel", this);
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(onCancel()));

    QPushButton *create = new QPushButton("Create", this);
    connect(create, SIGNAL(clicked()),
            this, SLOT(onCreate()));
    
    QGridLayout *layout_bottom = new QGridLayout;
    layout_bottom->setContentsMargins(5,5,5,5);
    
    layout_bottom->setHorizontalSpacing(5);
    layout_bottom->setVerticalSpacing(5);

    layout_bottom->addWidget(cancel, 0, 0);
    layout_bottom->addWidget(create, 0, 1);
    
    //
    // All Window
    //

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(5,5,5,5);
    
    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(5);
    
    layout->addLayout(layout_top, 0, 0);
    layout->addLayout(layout_bottom, 2, 0);

    setLayout(layout);
    
    
}

EdSpriteNewEventDialog::~EdSpriteNewEventDialog (void)
{

}

//==============================================================================
//==============================================================================

String EdSpriteNewEventDialog::getEventID (void)
{
    return _event_id->currentText().toUtf8().data();
}

String EdSpriteNewEventDialog::getEventParameters (void)
{
    return _event_parameters->text().toUtf8().data();
}

//==============================================================================
//==============================================================================

void EdSpriteNewEventDialog::onCancel (void)
{
    done(QDialog::Rejected);
}

void EdSpriteNewEventDialog::onCreate (void)
{
    _main_window->pushUndo();

    done(QDialog::Accepted);
}

//==============================================================================
//==============================================================================

#include "moc_EdSpriteNewEventDialog.cpp"


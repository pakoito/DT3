//==============================================================================
///	
///	File: EdLevelLaunchDialog.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelLaunchDialog.hpp"

// Qt include
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSizePolicy>

// Engine includes
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelLaunchDialog::EdLevelLaunchDialog(void)
    :   QDialog             (NULL)
{
    setWindowTitle("Launch Properties");
    
    std::string game_path = Globals::global("ED_GAME_PATH");
    DTboolean resolution_enabled = MoreStrings::cast_from_string<DTboolean>(Globals::global("ED_RESOLUTION_ENABLED"));
    std::string resolution_width = Globals::global("ED_RESOLUTION_WIDTH");
    std::string resolution_height = Globals::global("ED_RESOLUTION_HEIGHT");

    //
    // Top of window
    //

    QGridLayout *layout_top = new QGridLayout;
    layout_top->setContentsMargins(5,5,5,5);
    
    layout_top->setHorizontalSpacing(5);
    layout_top->setVerticalSpacing(5);
    
    layout_top->addWidget(new QLabel("Path to executable:", this), 0, 1);
    
    _game_path = new QLineEdit(game_path.c_str(), this);
    layout_top->addWidget(_game_path, 0, 2, 1, 3);

    _resolution_enabled = new QCheckBox(this);
    _resolution_enabled->setChecked(resolution_enabled);
    layout_top->addWidget(_resolution_enabled, 1, 0);
    
    layout_top->addWidget(new QLabel("Resolution:", this), 1, 1);
    
    _resolution_width = new QLineEdit(resolution_width.c_str(), this);
    layout_top->addWidget(_resolution_width, 1, 2);
    
    layout_top->addWidget(new QLabel("x", this), 1, 3);
    
    _resolution_height = new QLineEdit(resolution_height.c_str(), this);
    layout_top->addWidget(_resolution_height, 1, 4);

    
    //
    // Bottom of Window
    //
    
    QPushButton *cancel = new QPushButton("Cancel", this);
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(onCancel()));

    QPushButton *ok = new QPushButton("OK", this);
    connect(ok, SIGNAL(clicked()),
            this, SLOT(onOK()));
    
    QGridLayout *layout_bottom = new QGridLayout;
    layout_bottom->setContentsMargins(5,5,5,5);
    
    layout_bottom->setHorizontalSpacing(5);
    layout_bottom->setVerticalSpacing(5);

    layout_bottom->addWidget(cancel, 0, 0);
    layout_bottom->addWidget(ok, 0, 1);

    //
    // All Window
    //

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(5,5,5,5);
    
    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(5);
    
    layout->addLayout(layout_top, 0, 0);
    layout->addLayout(layout_bottom, 1, 0);
    
    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 0);

    setLayout(layout);
    
    // Fix the size
    QSize r = sizeHint();
    r.setWidth(500);
    setFixedSize(r);
}

EdLevelLaunchDialog::~EdLevelLaunchDialog (void)
{

}

//==============================================================================
//==============================================================================

void EdLevelLaunchDialog::onCancel (void)
{
    done(QDialog::Rejected);
}

void EdLevelLaunchDialog::onOK (void)
{
    Globals::set_global("ED_GAME_PATH", _game_path->text().toUtf8().data(), Globals::PERSISTENT);
    Globals::set_global("ED_RESOLUTION_ENABLED", _resolution_enabled->checkState() ? "1" : "0", Globals::PERSISTENT);
    Globals::set_global("ED_RESOLUTION_WIDTH", _resolution_width->text().toUtf8().data(), Globals::PERSISTENT);
    Globals::set_global("ED_RESOLUTION_HEIGHT", _resolution_height->text().toUtf8().data(), Globals::PERSISTENT);
    
    done(QDialog::Accepted);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelLaunchDialog.cpp"


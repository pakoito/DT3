//==============================================================================
///	
///	File: EdLevelMeshBuilder.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelMeshBuilder.hpp"

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

EdLevelMeshBuilder::EdLevelMeshBuilder(void)
    :   QDialog             (NULL)
{
    setWindowTitle("Mesh Builder");
    
    //
    // Top of window
    //

    QGridLayout *layout_top = new QGridLayout;
    layout_top->setContentsMargins(5,5,5,5);
    
    layout_top->setHorizontalSpacing(5);
    layout_top->setVerticalSpacing(5);
    
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

EdLevelMeshBuilder::~EdLevelMeshBuilder (void)
{

}

//==============================================================================
//==============================================================================

void EdLevelMeshBuilder::onCancel (void)
{
    done(QDialog::Rejected);
}

void EdLevelMeshBuilder::onOK (void)
{
    done(QDialog::Accepted);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelMeshBuilder.cpp"


//==============================================================================
///	
///	File: EdLevelLibraryDialog.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelLibraryDialog.hpp"

// Qt include
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>

// Engine includes


//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelLibraryDialog::EdLevelLibraryDialog(QWidget *parent, std::string &name_text, std::string &description_text)
    :   QDialog             (parent),
        _name_text          (name_text),
        _description_text   (description_text)
{
    // Script window
    QLabel *name_label = new QLabel("Name", parent);
    _name = new EdLevelLineEdit(parent);
    
    QLabel *description_label = new QLabel("Description", parent);
    _description = new QTextEdit(parent);
    
    
    QPushButton *save = new QPushButton("Save", parent);
    connect(save, SIGNAL(clicked()),
            this, SLOT(onSave()));
    
    QPushButton *cancel = new QPushButton("Cancel", parent);
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(onCancel()));

    QGridLayout *layout1 = new QGridLayout;
    layout1->setContentsMargins(5,5,5,5);
    
    layout1->setHorizontalSpacing(5);
    layout1->setVerticalSpacing(5);
    
    layout1->addWidget(name_label, 0, 0);
    layout1->addWidget(_name, 0, 1);
    
    layout1->addWidget(description_label, 1, 0);
    layout1->addWidget(_description, 1, 1);
        
    QGridLayout *layout2 = new QGridLayout;
    layout2->setContentsMargins(5,5,5,5);
    
    layout2->setHorizontalSpacing(5);
    layout2->setVerticalSpacing(5);

    layout2->addWidget(save, 2, 0);
    layout2->addWidget(cancel, 2, 1);
    
    layout1->addLayout(layout2, 2, 1);

    setLayout(layout1);
}

EdLevelLibraryDialog::~EdLevelLibraryDialog (void)
{

}

//==============================================================================
//==============================================================================

void EdLevelLibraryDialog::onSave (void)
{
    _name_text = _name->text().toUtf8().data();
    _description_text = _description->toPlainText().toUtf8().data();

    done(QDialog::Accepted);
}

void EdLevelLibraryDialog::onCancel (void)
{
    done(QDialog::Rejected);
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelLibraryDialog.cpp"


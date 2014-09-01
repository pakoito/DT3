//==============================================================================
///	
///	File: EdLevelResourceImage.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelResourceImage.hpp"
#include "EdLevelImageThumbnail.hpp"

// Qt include
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>

// Engine includes

//==============================================================================
//==============================================================================

EdLevelResourceImage::EdLevelResourceImage (const FilePath &path)
{
    
    EdLevelImageThumbnail *item = new EdLevelImageThumbnail(this);
    item->setImage(path);
    item->setMaximumWidth(128);
    item->setMinimumWidth(128);
    item->setMaximumHeight(128);
    item->setMinimumHeight(128);

    QLabel *title = new QLabel(this);
    title->setMaximumWidth(200);
    title->setMinimumWidth(200);
    title->setText(path.file_name().c_str());
    
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(2,2,2,2);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->setColumnStretch(1,1);
    layout->addWidget(item,1,0);
    layout->addWidget(title,0,0);

    setLayout(layout);
    
    setMinimumHeight(15+2+128);
    setMinimumWidth(200);
    setMaximumWidth(200);
}

EdLevelResourceImage::~EdLevelResourceImage	(void)
{

}

//==============================================================================
//==============================================================================

#include "moc_EdLevelResourceImage.cpp"


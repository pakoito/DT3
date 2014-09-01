//==============================================================================
///	
///	File: EdLevelSoundThumbnail.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelSoundThumbnail.hpp"
#include "EdLevelMainWindow.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes
#include "DT3Core/Resources/ResourceTypes/SoundResource.hpp"

//==============================================================================
//==============================================================================

EdLevelSoundThumbnail::EdLevelSoundThumbnail(QWidget *parent)
{
    _buffer = NULL;
    
    connect(    this,       SIGNAL(pressed()),
                this,       SLOT(doPressed())    );

}

EdLevelSoundThumbnail::~EdLevelSoundThumbnail(void)
{
    delete[] _buffer;
}

//==============================================================================
//==============================================================================

void EdLevelSoundThumbnail::setSound (const FilePath &path)
{
    _path = path;
}

//==============================================================================
//==============================================================================

void EdLevelSoundThumbnail::cache (void)
{
    std::shared_ptr<SoundResource> sound = SoundResource::import_resource(_path);
   
    DTuint _width = 128, _height = 128;
    DTuint half_y = _height/2;

    delete[] _buffer;
    _buffer = new Buffer[_width*_height];

    ::memset(_buffer,128,sizeof(Buffer) * _width*_height);

    DTsize num_samples = sound->num_samples();
    for (DTuint x = 0; x < _width; ++x) {
        
        DTuint s = static_cast<DTfloat>(x) / static_cast<DTfloat>(_width);
        DTsize num = num_samples / _width;
        
        DTfloat min_sample = 0.0F;
        DTfloat max_sample = 0.0F;
    
        sound->sample_window(s, num, min_sample, max_sample);
        
        if (min_sample < -1.0)  min_sample = -1.0F;
        if (max_sample > 1.0F)  max_sample = 1.0F;
        
        DTint min_ypos = min_sample * half_y + half_y;
        DTint max_ypos = max_sample * half_y + half_y;
        
        for (DTint y = min_ypos; y <= max_ypos; ++y) {
            Buffer &b = _buffer[y*_width + x];
            b.r = b.g = b.b = 0;
        }
    }

    _image = QImage( (DTubyte*) _buffer,_width,_height, QImage::Format_RGB32);
}

//==============================================================================
//==============================================================================

void EdLevelSoundThumbnail::paintEvent (QPaintEvent *event)
{
    if (_image.isNull())
        cache();

	QPainter painter(this);
    
    // Clear to black
    painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(QColor(0,0,0,255)));
    painter.drawRect(rect());
    
    // Draw Image
    DTint x=0,y=0;
    
    if (_image.width() < 128)   x = (128-_image.width())/2;
    if (_image.height() < 128)   y = (128-_image.height())/2;
    
    
	painter.drawImage(_image.rect().translated(x,y), _image);

}

//==============================================================================
//==============================================================================

void EdLevelSoundThumbnail::doPressed (void)
{
    QSound::play(_path.full_path().c_str());
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelSoundThumbnail.cpp"


//==============================================================================
///	
///	File: EdLevelImageThumbnail.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelImageThumbnail.hpp"
#include "EdLevelMainWindow.hpp"
#include "EdLevelScriptNodeStandard.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

// Engine includes
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"

//==============================================================================
//==============================================================================

EdLevelImageThumbnail::EdLevelImageThumbnail(QWidget *parent)
    :   QWidget (parent)
{
    _buffer = NULL;
}

EdLevelImageThumbnail::~EdLevelImageThumbnail()
{
    delete[] _buffer;
}

//==============================================================================
//==============================================================================

void EdLevelImageThumbnail::setImage (const FilePath &path)
{
    _path = path;
}

//==============================================================================
//==============================================================================

void EdLevelImageThumbnail::cache (void)
{
    std::shared_ptr<TextureResource2D> texture = TextureResource2D::import_resource(_path);
    if (texture->buffer() == NULL) {
        _image = QImage( (DTubyte*) NULL, 0U, 0U, QImage::Format_ARGB32);
        return;
    }
    
    DTuint _width = 128, _height = 128;
    
    DTuint center_x = _width/2;
    DTuint center_y = _height/2;
    
    DTfloat x_scale = static_cast<DTfloat>(_width) / static_cast<DTfloat>(texture->width());
    DTfloat y_scale = static_cast<DTfloat>(_height) / static_cast<DTfloat>(texture->height());
    DTfloat scale = std::min(1.0F, std::min(x_scale, y_scale));
    
    DTint thumb_width = static_cast<DTint>(texture->width() * scale);
    DTint thumb_height = static_cast<DTint>(texture->height() * scale);
    
    delete[] _buffer;
    _buffer = new Buffer[_width*_height];

    ::memset(_buffer,0,sizeof(Buffer) * _width*_height);
    
    // Resample image for thumbnail
    for (DTint y = center_y - thumb_height/2, yi = 0; y <= (center_y + thumb_height/2); ++y, yi += texture->height() / thumb_height) {
        for (DTint x = center_x - thumb_width/2, xi = 0; x <= (center_x + thumb_width/2); ++x, xi += texture->width() / thumb_width) {
            
            if (x < 0 || x >= _width) continue;
            if (y < 0 || y >= _height) continue;
            
            if (xi < 0 || xi >= texture->width()) continue;
            if (yi < 0 || yi >= texture->height()) continue;
            
            Color4b c = texture->pixel(xi,yi);
            Buffer &b = _buffer[_width * (_height-y-1) + x];
            
            b.r = c.r;
            b.g = c.g;
            b.b = c.b;
            b.a = c.a;
        }
    }

    _image = QImage( (DTubyte*) _buffer,_width,_height, QImage::Format_ARGB32);
}

//==============================================================================
//==============================================================================

void EdLevelImageThumbnail::paintEvent (QPaintEvent *event)
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

#include "moc_EdLevelImageThumbnail.cpp"


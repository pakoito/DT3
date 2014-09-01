#ifndef EDLEVELSOUNDTHUMBNAIL
#define EDLEVELSOUNDTHUMBNAIL
//==============================================================================
///	
///	File: EdLevelSoundThumbnail.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QPushButton>
#include <QtGui/QBrush>
#include <QtMultimedia/QSound>

// Engine includes
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelSoundThumbnail : public QPushButton
{
    Q_OBJECT

	public:
                            EdLevelSoundThumbnail		(QWidget *parent);
                            ~EdLevelSoundThumbnail		(void);

	public:
        void                setSound            (const FilePath &path);
		void                paintEvent          (QPaintEvent *event);

    public slots:
        void                doPressed           (void);

	private:
        void                cache               (void);

        struct Buffer {
            DTubyte b,g,r,a;
        } *_buffer;

		QImage              _image;
        FilePath            _path;
};

//==============================================================================
//==============================================================================

#endif

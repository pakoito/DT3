#ifndef EDLEVELIMAGETHUMBNAIL
#define EDLEVELIMAGETHUMBNAIL
//==============================================================================
///	
///	File: EdLevelImageThumbnail.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QWidget>
#include <QtGui/QBrush>

// Engine includes
#include "DT3Core/Resources/Importers/ImporterImage.hpp"
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

class EdLevelImageThumbnail : public QWidget
{
    Q_OBJECT

	public:
                            EdLevelImageThumbnail   (QWidget *parent);
                            ~EdLevelImageThumbnail  (void);

	public:
        void                setImage                (const FilePath &path);
		void                paintEvent              (QPaintEvent *event);

	private:
        void                cache                   (void);

        struct Buffer {
            DTubyte b,g,r,a;
        } *_buffer;
        
		QImage              _image;
        FilePath            _path;
};

//==============================================================================
//==============================================================================

#endif

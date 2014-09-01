#ifndef EDLEVELRESOURCEIMAGE
#define EDLEVELRESOURCEIMAGE
//==============================================================================
///	
///	File: EdLevelResourceImage.hpp
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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>

// Engine includes
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

class EdLevelResourceImage : public QWidget
{
    Q_OBJECT

	public:
										EdLevelResourceImage          (const FilePath &path);
										~EdLevelResourceImage         (void);
                                        
    private:
        FilePath                        _path;

};

//==============================================================================
//==============================================================================

#endif

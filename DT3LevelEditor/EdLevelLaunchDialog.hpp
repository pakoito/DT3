#ifndef EDLEVELLAUNCHDIALOG
#define EDLEVELLAUNCHDIALOG
//==============================================================================
///	
///	File: EdLevelLaunchDialog.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include

// Qt include
#include <QtWidgets/QDialog>

// Engine includes


//==============================================================================
/// Forward declarations
//==============================================================================

class QLineEdit;
class QCheckBox;

//==============================================================================
/// Class
//==============================================================================

class EdLevelLaunchDialog: public QDialog
{
    Q_OBJECT

	public:
										EdLevelLaunchDialog		(void);
                                        ~EdLevelLaunchDialog    (void);

    public slots:
        void                            onCancel                (void);
        void                            onOK                    (void);
    
    private:
        QLineEdit                       *_game_path;
    
        QCheckBox                       *_resolution_enabled;
        QLineEdit                       *_resolution_width;
        QLineEdit                       *_resolution_height;
};

//==============================================================================
//==============================================================================

#endif

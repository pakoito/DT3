#ifndef EDLEVELMESHBUILDER
#define EDLEVELMESHBUILDER
//==============================================================================
///	
///	File: EdLevelMeshBuilder.hpp
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

class EdLevelMeshBuilder: public QDialog
{
    Q_OBJECT

	public:
										EdLevelMeshBuilder		(void);
                                        ~EdLevelMeshBuilder     (void);

    public slots:
        void                            onCancel                (void);
        void                            onOK                    (void);
    
    private:
};

//==============================================================================
//==============================================================================

#endif

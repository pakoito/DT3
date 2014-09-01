#ifndef EDLEVELLIBRARYDIALOG
#define EDLEVELLIBRARYDIALOG
//==============================================================================
///	
///	File: EdLevelLibraryDialog.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>

// Engine includes

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelLibraryDialog: public QDialog
{
    Q_OBJECT

	public:
										EdLevelLibraryDialog		(QWidget *parent, std::string &name_text, std::string &description_text);
                                        ~EdLevelLibraryDialog       (void);

    public slots:
        void                            onSave                      (void);
        void                            onCancel                    (void);
        
    private:
        EdLevelLineEdit                 *_name;
        QTextEdit                       *_description;
    
        std::string                     &_name_text;
        std::string                     &_description_text;
};

//==============================================================================
//==============================================================================

#endif

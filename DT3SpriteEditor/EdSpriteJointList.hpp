#ifndef EDSPRITEJOINTLIST
#define EDSPRITEJOINTLIST
//==============================================================================
///	
///	File: EdSpriteJointList.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QAction>
#include <QtCore/QRegExp>

#include "EdSpriteLineEdit.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"

using namespace DT2;

//==============================================================================
/// Forward declarations
//==============================================================================

class EdSpriteMainWindow;

//==============================================================================
/// Class
//==============================================================================

class EdSpriteJointList : public QListWidget
{
    Q_OBJECT

	public:
										EdSpriteJointList		(QWidget *parent, QToolBar *toolbar, EdSpriteMainWindow *main_window);

    public slots:   
        void                            onSpriteChanged         (void);
        void                            onSelectionChanged      (void);
   
        void                            onItemChanged           (QListWidgetItem *item);
        void                            onItemSelectionChanged  (void);

        void                            dropEvent               (QDropEvent *event);

        void                            onOrderByName           (void);
        void                            onOrderByOrder          (void);

        void                            onShowFilter            (const QString & text);

	private:
        static int                      compare                 (KeyedSpriteAnimationPoseJoint *a, KeyedSpriteAnimationPoseJoint *b);
        void                            refreshList             (void);

        QToolBar                        *_toolbar;
        EdSpriteMainWindow              *_main_window;

		QAction                         *_order_by_name;
		QAction                         *_order_by_order;
        EdSpriteLineEdit                *_filter;
    
        enum {
            ORDER_BY_NAME,
            ORDER_BY_ORDER
        } _order;

        QRegExp                         _reg_exp_filter;

};

//==============================================================================
//==============================================================================

#endif

#ifndef EDLEVELSOUNDBUS
#define EDLEVELSOUNDBUS
//==============================================================================
///	
///	File: EdLevelSoundBus.hpp
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
#include <QtWidgets/QToolBar>

// Engine includes
#include "DT3Core/Scripting/ScriptingSoundBusIn.hpp"
#include "DT3Core/Scripting/ScriptingSoundBusOut.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

class EdLevelDocument;
class EdLevelLineEdit;
class EdLevelSoundVUMeter;
class QDial;
class QListWidget;
class QListWidgetItem;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelSoundBus : public QWidget
{
    Q_OBJECT

	public:
                                    EdLevelSoundBus         (QWidget *parent, std::shared_ptr<ScriptingSoundBusOut> bus);

		void						draw                    (QPainter *painter);
		void						paintEvent              (QPaintEvent *event);
        void                        resizeEvent             (QResizeEvent *event);

        void                        updateBus               (void);
        void                        updateProperties        (void);
        void                        updateList              (void);

    private:
    
        std::shared_ptr<ScriptingSoundBusIn>    _bus_in;
        std::shared_ptr<ScriptingSoundBusOut>   _bus_out;

        EdLevelLineEdit                         *_name;
        
        QDial                                   *_gain_left;
        QDial                                   *_gain_right;
        
        EdLevelLineEdit                         *_gain_left_num;
        EdLevelLineEdit                         *_gain_right_num;
        
        QListWidget                             *_list;
        EdLevelSoundVUMeter                     *_vu_meter;
        
    private slots:
        void                        nameChanged             ();
        void                        gainKnobsChanged        ();
        void                        listChanged             (QListWidgetItem * item);

    signals:
        void                        doCommand               (QString command);
        void                        doUndoBlock             (void);

};

//==============================================================================
//==============================================================================

#endif

#ifndef EDLEVELSOUNDWINDOW
#define EDLEVELSOUNDWINDOW
//==============================================================================
///	
///	File: EdLevelSoundWindow.hpp
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
#include <QtWidgets/QDial>
#include <QtCore/QBasicTimer>

// Engine includes

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class ScriptingSoundBusOut;
    class WorldNode;
    class PlugNode;
}

class EdLevelDocument;
class EdLevelSoundBus;
class QHBoxLayout;
class QScrollArea;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelSoundWindow : public QWidget
{
    Q_OBJECT

	public:
                                    EdLevelSoundWindow      (QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);

        void                        resizeEvent             (QResizeEvent *event);
        void                        timerEvent              (QTimerEvent *event);

    private:
        EdLevelDocument             *_document;
        QToolBar                    *_toolbar;
        
        // Internal representation of scene
        struct BusCache {
            BusCache(void)                                          {   _node = NULL; _widget = NULL;   }
            BusCache(std::shared_ptr<ScriptingSoundBusOut> node)    {   _node = node; _widget = NULL;   }
            
            bool   operator == (const BusCache& rhs) const          {	return _node == rhs._node;		}

            std::shared_ptr<ScriptingSoundBusOut>   _node;
            EdLevelSoundBus                         *_widget;
        };
                
        std::list<BusCache>                         _bus_cache;
        
        QHBoxLayout                 *_layout;
        QWidget                     *_scroll_widget;
        QScrollArea                 *_scroll_area;

        QBasicTimer                 _timer;

    public slots:
        void                        onAddNode               (WorldNode *node);
        void                        onRemoveNode            (WorldNode *node);
        
        void                        onRefreshSound          (void);
        void                        onRefreshProperties     (void);
        void                        onSelectionChanged      (const std::list<std::shared_ptr<PlugNode>> &selection_list);
        
        void                        onCommand               (QString command);

    signals:
        void                        doCommand               (QString command);

};

//==============================================================================
//==============================================================================

#endif

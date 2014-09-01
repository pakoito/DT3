#ifndef EDLEVELWORLDWINDOW
#define EDLEVELWORLDWINDOW
//==============================================================================
///	
///	File: EdLevelWorldWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelToolWindow.hpp"

// Qt include
#include <QtWidgets/QToolBar>
#include <QtWidgets/QComboBox>

// Engine includes
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlugNode;
    class CameraObject;
    class MaterialResource;
    class ShaderResource;
}

class EdLevelDocument;
class EdLevelTool;
class EdLevelToolEvent;
class QGridLayout;
class QCheckBox;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelWorldWindow : public EdLevelToolWindow
{
    Q_OBJECT

	public:
										EdLevelWorldWindow	(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);
        
	protected:
		void							draw				(QPainter *painter);

		void							initializeGL		(void);
		void							paintGL				(void);
		void							resizeGL			(int w, int h);

		void							mousePressEvent		(QMouseEvent *event);
		void							mouseMoveEvent		(QMouseEvent *event);
		void							mouseReleaseEvent	(QMouseEvent *event);

        void                            keyPressEvent       (QKeyEvent *event);

		//void							resizeEvent			(QResizeEvent *event);
		QSize							sizeHint			(void) const;
        
	private:
        void                            refreshCameras      (void);
        void                            updateBuiltInCameras(void);

        void                            toolContextMenu     (QMouseEvent *event);

        void                            drawScene           (const std::shared_ptr<CameraObject> &camera, DTfloat scale);
        void                            drawGrid            (const std::shared_ptr<CameraObject> &camera);
            
        DTfloat                         calcScale           (const std::shared_ptr<CameraObject> &camera);
        void                            pickGL              (QPointF pos, EdLevelToolEvent &tool_event);
    
        DTboolean                       getGridVisible      (void);
        DTfloat                         getGrid             (void);
    
        DTint                           _desired_width;
        DTint                           _desired_height;

        EdLevelDocument                 *_document;
        QToolBar                        *_toolbar;
        std::shared_ptr<CameraObject>   _camera;
    
        DTint                           _builtin_camera_index;
        DTint                           _custom_camera_index;
        
        std::shared_ptr<CameraObject>   _built_in_cameras[6];
        DTfloat                         _built_in_zoom;
        DTboolean                       _built_in_camera;
        
		QAction                         *_arrow_action;
		QAction                         *_pan_action;
		QAction                         *_rotate_action;
		QAction                         *_scale_action;
        QComboBox                       *_camera_selection;
        QComboBox                       *_resolution_selection;
    
        QComboBox                       *_grid_selection;
        QCheckBox                       *_grid_visible;
        
        std::shared_ptr<MaterialResource>   _grid_material;

        std::shared_ptr<ShaderResource>     _shader;
    
        QPointF                         _last_position;
        
        std::shared_ptr<EdLevelTool>    _tool;
    
        DrawBatcher                     _b;

        
        enum {
            MODE_NONE,
            MODE_ARROW,
            MODE_PAN,
            MODE_ROTATE,
            MODE_SCALE        
        } _mode;
        
    public slots:
        void                            onChangeCamera              (int index);        
        void                            onChangeResolution          (int index);        

        void                            onRefreshWorld              (void);
        void                            onSelectionChanged          (const std::list<std::shared_ptr<PlugNode>> &selection_list);
        
        void                            onArrowTool                 (void);
        void                            onPanTool                   (void);
        void                            onRotateTool                (void);
        void                            onScaleTool                 (void);

        void                            onSelectComponent           (void);
    
        void                            onChangeGrid                (int state);

    signals:
        void                            doSelectionChanged          (const std::list<std::shared_ptr<PlugNode>> &selection_list);
        void                            doUndoBlock                 (void);
};

//==============================================================================
//==============================================================================

#endif

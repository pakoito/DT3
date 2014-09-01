//==============================================================================
///	
///	File: EdLevelWorldWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelWorldWindow.hpp"
#include "EdLevelDocument.hpp"
#include "EdLevelToolEvent.hpp"
#include "EdLevelTool.hpp"
#include "EdLevelManipPan.hpp"
#include "EdLevelManipRotate.hpp"
#include "EdLevelManipScale.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtCore/QFile>

// Engine includes
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Graphics/DrawUtils.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include <iostream>

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelWorldWindow::EdLevelWorldWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   EdLevelToolWindow       (parent,document),
        _camera                 (NULL),
        _tool                   (NULL),
        _built_in_zoom          (100.0F),
        _builtin_camera_index   (-1),
        _custom_camera_index    (-1)
{
    
    setContextMenuPolicy(Qt::PreventContextMenu);
    setFocusPolicy(Qt::StrongFocus);
    
    setMinimumWidth(100);
    setMinimumHeight(100);

	_document = document;    
    _toolbar = toolbar;
    
    //
    // Actions and toolbar
    //

    _arrow_action = new QAction(tr("Arrow"), this);
    _arrow_action->setIcon(QIcon(":/images/arrow.png"));
    //_arrow_action->setShortcut(QKeySequence::New);
    _arrow_action->setStatusTip(tr("Manipulate Objects"));
    connect(_arrow_action, SIGNAL(triggered()), this, SLOT(onArrowTool()));

    _pan_action = new QAction(tr("Pan"), this);
    _pan_action->setIcon(QIcon(":/images/pan.png"));
    //_pan_action->setShortcut(QKeySequence::New);
    _pan_action->setStatusTip(tr("Pan Objects"));
    connect(_pan_action, SIGNAL(triggered()), this, SLOT(onPanTool()));

    _rotate_action = new QAction(tr("Rotate"), this);
    _rotate_action->setIcon(QIcon(":/images/rotate.png"));
    //_rotate_action->setShortcut(QKeySequence::New);
    _rotate_action->setStatusTip(tr("Rotate Objects"));
    connect(_rotate_action, SIGNAL(triggered()), this, SLOT(onRotateTool()));

    _scale_action = new QAction(tr("Scale"), this);
    _scale_action->setIcon(QIcon(":/images/scale.png"));
    //_scale_action->setShortcut(QKeySequence::New);
    _scale_action->setStatusTip(tr("Scale Objects"));
    connect(_scale_action, SIGNAL(triggered()), this, SLOT(onScaleTool()));


    toolbar->addAction(_arrow_action);
    toolbar->addAction(_pan_action);
    toolbar->addAction(_rotate_action);
    toolbar->addAction(_scale_action);
    
    QWidget *spacer1 = new QWidget(toolbar);
    spacer1->setMinimumWidth(10);
    toolbar->addWidget(spacer1);
    
    QLabel *camera_selection_label = new QLabel("Camera:",toolbar);
    camera_selection_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    toolbar->addWidget(camera_selection_label);

    _camera_selection = new QComboBox(toolbar);
    _camera_selection->setMinimumWidth(150);

	connect(	_camera_selection,      SIGNAL(activated(int)),
				this,                   SLOT(onChangeCamera(int))	);

    toolbar->addWidget(_camera_selection);
    
    
    QWidget *spacer2 = new QWidget(toolbar);
    spacer2->setMinimumWidth(10);
    toolbar->addWidget(spacer2);

    QLabel *resolution_selection_label = new QLabel("Resolution:",toolbar);
    resolution_selection_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    toolbar->addWidget(resolution_selection_label);

    _resolution_selection = new QComboBox(toolbar);
    _resolution_selection->setMinimumWidth(150);

    
    _resolution_selection->addItem("Full Resolution");
    _resolution_selection->addItem("1024x768 (iPad Landscape)");
    _resolution_selection->addItem("768x1024 (iPad Portrait)");
    _resolution_selection->addItem("960x640 (iPhone 4 Landscape)");
    _resolution_selection->addItem("640x960 (iPhone 4 Portrait)");
    _resolution_selection->addItem("480x320 (HVGA, iPhone 3GS Landscape)");
    _resolution_selection->addItem("320x480 (HVGA, iPhone 3GS Portrait)");
    _resolution_selection->addItem("2048x1536 (iPad3 Landscape)");
    _resolution_selection->addItem("1536x2048 (iPad3 Portrait)");
    _resolution_selection->insertSeparator(_resolution_selection->count());
    _resolution_selection->addItem("2560x1600 (Landscape)");
    _resolution_selection->addItem("1600x2560 (Portrait)");
    _resolution_selection->addItem("2560x1536 (Landscape)");
    _resolution_selection->addItem("1536x2560 (Portrait)");
    _resolution_selection->addItem("1920x1200 (Landscape)");
    _resolution_selection->addItem("1200x1920 (Portrait)");
    _resolution_selection->addItem("1920x1152 (Landscape)");
    _resolution_selection->addItem("1152x1920 (Portrait)");
    _resolution_selection->addItem("1536x1152 (Landscape)");
    _resolution_selection->addItem("1152x1536 (Portrait)");
    _resolution_selection->addItem("1280x800 (Xoom, Galaxy Tab 10.1, WXGA Landscape)");
    _resolution_selection->addItem("800x1280 (Xoom, Galaxy Tab 10.1, WXGA Portrait)");
    _resolution_selection->addItem("1280x768 (Landscape)");
    _resolution_selection->addItem("768x1280 (Portrait)");
    _resolution_selection->addItem("1024x580 (Kindle Landscape)");
    _resolution_selection->addItem("580x1024 (Kindle Portrait)");
    _resolution_selection->addItem("1024x600 (Galaxy Tab)");
    _resolution_selection->addItem("600x1024 (Galaxy Tab)");
    _resolution_selection->addItem("854x480 (WVGA854 Landscape)");
    _resolution_selection->addItem("480x854 (WVGA854 Portrait)");
    _resolution_selection->addItem("800x480 (Galaxy S II, WVGA800 Landscape)");
    _resolution_selection->addItem("480x800 (Galaxy S II, WVGA800 Portrait)");
    _resolution_selection->addItem("540x460 (Landscape)");
    _resolution_selection->addItem("460x540 (Portrait)");
    _resolution_selection->addItem("432x240 (FWQVGA Landscape)");
    _resolution_selection->addItem("240x432 (FWQVGA Portrait)");
    _resolution_selection->addItem("400x240 (WQVGA Landscape)");
    _resolution_selection->addItem("240x400 (WQVGA Portrait)");
    _resolution_selection->addItem("320x240 (QVGA Landscape)");
    _resolution_selection->addItem("240x320 (QVGA Portrait)");
    _resolution_selection->insertSeparator(_resolution_selection->count());
    _resolution_selection->addItem("1280x720 (HD720p)");
    _resolution_selection->addItem("1920x1080 (HD1080p)");

	connect(	_resolution_selection,  SIGNAL(activated(int)),
				this,                   SLOT(onChangeResolution(int))	);

    toolbar->addWidget(_resolution_selection);


    //
    // Grid selection
    //
    
    QWidget *spacer3 = new QWidget(toolbar);
    spacer3->setMinimumWidth(10);
    toolbar->addWidget(spacer3);

    QLabel *grid_selection_label = new QLabel("Grid:",toolbar);
    grid_selection_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    toolbar->addWidget(grid_selection_label);

    _grid_visible = new QCheckBox("",toolbar);
    _grid_visible->setCheckState(Qt::Checked);
    toolbar->addWidget(_grid_visible);
    
	connect(	_grid_visible,  SIGNAL(stateChanged(int)),
				this,           SLOT(onChangeGrid(int))	);


    _grid_selection = new QComboBox(toolbar);
    _grid_selection->setMinimumWidth(75);

    _grid_selection->setEditable(true);
    
    _grid_selection->addItem("None");
    _grid_selection->addItem("0.1");
    _grid_selection->addItem("0.2");
    _grid_selection->addItem("0.5");
    _grid_selection->addItem("1.0");
    _grid_selection->addItem("1.2");
    _grid_selection->addItem("1.5");
    _grid_selection->addItem("2.0");
    _grid_selection->addItem("5.0");
    _grid_selection->addItem("10.0");
    _grid_selection->addItem("20.0");
    _grid_selection->addItem("100.0");

	connect(	_grid_selection,  SIGNAL(activated(int)),
				this,               SLOT(onChangeGrid(int))	);

    toolbar->addWidget(_grid_selection);
    
    // 
    // Set up window
    //
    
    _shader = ShaderResource::import_resource(FilePath("{ED_TOOL_SHADER}"));

    _grid_material = MaterialResource::create();
    _grid_material->set_blend_enable(false);
    _grid_material->set_depth_enable(true);

    
    // Initialize builtin cameras
    for (DTuint i = 0; i < ARRAY_SIZE(_built_in_cameras); ++i) {
        _built_in_cameras[i] = CameraObject::create();
    }
    
    // Front
    _built_in_cameras[0]->set_orientation( Matrix3( 1.0F,   0.0F,   0.0F,
                                                    0.0F,   1.0F,   0.0F,
                                                    0.0F,   0.0F,   1.0F    )   );
                                                    
    // Back
    _built_in_cameras[1]->set_orientation( Matrix3( -1.0F,  0.0F,   0.0F,
                                                    0.0F,   1.0F,   0.0F,
                                                    0.0F,   0.0F,   -1.0F    )   );

    // Top
    _built_in_cameras[2]->set_orientation( Matrix3( 1.0F,   0.0F,   0.0F,
                                                    0.0F,   0.0F,   1.0F,
                                                    0.0F,   -1.0F,  0.0F    )   );
    // Bottom
    _built_in_cameras[3]->set_orientation( Matrix3( 1.0F,   0.0F,   0.0F,
                                                    0.0F,   0.0F,   -1.0F,
                                                    0.0F,   1.0F,   0.0F    )   );
    // Left
    _built_in_cameras[4]->set_orientation( Matrix3( 0.0F,   0.0F,   -1.0F,
                                                    0.0F,   1.0F,   0.0F,
                                                    1.0F,   0.0F,   0.0F    )   );
    // Right
    _built_in_cameras[5]->set_orientation( Matrix3( 0.0F,   0.0F,   1.0F,
                                                    0.0F,   1.0F,   0.0F,
                                                    -1.0F,   0.0F,  0.0F    )   );
                                                    
    setAutoBufferSwap(false);
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::onChangeCamera(int index)
{
    CameraObject *current_camera = (CameraObject *) _camera_selection->itemData(index).value<void*>();
    _camera = checked_static_cast<CameraObject>(current_camera->shared_from_this());
    
    // Treat builtin cameras specially
    _built_in_camera =  (_camera == _built_in_cameras[0]) |
                        (_camera == _built_in_cameras[1]) |
                        (_camera == _built_in_cameras[2]) |
                        (_camera == _built_in_cameras[3]) |
                        (_camera == _built_in_cameras[4]) |
                        (_camera == _built_in_cameras[5]);
    
    if (_built_in_camera)   _builtin_camera_index = index;
    else                    _custom_camera_index = index;

    update();
}

void EdLevelWorldWindow::onChangeResolution(int index)
{

    QString item_text = _resolution_selection->itemText(index);
    QStringList res = item_text.split(" ");
    QStringList res_elems = res.front().split("x");
    
    if (res_elems.size() != 2) {
        _desired_width = 0;
        _desired_height = 0;
    } else {
        _desired_width = res_elems.at(0).toInt();
        _desired_height = res_elems.at(1).toInt();
    }

    updateGeometry();
    update();
}

void EdLevelWorldWindow::onChangeGrid (int state)
{
    update();
}

QSize EdLevelWorldWindow::sizeHint (void) const
{
    return QSize(_desired_width,_desired_height);
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::refreshCameras()
{
    std::shared_ptr<World> world = _document->world();
    const std::list<std::shared_ptr<WorldNode>>& objects = world->nodes();
    
    _camera_selection->blockSignals(true);
    
    // Remember currently selected camera
    int current_camera_index = _camera_selection->currentIndex();
    int count_cameras = 0;
    
    CameraObject *current_camera = NULL;
    if (current_camera_index >= 0)
        current_camera = (CameraObject *) _camera_selection->itemData(current_camera_index).value<void*>();
            
    _camera_selection->clear();

    // Add the new cameras to the list
    FOR_EACH (i,objects) {
        if ( (**i).isa(CameraObject::kind()) ) {
            _camera_selection->addItem( (**i).name().c_str(), qVariantFromValue( (void*) i->get() ) );
            ++count_cameras;
        }
    }
    
    // Add built in cameras
    if (count_cameras > 0)
        _camera_selection->insertSeparator(_resolution_selection->count());
        
    _camera_selection->addItem("Front",  qVariantFromValue( (void*) _built_in_cameras[0].get()) );
    _camera_selection->addItem("Back",  qVariantFromValue( (void*) _built_in_cameras[1].get()) );
    _camera_selection->addItem("Top",  qVariantFromValue( (void*) _built_in_cameras[2].get()) );
    _camera_selection->addItem("Bottom",  qVariantFromValue( (void*) _built_in_cameras[3].get()) );
    _camera_selection->addItem("Left",  qVariantFromValue( (void*) _built_in_cameras[4].get()) );
    _camera_selection->addItem("Right",  qVariantFromValue( (void*) _built_in_cameras[5].get()) );

    // Find the selected camera and select it again
    current_camera_index = _camera_selection->findData( qVariantFromValue( (void*) current_camera) );
    if (current_camera_index >= 0) {
        _camera_selection->setCurrentIndex(current_camera_index);
    } else if (_camera_selection->count() > 0) {
        _camera_selection->setCurrentIndex(0);
        current_camera_index = 0;
    } else {
        _camera_selection->setCurrentIndex(-2); // Just so there will be a change later on
    }
 
    _camera_selection->blockSignals(false);
            
    onChangeCamera(current_camera_index);
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::initializeGL(void)
{
	System::renderer()->open_display(width(), height());
    
    ::glEnable(GL_MULTISAMPLE);
    GLint bufs;
    GLint samples;
    ::glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    ::glGetIntegerv(GL_SAMPLES, &samples);
    qDebug("Have %d buffers and %d samples", bufs, samples);
}

//==============================================================================
//==============================================================================

DTfloat EdLevelWorldWindow::calcScale(const std::shared_ptr<CameraObject> &camera)
{
    DTfloat scale = 1.0F;
                        
    // Calculate a manipulator scale
    if (_tool) {
    
        if (camera->is_perspective()) {
            DTfloat dist = Vector3::dot(-camera->orientation().z_axis(), (_tool->getManipulatorTransform().translation() - camera->translation()));
            
            DTfloat one_unit_angle = std::atan(1.0F/dist);
            DTfloat camera_angle = camera->angle() * PI / 180.0F;
            
            scale = camera_angle/2.0F * 0.3F / one_unit_angle; // Approx %15 angular view
        } else {
            scale = 0.25F * _built_in_zoom;
        }
    }

    return scale;
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::drawScene(const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    ::glPushName(0);
    _document->world()->draw(0.0F);
    ::glPopName();

    if (_tool) {
        ::glPushName(0);
        _tool->draw(this, _camera, scale);
        ::glPopName();
    }
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::updateBuiltInCameras(void)
{

    if (_built_in_camera) {
        DTfloat aspect = static_cast<DTfloat>(width()) / static_cast<DTfloat>(height());
    
        // Change positions of builtin cameras
        for (DTuint i = 0; i < ARRAY_SIZE(_built_in_cameras); ++i) {
            _built_in_cameras[i]->set_ortho(-_built_in_zoom*aspect, _built_in_zoom*aspect, -_built_in_zoom, _built_in_zoom, -10000.0F, 10000.0F);
        }
    }

}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::paintGL(void)
{
	makeCurrent();

    updateBuiltInCameras();

    //refreshCameras();
    
	
	System::renderer()->change_display (width(), height());

    ::glClearColor( 0.15F, 0.15F, 0.15F, 0.0F );
    ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    if (!_camera) {
        swapBuffers();
        //doneCurrent();
        return;
    }

    // Save aspect ratio
    DTfloat save_aspect = _camera->aspect_ratio_mul();
    _camera->set_aspect_ratio_mul( static_cast<DTfloat>(width()) / static_cast<DTfloat>(height()) / System::renderer()->screen_aspect() );
        
    // Activate the camera
    //DrawUtils::activate_camera(_camera);
        
    // Replace the camera temporarily in the world
    std::shared_ptr<CameraObject> save_camera = _document->world()->camera();
    _document->world()->set_camera(_camera);
   
    // Draw the objects
    drawScene(_camera, calcScale(_camera));
    
    // Draw the selections
    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection();
    
    FOR_EACH (i,selection) {
        std::shared_ptr<PlaceableObject> placeable = checked_cast<PlaceableObject>(*i);
        if (!placeable) {
            continue;
        }
        
        DrawUtils::draw_selection (_b, _camera, _grid_material, _shader, placeable->transform(), Color4b::green, placeable->radius());
    }

    if (getGridVisible()) {
        drawGrid (_camera);
    }
    
    _document->world()->set_camera(save_camera);
    
    // Set aspect ratio back to what it was
    _camera->set_aspect_ratio_mul(save_aspect);
    
    swapBuffers();
	//doneCurrent();
}

void EdLevelWorldWindow::pickGL(QPointF pos, EdLevelToolEvent &tool_event)
{
	makeCurrent();

    updateBuiltInCameras();
	
	System::renderer()->change_display (width(), height());
    
    if (!_camera)
        return;
        
    
    //
    // 1. Specify the array to be used for the returned hit records
    //
    
    GLuint buffer[1024];
    ::glSelectBuffer(ARRAY_SIZE(buffer), buffer);
    
    //
    // 2. Enter Selection mode
    //
    
    ::glRenderMode(GL_SELECT);
    
    //
    // 3. Initialize the name stack
    //
    
    ::glInitNames();
    
    //
    // 4. Specify the viewing volume
    //
    
    // Save aspect ratio
    DTfloat save_aspect = _camera->aspect_ratio_mul();
    _camera->set_aspect_ratio_mul( static_cast<DTfloat>(width()) / static_cast<DTfloat>(height()) / System::renderer()->screen_aspect() );
        
    // Activate the camera
    DTint viewport[4] = { 0, 0, width(), height() };
    _camera->set_picking(pos.x(), height() - pos.y(), 5.0F, 5.0F, viewport);
    
    //
    // 5. Draw the scene
    //
        
    //DrawUtils::activate_camera(_camera);

    // Replace the camera temporarily in the world
    std::shared_ptr<CameraObject> save_camera = _document->world()->camera();
    _document->world()->set_camera(_camera);
   
    DTfloat scale = calcScale(_camera);
    drawScene(_camera, scale);
    
    _document->world()->set_camera(save_camera);
    
	
    //
    // 6. Exit selection mode
    //
    
    _camera->end_picking();
    _camera->calculate_frustum();    // Force refresh of internal matrices


    GLint hits = ::glRenderMode(GL_RENDER);
    
    //
    // 7. Build Hit event
    //
    
    // Build a ray used for mouse interactions 
    DTfloat x = static_cast<DTfloat>(pos.x()) / static_cast<DTfloat>(width()) * 2.0F - 1.0F;                  // -1.0 to 1.0
    DTfloat y = static_cast<DTfloat>(height() - pos.y()) / static_cast<DTfloat>(height()) * 2.0F - 1.0F;  // -1.0 to 1.0
    
    tool_event._camera = _camera;
    tool_event._ray_src = _camera->unproject_point( Vector3(x,y,-1.0F) );
    tool_event._ray_dest = _camera->unproject_point( Vector3(x,y,1.0F) );
    tool_event._control_id = 0;
    
    std::cout << x << " " << y << std::endl;
    std::cout << "src:  " << tool_event._ray_src.x << " " << tool_event._ray_src.y << " " << tool_event._ray_src.z << std::endl;
    std::cout << "dest: " << tool_event._ray_dest.x << " " << tool_event._ray_dest.y << " " << tool_event._ray_dest.z << std::endl;

    //
    // 8. Report hits
    //

    GLuint *ptr = buffer;
    
    DTfloat selection_z = std::numeric_limits<DTfloat>::infinity();
    std::shared_ptr<WorldNode> selection;
    DTboolean using_tool = false;
    
    for (int i = 0; i < hits; ++i) {
        GLuint num_ids = *ptr;  ++ptr;
        DTfloat zmin = (DTfloat) ((DTdouble) (*ptr) / 0x7FFFFFFF);	++ptr;
        DTfloat zmax = (DTfloat) ((DTdouble) (*ptr) / 0x7FFFFFFF);	++ptr;
        
        LOG_MESSAGE << "zmin :" << zmin << "  " << "zmax: " << zmax;
        
        // Check for tool selection
        if (num_ids == 2) {
            /*GLuint id1 = *ptr;*/ ++ptr;   
            GLuint id2 = *ptr; ++ptr;
            
            tool_event._control_id = id2;
            using_tool = true;
            
        // Check for object selection
        } else if (num_ids == 1) {
            GLuint id1 = *ptr; ++ptr;
            
            if (tool_event._event_type == EdLevelToolEvent::MOUSE_DOWN && ((zmin + zmax) * 0.5F) < selection_z) {
                // Change the selection
                selection = _document->world()->node_by_id(id1);
            }
            
        }
        
    }
    
    //
    // 9. Handle event
    //
    
    if (tool_event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
    
        if ( (using_tool && _tool) ) {
            _tool->doEvent(this, tool_event);
        } else if (selection) {
            std::list<std::shared_ptr<PlugNode>> selection_list;
            selection_list.push_back(selection);
            emit doSelectionChanged(selection_list);
        } 
        
    } else {
    
        if (_tool) {
            _tool->doEvent(this, tool_event);
        }
            
    }
    
    
    
    // Set aspect ratio back to what it was
    _camera->set_aspect_ratio_mul(save_aspect);
    _camera->calculate_frustum();    // Force refresh of internal matrices

	doneCurrent();
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::resizeGL( int w, int h )
{	
    ::glViewport(0, 0, w, h);
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::onSelectComponent (void)
{
    QAction *action = qobject_cast<QAction*>(sender());

    LOG_MESSAGE << "Creating tool: " << action->data().toString().toUtf8().data();

    std::shared_ptr<EdLevelTool> tool = checked_static_cast<EdLevelTool>(Factory::create_object(action->data().toString().toUtf8().data()));
    
    _tool = tool;
    
    if (_tool)
        _tool->setSelection(_document->selection());

    update();
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::toolContextMenu(QMouseEvent *event)
{
    // Add Components
    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection();
    if (selection.size() != 1)
        return;
        
    std::shared_ptr<PlugNode> front = selection.front();
    DTboolean is_object_base = front->isa(ObjectBase::kind());
    DTboolean is_placeable = front->isa(PlaceableObject::kind());
    

    // Context Menu
    QMenu *context_menu = new QMenu();
                
    // Pan    
    QAction *pan_action = new QAction(this);
    pan_action->setVisible(true);
    pan_action->setText("Pan");
    pan_action->setData(QString("EdLevelManipPan"));
    pan_action->setEnabled(is_placeable);
    
    connect(pan_action,     SIGNAL(triggered()),
            this,           SLOT(onSelectComponent()));
                                
    context_menu->addAction(pan_action);

    // Rotate    
    QAction *rotate_action = new QAction(this);
    rotate_action->setVisible(true);
    rotate_action->setText("Rotate");
    rotate_action->setData(QString("EdLevelManipRotate"));
    rotate_action->setEnabled(is_placeable);

    connect(rotate_action,  SIGNAL(triggered()),
            this,           SLOT(onSelectComponent()));
                                
    context_menu->addAction(rotate_action);

    // Scale    
    QAction *scale_action = new QAction(this);
    scale_action->setVisible(true);
    scale_action->setText("Scale");
    scale_action->setData(QString("EdLevelManipScale"));
    scale_action->setEnabled(is_placeable);

    connect(scale_action,   SIGNAL(triggered()),
            this,           SLOT(onSelectComponent()));
                                
    context_menu->addAction(scale_action);
    

    // Separator
    context_menu->addSeparator();

    // Base Class
    DTcharacter *class_id = front->class_id_child();
    std::shared_ptr<EdLevelTool> tool = checked_cast<EdLevelTool>(Factory::create_tool(class_id));
    
    if (tool) {    
        QAction *component = new QAction(this);
        component->setVisible(true);
        component->setText(front->name().c_str());
        component->setVisible(true);
        component->setData(QString(tool->class_id_child()));

        connect(component,  SIGNAL(triggered()),
                this,       SLOT(onSelectComponent()));
                                    
        context_menu->addAction(component);
    }

    if (is_object_base) {
        std::shared_ptr<ObjectBase> base = checked_static_cast<ObjectBase>(front);
    
        // Components
        std::list<std::shared_ptr<ComponentBase>> components = base->all_components();
                                    
        // Sub component menu items
        FOR_EACH (i,components) {
        
            DTcharacter *class_id = (*i)->class_id_child();
            std::shared_ptr<EdLevelTool> tool = checked_cast<EdLevelTool>(Factory::create_tool(class_id));
                    
            if (tool) {    
                QAction *component = new QAction(this);
                component->setVisible(true);
                component->setText((*i)->name().c_str());
                component->setVisible(true);
                component->setData(QString(tool->class_id_child()));

                connect(component,  SIGNAL(triggered()),
                        this,       SLOT(onSelectComponent()));
                                            
                context_menu->addAction(component);
            }
            
        }
    }
        
    context_menu->exec(event->globalPos());
}

//==============================================================================
//==============================================================================

DTboolean EdLevelWorldWindow::getGridVisible (void)
{
    return (_grid_visible->checkState() == Qt::Checked);
}

DTfloat EdLevelWorldWindow::getGrid (void)
{
    bool ok;
    float grid = _grid_selection->currentText().toFloat(&ok);
    
    if (!ok)    return 0.0F;
    else        return grid;
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::mousePressEvent(QMouseEvent *event)
{
    emit doUndoBlock();

    update();
        
	if (event->buttons() == Qt::RightButton && !(event->modifiers() & Qt::ALT)) {
    
        toolContextMenu(event);
        
    } else {
        _last_position = event->pos();
        
        if (!(event->modifiers() & Qt::ALT)) {
            // Do picking
            EdLevelToolEvent tool_event;
            tool_event._event_type = EdLevelToolEvent::MOUSE_DOWN;
            tool_event._mouse_x = event->pos().x();
            tool_event._mouse_y = event->pos().y();
            tool_event._window_width = width();
            tool_event._window_height = height();
            tool_event._grid = getGrid();
            tool_event._modifiers = (event->modifiers() & Qt::ALT ? EdLevelToolEvent::MODIFIER_OPTION_ALT : 0) |
                                    (event->modifiers() & Qt::CTRL ? EdLevelToolEvent::MODIFIER_CONTROL : 0) |
                                    (event->modifiers() & Qt::SHIFT ? EdLevelToolEvent::MODIFIER_SHIFT : 0) |
                                    (event->modifiers() & Qt::META ? EdLevelToolEvent::MODIFIER_META : 0);
                        
            pickGL(event->pos(), tool_event);
        }
        update();
    }
}

void EdLevelWorldWindow::mouseMoveEvent(QMouseEvent *event)
{    
    QPointF delta = event->pos() - _last_position;
    _last_position = event->pos();
    
    DTboolean pan = (event->buttons() == Qt::MidButton) || ( (event->buttons() == Qt::LeftButton) && (event->modifiers() & Qt::ALT) && (event->modifiers() & Qt::SHIFT) );
    DTboolean rot = (event->buttons() == Qt::RightButton) && (event->modifiers() & Qt::ALT) && !_built_in_camera;
    DTboolean zoom = (event->buttons() == Qt::LeftButton) && (event->modifiers() & Qt::ALT);
    
    if (_camera && (pan || rot || zoom) ) {
        Vector3 position = _camera->translation();
        Matrix3 orientation = _camera->orientation();

        if (pan) {
            position = position - orientation.y_axis() * delta.y() + orientation.x_axis() * delta.x();
            
        } else if (rot) {
        
            Matrix3 roty, rotx;
            roty = Matrix3::set_rotation_y(delta.x() * 0.01F);
            rotx = Matrix3::set_rotation_x(delta.y() * 0.01F);
            
            orientation = (roty * orientation * rotx).orthoed();
            
        } else if (zoom) {
        
            if (_built_in_camera) {
                _built_in_zoom += delta.y();
                
                if (_built_in_zoom < 1.0F)          _built_in_zoom = 1.0F;
                else if (_built_in_zoom > 10000.0F)  _built_in_zoom = 10000.0F;
                
            } else {

                Matrix3 roty;
                roty = Matrix3::set_rotation_y(delta.x() * 0.01F);
                
                Vector3 straight_z(orientation.z_axis());
                straight_z.y = 0.0F;
                straight_z.normalize();

                orientation = (roty * orientation).orthoed();
                position = position + straight_z * delta.y();                
            }
        } 
        
        
        if (_built_in_camera) {
        
            // Change positions of builtin cameras
            for (DTuint i = 0; i < ARRAY_SIZE(_built_in_cameras); ++i) {
            
                // Set the transform of the camera
                Matrix4 transform = _built_in_cameras[i]->transform();
                transform.set_translation(position);
                _built_in_cameras[i]->set_transform(transform);
                
            }
            
            update();

        } else {
            // Set the transform of the camera
            Matrix4 transform = Matrix4::identity();
            transform.set_translation(position);
            transform.set_orientation(orientation);

            TextBufferStream stream;
            stream << transform;
            onCommand(QString("SetTransform ") + _camera->full_name().c_str() + " (" + stream.buffer().c_str() + ")");
        }
        
    } else {
        // Do picking
        EdLevelToolEvent tool_event;
        tool_event._event_type = EdLevelToolEvent::MOUSE_DRAG;
        tool_event._mouse_x = event->pos().x();
        tool_event._mouse_y = event->pos().y();
        tool_event._window_width = width();
        tool_event._window_height = height();
        tool_event._grid = getGrid();
        tool_event._modifiers = (event->modifiers() & Qt::ALT ? EdLevelToolEvent::MODIFIER_OPTION_ALT : 0) |
                                (event->modifiers() & Qt::CTRL ? EdLevelToolEvent::MODIFIER_CONTROL : 0) |
                                (event->modifiers() & Qt::SHIFT ? EdLevelToolEvent::MODIFIER_SHIFT : 0) |
                                (event->modifiers() & Qt::META ? EdLevelToolEvent::MODIFIER_META : 0);

        pickGL(event->pos(), tool_event);
        update();
    }
    
}

void EdLevelWorldWindow::mouseReleaseEvent	(QMouseEvent *event)
{
    // Do picking
    EdLevelToolEvent tool_event;
    tool_event._event_type = EdLevelToolEvent::MOUSE_UP;
    tool_event._mouse_x = event->pos().x();
    tool_event._mouse_y = event->pos().y();
    tool_event._window_width = width();
    tool_event._window_height = height();
    tool_event._grid = getGrid();
    tool_event._modifiers = (event->modifiers() & Qt::ALT ? EdLevelToolEvent::MODIFIER_OPTION_ALT : 0) |
                            (event->modifiers() & Qt::CTRL ? EdLevelToolEvent::MODIFIER_CONTROL : 0) |
                            (event->modifiers() & Qt::SHIFT ? EdLevelToolEvent::MODIFIER_SHIFT : 0) |
                            (event->modifiers() & Qt::META ? EdLevelToolEvent::MODIFIER_META : 0);

    pickGL(event->pos(), tool_event);
    update();
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::keyPressEvent (QKeyEvent *event)
{
    int key = event->key();
    
    DTboolean is_builtin_tool = _tool && (_tool->isa(EdLevelManipPan::kind()) || _tool->isa(EdLevelManipRotate::kind()) || _tool->isa(EdLevelManipScale::kind()));

    if (key == Qt::Key_Q) {
        onArrowTool();
        event->accept();
    } else if (key == Qt::Key_W) {
        onPanTool();
        event->accept();
    } else if (key == Qt::Key_E) {
        onRotateTool();
        event->accept();
    } else if (key == Qt::Key_R) {
        onScaleTool();
        event->accept();
    } else if ( (event->matches(QKeySequence::Delete) || key == 0x1000003) && (!_tool || is_builtin_tool)) {
        
        std::list<std::shared_ptr<PlugNode>> selection = _document->selection();
        FOR_EACH (i,selection) {
        
            std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(*i);
            if (node) {
                std::string nodename = node->name();

                std::string cmd = "Remove \"" + nodename + "\"";
                emit doCommand(cmd.c_str());
            }
        }
    
    } else if (key == Qt::Key_Space) {
    
        // Swap cameras
        if (_camera_selection->currentIndex() == _builtin_camera_index) {
        
            if (    _custom_camera_index >= 0 &&
                    _custom_camera_index < _camera_selection->count() &&
                    _camera_selection->itemData(_custom_camera_index).value<void*>()) {
                _camera_selection->setCurrentIndex(_custom_camera_index);
                onChangeCamera(_custom_camera_index);
            }
            
        } else {
        
            if (    _builtin_camera_index >= 0 &&
                    _builtin_camera_index < _camera_selection->count() &&
                    _camera_selection->itemData(_builtin_camera_index).value<void*>()) {
                _camera_selection->setCurrentIndex(_builtin_camera_index);
                onChangeCamera(_builtin_camera_index);
            }
            
        }
        
        event->accept();
    } else {
        EdLevelToolEvent tool_event;
        tool_event._event_type = EdLevelToolEvent::KEY_DOWN;
        tool_event._key_code = key;
        tool_event._grid = getGrid();
        tool_event._camera = _camera;
        tool_event._modifiers = (event->modifiers() & Qt::ALT ? EdLevelToolEvent::MODIFIER_OPTION_ALT : 0) |
                                (event->modifiers() & Qt::CTRL ? EdLevelToolEvent::MODIFIER_CONTROL : 0) |
                                (event->modifiers() & Qt::SHIFT ? EdLevelToolEvent::MODIFIER_SHIFT : 0) |
                                (event->modifiers() & Qt::META ? EdLevelToolEvent::MODIFIER_META : 0);

        if (_tool) {
            _tool->doEvent(this, tool_event);
        }
        update();

    }

}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::drawGrid (const std::shared_ptr<CameraObject> &camera)
{    
    DTfloat grid = getGrid();
    if (grid <= 0.0F)
        grid = 1.0F;
    
    const DTint SIZE = 100;
    
    _b.batch_begin(camera, _grid_material, _shader, Matrix4::identity(), DT3GL_PRIM_LINES, DrawBatcher::FMT_V | DrawBatcher::FMT_C);
    
    // Minor lines
    for (DTint p = -SIZE; p <= SIZE; ++p) {
        if (p == 0) continue;

        if ( (p%10) != 0) {
            _b.add().v(p * grid, 0.0F, -SIZE * grid).c(Color4b::grey25);
            _b.add().v(p * grid, 0.0F, +SIZE * grid).c(Color4b::grey25);
            _b.add().v(-SIZE * grid, 0.0F, p * grid).c(Color4b::grey25);
            _b.add().v(+SIZE * grid, 0.0F, p * grid).c(Color4b::grey25);
        }
        
        _b.batch_split();
    }
    
    // Major lines
    for (DTint p = -SIZE; p <= SIZE; ++p) {
        if (p == 0) continue;

        if ( (p%10) == 0) {
            _b.add().v(p * grid, 0.0F, -SIZE * grid).c(Color4b::grey50);
            _b.add().v(p * grid, 0.0F, +SIZE * grid).c(Color4b::grey50);
            _b.add().v(-SIZE * grid, 0.0F, p * grid).c(Color4b::grey50);
            _b.add().v(+SIZE * grid, 0.0F, p * grid).c(Color4b::grey50);
        }
        
        _b.batch_split();
    }
    
    

    _b.add().v(-SIZE * grid, 0.0F, 0.0F).c(Color4b::red);
    _b.add().v(SIZE * grid, 0.0F, 0.0F).c(Color4b::red);

    _b.batch_split();

    _b.add().v(0.0F, 0.0F, -SIZE * grid).c(Color4b::blue);
    _b.add().v(0.0F, 0.0F, SIZE * grid).c(Color4b::blue);

    _b.batch_split();
    
    _b.batch_end();
    _b.flush();
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::onArrowTool (void)
{   
    _tool.reset();
    
    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection();

    // Check that all objects are the same type
    if (selection.size() > 0) {
        
        DTcharacter *class_id = selection.front()->class_id_child();
    
        FOR_EACH (i,selection) {
            
            if ( (**i).class_id_child() != class_id )
                return; // Different so no further action needed
        }
    
        _tool = checked_cast<EdLevelTool>(Factory::create_tool(class_id));
        if (_tool)
            _tool->setSelection(selection);
    }
    
    update();
}

void EdLevelWorldWindow::onPanTool (void)
{   
    _tool = EdLevelManipPan::create();

    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection();
    _tool->setSelection(selection);

    update();
}

void EdLevelWorldWindow::onRotateTool (void)
{
    _tool = EdLevelManipRotate::create();

    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection();
    _tool->setSelection(selection);

    update();
}

void EdLevelWorldWindow::onScaleTool (void)
{
    _tool = EdLevelManipScale::create();

    const std::list<std::shared_ptr<PlugNode>>& selection = _document->selection();
    _tool->setSelection(selection);

    update();
}

//==============================================================================
//==============================================================================

void EdLevelWorldWindow::onRefreshWorld (void)
{
    refreshCameras();
    update();
}

void EdLevelWorldWindow::onSelectionChanged (const std::list<std::shared_ptr<PlugNode>> &selection_list)
{
    if (_tool) {
        _tool->setSelection(selection_list);
    }

    update();
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelWorldWindow.cpp"


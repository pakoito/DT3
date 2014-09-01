//==============================================================================
///	
///	File: EdSpriteEditor.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteEditor.hpp"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include "System.hpp"
#include "DeviceGraphics.hpp"
#include "ConsoleStream.hpp"
#include "DrawBatcher.hpp"
#include "DrawBatcherQuads.hpp"
#include "ShaderResource.hpp"
#include "CameraObject.hpp"

using namespace DT2;

//==============================================================================
//==============================================================================

const float EdSpriteEditor::HANDLE_CLICK_SIZE = 7.0F;
const float EdSpriteEditor::HANDLE_ROTATE_RADIUS = 50.0F;

//==============================================================================
//==============================================================================

EdSpriteEditor::EdSpriteEditor(EdSpriteMainWindow *main_window, QGLWidget *share)
    :   QGLWidget               (main_window, share),
        _zoom                   (1.0F),
        _translate              (0.0F,0.0F),
        _current_handle_joint    (NULL)
{
    _main_window = main_window;
    
    _bounds_material.setBlending(false);
    _bounds_material.setDepthTest(false);
    _bounds_material.setCulling(false);
    _bounds_material.setColor(Color(0.3F,0.3F,0.3F,1.0F));
	_bounds_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _bounds_material.setRecacheParameters(true);

    _outline_material.setBlending(false);
    _outline_material.setDepthTest(false);
    _outline_material.setCulling(false);
    _outline_material.setColor(Color(0.5F,0.5F,0.5F,1.0F));
	_outline_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _outline_material.setRecacheParameters(true);

    _selected_material.setBlending(false);
    _selected_material.setDepthTest(false);
    _selected_material.setCulling(false);
    _selected_material.setColor(Color(0.0F,1.0F,0.0F,1.0F));
	_selected_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _selected_material.setRecacheParameters(true);

    _handle_material.setBlending(false);
    _handle_material.setDepthTest(false);
    _handle_material.setCulling(false);
    _handle_material.setColor(Color(1.0F,0.0F,0.0F,1.0F));
	_handle_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _handle_material.setRecacheParameters(true);

    setAutoBufferSwap(false);
}

//==============================================================================
//==============================================================================

void EdSpriteEditor::updateTransforms (void)
{
    // Update viewport transform
    DTfloat width = System::getRenderer()->getScreenWidth();
    DTfloat height = System::getRenderer()->getScreenHeight();
                                    
    Matrix4 obj_to_proj = _camera.getProjection() * _camera.getModelview();
    Matrix4 proj_to_vp = Matrix4(   width/2.0F,     0.0F,           0.0F,       width/2.0F,
                                    0.0F,           -height/2.0F,   0.0F,       height/2.0F,
                                    0.0F,           0.0F,           0.0F,       0.0F,
                                    0.0F,           0.0F,           0.0F,       1.0F        );
    Matrix4 obj_to_viewport = proj_to_vp * obj_to_proj;
                                                                               
    _object_to_viewport_transform = Matrix3(    obj_to_viewport._m11,   obj_to_viewport._m12,   obj_to_viewport._m14,
                                                obj_to_viewport._m21,   obj_to_viewport._m22,   obj_to_viewport._m24,
                                                0.0F,                   0.0F,                   1.0F                    );

    // Object transforms
    _main_window->getSprite()->updateTransforms();
    _main_window->getSprite()->updateOrder();
}

Vector2 EdSpriteEditor::getHandleObjectPosition (const KeyedSpriteAnimationPoseJoint *joint, DTint handle)
{
    switch (handle) {
        case HANDLE_PLUS_X: {
            return joint->getJointToObjectTransform() * Vector2(joint->getPlusX(), (joint->getPlusY() - joint->getMinusY()) * 0.5F);
        } break;

        case HANDLE_MINUS_X: {
            return joint->getJointToObjectTransform() * Vector2(-joint->getMinusX(), (joint->getPlusY() - joint->getMinusY()) * 0.5F);
        } break;

        case HANDLE_PLUS_Y: {
            return joint->getJointToObjectTransform() * Vector2( (joint->getPlusX() - joint->getMinusX()) * 0.5F, joint->getPlusY());
        } break;

        case HANDLE_MINUS_Y: {
            return joint->getJointToObjectTransform() * Vector2( (joint->getPlusX() - joint->getMinusX()) * 0.5F, -joint->getMinusY());
        } break;

        case HANDLE_ROTATE: {
            return joint->getJointToObjectTransform() * Vector2(0.0F, 0.0F);
        } break;

        case HANDLE_POSITION: {
            return joint->getJointToObjectTransform() * Vector2(0.0F, 0.0F);
        } break;

        default: {
            DTint i = handle - HANDLE_GRID;
        
            const Array<Vector2>& grid = joint->gridPoints();
            Vector2 v = grid[i];
            
            Vector2 p0 = joint->getJointToObjectTransform() * Vector2(-joint->getMinusX(), -joint->getMinusY());
            Vector2 p1 = joint->getJointToObjectTransform() * Vector2(joint->getPlusX(), -joint->getMinusY());
            Vector2 p2 = joint->getJointToObjectTransform() * Vector2(joint->getPlusX(), joint->getPlusY());
            Vector2 p3 = joint->getJointToObjectTransform() * Vector2(-joint->getMinusX(), joint->getPlusY());

            Vector2 p1p0 = p1-p0;
            Vector2 p2p3 = p2-p3;

            return (p0 + p1p0*v.x) * v.y + (p3 + p2p3*v.x) * (1.0F - v.y);
        } break;
    };

    return Vector2(0.0F,0.0F);
}

DTint EdSpriteEditor::getClickedHandle (const Matrix3 &object_to_viewport_transform, const KeyedSpriteAnimationPoseJoint *joint, int x, int y)
{
    Vector2 handle_pos;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_PLUS_X);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_PLUS_X;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_MINUS_X);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_MINUS_X;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_PLUS_Y);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_PLUS_Y;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_MINUS_Y);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_MINUS_Y;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_POSITION);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_POSITION;
    
    const Array<Vector2>& grid = joint->gridPoints();
    for (DTint i = 0; i < grid.size(); ++i) {
        handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_GRID + i);
        if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    
            return HANDLE_GRID + i;
    }


    if (isClickedPart (object_to_viewport_transform, joint, x, y))
        return HANDLE_ROTATE;

    return HANDLE_NONE;
}

bool EdSpriteEditor::isClickedPart (const Matrix3 &object_to_viewport_transform, const KeyedSpriteAnimationPoseJoint *joint, int x, int y)
{
    Vector2 position = joint->getJointToObjectTransform().inversed() * _object_to_viewport_transform.inversed() * Vector2(x,y);

    float plusx = joint->getPlusX();
    float minusx = joint->getMinusX();
    float plusy = joint->getPlusY();
    float minusy = joint->getMinusY();
    
    float min_x = std::min(-minusx, plusx);
    float max_x = std::max(-minusx, plusx);

    float min_y = std::min(-minusy, plusy);
    float max_y = std::max(-minusy, plusy);

    if (position.x >= min_x && position.x <= max_x && position.y >= min_y && position.y <= max_y)
        return true;

    return false;
}

int EdSpriteEditor::getClickedPart (const Matrix3 &object_to_viewport_transform, int x, int y)
{
    Array<KeyedSpriteAnimationPoseJoint*> &joints = _main_window->getSprite()->getJoints();

    for (DTuint i = 0; i < joints.size(); ++i) {
        if (isClickedPart(object_to_viewport_transform, joints[i], x,y))
            return i;
    }

    return -1;
}

//==============================================================================
//==============================================================================

void EdSpriteEditor::drawCube (const Vector3 &position)
{
    DTfloat size = _zoom * 0.01F;

    DrawBatcherQuads b;
    b.batchBegin(&_handle_material, Matrix4::identity(), DrawBatcher::FMT_V);
    
    b.vertex( position + Vector3(-size,size,-size) );
    b.vertex( position + Vector3(size,size,-size) );
    b.vertex( position + Vector3(size,size,size) );
    b.vertex( position + Vector3(-size,size,size) );

    b.vertex( position + Vector3(-size,-size,-size) );
    b.vertex( position + Vector3(size,-size,-size) );
    b.vertex( position + Vector3(size,-size,size) );
    b.vertex( position + Vector3(-size,-size,size) );
    
    b.vertex( position + Vector3(-size,size,-size) );
    b.vertex( position + Vector3(-size,-size,-size) );
    b.vertex( position + Vector3(-size,-size,size) );
    b.vertex( position + Vector3(-size,size,size) );
    
    b.vertex( position + Vector3(size,size,-size) );
    b.vertex( position + Vector3(size,-size,-size) );
    b.vertex( position + Vector3(size,-size,size) );
    b.vertex( position + Vector3(size,size,size) );
    
    b.vertex( position + Vector3(-size,size,size) );
    b.vertex( position + Vector3(-size,-size,size) );
    b.vertex( position + Vector3(size,-size,size) );
    b.vertex( position + Vector3(size,size,size) );

    b.vertex( position + Vector3(-size,size,-size) );
    b.vertex( position + Vector3(-size,-size,-size) );
    b.vertex( position + Vector3(size,-size,-size) );
    b.vertex( position + Vector3(size,size,-size) );
    
    b.batchEnd();
}


void EdSpriteEditor::drawPart(const Matrix3 &object_to_viewport_transform, KeyedSpriteAnimationPoseJoint *joint, bool selected)
{
    
    float plusx = joint->getPlusX();
    float minusx = joint->getMinusX();
    float plusy = joint->getPlusY();
    float minusy = joint->getMinusY();

    Vector2 p0 = joint->getJointToObjectTransform() * Vector2(-minusx, -minusy);
    Vector2 p1 = joint->getJointToObjectTransform() * Vector2(plusx, -minusy);
    Vector2 p2 = joint->getJointToObjectTransform() * Vector2(plusx, plusy);
    Vector2 p3 = joint->getJointToObjectTransform() * Vector2(-minusx, plusy);
    Vector2 center = getHandleObjectPosition(joint, HANDLE_ROTATE);
    
    //
    // Draw textured joint
    //
    
    if (joint->getVisible()) {
    
        MaterialResource *material = _main_window->getSprite()->getMaterialProperty();

        DrawBatcherQuads b;
        b.batchBegin (material, Matrix4::identity(), DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1);

        joint->draw(b);
        
        b.batchEnd();
        b.flush();
        
    }

    
    //
    // Draw outlined joint
    //
    
    if (!selected) {
    
        DrawBatcher b;
        b.batchBegin (&_outline_material, Matrix4::identity(), DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);
        b.vertex(Vector3(p0.x,p0.y,0.0F));
        b.vertex(Vector3(p1.x,p1.y,0.0F));
        b.vertex(Vector3(p2.x,p2.y,0.0F));
        b.vertex(Vector3(p3.x,p3.y,0.0F));
        b.batchEnd();
        b.flush();
    
        b.batchBegin (&_outline_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);
        b.vertex(Vector3(center.x-0.01F,center.y-0.01F,0.0F));
        b.vertex(Vector3(center.x+0.01F,center.y+0.01F,0.0F));
        b.vertex(Vector3(center.x-0.01F,center.y+0.01F,0.0F));
        b.vertex(Vector3(center.x+0.01F,center.y-0.01F,0.0F));
        b.batchEnd();
        b.flush();
    
            
    } else {
    
        DrawBatcher b;
        b.batchBegin (&_selected_material, Matrix4::identity(), DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);
        b.vertex(Vector3(p0.x,p0.y,0.0F));
        b.vertex(Vector3(p1.x,p1.y,0.0F));
        b.vertex(Vector3(p2.x,p2.y,0.0F));
        b.vertex(Vector3(p3.x,p3.y,0.0F));
        b.batchEnd();
        b.flush();

        b.batchBegin (&_selected_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);
        b.vertex(Vector3(center.x-0.01F,center.y-0.01F,0.0F));
        b.vertex(Vector3(center.x+0.01F,center.y+0.01F,0.0F));
        b.vertex(Vector3(center.x-0.01F,center.y+0.01F,0.0F));
        b.vertex(Vector3(center.x+0.01F,center.y-0.01F,0.0F));
        b.batchEnd();
        b.flush();

        // Draw pivot
        drawCube(Vector3(center.x, center.y, 0.0F));

        // Draw selection handles
        center = getHandleObjectPosition(joint, HANDLE_PLUS_X);
        drawCube(Vector3(center.x, center.y, 0.0F));

        center = getHandleObjectPosition(joint, HANDLE_MINUS_X);
        drawCube(Vector3(center.x, center.y, 0.0F));

        center = getHandleObjectPosition(joint, HANDLE_PLUS_Y);
        drawCube(Vector3(center.x, center.y, 0.0F));

        center = getHandleObjectPosition(joint, HANDLE_MINUS_Y);
        drawCube(Vector3(center.x, center.y, 0.0F));

        center = getHandleObjectPosition(joint, HANDLE_POSITION);
        drawCube(Vector3(center.x, center.y, 0.0F));

        const Array<Vector2>& grid = joint->gridPoints();
        for (DTint i = 0; i < grid.size(); ++i) {
        
            center = getHandleObjectPosition(joint, HANDLE_GRID+i);
            drawCube(Vector3(center.x, center.y, 0.0F));
        }

    }

}

void EdSpriteEditor::initializeGL (void)
{
	System::getRenderer()->openDisplay(width(), height());
    
    //::glEnable(GL_MULTISAMPLE);
    GLint bufs;
    GLint samples;
    ::glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    ::glGetIntegerv(GL_SAMPLES, &samples);
    qDebug("Have %d buffers and %d samples", bufs, samples);
}

void EdSpriteEditor::paintGL (void)
{
    updateTransforms();
    
	makeCurrent();
	
	System::getRenderer()->changeDisplay (width(), height());

    ::glClearColor( 0.2F, 0.2F, 0.2F, 0.0F );
    ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    DTfloat aspect = System::getRenderer()->getScreenAspect();
    
    _camera.setOrtho(   -_zoom*aspect - _translate.x,   _zoom*aspect - _translate.x,
                        -_zoom - _translate.y,          _zoom - _translate.y,
                        -1.0F,1.0F);
    System::getRenderer()->activateCamera(&_camera);
    
    //
    // Draw bounds
    //
    
    DrawBatcher b;
    b.batchBegin (&_bounds_material, Matrix4::identity(), DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);
    b.vertex ( Vector3(-0.5F,-0.5F,0.0F) );
    b.vertex ( Vector3(0.5F,-0.5F,0.0F) );
    b.vertex ( Vector3(0.5F,0.5F,0.0F) );
    b.vertex ( Vector3(-0.5F,0.5F,0.0F) );
    b.batchEnd();
	b.flush();

    b.batchBegin (&_bounds_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);
    b.vertex ( Vector3(0.0F,-100.0F,0.0F) );
    b.vertex ( Vector3(0.0F,100.0F,0.0F) );
    b.vertex ( Vector3(-100.0F,0.0F,0.0F) );
    b.vertex ( Vector3(100.0F,0.0F,0.0F) );
    b.batchEnd();
	b.flush();
    
    //
    // Draw Sprite
    //
    
    // Draw unselected parts
    Array<KeyedSpriteAnimationPoseJoint*> &joints = _main_window->getSprite()->getJoints();

    for (DTuint i = 0; i < joints.size(); ++i) {
        if (_main_window->isPartSelected(joints[i]))
            continue;

        drawPart(_object_to_viewport_transform, joints[i], false );

        KeyedSpriteAnimationPoseJoint *parent = _main_window->getSprite()->getParentOfJoint(joints[i]);
        if (parent) {
            Vector2 p0 = joints[i]->getJointToObjectTransform() * Vector2(0.0F, 0.0F);
            Vector2 p1 = parent->getJointToObjectTransform() * Vector2(0.0F, 0.0F);
            
            b.batchBegin (&_bounds_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);
            b.vertex ( Vector3(p0.x,p0.y,0.0F) );
            b.vertex ( Vector3(p1.x,p1.y,0.0F) );
            b.batchEnd();
            b.flush();            
        }
    }

    // Draw selected parts
    for (DTuint i = 0; i < joints.size(); ++i) {
        if (!_main_window->isPartSelected(joints[i]))
            continue;
    
        drawPart(_object_to_viewport_transform, joints[i], true );

        KeyedSpriteAnimationPoseJoint *parent = _main_window->getSprite()->getParentOfJoint(joints[i]);
        if (parent) {
            Vector2 p0 = joints[i]->getJointToObjectTransform() * Vector2(0.0F, 0.0F);
            Vector2 p1 = parent->getJointToObjectTransform() * Vector2(0.0F, 0.0F);
            
            b.batchBegin (&_bounds_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);
            b.vertex ( Vector3(p0.x,p0.y,0.0F) );
            b.vertex ( Vector3(p1.x,p1.y,0.0F) );
            b.batchEnd();
            b.flush();            
        }
    }
    
    
    
	swapBuffers();
	
	doneCurrent();
}

void EdSpriteEditor::resizeGL (int w, int h)
{
    ::glViewport(0, 0, w, h);
}

//==============================================================================
//==============================================================================

void EdSpriteEditor::mousePressEvent (QMouseEvent *event)
{
    _main_window->pushUndo();

    updateTransforms();

    _last_mouse_x = event->x();
    _last_mouse_y = event->y();
    
    
    Vector2 position = _object_to_viewport_transform.inversed() * Vector2(_last_mouse_x,_last_mouse_y);
    LOG_MESSAGE << position.x << "  " << position.y;


    Array<KeyedSpriteAnimationPoseJoint*> &joints = _main_window->getSprite()->getJoints();

    // Check if handle clicked
    for (int i = 0; i < joints.size(); ++i) {

        if (_main_window->isPartSelected(joints[i])) {
            _current_handle = getClickedHandle( _object_to_viewport_transform, joints[i], event->x(), event->y());

            if (_current_handle != HANDLE_NONE) {
                _current_handle_joint = joints[i];
                _main_window->fireSpriteChanged();
                return;
            }
        }
    }

    // No Handle clicked, so maybe the selection is changing
    for (DTint i = (DTint) joints.size()-1; i >= 0; --i) {

        if ( isClickedPart( _object_to_viewport_transform, joints[i], event->x(), event->y()) ) {
            if (event->modifiers() & Qt::ShiftModifier) {
                if (_main_window->isPartSelected(joints[i]))
                    _main_window->removePartSelection(joints[i]);
                else
                    _main_window->addPartSelection(joints[i]);
            } else {
                _main_window->clearPartSelection();
                _main_window->addPartSelection(joints[i]);

                _current_handle = HANDLE_ROTATE;
                _current_handle_joint = joints[i];
            }

            // Add a keyframe
            _main_window->keyJoint(_current_handle_joint);

            _main_window->fireSpriteChanged();
            return;
        }
    }

    _main_window->clearPartSelection();
    _main_window->fireSpriteChanged();
}

void EdSpriteEditor::mouseMoveEvent (QMouseEvent *event)
{
    float aspect = (float) width() / (float) height();

    int dx = event->x() - _last_mouse_x;
    int dy = event->y() - _last_mouse_y;

    if ( ((event->buttons() & Qt::MidButton) && (event->buttons() & Qt::LeftButton)) ||
         ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ALT) && (event->modifiers() & Qt::SHIFT)) ) {
       
        _zoom += (dx + dy) * 0.01F;
        if (_zoom < 0.1F)       _zoom = 0.1F;
        if (_zoom > 10.0F)      _zoom = 10.0F;

    } else if ( (event->buttons() & Qt::MidButton) ||
          ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ALT) ) ) {

        _translate.x += dx / (float) width() * 2.0F * aspect * _zoom;
        _translate.y += -dy / (float) height() * 2.0F * _zoom;

    } else if (event->buttons() & Qt::LeftButton) {

        if (_current_handle_joint) {
            Vector2 event_object = _current_handle_joint->getJointToObjectTransform().inversed() * _object_to_viewport_transform.inversed() * Vector2(event->x(),event->y());
            Vector2 last_object = _current_handle_joint->getJointToObjectTransform().inversed() * _object_to_viewport_transform.inversed() * Vector2(_last_mouse_x, _last_mouse_y);
            Vector2 diff = event_object - last_object;
            
            
            switch (_current_handle) {
                case HANDLE_NONE: {
                
                } break;
                
                case HANDLE_PLUS_X: {
                    _current_handle_joint->setPlusX( _current_handle_joint->getPlusX() + diff.x);
                } break;

                case HANDLE_MINUS_X: {
                    _current_handle_joint->setMinusX( _current_handle_joint->getMinusX() - diff.x);
                } break;

                case HANDLE_PLUS_Y: {
                    _current_handle_joint->setPlusY( _current_handle_joint->getPlusY() + diff.y);
                } break;

                case HANDLE_MINUS_Y: {
                    _current_handle_joint->setMinusY( _current_handle_joint->getMinusY() - diff.y);
                } break;

                case HANDLE_POSITION: {
                    KeyedSpriteAnimationPoseJoint *parent = _main_window->getSprite()->getParentOfJoint(_current_handle_joint);
                    Vector2 event_object_parent = (parent ? parent->getJointToObjectTransform().inversed() : Matrix3::identity()) * _object_to_viewport_transform.inversed() * Vector2(event->x(),event->y());
                    Vector2 last_object_parent = (parent ? parent->getJointToObjectTransform().inversed() : Matrix3::identity()) * _object_to_viewport_transform.inversed() * Vector2(_last_mouse_x, _last_mouse_y);
                    Vector2 diff_parent = event_object_parent - last_object_parent;

                    _current_handle_joint->setTranslation(_current_handle_joint->getTranslation() + diff_parent);
                } break;

                case HANDLE_ROTATE: {
                    Vector3 axis = Vector3::cross(  Vector3(last_object.x, last_object.y, 0.0F).normalized(), Vector3(event_object.x, event_object.y, 0.0F).normalized() );
                    _current_handle_joint->setRotation( _current_handle_joint->getRotation() + axis.abs() * (axis.z < 0.0F ? 1.0F : -1.0F) );
                } break;

                default: {
                    DTint i = _current_handle - HANDLE_GRID;
                    
                    Vector2 p0 = Vector2(-_current_handle_joint->getMinusX(), -_current_handle_joint->getMinusY());
                    Vector2 p1 = Vector2(_current_handle_joint->getPlusX(), -_current_handle_joint->getMinusY());
                    Vector2 p2 = Vector2(_current_handle_joint->getPlusX(), _current_handle_joint->getPlusY());
                    Vector2 p3 = Vector2(-_current_handle_joint->getMinusX(), _current_handle_joint->getPlusY());

                    Vector2 p1p0 = p1-p0;
                    Vector2 p2p3 = p2-p3;
                    
                    DTfloat p1p0l = p1p0.abs();
                    DTfloat p2p3l = p2p3.abs();
                    
                    DTfloat t0 = Vector2::dot( event_object - p0, p1p0 ) / (p1p0l*p1p0l);
                    DTfloat t1 = Vector2::dot( event_object - p3, p2p3 ) / (p2p3l*p2p3l);
                    
                    DTfloat tx = (t0+t1)*0.5F;

                    Vector2 pt0 = p1p0 * tx + p0;
                    Vector2 pt1 = p2p3 * tx + p3;
                    
                    Vector2 pt1pt0 = pt1-pt0;
                    DTfloat pt1pt0l = pt1pt0.abs();

                    DTfloat ty = 1.0F - Vector2::dot( event_object - pt0, pt1pt0 ) / (pt1pt0l*pt1pt0l);
                    
                    Array<Vector2>& grid = _current_handle_joint->gridPoints();
                    grid[i] = Vector2(tx,ty);
                }
            };
            
            // Add a keyframe
            _main_window->keyJoint(_current_handle_joint);
            
            updateTransforms();
            
        }

    }

    _last_mouse_x = event->x();
    _last_mouse_y = event->y();
    
    _main_window->fireSpriteChanged();
}

void EdSpriteEditor::mouseReleaseEvent (QMouseEvent *event)
{

}

void EdSpriteEditor::keyPressEvent (QKeyEvent *event)
{

}

//==============================================================================
//==============================================================================

void EdSpriteEditor::onSpriteChanged (void)
{
    update();
}

//==============================================================================
//==============================================================================

#include "moc_EdSpriteEditor.cpp"


//==============================================================================
///	
///	File: EdSpriteUVEditor.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteUVEditor.hpp"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include "System.hpp"
#include "DeviceGraphics.hpp"
#include "ConsoleStream.hpp"
#include "DrawBatcher.hpp"
#include "DrawBatcherQuads.hpp"
#include "ShaderResource.hpp"
#include "CameraObject.hpp"

//#include <GL/glew.h>

using namespace DT2;

//==============================================================================
//==============================================================================

const float EdSpriteUVEditor::HANDLE_CLICK_SIZE = 7.0F;
const float EdSpriteUVEditor::HANDLE_ROTATE_RADIUS = 50.0F;

//==============================================================================
//==============================================================================

EdSpriteUVEditor::EdSpriteUVEditor(EdSpriteMainWindow *main_window, QGLWidget *share)
    :   QGLWidget               (main_window, share),
        _zoom                   (1.0F),
        _translate              (-0.5F,-0.5F),
        _current_handle_part    (NULL)
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
    _outline_material.setColor(Color(1.0F,1.0F,1.0F,1.0F));
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

void EdSpriteUVEditor::updateTransforms (void)
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
}

Vector2 EdSpriteUVEditor::getHandleObjectPosition (const KeyedSpriteAnimationPoseJoint *joint, HandleID handle)
{
    switch (handle) {
        case HANDLE_PLUS_U: {
            return Vector2(joint->getPlusU(), (joint->getPlusV() + joint->getMinusV()) * 0.5F);
        } break;

        case HANDLE_MINUS_U: {
            return Vector2(joint->getMinusU(), (joint->getPlusV() + joint->getMinusV()) * 0.5F);
        } break;

        case HANDLE_PLUS_V: {
            return Vector2( (joint->getPlusU() + joint->getMinusU()) * 0.5F, joint->getPlusV());
        } break;

        case HANDLE_MINUS_V: {
            return Vector2( (joint->getPlusU() + joint->getMinusU()) * 0.5F, joint->getMinusV());
        } break;

    };

    return Vector2(0.0F,0.0F);
}

EdSpriteUVEditor::HandleID EdSpriteUVEditor::getClickedHandle (const Matrix3 &object_to_viewport_transform, const KeyedSpriteAnimationPoseJoint *joint, int x, int y)
{
    Vector2 handle_pos;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_PLUS_U);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_PLUS_U;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_MINUS_U);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_MINUS_U;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_PLUS_V);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_PLUS_V;

    handle_pos = object_to_viewport_transform * getHandleObjectPosition(joint, HANDLE_MINUS_V);
    if ( (handle_pos - Vector2(x,y)).abs() <= HANDLE_CLICK_SIZE)    return HANDLE_MINUS_V;

    return HANDLE_NONE;
}

bool EdSpriteUVEditor::isClickedPart (const Matrix3 &object_to_viewport_transform, const KeyedSpriteAnimationPoseJoint *joint, int x, int y)
{
    Vector2 position = _object_to_viewport_transform.inversed() * Vector2(x,y);

    float plusu = joint->getPlusU();
    float minusu = joint->getMinusU();
    float plusv = joint->getPlusV();
    float minusv = joint->getMinusV();
    
    if (plusu < minusu) std::swap(plusu,minusu);
    if (plusv < minusv) std::swap(plusv,minusv);

    if (position.x >= minusu && position.x <= plusu && position.y >= minusv && position.y <= plusv)
        return true;

    return false;
}

int EdSpriteUVEditor::getClickedPart (const Matrix3 &object_to_viewport_transform, int x, int y)
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

void EdSpriteUVEditor::drawCube (const Vector3 &position)
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


void EdSpriteUVEditor::drawPart(const Matrix3 &object_to_viewport_transform, KeyedSpriteAnimationPoseJoint *joint, bool selected)
{
    float plusu = joint->getPlusU();
    float minusu = joint->getMinusU();
    float plusv = joint->getPlusV();
    float minusv = joint->getMinusV();

    Vector2 p0 = Vector2(minusu, plusv);
    Vector2 p1 = Vector2(minusu, minusv);
    Vector2 p2 = Vector2(plusu, minusv);
    Vector2 p3 = Vector2(plusu, plusv);

    if (!selected) {
    
        DrawBatcher b;
        b.batchBegin (&_outline_material, Matrix4::identity(), DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);
        b.vertex(Vector3(p0.x,p0.y,0.0F));
        b.vertex(Vector3(p1.x,p1.y,0.0F));
        b.vertex(Vector3(p2.x,p2.y,0.0F));
        b.vertex(Vector3(p3.x,p3.y,0.0F));
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

        // Draw selection handles
        Vector2 center = getHandleObjectPosition(joint, HANDLE_PLUS_U);
        drawCube(Vector3(center.x, center.y, 0.0F));

        center = getHandleObjectPosition(joint, HANDLE_MINUS_U);
        drawCube(Vector3(center.x, center.y, 0.0F));

        center = getHandleObjectPosition(joint, HANDLE_PLUS_V);
        drawCube(Vector3(center.x, center.y, 0.0F));

        center = getHandleObjectPosition(joint, HANDLE_MINUS_V);
        drawCube(Vector3(center.x, center.y, 0.0F));

    }

}

void EdSpriteUVEditor::initializeGL (void)
{
	System::getRenderer()->openDisplay(width(), height());
    
    //::glEnable(GL_MULTISAMPLE);
    GLint bufs;
    GLint samples;
    ::glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    ::glGetIntegerv(GL_SAMPLES, &samples);
    qDebug("Have %d buffers and %d samples", bufs, samples);
}

void EdSpriteUVEditor::paintGL (void)
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
    // Draw textured part
    //
    
    MaterialResource *material = _main_window->getSprite()->getMaterialProperty();
    if (material) {
        
        DrawBatcherQuads b;
        b.batchBegin (material, Matrix4::identity(), DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1);
        b.vertex(Vector3(0.0F,0.0F,0.0F), Texcoord2(0.0F, 0.0F));
        b.vertex(Vector3(1.0F,0.0F,0.0F), Texcoord2(1.0F, 0.0F));
        b.vertex(Vector3(1.0F,1.0F,0.0F), Texcoord2(1.0F, 1.0F));
        b.vertex(Vector3(0.0F,1.0F,0.0F), Texcoord2(0.0F, 1.0F));
        b.batchEnd();
        b.flush();
        
    }
    
    //
    // Draw bounds
    //
    
    DrawBatcher b;
    b.batchBegin (&_bounds_material, Matrix4::identity(), DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);
    b.vertex ( Vector3(0.0F,0.0F,0.0F) );
    b.vertex ( Vector3(1.0F,0.0F,0.0F) );
    b.vertex ( Vector3(1.0F,1.0F,0.0F) );
    b.vertex ( Vector3(0.0F,1.0F,0.0F) );
    b.batchEnd();
	b.flush();
    
    //
    // Draw Sprite
    //
    
    // Draw Sprite
    Array<KeyedSpriteAnimationPoseJoint*> &joints = _main_window->getSprite()->getJoints();

    for (DTuint i = 0; i < joints.size(); ++i) {
        drawPart(_object_to_viewport_transform, joints[i], _main_window->isPartSelected(joints[i]) );
    }
    
	swapBuffers();
	
	doneCurrent();
}

void EdSpriteUVEditor::resizeGL (int w, int h)
{
    ::glViewport(0, 0, w, h);
}

//==============================================================================
//==============================================================================

void EdSpriteUVEditor::mousePressEvent (QMouseEvent *event)
{
    _main_window->pushUndo();

    updateTransforms();

    _last_mouse_x = event->x();
    _last_mouse_y = event->y();
    
    
    Vector2 position = _object_to_viewport_transform.inversed() * Vector2(_last_mouse_x,_last_mouse_y);
    LOG_MESSAGE << position.x << "  " << position.y;


    Array<KeyedSpriteAnimationPoseJoint*> &joints = _main_window->getSprite()->getJoints();

    // Check if handle clicked
    for (DTsize i = 0; i < joints.size(); ++i) {

        if (_main_window->isPartSelected(joints[i])) {
            _current_handle = getClickedHandle( _object_to_viewport_transform, joints[i], event->x(), event->y());

            if (_current_handle != HANDLE_NONE) {
                _current_handle_part = joints[i];
                _main_window->fireSpriteChanged();
                return;
            }
        }
    }

    // No Handle clicked, so maybe the selection is changing
    for (DTsize i = joints.size()-1; i >= 0; --i) {

        if ( isClickedPart( _object_to_viewport_transform, joints[i], event->x(), event->y()) ) {
            if (event->modifiers() & Qt::ShiftModifier) {
                if (_main_window->isPartSelected(joints[i]))
                    _main_window->removePartSelection(joints[i]);
                else
                    _main_window->addPartSelection(joints[i]);
            } else {
                _main_window->clearPartSelection();
                _main_window->addPartSelection(joints[i]);
            }
            
            // Add a keyframe
            _main_window->keyJoint(_current_handle_part);
            
            _main_window->fireSpriteChanged();
            return;
        }
    }

    _main_window->clearPartSelection();

    _main_window->fireSpriteChanged();
}

void EdSpriteUVEditor::mouseMoveEvent (QMouseEvent *event)
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

        if (_current_handle_part) {
            Vector2 event_object = _object_to_viewport_transform.inversed() * Vector2(event->x(),event->y());
            Vector2 last_object = _object_to_viewport_transform.inversed() * Vector2(_last_mouse_x, _last_mouse_y);
            Vector2 diff = event_object - last_object;
            
            
            switch (_current_handle) {
                case HANDLE_PLUS_U: {
                    _current_handle_part->setPlusU( _current_handle_part->getPlusU() + diff.x);
                } break;

                case HANDLE_MINUS_U: {
                    _current_handle_part->setMinusU( _current_handle_part->getMinusU() + diff.x);
                } break;

                case HANDLE_PLUS_V: {
                    _current_handle_part->setPlusV( _current_handle_part->getPlusV() + diff.y);
                } break;

                case HANDLE_MINUS_V: {
                    _current_handle_part->setMinusV( _current_handle_part->getMinusV() + diff.y);
                } break;

            };

            // Add a keyframe
            _main_window->keyJoint(_current_handle_part);

            updateTransforms();
            
        }

    }

    _main_window->fireSpriteChanged();

    _last_mouse_x = event->x();
    _last_mouse_y = event->y();
}

void EdSpriteUVEditor::mouseReleaseEvent (QMouseEvent *event)
{

}

void EdSpriteUVEditor::keyPressEvent (QKeyEvent *event)
{

}

//==============================================================================
//==============================================================================

void EdSpriteUVEditor::onSpriteChanged (void)
{
    update();
}

//==============================================================================
//==============================================================================

#include "moc_EdSpriteUVEditor.cpp"


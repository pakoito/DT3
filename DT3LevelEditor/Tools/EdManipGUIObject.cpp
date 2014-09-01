//==============================================================================
///	
///	File: EdManipGUIObject.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdManipGUIObject.hpp"
#include "EdLevelToolEvent.hpp"

// Qt include
// Engine includes

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_TOOL(EdManipGUIObject)

//==============================================================================
//==============================================================================

EdManipGUIObject::EdManipGUIObject (void)
    :   _starting_location_t1   (0.0F),
        _starting_location_t2   (0.0F)
{
    _line_material.setBlendEnable(false);
    _line_material.setDepthEnable(true);
    _line_material.setCullMode(DT3GL_CULL_NONE);
    _line_material.setColor(Color(1.0F,1.0F,1.0F,1.0F));
	_line_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    
    _red_material.setBlendEnable(false);
    _red_material.setDepthEnable(true);
    _red_material.setCullMode(DT3GL_CULL_NONE);
    _red_material.setColor(Color(1.0F,0.0F,0.0F,1.0F));
	_red_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    
}

EdManipGUIObject::~EdManipGUIObject    (void)
{

}

//==============================================================================
//==============================================================================

void EdManipGUIObject::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    if (getSelection().size() != 1)
        return;
        
    GUIObject *gui_object = checkedCast<GUIObject*>(getSelection().front());
    if (!gui_object)
        return;
    
        
    // Override the camera with a screen aligned one
    CameraObject gui_camera = *camera;  // Copy all camera attributes
    gui_camera.setOrtho(0.0F,1.0F,0.0F,1.0F,1.0F,-1.0F);
    gui_camera.setTranslationLocal(Vector3(0.0F,0.0F,0.0F));
    gui_camera.setOrientationLocal(Matrix3::identity());
        
    DrawUtils::activateCamera(&gui_camera);
        
    Matrix4 axis;
    Matrix4 transform = getManipulatorTransform();
    
    scale = 7.0F / System::getRenderer()->getScreenWidth();

    ::glPushName(0);
    
    Rectangle r = gui_object->getRectangle();
    
    Vector3 p0 = Vector3(r.getMinusX(), r.getMinusY(), 0.0F);
    Vector3 p1 = Vector3(r.getPlusX(), r.getMinusY(), 0.0F);
    Vector3 p2 = Vector3(r.getPlusX(), r.getPlusY(), 0.0F);
    Vector3 p3 = Vector3(r.getMinusX(), r.getPlusY(), 0.0F);
    
    // Translate
    ::glLoadName(TRANSLATE);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        Vector3(0.0F,0.0F,0.0F), 
                        scale);
    drawCube(&_red_material, transform * axis);
    
    // X Axis
    ::glLoadName(MINUS_X);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        (p0+p3)*0.5F, 
                        scale);
    drawCube(&_red_material, transform * axis);

    ::glLoadName(PLUS_X);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        (p1+p2)*0.5F, 
                        scale);
    drawCube(&_red_material, transform * axis);
    
    // Y Axis
    ::glLoadName(MINUS_Y);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        (p0+p1)*0.5F, 
                        scale);
    drawCube(&_red_material, transform * axis);

    ::glLoadName(PLUS_Y);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        (p2+p3)*0.5F, 
                        scale);
    drawCube(&_red_material, transform * axis);

    ::glLoadName(0);
    
    // Draw Lines
    DrawBatcher b;
    b.batchBegin(&_line_material, transform, DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);

    b.vertex(p0);
    b.vertex(p1);
    b.vertex(p2);
    b.vertex(p3);

    b.batchEnd();

    
    ::glPopName();  // Pop component
    
    
    DrawUtils::activateCamera(camera);
}

void EdManipGUIObject::drawCube (MaterialResource *mat, Matrix4 transform)
{
    DrawBatcherQuads b;
    b.batchBegin(mat, transform, DrawBatcher::FMT_V);
    
    b.vertex( Vector3(-1.0F,1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,1.0F,1.0F) );
    b.vertex( Vector3(-1.0F,1.0F,1.0F) );

    b.vertex( Vector3(-1.0F,-1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,-1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,-1.0F,1.0F) );
    b.vertex( Vector3(-1.0F,-1.0F,1.0F) );
    
    b.vertex( Vector3(-1.0F,1.0F,-1.0F) );
    b.vertex( Vector3(-1.0F,-1.0F,-1.0F) );
    b.vertex( Vector3(-1.0F,-1.0F,1.0F) );
    b.vertex( Vector3(-1.0F,1.0F,1.0F) );
    
    b.vertex( Vector3(1.0F,1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,-1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,-1.0F,1.0F) );
    b.vertex( Vector3(1.0F,1.0F,1.0F) );
    
    b.vertex( Vector3(-1.0F,1.0F,1.0F) );
    b.vertex( Vector3(-1.0F,-1.0F,1.0F) );
    b.vertex( Vector3(1.0F,-1.0F,1.0F) );
    b.vertex( Vector3(1.0F,1.0F,1.0F) );

    b.vertex( Vector3(-1.0F,1.0F,-1.0F) );
    b.vertex( Vector3(-1.0F,-1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,-1.0F,-1.0F) );
    b.vertex( Vector3(1.0F,1.0F,-1.0F) );
    
    b.batchEnd();
}

//==============================================================================
//==============================================================================

void EdManipGUIObject::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    if (getSelection().size() != 1)
        return;
        
    GUIObject *gui_object = checkedCast<GUIObject*>(getSelection().front());
    if (!gui_object)
        return;

    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        LOG_MESSAGE << "Control ID " << event._control_id;
        _starting_control_id = event._control_id;
    } else if (event._event_type == EdLevelToolEvent::MOUSE_UP) {
        _starting_control_id = 0;
    }

    
    // Ignore if no axis is dragged
    if (_starting_control_id == NONE)
        return;
        
    DTfloat x_screen = (DTfloat) event.getMouseX() / (DTfloat) System::getRenderer()->getScreenWidth();
    DTfloat y_screen = 1.0F - (DTfloat) event.getMouseY() / (DTfloat) System::getRenderer()->getScreenHeight();

    Matrix4 transform = getManipulatorTransform();
    Vector3 obj_pos = transform.inversed() * Vector3(x_screen, y_screen, 0.0F);
    
    if (_starting_control_id == MINUS_X) {
        gui_object->setWidth(-obj_pos.x * 2.0F);
    } else if (_starting_control_id == PLUS_X) {
        gui_object->setWidth(obj_pos.x * 2.0F);
    } else if (_starting_control_id == MINUS_Y) {
        gui_object->setHeight(-obj_pos.y * 2.0F);
    } else if (_starting_control_id == PLUS_Y) {
        gui_object->setHeight(obj_pos.y * 2.0F);
    } else if (_starting_control_id == TRANSLATE) {
        gui_object->setTranslationLocal( gui_object->getTranslationLocal() + obj_pos);
    }


    /*
    // Raytrace the position on the axis closest to the clicked point
    DTfloat t1;
    DTfloat t2;
    
    PrimitiveCollisions::distRayRay(    event.getRaySource(), event.getRayDest() - event.getRaySource(),
                                        translation, axis,
                                        t1, t2);

    LOG_MESSAGE << castToString(translation + axis * t2);
    
    // Move the manipulator if dragging
    if (event._event_type == EdLevelToolEvent::MOUSE_DRAG) {
    
        if (_starting_type == COLLISION_BOX) {
        
            std::vector<Box> &collision_boxes = battleship->getCollisionBoxes();
            Box &box = collision_boxes[_starting_index];

            DTfloat value = t2 - _starting_location_t2;
            switch (_starting_control) {
                case MINUS_X:   box.setMinusX(value);   break;
                case PLUS_X:    box.setPlusX(value);    break;
                case MINUS_Y:   box.setMinusY(value);   break;
                case PLUS_Y:    box.setPlusY(value);    break;
                case MINUS_Z:   box.setMinusZ(value);   break;
                case PLUS_Z:    box.setPlusZ(value);    break;
            };
        
        } else if (_starting_type == THRUSTER) {
            
        } else {
        
        }

    } else {
        _starting_location_t1 = t1;
        _starting_location_t2 = t2;
    }*/
    
}

//==============================================================================
//==============================================================================

Matrix4 EdManipGUIObject::getManipulatorTransform  (void) const
{
    if (getSelection().size() != 1)
        return Matrix4::identity();

    GUIObject *gui_object = checkedCast<GUIObject*>(getSelection().front());
    if (!gui_object)
        return Matrix4::identity();

    return gui_object->getDrawTransform();
}

//==============================================================================
//==============================================================================

} // DT3


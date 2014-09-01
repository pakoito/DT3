//==============================================================================
///	
///	File: EdManipScriptingSpline.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdManipScriptingSpline.hpp"
#include "EdLevelToolEvent.hpp"

// Qt include
// Engine includes

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_TOOL(EdManipScriptingSpline)

//==============================================================================
//==============================================================================

EdManipScriptingSpline::EdManipScriptingSpline (void)
    :   _starting_location_dist (0.0F)
{
    _line_material.setBlending(false);
    _line_material.setDepthTest(true);
    _line_material.setCulling(false);
    _line_material.setColor(Color(1.0F,1.0F,1.0F,1.0F));
	_line_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _line_material.setRecacheParameters(true);
    
    _grey_line_material.setBlending(false);
    _grey_line_material.setDepthTest(true);
    _grey_line_material.setCulling(false);
    _grey_line_material.setColor(Color(0.7F,0.7F,0.7F,1.0F));
	_grey_line_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _grey_line_material.setRecacheParameters(true);
    
    _shadow_line_material.setBlending(false);
    _shadow_line_material.setDepthTest(true);
    _shadow_line_material.setCulling(false);
    _shadow_line_material.setColor(Color(0.1F,0.1F,0.1F,1.0F));
	_shadow_line_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _shadow_line_material.setRecacheParameters(true);

    _red_material.setBlending(false);
    _red_material.setDepthTest(true);
    _red_material.setCulling(false);
    _red_material.setColor(Color(1.0F,0.0F,0.0F,1.0F));
	_red_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _red_material.setRecacheParameters(true);

    _green_material.setBlending(false);
    _green_material.setDepthTest(true);
    _green_material.setCulling(false);
    _green_material.setColor(Color(0.0F,1.0F,0.0F,1.0F));
	_green_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _green_material.setRecacheParameters(true);
    
    _blue_material.setBlending(false);
    _blue_material.setDepthTest(true);
    _blue_material.setCulling(false);
    _blue_material.setColor(Color(0.0F,0.0F,1.0F,1.0F));
	_blue_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    _blue_material.setRecacheParameters(true);
}

EdManipScriptingSpline::~EdManipScriptingSpline    (void)
{

}

//==============================================================================
//==============================================================================

DTuint EdManipScriptingSpline::toID(DTuint index, DTuint type)
{
    DTuint id = (( (DTuint)index & 0xFFFFFF) << 8) |
                (( (DTuint)type & 0xFF) << 0); 
    return id;
}

void EdManipScriptingSpline::fromID(DTuint id, DTuint &index, DTuint &type)
{
    index = (id >> 8) & 0xFFFFFF;
    type = (id >> 0) & 0xFF;
}

//==============================================================================
//==============================================================================

void EdManipScriptingSpline::draw (EdLevelToolWindow *parent, CameraObject *camera, DTfloat scale)
{
    if (getSelection().size() != 1)
        return;
        
    ScriptingSpline *spline_object = checkedCast<ScriptingSpline*>(getSelection().front());
    if (!spline_object)
        return;
        
    DTuint num_points = spline_object->getNumPoints();
        
    if (num_points <= 0)
        return;
        
    ::glPushName(0);
    
    DTuint max_distance = spline_object->getMaxDistance();
    System::getRenderer()->activateMaterial(&_line_material);
    
        
    // Draw Lines
    DrawBatcher b;
    b.batchBegin(&_grey_line_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

    for (DTuint k = 0; k < num_points; ++k) {
        Vector3 pt = spline_object->getPointTransform(k).translation();
        Vector3 tangent = spline_object->getPointTangent(k);    

        b.vertex(pt);
        b.vertex(pt + tangent);
    }

    b.batchEnd();
    b.flush();


    if (max_distance > 0.0F) {

        // Draw Lines
        b.batchBegin(&_line_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

        const DTfloat step = max_distance / 100.0F;
        for (DTfloat t = 0.0F; t+step <= max_distance; t += step) {
            Matrix4 t0,t1;
            
            spline_object->interpolate(t,t0);
            spline_object->interpolate(t+step,t1);
            
            Vector3 p0 = t0.translation();
            Vector3 p1 = t1.translation();
        
            b.vertex(p0);
            b.vertex(p1);
        }

        b.batchEnd();
        b.flush();

        // Draw Ticks
        b.batchBegin(&_grey_line_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

        for (DTfloat t = 0.0F; t < max_distance; t += 1.0F) {
            Matrix4 t0;
            
            spline_object->interpolate(t,t0);
            
            Vector3 p0 = t0.translation();
        
            b.vertex(p0 + t0.orientation() * Vector3(0.0F,0.5F,0.0F) );
            b.vertex(p0 - t0.orientation() * Vector3(0.0F,0.1F,0.0F) );
        }

        b.batchEnd();
        b.flush();


        // Draw Shadow Lines
        b.batchBegin(&_shadow_line_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

        for (DTfloat t = 0.0F; t+step <= max_distance; t += step) {
            Matrix4 t0,t1;
            
            spline_object->interpolate(t,t0);
            spline_object->interpolate(t+step,t1);
            
            Vector3 p0 = t0.translation();
            Vector3 p1 = t1.translation();
            
            p0.y = 0.0F;
            p1.y = 0.0F;
        
            b.vertex(p0);
            b.vertex(p1);
        }

        b.batchEnd();
        b.flush();
    
    }


    for (DTuint k = 0; k < num_points; ++k) {
        Matrix4 transform = spline_object->getPointTransform(k);
        Vector3 pt = spline_object->getPointTransform(k).translation();
        Vector3 tangent = spline_object->getPointTangent(k);    
        
        ::glLoadName(toID(k, VERTEX));
        drawCube(&_red_material, Matrix4(Matrix3::identity(), pt, scale));

        ::glLoadName(toID(k, TANGENT));
        drawCube(&_red_material, Matrix4(Matrix3::identity(), pt+tangent, scale));
        
        transform = Matrix4(transform.orientation(), transform.translation(), scale);

        ::glLoadName(0);

        // X Axis
        Matrix4 axis;
        ::glLoadName(toID(k, ROTATE_X));
        axis = Matrix4(     Matrix3(    0.0F, 1.0F, 0.0F,
                                        1.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, -1.0F), 
                            Vector3(0.0F,0.0F,0.0F), 
                            1.0F);
        drawRing(&_red_material, transform * axis);

        // Y Axis
        ::glLoadName(toID(k, ROTATE_Y));
        axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                        0.0F, 1.0F, 0.0F,
                                        0.0F, 0.0F, 1.0F), 
                            Vector3(0.0F,0.0F,0.0F), 
                            1.0F);
        drawRing(&_green_material, transform * axis);

        // Z Axis
        ::glLoadName(toID(k, ROTATE_Z));
        axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, -1.0F,
                                        0.0F, 1.0F, 0.0F), 
                            Vector3(0.0F,0.0F,0.0F), 
                            1.0F);
        drawRing(&_blue_material, transform * axis);
    }
    
    ::glPopName();  // Pop component    
}

void EdManipScriptingSpline::drawCube (MaterialResource *mat, Matrix4 transform)
{
    DrawBatcherQuads b;
    b.batchBegin(mat, transform, DrawBatcher::FMT_V);
    
    b.vertex( Vector3(-0.05F,0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,0.05F,0.05F) );
    b.vertex( Vector3(-0.05F,0.05F,0.05F) );

    b.vertex( Vector3(-0.05F,-0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,-0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,-0.05F,0.05F) );
    b.vertex( Vector3(-0.05F,-0.05F,0.05F) );
    
    b.vertex( Vector3(-0.05F,0.05F,-0.05F) );
    b.vertex( Vector3(-0.05F,-0.05F,-0.05F) );
    b.vertex( Vector3(-0.05F,-0.05F,0.05F) );
    b.vertex( Vector3(-0.05F,0.05F,0.05F) );
    
    b.vertex( Vector3(0.05F,0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,-0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,-0.05F,0.05F) );
    b.vertex( Vector3(0.05F,0.05F,0.05F) );
    
    b.vertex( Vector3(-0.05F,0.05F,0.05F) );
    b.vertex( Vector3(-0.05F,-0.05F,0.05F) );
    b.vertex( Vector3(0.05F,-0.05F,0.05F) );
    b.vertex( Vector3(0.05F,0.05F,0.05F) );

    b.vertex( Vector3(-0.05F,0.05F,-0.05F) );
    b.vertex( Vector3(-0.05F,-0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,-0.05F,-0.05F) );
    b.vertex( Vector3(0.05F,0.05F,-0.05F) );
    
    b.batchEnd();
}

void EdManipScriptingSpline::drawRing (MaterialResource *mat, Matrix4 transform)
{
    DrawBatcher b;
    b.batchBegin(mat, transform, DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);
    
    const DTuint MAX_SEGMENTS = 64;
    for (DTuint i = 0; i <= MAX_SEGMENTS; ++i) {
        DTfloat angle = (DTfloat) i / (DTfloat) MAX_SEGMENTS * 2.0F * PI;
        b.vertex( Vector3(1.0F * std::cos(angle), 0.0F, 1.0F * std::sin(angle)) );
    }
    
    b.batchEnd();
}


//==============================================================================
//==============================================================================

void EdManipScriptingSpline::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    if (getSelection().size() != 1)
        return;
        
    ScriptingSpline *spline_object = checkedCast<ScriptingSpline*>(getSelection().front());
    if (!spline_object)
        return;
        
    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        LOG_MESSAGE << "Control ID " << event._control_id;
        fromID(event._control_id, _starting_index, _starting_type);
    } else if (event._event_type == EdLevelToolEvent::MOUSE_UP) {
        _starting_type = 0;
        _starting_index = 0;
    }
        
    // Ignore if no axis is dragged
    if (_starting_type == NONE)
        return;
        
        
        
    Vector3 forwards = event.getCamera()->getForwards();
    Vector3 pt;
    
    if (_starting_type == VERTEX) {
        pt = spline_object->getPointTransform(_starting_index).translation();
    } else if (_starting_type == TANGENT) {
        pt = spline_object->getPointTangent(_starting_index) + spline_object->getPointTransform(_starting_index).translation();
    }

    
    // Move the manipulator if dragging
    if (event._event_type == EdLevelToolEvent::MOUSE_DRAG) {
    
        DTint dx = event.getMouseX() - _mouse_x;
        DTint dy = event.getMouseY() - _mouse_y;
        _mouse_x = event.getMouseX();
        _mouse_y = event.getMouseY();
  
        Vector3 ray = event.getRayDest() - event.getRaySource();
        DTfloat dist = Vector3::dot(forwards, ray);
        ray *= _starting_location_dist / dist;

        if (_starting_type == VERTEX) { 
            Matrix3 orientation = spline_object->getPointTransform(_starting_index).orientation();

            spline_object->setPointTransform(_starting_index, Matrix4(orientation, event.getRaySource() + ray));
            
        } else if (_starting_type == TANGENT) 
            spline_object->setPointTangent(_starting_index, event.getRaySource() + ray - spline_object->getPointTransform(_starting_index).translation());
        else if (_starting_type == ROTATE_X || _starting_type == ROTATE_Y || _starting_type == ROTATE_Z) {
            
            Vector3 axis;
            switch (_starting_type) {
                case ROTATE_X:      axis = Vector3(1.0F,0.0F,0.0F);         break;
                case ROTATE_Y:      axis = Vector3(0.0F,1.0F,0.0F);         break;
                case ROTATE_Z:      axis = Vector3(0.0F,0.0F,1.0F);         break;
            };
            axis.normalize();
        
            // Rotate the object
            Matrix3 rotation = Matrix3::setRotationAround (axis, (dy+dx) * 0.05F);

            Matrix4 transform;
            transform = spline_object->getPointTransform(_starting_index);
            
            Matrix3 orientation = transform.orientation() * rotation;
            Vector3 translation = transform.translation();
            
            spline_object->setPointTransform(_starting_index, Matrix4(orientation, translation) );
        }
            
    } else {
        _mouse_x = event.getMouseX();
        _mouse_y = event.getMouseY();
        _starting_location_dist = Vector3::dot(forwards, pt - event.getRaySource());
    }
    
}

//==============================================================================
//==============================================================================

Matrix4 EdManipScriptingSpline::getManipulatorTransform  (void) const
{
    return Matrix4::identity();
}

//==============================================================================
//==============================================================================

} // DT2


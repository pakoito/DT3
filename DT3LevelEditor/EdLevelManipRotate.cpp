//==============================================================================
///	
///	File: EdLevelManipRotate.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <GL/glew.h>

// Editor include
#include "EdLevelManipRotate.hpp"
#include "EdLevelToolEvent.hpp"

// Qt include

// Engine includes
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"
#include "DT3Core/Types/Graphics/DrawUtils.hpp"
#include "DT3Core/Types/Math/PrimitiveCollisions.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelManipRotate::EdLevelManipRotate (void)
{
    _shader = ShaderResource::import_resource(FilePath("{ED_TOOL_SHADER}"));
    _tool_material = MaterialResource::import_resource(FilePath("{ED_TOOL_MATERIAL}"));
}

//==============================================================================
//==============================================================================

void EdLevelManipRotate::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    DrawBatcher b;
    
    Matrix4 axis;

    Matrix4 transform = getManipulatorTransform();
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();
    
    transform = Matrix4(orientation, translation, scale);

    ::glPushName(0);
    
    // X Axis
    ::glLoadName(ROTATE_X);
    axis = Matrix4(     Matrix3(    0.0F, 1.0F, 0.0F,
                                    1.0F, 0.0F, 0.0F,
                                    0.0F, 0.0F, -1.0F), 
                        Vector3(0.0F,0.0F,0.0F), 
                        1.0F);
    
    DrawUtils::draw_ring (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::red,
                            1.0F);
    b.flush();
    
    // Y Axis
    ::glLoadName(ROTATE_Y);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        Vector3(0.0F,0.0F,0.0F), 
                        1.0F);

    DrawUtils::draw_ring (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::green,
                            1.0F);
    b.flush();

    // Z Axis
    ::glLoadName(ROTATE_Z);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 0.0F, -1.0F,
                                    0.0F, 1.0F, 0.0F), 
                        Vector3(0.0F,0.0F,0.0F), 
                        1.0F);

    DrawUtils::draw_ring (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::blue,
                            1.0F);
    b.flush();
    
    // Rotate
    ::glLoadName(ROTATE_SCREEN);
    axis = Matrix4(     Matrix3(transform).inversed() * 
                        camera->orientation() *
                        Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 0.0F, -1.0F,
                                    0.0F, 1.0F, 0.0F), 
                        Vector3(0.0F,0.0F,0.0F), 
                        1.2F * scale);

    DrawUtils::draw_ring (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::white,
                            1.0F);
    b.flush();
    
    ::glPopName();

}

//==============================================================================
//==============================================================================

void EdLevelManipRotate::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        LOG_MESSAGE << "Control ID " << event._control_id;
        _starting_axis = event._control_id;
    } else if (event._event_type == EdLevelToolEvent::MOUSE_UP) {
        _starting_axis = 0;
    }

    // Ignore if no axis is dragged
    if (_starting_axis != ROTATE_X && 
        _starting_axis != ROTATE_Y && 
        _starting_axis != ROTATE_Z && 
        _starting_axis != ROTATE_SCREEN &&
        _starting_axis != ROTATE)
        return;
        
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        _mouse_x = event.mouseX();
        _mouse_y = event.mouseY();

    } else {
        DTint dx = event.mouseX() - _mouse_x;
        DTint dy = event.mouseY() - _mouse_y;
        _mouse_x = event.mouseX();
        _mouse_y = event.mouseY();

        Matrix4 transform = getManipulatorTransform();
        Matrix3 orientation = transform.orientation();
        //Vector3 translation = transform.translation();

        Vector3 axis;
        switch (_starting_axis) {
            case ROTATE_X:      axis = Vector3(1.0F,0.0F,0.0F);         break;
            case ROTATE_Y:      axis = Vector3(0.0F,1.0F,0.0F);         break;
            case ROTATE_Z:      axis = Vector3(0.0F,0.0F,1.0F);         break;
            case ROTATE_SCREEN: axis = orientation.inversed() * (transform.translation() - event.getCamera()->translation());   break;
        };
        
        // Fix axis if needed
        if (axis.abs2() < 0.0001F) {
            axis = orientation.inversed() * event.getCamera()->orientation().z_axis();
        } 
        
        axis.normalize();
        
        
        /*// Camera Space Axis
        Vector3 camera_space_axis = event.getCamera()->getOrientation().inversed() * orientation * axis;
        camera_space_axis.normalize();*/

        // Rotate the object
        Matrix3 rotation = Matrix3::set_rotation_around (axis.normalized(), (dy+dx) * 0.05F);

        applyCombinedTransform (parent, Matrix4(rotation, Vector3(0.0F,0.0F,0.0F)));
    }
}

//==============================================================================
//==============================================================================

Matrix4 EdLevelManipRotate::getManipulatorTransform  (void) const
{
    Matrix4 transform = getCombinedTransform();
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();

    return Matrix4(orientation,translation);
}

//==============================================================================
//==============================================================================


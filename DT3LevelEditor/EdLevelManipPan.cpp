//==============================================================================
///	
///	File: EdLevelManipPan.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <GL/glew.h>

// Editor include
#include "EdLevelManipPan.hpp"
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

EdLevelManipPan::EdLevelManipPan (void)
    :   _starting_location_t1   (0.0F),
        _starting_location_t2   (0.0F)
{
    _shader = ShaderResource::import_resource(FilePath("{ED_TOOL_SHADER}"));
    _tool_material = MaterialResource::import_resource(FilePath("{ED_TOOL_MATERIAL}"));
}

//==============================================================================
//==============================================================================

void EdLevelManipPan::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    DrawBatcher b;

    Matrix4 axis;
    
    Matrix4 transform = getManipulatorTransform();
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();
    transform = Matrix4(orientation, translation, scale);


    ::glPushName(0);
    
    // Center
    ::glLoadName(CENTER);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        Vector3(0.0F,0.0F,0.0F), 
                        1.0F);
    
    DrawUtils::draw_cube (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::white,
                            0.05F);
    b.flush();
    
    DTboolean draw_x =  std::abs( Vector3::dot( Vector3(1.0F,0.0F,0.0F),camera->orientation().z_axis()) ) < 0.97F;
    DTboolean draw_y =  std::abs( Vector3::dot( Vector3(0.0F,1.0F,0.0F),camera->orientation().z_axis()) ) < 0.97F;
    DTboolean draw_z =  std::abs( Vector3::dot( Vector3(0.0F,0.0F,1.0F),camera->orientation().z_axis()) ) < 0.97F;

    
    if (draw_x) {
        // X Axis
        ::glLoadName(PLUS_X);
        axis = Matrix4(     Matrix3(    0.0F, 1.0F, 0.0F,
                                        1.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, -1.0F), 
                            Vector3(1.0F,0.0F,0.0F), 
                            1.0F);
        
        DrawUtils::draw_cone(   b,
                                camera,
                                _tool_material,
                                _shader,
                                transform * axis,
                                Color4b::red,
                                0.1F,
                                0.1F);
        b.flush();
    }

    if (draw_y) {
        // Y Axis
        ::glLoadName(PLUS_Y);
        axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                        0.0F, 1.0F, 0.0F,
                                        0.0F, 0.0F, 1.0F), 
                            Vector3(0.0F,1.0F,0.0F), 
                            1.0F);
        
        DrawUtils::draw_cone(   b,
                                camera,
                                _tool_material,
                                _shader,
                                transform * axis,
                                Color4b::green,
                                0.1F,
                                0.1F);
        b.flush();
    }

    if (draw_z) {
        // Z Axis
        ::glLoadName(PLUS_Z);
        axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, -1.0F,
                                        0.0F, 1.0F, 0.0F), 
                            Vector3(0.0F,0.0F,1.0F), 
                            1.0F);
        
        DrawUtils::draw_cone(   b,
                                camera,
                                _tool_material,
                                _shader,
                                transform * axis,
                                Color4b::blue,
                                0.1F,
                                0.1F);
        b.flush();
    }
    
    ::glPopName();  // Pop component
    
    // Draw Lines
    b.batch_begin(camera, _tool_material, _shader, transform, DT3GL_PRIM_LINES, DrawBatcher::FMT_V);

    if (draw_x) {
        b.add().v(0.0F,0.0F,0.0F);
        b.add().v(1.0F,0.0F,0.0F);
    }
    
    if (draw_y) {
        b.add().v(0.0F,0.0F,0.0F);
        b.add().v(0.0F,1.0F,0.0F);
    }

    if (draw_z) {
        b.add().v(0.0F,0.0F,0.0F);
        b.add().v(0.0F,0.0F,1.0F);
    }

    b.batch_end();
    b.flush();
    
}

//==============================================================================
//==============================================================================

void EdLevelManipPan::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        LOG_MESSAGE << "Control ID " << event._control_id;
        _starting_axis = event._control_id;
    } else if (event._event_type == EdLevelToolEvent::MOUSE_UP) {
        _starting_axis = 0;
    }
    
    // Ignore if no axis is dragged
    if (_starting_axis != PLUS_X &&
        _starting_axis != PLUS_Y &&
        _starting_axis != PLUS_Z &&
        _starting_axis != CENTER)
        return;

    Matrix4 transform = getManipulatorTransform();
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();

    Vector3 axis;
    switch (_starting_axis) {
        case PLUS_X:    axis = orientation.x_axis().normalized(); break;
        case PLUS_Y:    axis = orientation.y_axis().normalized(); break;
        case PLUS_Z:    axis = orientation.z_axis().normalized(); break;
        default:        axis.clear();
    };
    
    // Raytrace the position on the axis closest to the clicked point
    DTfloat t1;
    DTfloat t2 = 0.0F;
    
    if (_starting_axis != CENTER) {
        PrimitiveCollisions::dist_ray_ray(  event.getRaySource(), event.getRayDestination() - event.getRaySource(),
                                            translation, axis,
                                            t1, t2);
    } else {
        PrimitiveCollisions::dist_ray_point(event.getRaySource(), event.getRayDestination() - event.getRaySource(), translation, t1);
    
    }

    LOG_MESSAGE << MoreStrings::cast_to_string(translation + axis * t2);
    
    // Move the manipulator if dragging
    if (event._event_type == EdLevelToolEvent::MOUSE_DRAG) {
    
        if (_starting_axis != CENTER) {
            Vector3 translate = axis * (t2 - _starting_location_t2);
            snapToGrid (translate, event.getGrid());
            
            Matrix3 rotate(Matrix3::identity());
            
            // Set the new transform
            transform = Matrix4(rotate,translate);
        } else {
            Vector3 translate = event.getRaySource() + (event.getRayDestination() - event.getRaySource()) * _starting_location_t1 - translation;
            snapToGrid (translate, event.getGrid());

            Matrix3 rotate(Matrix3::identity());
            
            // Set the new transform
            transform = Matrix4(rotate,translate);
        }
        
        applyCombinedTransform(parent, transform, event.getGrid());
    } else {
        _starting_location_t1 = t1;
        _starting_location_t2 = t2;
    }
}

//==============================================================================
//==============================================================================

Matrix4 EdLevelManipPan::getManipulatorTransform  (void) const
{
    Matrix4 transform = getCombinedTransform();
	Matrix3 orientation = Matrix3::identity();
	Vector3 translation = transform.translation();

    return Matrix4(orientation, translation);
}

//==============================================================================
//==============================================================================


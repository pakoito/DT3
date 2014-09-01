//==============================================================================
///	
///	File: EdLevelManipScale.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <GL/glew.h>

// Editor include
#include "EdLevelManipScale.hpp"
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

EdLevelManipScale::EdLevelManipScale (void)
{
    _shader = ShaderResource::import_resource(FilePath("{ED_TOOL_SHADER}"));
    _tool_material = MaterialResource::import_resource(FilePath("{ED_TOOL_MATERIAL}"));
}

//==============================================================================
//==============================================================================

void EdLevelManipScale::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
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


    // X Axis
    ::glLoadName(PLUS_X);
    axis = Matrix4(     Matrix3(    0.0F, 1.0F, 0.0F,
                                    1.0F, 0.0F, 0.0F,
                                    0.0F, 0.0F, -1.0F), 
                        Vector3(1.0F,0.0F,0.0F), 
                        1.0F);

    DrawUtils::draw_cube (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::red,
                            0.05F);
    b.flush();

    // Y Axis
    ::glLoadName(PLUS_Y);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 1.0F, 0.0F,
                                    0.0F, 0.0F, 1.0F), 
                        Vector3(0.0F,1.0F,0.0F), 
                        1.0F);

    DrawUtils::draw_cube (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::green,
                            0.05F);
    b.flush();

    // Z Axis
    ::glLoadName(PLUS_Z);
    axis = Matrix4(     Matrix3(    1.0F, 0.0F, 0.0F,
                                    0.0F, 0.0F, -1.0F,
                                    0.0F, 1.0F, 0.0F), 
                        Vector3(0.0F,0.0F,1.0F), 
                        1.0F);

    DrawUtils::draw_cube (  b,
                            camera,
                            _tool_material,
                            _shader,
                            transform * axis,
                            Color4b::blue,
                            0.05F);
    b.flush();
    
    ::glPopName();
    
    // Draw Lines
    b.batch_begin(camera, _tool_material, _shader, transform, DT3GL_PRIM_LINES, DrawBatcher::FMT_V);

    b.add().v(0.0F,0.0F,0.0F);
    b.add().v(1.0F,0.0F,0.0F);
    b.add().v(0.0F,0.0F,0.0F);
    b.add().v(0.0F,1.0F,0.0F);
    b.add().v(0.0F,0.0F,0.0F);
    b.add().v(0.0F,0.0F,1.0F);

    b.batch_end();
}

//==============================================================================
//==============================================================================

void EdLevelManipScale::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
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
            case PLUS_X:    axis = orientation.x_axis(); break;
            case PLUS_Y:    axis = orientation.y_axis(); break;
            case PLUS_Z:    axis = orientation.z_axis(); break;
        };
        axis.normalize();

        applyCombinedScale (parent, 1.0F + (dy+dx) * 0.05F);
    }
}

//==============================================================================
//==============================================================================

Matrix4 EdLevelManipScale::getManipulatorTransform  (void) const
{
    Matrix4 transform = getCombinedTransform();
	Matrix3 orientation = transform.orientation();
	Vector3 translation = transform.translation();

    return Matrix4(orientation, translation);
}

//==============================================================================
//==============================================================================


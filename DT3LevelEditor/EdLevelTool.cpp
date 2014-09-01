//==============================================================================
///	
///	File: EdLevelTool.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <GL/glew.h>

// Editor include
#include "EdLevelTool.hpp"
#include "EdLevelToolEvent.hpp"
#include "EdLevelToolWindow.hpp"

// Qt include

// Engine includes
#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_CREATION(EdLevelTool)

//==============================================================================
//==============================================================================

EdLevelTool::EdLevelTool (void)
{

}

//==============================================================================
//==============================================================================

void EdLevelTool::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{

}

//==============================================================================
//==============================================================================

void EdLevelTool::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{

}

//==============================================================================
//==============================================================================

Matrix4 EdLevelTool::getCombinedTransform (void) const
{
    int count = 0;
    Vector3 translation(0.0F,0.0F,0.0F);
    Matrix3 orientation(Matrix3::identity());
    Vector3 x_axis(0.0F,0.0F,0.0F);
    Vector3 y_axis(0.0F,0.0F,0.0F);
    Vector3 z_axis(0.0F,0.0F,0.0F);
    
    if (_selection.size() > 0) {

        FOR_EACH (i,_selection) {
            
            if ( (*i) && (**i).isa(PlaceableObject::kind())) {
                std::shared_ptr<PlaceableObject> placeable = checked_static_cast<PlaceableObject>(*i);
                
                translation += placeable->translation();
                x_axis += placeable->orientation().x_axis();
                y_axis += placeable->orientation().y_axis();
                z_axis += placeable->orientation().z_axis();
                
                ++count;
            }
        
        }

        if (count > 0) {
            translation = translation / (float) count;
            
            orientation = Matrix3(x_axis, y_axis, z_axis).orthoed();
        }
        
        Matrix4 transform = Matrix4::identity();
        transform.set_orientation(orientation);
        transform.set_translation(translation);
        return transform;
        
    } else {
        return Matrix4::identity();
    }
}

void EdLevelTool::applyCombinedTransform  (EdLevelToolWindow *parent, const Matrix4 &transform, DTfloat grid)
{
    // Do everything relative to the manipulator
    Matrix4 manipulator_transform = getManipulatorTransform();
    Matrix4 manipulator_transform_inverse = manipulator_transform.inversed();
    
    Vector3 manipulator_translation = manipulator_transform.translation();
    Vector3 old_manipulator_translation = manipulator_translation;
    
    snapToGrid(manipulator_translation, grid);
    
    Vector3 offset = manipulator_translation - old_manipulator_translation;

    FOR_EACH (i,_selection) {
        
        if ( (*i) && (**i).isa(PlaceableObject::kind())) {
            std::shared_ptr<PlaceableObject> placeable = checked_static_cast<PlaceableObject>(*i);
            
            // Get the original transform
            Matrix4 placeable_transform = placeable->transform();
            Vector3 old_scale = placeable_transform.scale();
            
            // Cumulative transform
            placeable_transform = manipulator_transform * transform * manipulator_transform_inverse * placeable_transform;
            placeable_transform.ortho();

            placeable_transform.set_scale(old_scale);
            
            // Snap to grid
            DT3::Vector3 translation = placeable_transform.translation();
            translation += offset;
            placeable_transform.set_translation(translation);
            
            TextBufferStream stream;
            stream << placeable_transform;

            // Set the new transform
            parent->onCommand(QString("SetTransform \"") + placeable->full_name().c_str() + "\" (" + stream.buffer().c_str() + ")");
        }
    
    }
}

void EdLevelTool::applyCombinedScale (EdLevelToolWindow *parent, DTfloat scale)
{
    FOR_EACH (i,_selection) {
        
        if ( (*i) && (**i).isa(PlaceableObject::kind())) {
            std::shared_ptr<PlaceableObject> placeable = checked_static_cast<PlaceableObject>(*i);
            
            // Get the original transform
            Vector3 placeable_scale = placeable->scale();
                    
            TextBufferStream stream;
            stream << (placeable_scale * scale);

            // Set the new transform
            parent->onCommand(QString("SetScale \"") + placeable->full_name().c_str() + "\" (" + stream.buffer().c_str() + ")");
        }
    
    }
    
}

//==============================================================================
//==============================================================================

void EdLevelTool::snapToGrid (Vector3 &translation, DTfloat grid)
{
    if (grid <= 0.0F)
        return;
    
    translation.x = std::floor(translation.x / grid + 0.5F) * grid;
    translation.y = std::floor(translation.y / grid + 0.5F) * grid;
    translation.z = std::floor(translation.z / grid + 0.5F) * grid;
    
}

//==============================================================================
//==============================================================================

void EdLevelTool::clearSelection (void)
{
    _selection.clear();
}

void EdLevelTool::setSelection (const std::list<std::shared_ptr<PlugNode>> &s)
{
    clearSelection();
    
    FOR_EACH (i,s) {
        _selection.push_back(*i);
    }
}

//==============================================================================
//==============================================================================


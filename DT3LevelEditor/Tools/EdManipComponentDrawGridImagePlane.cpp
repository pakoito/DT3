//==============================================================================
///	
///	File: EdManipComponentDrawGridImagePlane.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdManipComponentDrawGridImagePlane.hpp"
#include "EdLevelToolEvent.hpp"

// Qt include
// Engine includes

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_TOOL(EdManipComponentDrawGridImagePlane)

//==============================================================================
//==============================================================================

EdManipComponentDrawGridImagePlane::EdManipComponentDrawGridImagePlane (void)
    :   _starting_location_dist (0.0F)
{
    _line_material.setBlendEnable(false);
    _line_material.setDepthEnable(true);
    _line_material.setCullMode(DT3GL_CULL_NONE);
    _line_material.setColor(Color(1.0F,1.0F,1.0F,1.0F));
	_line_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    
    _grey_line_material.setBlendEnable(false);
    _grey_line_material.setDepthEnable(true);
    _grey_line_material.setCullMode(DT3GL_CULL_NONE);
    _grey_line_material.setColor(Color(0.7F,0.7F,0.7F,1.0F));
	_grey_line_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
    
    _shadow_line_material.setBlendEnable(false);
    _shadow_line_material.setDepthEnable(true);
    _shadow_line_material.setCullMode(DT3GL_CULL_NONE);
    _shadow_line_material.setColor(Color(0.1F,0.1F,0.1F,1.0F));
	_shadow_line_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));

    _red_material.setBlendEnable(false);
    _red_material.setDepthEnable(true);
    _red_material.setCullMode(DT3GL_CULL_NONE);
    _red_material.setColor(Color(1.0F,0.0F,0.0F,1.0F));
	_red_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
}

EdManipComponentDrawGridImagePlane::~EdManipComponentDrawGridImagePlane    (void)
{

}

//==============================================================================
//==============================================================================

DTuint EdManipComponentDrawGridImagePlane::toID(DTint x, DTint y)
{    
    return (x & 0xFFFF) << 16 | (y & 0xFFFF);
}

void EdManipComponentDrawGridImagePlane::fromID(DTint id, DTint &x, DTint &y)
{    
    x = (id >> 16) & 0xFFFF;
    y = id & 0xFFFF;
}

//==============================================================================
//==============================================================================

void EdManipComponentDrawGridImagePlane::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    if (getSelection().size() != 1)
        return;
        
    PlaceableObject *placeable = checkedCast<PlaceableObject*>(getSelection().front());
    if (!placeable)
        return;
        
    ComponentDrawGridImagePlane *grid = checkedCast<ComponentDrawGridImagePlane*>( placeable->getComponentByType(ComponentBase::COMPONENT_DRAW) );
    if (!grid)
        return;
        
    DTfloat width, height;
    if (grid->getAspect() > 1.0F) {
        width = 1.0F;
        height = 1.0F / grid->getAspect();
        
    } else if (grid->getAspect() < 1.0F) {
        width = grid->getAspect();
        height = 1.0F;

    } else {
        width = 1.0F;
        height = 1.0F;
    }
                
    ::glPushName(0);
        
    for (DTuint x = 0; x < grid->getNumX(); ++x) {
        for (DTuint y = 0; y < grid->getNumY(); ++y) {
            
            Vector2 pt = grid->getGrid().getPoint(x,y);
            
            pt.x *= width;
            pt.y *= height;
            
            ::glLoadName(toID(x,y));
            drawCube( &_red_material, placeable->getTransform() * Matrix4(Matrix3::identity(), Vector3(pt)) * Matrix4(Matrix3::identity(), Vector3(0.0F,0.0F,0.0F), 1.0F / placeable->getScale() * scale) );
        }
    }
        
    ::glPopName();  // Pop component
}

void EdManipComponentDrawGridImagePlane::drawCube (MaterialResource *mat, Matrix4 transform)
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

//==============================================================================
//==============================================================================

void EdManipComponentDrawGridImagePlane::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    if (getSelection().size() != 1)
        return;
        
    PlaceableObject *placeable = checkedCast<PlaceableObject*>(getSelection().front());
    if (!placeable)
        return;
        
    ComponentDrawGridImagePlane *grid = checkedCast<ComponentDrawGridImagePlane*>( placeable->getComponentByType(ComponentBase::COMPONENT_DRAW) );
    if (!grid)
        return;

    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        LOG_MESSAGE << "Control ID " << event._control_id;
        fromID(event._control_id, _starting_x, _starting_y);
    } else if (event._event_type == EdLevelToolEvent::MOUSE_UP) {
        _starting_x = 0;
        _starting_y = 0;
    }
    
    // Make sure index is valid
    if (_starting_x < 0 && _starting_x >= grid->getNumX() &&
        _starting_y < 0 && _starting_y >= grid->getNumY() )
        return;

    Vector3 forwards = event.getCamera()->getForwards();
    Vector2 pt = grid->getGrid().getPoint(_starting_x,_starting_y);
    
    DTfloat width, height;
    if (grid->getAspect() > 1.0F) {
        width = 1.0F;
        height = 1.0F / grid->getAspect();
        
    } else if (grid->getAspect() < 1.0F) {
        width = grid->getAspect();
        height = 1.0F;

    } else {
        width = 1.0F;
        height = 1.0F;
    }

    pt.x *= width;
    pt.y *= height;
    
    Matrix4 transform = placeable->getTransform();
    Vector3 pt3 = transform * Vector3(pt);

    // Move the manipulator if dragging
    if (event._event_type == EdLevelToolEvent::MOUSE_DRAG) {
  
        Vector3 ray = event.getRayDest() - event.getRaySource();
        DTfloat dist = Vector3::dot(forwards, ray);
        ray *= _starting_location_dist / dist;
        
        Vector2 pos = Vector2(transform.inversed() * (event.getRaySource() + ray));
        pos.x /= width; // Account for aspect ratio
        pos.y /= height;
        
        grid->getGrid().setPoint(_starting_x, _starting_y, pos);
            
    } else {
        _starting_location_dist = Vector3::dot(forwards, pt3 - event.getRaySource());
    }
    
    
}

//==============================================================================
//==============================================================================

Matrix4 EdManipComponentDrawGridImagePlane::getManipulatorTransform  (void) const
{
    return getCombinedTransform();
}

//==============================================================================
//==============================================================================

} // DT3


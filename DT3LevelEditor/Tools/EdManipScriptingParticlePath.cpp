//==============================================================================
///	
///	File: EdManipScriptingParticlePath.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdManipScriptingParticlePath.hpp"
#include "EdLevelToolEvent.hpp"

// Qt include
// Engine includes

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_TOOL(EdManipScriptingParticlePath)

//==============================================================================
//==============================================================================

EdManipScriptingParticlePath::EdManipScriptingParticlePath (void)
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

EdManipScriptingParticlePath::~EdManipScriptingParticlePath    (void)
{

}

//==============================================================================
//==============================================================================

DTuint EdManipScriptingParticlePath::toID(DTuint index, DTuint type)
{
    DTuint id = ( (DTuint)index & 0xFFFFFF) << 8 |
                ( (DTuint)type & 0xFF) << 0; 
    return id;
}

void EdManipScriptingParticlePath::fromID(DTuint id, DTuint &index, DTuint &type)
{
    index = (id >> 8) & 0xFFFFFF;
    type = (id >> 0) & 0xFF;
}

//==============================================================================
//==============================================================================

void EdManipScriptingParticlePath::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    if (getSelection().size() != 1)
        return;
        
    ScriptingParticlePath *particles_object = checkedCast<ScriptingParticlePath*>(getSelection().front());
    if (!particles_object)
        return;
        
    ::glPushName(0);
    
    DTsize num_points = particles_object->getNumPoints();

    System::getRenderer()->activateMaterial(&_line_material);
    
    // Draw Lines
    DrawBatcher b;
    b.batchBegin(&_grey_line_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

    for (DTuint k = 0; k < num_points; ++k) {
        Vector3 pt = particles_object->getPoint(k);
        Vector3 tangent = particles_object->getPointTangent(k);    

        b.vertex(pt);
        b.vertex(pt + tangent);
    }

    b.batchEnd();
    b.flush();


    // Draw Lines
    b.batchBegin(&_line_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

    // Draw curves
    const DTfloat step =  num_points / 100.0F;
    for (DTfloat t = 0.0F; t+step < num_points; t += step) {
        Vector3 p0,p1;
        
        particles_object->interpolate(t,p0);
        particles_object->interpolate(t+step,p1);
    
        b.vertex(p0);
        b.vertex(p1);
    }

    b.batchEnd();
    b.flush();


    // Draw Shadow Lines
    b.batchBegin(&_shadow_line_material, Matrix4::identity(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

    // Draw curves
    for (DTfloat t = 0.0F; t+step < num_points; t += step) {
        Vector3 p0,p1;
        
        particles_object->interpolate(t,p0);
        particles_object->interpolate(t+step,p1);
        
        p0.y = 0.0F;
        p1.y = 0.0F;
    
        b.vertex(p0);
        b.vertex(p1);
    }

    b.batchEnd();
    b.flush();



    for (DTuint k = 0; k < num_points; ++k) {
        Vector3 pt = particles_object->getPoint(k);
        Vector3 tangent = particles_object->getPointTangent(k);    
        
        ::glLoadName(toID(k, VERTEX));
        drawCube(&_red_material, Matrix4(Matrix3::identity(), pt, scale));

        ::glLoadName(toID(k, TANGENT));
        drawCube(&_red_material, Matrix4(Matrix3::identity(), pt+tangent, scale));
    }
    
    
    ::glPopName();  // Pop component    
}

void EdManipScriptingParticlePath::drawCube (MaterialResource *mat, Matrix4 transform)
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

void EdManipScriptingParticlePath::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    if (getSelection().size() != 1)
        return;
        
    ScriptingParticlePath *particles_object = checkedCast<ScriptingParticlePath*>(getSelection().front());
    if (!particles_object)
        return;
        
    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
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
        pt = particles_object->getPoint(_starting_index);
    } else if (_starting_type == TANGENT) {
        pt = particles_object->getPointTangent(_starting_index) + particles_object->getPoint(_starting_index);
    }

    
    // Move the manipulator if dragging
    if (event._event_type == EdLevelToolEvent::MOUSE_DRAG) {
  
        Vector3 ray = event.getRayDest() - event.getRaySource();
        DTfloat dist = Vector3::dot(forwards, ray);
        ray *= _starting_location_dist / dist;

        if (_starting_type == VERTEX)       
            particles_object->setPoint(_starting_index, event.getRaySource() + ray);
        else if (_starting_type == TANGENT) 
            particles_object->setPointTangent(_starting_index, event.getRaySource() + ray - particles_object->getPoint(_starting_index));
            
    } else {
        _starting_location_dist = Vector3::dot(forwards, pt - event.getRaySource());
    }
    
}

//==============================================================================
//==============================================================================

Matrix4 EdManipScriptingParticlePath::getManipulatorTransform  (void) const
{
    return Matrix4::identity();
}

//==============================================================================
//==============================================================================

} // DT3


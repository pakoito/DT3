//==============================================================================
///	
///	File: EdManipComponentDrawPolyImagePlane.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdManipComponentDrawPolyImagePlane.hpp"
#include "EdLevelToolEvent.hpp"

// Qt include
// Engine includes

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_TOOL(EdManipComponentDrawPolyImagePlane)

//==============================================================================
//==============================================================================

EdManipComponentDrawPolyImagePlane::EdManipComponentDrawPolyImagePlane (void)
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

EdManipComponentDrawPolyImagePlane::~EdManipComponentDrawPolyImagePlane    (void)
{

}

//==============================================================================
//==============================================================================

DTuint EdManipComponentDrawPolyImagePlane::toID(DTuint type, DTuint n)
{    
    return (type & 0x0000000F) | ((n + 1) << 4);
}

void EdManipComponentDrawPolyImagePlane::fromID(DTuint id, DTuint &type, DTuint &n)
{    
    n = (id >> 4) - 1;
    type = id & 0x0000000F;
}

//==============================================================================
//==============================================================================

void EdManipComponentDrawPolyImagePlane::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    if (getSelection().size() != 1)
        return;
        
    PlaceableObject *placeable = checkedCast<PlaceableObject*>(getSelection().front());
    if (!placeable)
        return;
        
    ComponentDrawPolyImagePlane *poly = checkedCast<ComponentDrawPolyImagePlane*>( placeable->getComponentByType(ComponentBase::COMPONENT_DRAW) );
    if (!poly)
        return;
        
    DTfloat width, height;
    if (poly->getAspect() > 1.0F) {
        width = 1.0F;
        height = 1.0F / poly->getAspect();
        
    } else if (poly->getAspect() < 1.0F) {
        width = poly->getAspect();
        height = 1.0F;

    } else {
        width = 1.0F;
        height = 1.0F;
    }
    
    if (poly->getFlipHorz()) width *= -1.0F;
    if (poly->getFlipVert()) height *= -1.0F;
            
    Matrix4 transform = placeable->getTransform() * Matrix4(    width,  0.0F,   0.0F,   0.0F,
                                                                0.0F,   height, 0.0F,   0.0F,
                                                                0.0F,   0.0F,   1.0F,   0.0F,
                                                                0.0F,   0.0F,   0.0F,   1.0F);

                    
    ::glPushName(0);
        
    // Draw hulls
	for (DTuint h = 0; h < poly->_hulls.size(); ++h) {
		ComponentDrawPolyImagePlaneHull &hull = *(poly->_hulls[h]);
		
		Vector3 center(0.0F,0.0F,0.0F);
		for (DTint p = 0; p < hull._points.size(); ++p) {
			Vector3 pt = Vector3(poly->_poly_points[hull._points[p]].position);
			center += pt;
		}
		center /= hull._points.size();
		
        // Draw Lines
        DrawBatcher b;
        b.batchBegin(&_grey_line_material, transform, DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

		for (DTint p = 0; p < hull._points.size(); ++p) {
			DTint p_next = (p + 1) % hull._points.size();
		
			Vector3 pt1 = Vector3(poly->_poly_points[hull._points[p]].position);
			pt1 += (center - pt1).normalized() * 1.0F;
			
			Vector3 pt2 = Vector3(poly->_poly_points[hull._points[p_next]].position);
			pt2 += (center - pt2).normalized() * 1.0F;
			
			::glColor4f(0.1F,0.3F,0.1F,0.0F);
			
			b.vertex(Vector3(pt1.x,pt1.y,pt1.z));
			b.vertex(Vector3(pt2.x,pt2.y,pt2.z));
		}
		
        b.batchEnd();
        b.flush();

	}
        
    // Draw Lines
    DrawBatcher b;

	// Draw Connections
    DTuint size = (DTuint) poly->_poly_points.size();
	for (DTuint i = 0; i < size; ++i) {
		Vector3 p0 = Vector3(poly->_poly_points[i].position);
		Vector3 p1 = Vector3(poly->_poly_points[(i+1)%size].position);
	
		::glLoadName(((i+1+1) << 4) | 1);
        
        b.batchBegin(&_grey_line_material, transform, DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);
        
        b.vertex(Vector3(p0.x,p0.y,p0.z));
        b.vertex(Vector3(p1.x,p1.y,p1.z));
        
        b.batchEnd();
        b.flush();

	}
    

	// Draw Handles
	for (DTuint i = 0; i < size; ++i) {
		Vector3 p0 = transform * Vector3(poly->_poly_points[i].position);
	
		::glLoadName(((i+1) << 4) | 2);
        drawCube(&_red_material, Matrix4(Matrix3::identity(), p0, scale));
	}

        
    ::glPopName();  // Pop component
}

void EdManipComponentDrawPolyImagePlane::drawCube (MaterialResource *mat, Matrix4 transform)
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

void EdManipComponentDrawPolyImagePlane::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    if (getSelection().size() != 1)
        return;
        
    PlaceableObject *placeable = checkedCast<PlaceableObject*>(getSelection().front());
    if (!placeable)
        return;
        
    ComponentDrawPolyImagePlane *poly = checkedCast<ComponentDrawPolyImagePlane*>( placeable->getComponentByType(ComponentBase::COMPONENT_DRAW) );
    if (!poly)
        return;

    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        LOG_MESSAGE << "Control ID " << event._control_id;
        fromID(event._control_id, _starting_type, _starting_n);
        
        // Clicked edge
        if (_starting_type == 1) {
            poly->_poly_points.insert(_starting_n, ComponentDrawPolyImagePlanePoint(Vector2(0.0F,0.0F),Texcoord2(0.0F,0.0F)));
		
            // Change selection CP to new point
            _starting_type = 2;
            // _starting_n = _starting_n; // Index stays the same as the edge
        }
    } else if (event._event_type == EdLevelToolEvent::MOUSE_UP) {
        _starting_type = 0;
        _starting_n = 0;
    }
    
    if (_starting_type != 2)
        return;

    Vector3 forwards = event.getCamera()->getForwards();
    Vector2 pt = poly->_poly_points[_starting_n].position;
    
    DTfloat width, height;
    if (poly->getAspect() > 1.0F) {
        width = 1.0F;
        height = 1.0F / poly->getAspect();
        
    } else if (poly->getAspect() < 1.0F) {
        width = poly->getAspect();
        height = 1.0F;

    } else {
        width = 1.0F;
        height = 1.0F;
    }

    if (poly->getFlipHorz()) width *= -1.0F;
    if (poly->getFlipVert()) height *= -1.0F;
            
    Matrix4 transform = placeable->getTransform() * Matrix4(    width,  0.0F,   0.0F,   0.0F,
                                                                0.0F,   height, 0.0F,   0.0F,
                                                                0.0F,   0.0F,   1.0F,   0.0F,
                                                                0.0F,   0.0F,   0.0F,   1.0F);

    pt.x *= width;
    pt.y *= height;
    
    Vector3 pt3 = transform * Vector3(pt);

    // Move the manipulator if dragging
    if (event._event_type != EdLevelToolEvent::MOUSE_DRAG) {
        _starting_location_dist = Vector3::dot(forwards, pt3 - event.getRaySource());
    }
  
    Vector3 ray = event.getRayDest() - event.getRaySource();
    DTfloat dist = Vector3::dot(forwards, ray);
    ray *= _starting_location_dist / dist;
    
    Vector2 pos = Vector2(transform.inversed() * (event.getRaySource() + ray));
    pos.x /= width; // Account for aspect ratio
    pos.y /= height;
    
    poly->_poly_points[_starting_n].position = pos;
        
    poly->calcHulls();
    poly->calcDefaultTexcoords();
        
}

//==============================================================================
//==============================================================================

Matrix4 EdManipComponentDrawPolyImagePlane::getManipulatorTransform  (void) const
{
    return getCombinedTransform();
}

//==============================================================================
//==============================================================================

} // DT3


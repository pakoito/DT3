//==============================================================================
///	
///	File: EdManipComponentUtilPathNodes.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdManipComponentUtilPathNodes.hpp"
#include "EdLevelToolEvent.hpp"

// Qt include
// Engine includes

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_TOOL(EdManipComponentUtilPathNodes)

//==============================================================================
//==============================================================================

EdManipComponentUtilPathNodes::EdManipComponentUtilPathNodes (void)
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

    _green_material.setBlendEnable(false);
    _green_material.setDepthEnable(true);
    _green_material.setCullMode(DT3GL_CULL_NONE);
    _green_material.setColor(Color(0.0F,1.0F,0.0F,1.0F));
	_green_material.setShader(ShaderResource::getShader(FilePath("{editorline.shdr}")));
}

EdManipComponentUtilPathNodes::~EdManipComponentUtilPathNodes    (void)
{

}

//==============================================================================
// Indices are stored like this:
//  0 FFF FFF F
//    n1  n2  type
//==============================================================================

DTint EdManipComponentUtilPathNodes::toID(DTint type, DTint n1, DTint n2)
{
    return (( (n1+1) & 0xFFF) << (4*4)) | (( (n2+1) & 0xFFF) << (4*1)) | (( type & 0x00F) << (4*0));
}

void EdManipComponentUtilPathNodes::fromID(DTint id, DTint &type, DTint &n1, DTint &n2)
{    
    n1 = ((id >> (4*4)) & 0xFFF) - 1;
    n2 = ((id >> (4*1)) & 0xFFF) - 1;
    type = (id >> (4*0)) & 0x00F;
}

//==============================================================================
//==============================================================================

void EdManipComponentUtilPathNodes::draw (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale)
{
    if (getSelection().size() != 1)
        return;
        
    PlaceableObject *placeable = checkedCast<PlaceableObject*>(getSelection().front());
    if (!placeable)
        return;
        
    ComponentUtilPathNodes *nodes = checkedCast<ComponentUtilPathNodes*>( placeable->getComponentByType(ComponentBase::COMPONENT_UTILITY) );
    if (!nodes)
        return;
        
            
    Matrix4 transform = placeable->getTransform();
    Matrix4 camera_orientation = Matrix4(camera->getOrientation());

                    
    ::glPushName(0);
    
    
    LOG_MESSAGE << "Selection: " << _starting_type << " " << _starting_n1 << " " << _starting_n2;
                    
	// Draw Handle Labels
	for (DTint i = 0; i < nodes->_nodes.size(); ++i) {
		Vector3 p0 = transform * Vector3(nodes->_nodes[i].position);
	
		::glLoadName(0);

        Vector3 raster_p0 = camera->projectPointToPixel(p0);
        DrawUtils::drawBitFont(&_red_material, Vector2(raster_p0.x + 4, raster_p0.y - 3), castToString(i));
    }

	// Draw Handles
	for (DTint i = 0; i < nodes->_nodes.size(); ++i) {
		Vector3 p0 = transform * Vector3(nodes->_nodes[i].position);
	
		::glLoadName(toID(HANDLE, i));

        if (_starting_type == HANDLE && _starting_n1 == i )
            drawCube(&_green_material, Matrix4(Matrix3::identity(), p0, scale));
        else
            drawCube(&_red_material, Matrix4(Matrix3::identity(), p0, scale));
        
    }
    
    // Draw Rings
    const DTint NUM_LINES = 32;
    const DTint NUM_CONNECTION_LINES = 32;
    const DTfloat RADIUS = scale * 0.25F;
    
	for (DTint i = 0; i < nodes->_nodes.size(); ++i) {
		Vector3 p0 = transform * Vector3(nodes->_nodes[i].position);
	
		::glLoadName(toID(RING, i));
        
        DrawBatcher b;
        b.batchBegin(&_red_material, placeable->getTransform(), DrawBatcher::BATCH_LINE_LOOP, DrawBatcher::FMT_V);

        for (DTint j = 0; j < NUM_LINES; ++j) {
            DTfloat angle = (DTfloat) j / (DTfloat) NUM_LINES * 2.0F * PI;
            
            DTfloat sin_angle = std::sin(angle);
            DTfloat cos_angle = std::cos(angle);
            
            b.vertex(p0 + camera_orientation * Vector3(cos_angle, sin_angle, 0.0F) * RADIUS);
        }
        
        b.batchEnd();
        b.flush();

	}
    
    // Draw Connections
	for (DTint i = 0; i < nodes->_nodes.size(); ++i) {
	        
        std::vector<DTint> &outgoing = nodes->_nodes[i].outgoing;
        
        for (DTint j = 0; j < outgoing.size(); ++j) {
            Vector3 p0 = transform * Vector3(nodes->_nodes[i].position);
            Vector3 p1 = transform * Vector3(nodes->_nodes[outgoing[j]].position);
            
            if (p0 != p1) {
                Vector3 diff = (p1 - p0).normalized();
                Vector3 perp = Vector3::cross(diff, camera->getOrientation().zAxis()).normalized();
                
                p0 += diff * RADIUS;
                p1 -= diff * RADIUS;
            
                ::glLoadName(toID(CONNECTION, i, outgoing[j]));

                DrawBatcher b;
                
                if (_starting_type == CONNECTION && _starting_n1 == i && _starting_n2 == outgoing[j] )
                    b.batchBegin(&_green_material, placeable->getTransform(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);
                else
                    b.batchBegin(&_red_material, placeable->getTransform(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

                Vector3 last_p = p0;
                
                for (DTint i = 1; i < NUM_CONNECTION_LINES; ++i) {
                    DTfloat t = (DTfloat) i / (NUM_CONNECTION_LINES-1);
                    Vector3 p = (p1-p0) * t + p0;
                    
                    DTfloat bias = scale * (std::cos(t - 0.5F) - std::cos(0.5F)) * 2.0F;
                    
                    b.vertex(last_p);
                    
                    last_p = p + perp*bias;
                    b.vertex(last_p);
                }
                
                b.vertex(p1);
                b.vertex(p1 - (diff + perp) * scale * 0.2F);
                b.vertex(p1);
                b.vertex(p1 - (diff - perp) * scale * 0.2F);
                
                b.batchEnd();
                b.flush();
            }
        }
    
	}
    
    // Draw Connection feedback
    if (_feedback_from_connection >= 0 && _feedback_to_connection >= 0 && _feedback_from_connection != _feedback_to_connection) {
    
        ::glLoadName(0);
        
        LOG_MESSAGE << "Feedbak: " << _feedback_from_connection << " " << _feedback_to_connection;
        
        Vector3 p0 = transform * Vector3(nodes->_nodes[_feedback_from_connection].position);
        Vector3 p1 = transform * Vector3(nodes->_nodes[_feedback_to_connection].position);
        
        if (p0 != p1) {
            Vector3 diff = (p1 - p0).normalized();
            p0 += diff * RADIUS;
            p1 -= diff * RADIUS;

            DrawBatcher b;
            b.batchBegin(&_grey_line_material, placeable->getTransform(), DrawBatcher::BATCH_LINES, DrawBatcher::FMT_V);

            b.vertex(p0);
            b.vertex(p1);

            b.batchEnd();
            b.flush();
        }
    }
        
    ::glPopName();  // Pop component
}

void EdManipComponentUtilPathNodes::drawCube (MaterialResource *mat, Matrix4 transform)
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

void EdManipComponentUtilPathNodes::doEvent (EdLevelToolWindow *parent, const EdLevelToolEvent &event)
{
    if (getSelection().size() != 1)
        return;
        
    PlaceableObject *placeable = checkedCast<PlaceableObject*>(getSelection().front());
    if (!placeable)
        return;
        
    ComponentUtilPathNodes *nodes = checkedCast<ComponentUtilPathNodes*>( placeable->getComponentByType(ComponentBase::COMPONENT_UTILITY) );
    if (!nodes)
        return;
    
    // Key down
    if (event._event_type == EdLevelToolEvent::KEY_DOWN) {
        
        // Delete key
        if (event._key_code == 0x1000003 && _starting_type == HANDLE) {
            nodes->deleteNode(_starting_n1);
            
            _starting_type = 0;
            _starting_n1 = 0;
            _starting_n2 = 0;

            return;
        } else if (event._key_code == 0x1000003 && _starting_type == CONNECTION) {
            nodes->deleteConnection(_starting_n1,_starting_n2);
            
            _starting_type = 0;
            _starting_n1 = 0;
            _starting_n2 = 0;

            return;
        }

    }
        
    // Mouse down
    if (event._event_type == EdLevelToolEvent::MOUSE_DOWN) {
        LOG_MESSAGE << "Control ID " << event._control_id;
        fromID(event._control_id, _starting_type, _starting_n1, _starting_n2);
        
    } else if (event._event_type == EdLevelToolEvent::MOUSE_UP) {
//        _starting_type = 0;
//        _starting_n1 = 0;
//        _starting_n2 = 0;
        
        // Make the connection
        if (_feedback_from_connection >= 0 && _feedback_to_connection >= 0 && _feedback_from_connection != _feedback_to_connection) {
            nodes->addConnection(_feedback_from_connection, _feedback_to_connection);
        }
        
        _feedback_from_connection = -1;
        _feedback_to_connection = -1;
    }
    
    Matrix4 transform = placeable->getTransform();
    Vector3 forwards = event.getCamera()->getForwards();

    // Move the handle
    if (_starting_type == HANDLE) {
    
        Vector3 pt = nodes->_nodes[_starting_n1].position;
                            
        Vector3 pt3 = transform * Vector3(pt);

        // Move the manipulator if dragging
        if (event._event_type != EdLevelToolEvent::MOUSE_DRAG) {
            _starting_location_dist = Vector3::dot(forwards, pt3 - event.getRaySource());
        }
      
        Vector3 ray = event.getRayDest() - event.getRaySource();
        DTfloat dist = Vector3::dot(forwards, ray);
        ray *= _starting_location_dist / dist;
        
        Vector3 pos = transform.inversed() * (event.getRaySource() + ray);
        
        nodes->_nodes[_starting_n1].position = pos;
        
    // Make a connection
    } else if (_starting_type == RING) {
    
        DTfloat closest_i = 0;
        DTfloat closest_distance = std::numeric_limits<DTfloat>::infinity();
    
        // Find the closest node to the cursor
        for (DTint i = 0; i < nodes->_nodes.size(); ++i) {
            Vector3 pt = transform * Vector3(nodes->_nodes[i].position);
        
            DTfloat t = Vector3::dot(forwards, pt - event.getRaySource());
            DTfloat dist = (pt - (event.getRaySource() + forwards * t)).abs2();

            if (dist < closest_distance) {
                closest_distance = dist;
                closest_i = i;
            }
            
        }
        
        _feedback_from_connection = _starting_n1;
        _feedback_to_connection = closest_i;
    
    }
    
    
}

//==============================================================================
//==============================================================================

Matrix4 EdManipComponentUtilPathNodes::getManipulatorTransform  (void) const
{
    return getCombinedTransform();
}

//==============================================================================
//==============================================================================

} // DT3


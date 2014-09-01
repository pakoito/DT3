//==============================================================================
///	
///	File: ComponentDrawCamera.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentDrawCamera.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

namespace {

    const DTfloat THICKNESS = 0.214613F;
    const DTfloat SIDE_THICKNESS = 0.145276F;
    const DTfloat LENS_LENGTH = 0.4F;
    const DTfloat LENS_THICKNESS = 0.2F;

    Vector2 box[] = {
        Vector2(0.497588F,0.0F),
        Vector2(0.497588F,1.275792F),
        Vector2(-0.347154F,1.275792F),
        Vector2(-0.347154F,0.0F),
        Vector2(0.497588F,0.0F)
    };

    Vector2 can[] = {
        Vector2(0.497588F,0.286402F),
        Vector2(0.560251F,0.279191F),
        Vector2(0.685578F,0.264767F),
        Vector2(0.891234F,0.338227F),
        Vector2(1.026228F,0.482607F),
        Vector2(1.113767F,0.664798F),
        Vector2(1.114635F,0.867833F),
        Vector2(1.023624F,1.053824F),
        Vector2(0.900785F,1.183003F),
        Vector2(0.649981F,1.27875F),
        //Vector2(0.630428F,1.183169F),
        Vector2(0.61229F,1.60861F),
        Vector2(0.496101F,1.738196F),
        Vector2(0.319402F,1.870589F),
        Vector2(0.100924F,1.905739F),
        Vector2(-0.0982205F,1.841039F),
        Vector2(-0.263489F,1.68282F),
        Vector2(-0.340588F,1.51251F),
        Vector2(-0.325679F,1.354698F),
        Vector2(-0.318225F,1.275792F) 
    };

    Vector2 side[] = {
        Vector2(-0.342071F,0.25014F),
        Vector2(0.403644F,0.25014F),
        Vector2(0.403644F,1.103254F),
        Vector2(-0.342071F,1.190836F),
        Vector2(-0.342071F,0.25014F)
    };

    Vector2 lens[] = {
        Vector2(LENS_THICKNESS,0.0F),
        Vector2(LENS_THICKNESS,-LENS_LENGTH),
        Vector2(-LENS_THICKNESS,-LENS_LENGTH),
        Vector2(-LENS_THICKNESS,0.0F),
        Vector2(LENS_THICKNESS,0.0F)
    };
}

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentDrawCamera,"Drawing","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentDrawCamera)

IMPLEMENT_PLUG_INFO_INDEX(_material)
IMPLEMENT_PLUG_INFO_INDEX(_shader)
IMPLEMENT_PLUG_INFO_INDEX(_color)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentDrawCamera)

    PLUG_INIT(_material, "Material")
		.set_input(true);

    PLUG_INIT(_shader, "Shader")
		.set_input(true);

	PLUG_INIT(_color,"Color")
		.set_input(true)
		.set_output(true);
      
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentDrawCamera::ComponentDrawCamera (void)
    :   _material           (PLUG_INFO_INDEX(_material)),
        _shader             (PLUG_INFO_INDEX(_shader)),
        _color              (PLUG_INFO_INDEX(_color), Color4f(1.0F,0.0F,0.0F,1.0F))
{  

}
		
ComponentDrawCamera::ComponentDrawCamera (const ComponentDrawCamera &rhs)
    :   ComponentBase       (rhs),
        _material           (rhs._material),
        _shader             (rhs._shader),
		_color              (rhs._color)
{   

}

ComponentDrawCamera & ComponentDrawCamera::operator = (const ComponentDrawCamera &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
        _material = rhs._material;
        _shader = rhs._shader;
        _color = rhs._color;
    }
    return (*this);
}
			
ComponentDrawCamera::~ComponentDrawCamera (void)
{

}

//==============================================================================
//==============================================================================

void ComponentDrawCamera::initialize (void)
{
	ComponentBase::initialize();
    
    
}

//==============================================================================
//==============================================================================

void ComponentDrawCamera::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

    archive->push_domain (class_id());
	
    *archive << ARCHIVE_DATA_ACCESSORS("Material", ComponentDrawCamera::material, ComponentDrawCamera::set_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Shader", ComponentDrawCamera::shader, ComponentDrawCamera::set_shader, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_color, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentDrawCamera::draw (const std::shared_ptr<CameraObject> &camera, const DTfloat lag)
{
    if (!_material.as_ref() || !_shader.as_ref())
        return;
    
    if (color().a_as_float() <= 0)
        return;
        
    CameraObject *placeable = checked_cast<CameraObject*>(owner());
    if (!placeable)
        return;
    
    Color4b c(_color);

    DrawBatcher b;
    b.batch_begin(  camera,
                    _material,
                    _shader,
                    placeable->transform(),
                    DT3GL_PRIM_LINES,
                    DrawBatcher::FMT_V | DrawBatcher::FMT_C);

    // Can
    for (DTuint i = 0; i < ARRAY_SIZE(can); ++i) {        
        b.add().v(THICKNESS, can[i].x, can[i].y).c(c);
        b.add().v(-THICKNESS, can[i].x, can[i].y).c(c);
    }

    for (DTuint i = 0; i < ARRAY_SIZE(can)-1; ++i) {        
        b.add().v(THICKNESS, can[i].x, can[i].y).c(c);
        b.add().v(THICKNESS, can[i+1].x, can[i+1].y).c(c);
        
        b.add().v(-THICKNESS, can[i].x, can[i].y).c(c);
        b.add().v(-THICKNESS, can[i+1].x, can[i+1].y).c(c);
    }
    
    // Box
    for (DTuint i = 0; i < ARRAY_SIZE(box); ++i) {        
        b.add().v(THICKNESS, box[i].x, box[i].y).c(c);
        b.add().v(-THICKNESS, box[i].x, box[i].y).c(c);
    }

    for (DTuint i = 0; i < ARRAY_SIZE(box)-1; ++i) {        
        b.add().v(THICKNESS, box[i].x, box[i].y).c(c);
        b.add().v(THICKNESS, box[i+1].x, box[i+1].y).c(c);
        
        b.add().v(-THICKNESS, box[i].x, box[i].y).c(c);
        b.add().v(-THICKNESS, box[i+1].x, box[i+1].y).c(c);
    }

    // Side
    for (DTuint i = 0; i < ARRAY_SIZE(side); ++i) {        
        b.add().v(THICKNESS, side[i].x, side[i].y).c(c);
        b.add().v(SIDE_THICKNESS+THICKNESS, side[i].x, side[i].y).c(c);
    }

    for (DTuint i = 0; i < ARRAY_SIZE(side)-1; ++i) {        
        b.add().v(THICKNESS, side[i].x, side[i].y).c(c);
        b.add().v(THICKNESS, side[i+1].x, side[i+1].y).c(c);
        
        b.add().v(SIDE_THICKNESS+THICKNESS, side[i].x, side[i].y).c(c);
        b.add().v(SIDE_THICKNESS+THICKNESS, side[i+1].x, side[i+1].y).c(c);
    }
    
    // Lens
    for (DTuint i = 0; i < ARRAY_SIZE(lens); ++i) {        
        b.add().v(LENS_THICKNESS, lens[i].x, lens[i].y).c(c);
        b.add().v(-LENS_THICKNESS, lens[i].x, lens[i].y).c(c);
    }

    for (DTuint i = 0; i < ARRAY_SIZE(lens)-1; ++i) {        
        b.add().v(LENS_THICKNESS, lens[i].x, lens[i].y).c(c);
        b.add().v(LENS_THICKNESS, lens[i+1].x, lens[i+1].y).c(c);
        
        b.add().v(-LENS_THICKNESS, lens[i].x, lens[i].y).c(c);
        b.add().v(-LENS_THICKNESS, lens[i+1].x, lens[i+1].y).c(c);
    }
    
    b.batch_end();
	b.flush();

    // Frustum
    
	placeable->calculate_frustum();
	Matrix4 projection_inv = placeable->projection().inversed();
	
	Vector3 near_p0, near_p1, near_p2, near_p3;
	Vector3 far_p0, far_p1, far_p2, far_p3;
	
	near_p0 = MoreMath::transform_4h(projection_inv, Vector3(-1.0F,-1.0F,-1.0F));
	near_p1 = MoreMath::transform_4h(projection_inv, Vector3(1.0F,-1.0F,-1.0F));
	near_p2 = MoreMath::transform_4h(projection_inv, Vector3(1.0F,1.0F,-1.0F));
	near_p3 = MoreMath::transform_4h(projection_inv, Vector3(-1.0F,1.0F,-1.0F));
	
	far_p0 = MoreMath::transform_4h(projection_inv, Vector3(-1.0F,-1.0F,1.0F));
	far_p1 = MoreMath::transform_4h(projection_inv, Vector3(1.0F,-1.0F,1.0F));
	far_p2 = MoreMath::transform_4h(projection_inv, Vector3(1.0F,1.0F,1.0F));
	far_p3 = MoreMath::transform_4h(projection_inv, Vector3(-1.0F,1.0F,1.0F));

    b.batch_begin(  camera,
                    _material,
                    _shader,
                    placeable->transform(),
                    DT3GL_PRIM_LINE_LOOP,
                    DrawBatcher::FMT_V | DrawBatcher::FMT_C);

	b.add().v(near_p0).c(c);
	b.add().v(near_p1).c(c);
	b.add().v(near_p2).c(c);
	b.add().v(near_p3).c(c);
	b.batch_end();
	b.flush();
	
    b.batch_begin(  camera,
                    _material,
                    _shader,
                    placeable->transform(),
                    DT3GL_PRIM_LINE_LOOP,
                    DrawBatcher::FMT_V | DrawBatcher::FMT_C);

	b.add().v(far_p0).c(c);
	b.add().v(far_p1).c(c);
	b.add().v(far_p2).c(c);
	b.add().v(far_p3).c(c);
	b.batch_end();
	b.flush();
	
    b.batch_begin(  camera,
                    _material,
                    _shader,
                    placeable->transform(),
                    DT3GL_PRIM_LINE_LOOP,
                    DrawBatcher::FMT_V | DrawBatcher::FMT_C);

	b.add().v(near_p0).c(c);
	b.add().v(near_p1).c(c);
	b.add().v(far_p1).c(c);
	b.add().v(far_p0).c(c);
	b.batch_end();
	b.flush();
	
    b.batch_begin(  camera,
                    _material,
                    _shader,
                    placeable->transform(),
                    DT3GL_PRIM_LINE_LOOP,
                    DrawBatcher::FMT_V | DrawBatcher::FMT_C);

	b.add().v(near_p2).c(c);
	b.add().v(near_p3).c(c);
	b.add().v(far_p3).c(c);
	b.add().v(far_p2).c(c);
	b.batch_end();
	b.flush();

    

}

//==============================================================================
//==============================================================================

void ComponentDrawCamera::add_to_owner (ObjectBase *owner)
{
    ComponentBase::add_to_owner(owner);
    
    World *w = owner->world();

    w->register_for_draw(owner, make_callback(this, &type::draw));
}

void ComponentDrawCamera::remove_from_owner (void)
{
    World *w = owner()->world();

    w->unregister_for_draw(owner(), make_callback(this, &type::draw));

    ComponentBase::remove_from_owner();
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ComponentDrawGridImagePlane.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentDrawGridImagePlane.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/ArchivePropertyReaderWriter.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentDrawGridImagePlane,"Drawing","EdManipComponentDrawGridImagePlane")
IMPLEMENT_PLUG_NODE(ComponentDrawGridImagePlane)

IMPLEMENT_PLUG_INFO_INDEX(_material)
IMPLEMENT_PLUG_INFO_INDEX(_shader)
IMPLEMENT_PLUG_INFO_INDEX(_color)
IMPLEMENT_PLUG_INFO_INDEX(_grid)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentDrawGridImagePlane)
        
    PLUG_INIT(_material, "Material")
		.set_input(true);

    PLUG_INIT(_shader, "Shader")
		.set_input(true);
    
    PLUG_INIT(_color, "Color")
		.set_input(true);
        
    PLUG_INIT(_grid, "Grid")
		.set_input(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentDrawGridImagePlane::ComponentDrawGridImagePlane (void)
    :   _material           (PLUG_INFO_INDEX(_material)),
        _shader             (PLUG_INFO_INDEX(_shader)),
        _color              (PLUG_INFO_INDEX(_color), Color4f(1.0F,1.0F,1.0F,1.0F)),
        _grid               (PLUG_INFO_INDEX(_grid)),
        _aspect             (1.0F)
{

}
		
ComponentDrawGridImagePlane::ComponentDrawGridImagePlane (const ComponentDrawGridImagePlane &rhs)
    :   ComponentBase       (rhs),
        _material           (rhs._material),
        _shader             (rhs._shader),
        _color              (rhs._color),
        _grid               (rhs._grid),
        _aspect             (rhs._aspect)
{

}

ComponentDrawGridImagePlane & ComponentDrawGridImagePlane::operator = (const ComponentDrawGridImagePlane &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
        _material = rhs._material;
        _shader = rhs._shader;
        _aspect = rhs._aspect;
        _color = rhs._color;
        _grid = rhs._grid;
    }
    return (*this);
}
			
ComponentDrawGridImagePlane::~ComponentDrawGridImagePlane (void)
{

}

//==============================================================================
//==============================================================================

void ComponentDrawGridImagePlane::initialize (void)
{
	ComponentBase::initialize();
}

//==============================================================================
//==============================================================================

void ComponentDrawGridImagePlane::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

    archive->push_domain (class_id());
	
    *archive << ARCHIVE_DATA_ACCESSORS("Material", ComponentDrawGridImagePlane::material, ComponentDrawGridImagePlane::set_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Shader", ComponentDrawGridImagePlane::shader, ComponentDrawGridImagePlane::set_shader, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_color, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_aspect, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_grid, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA_ACCESSORS("Num_X", ComponentDrawGridImagePlane::num_x, ComponentDrawGridImagePlane::set_num_x, DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Num_Y", ComponentDrawGridImagePlane::num_y, ComponentDrawGridImagePlane::set_num_y, DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentDrawGridImagePlane::set_num_x (const DTuint num_x)
{
    _grid->set_num_x(num_x);
}

const DTuint ComponentDrawGridImagePlane::num_x (void) const
{
    return _grid->num_x();
}

void ComponentDrawGridImagePlane::set_num_y (const DTuint num_y)
{
    _grid->set_num_y(num_y);
}

const DTuint ComponentDrawGridImagePlane::num_y (void) const
{
    return _grid->num_y();
}

//==============================================================================
//==============================================================================

void ComponentDrawGridImagePlane::draw (const std::shared_ptr<CameraObject> &camera, const DTfloat lag)
{
    if (!_material.as_ref() || !_shader.as_ref())
        return;
    
    if (color().a_as_float() <= 0)
        return;
        
    PlaceableObject *placeable = checked_cast<PlaceableObject*>(owner());
    if (!placeable)
        return;
    
    DTfloat width, height;
    if (aspect() > 1.0F) {
        width = 1.0F;
        height = 1.0F / aspect();
        
    } else if (aspect() < 1.0F) {
        width = aspect();
        height = 1.0F;

    } else {
        width = 1.0F;
        height = 1.0F;
    }
    
    DTuint num_x = _grid->num_x();
    DTuint num_y = _grid->num_y();
    
    if (num_x <= 0 || num_y <= 0)
        return;

    Color4b c(_color);
    
    for (DTuint x = 0; x < (num_x-1); ++x) {
        _b.batch_begin(  camera,
                        _material,
                        _shader,
                        placeable->transform(),
                        DT3GL_PRIM_TRI_STRIP,
                        DrawBatcher::FMT_V | DrawBatcher::FMT_T0 | DrawBatcher::FMT_C,
                        4);

        DTfloat x0 = x / (DTfloat)(num_x-1);
        DTfloat x1 = (x+1) / (DTfloat)(num_x-1);
        
        for (DTuint y = 0; y < num_y; ++y) {
            Vector2 p0 = _grid->point(x,y);
            Vector2 p1 = _grid->point(x+1,y);
            
            p0.x *= width;
            p0.y *= height;
            p1.x *= width;
            p1.y *= height;
            
            DTfloat y0 = y / (DTfloat)(num_y-1);
            
            _b.add().v(Vector3(p1)).t0(x1, y0).c(c);
            _b.add().v(Vector3(p0)).t0(x0, y0).c(c);
        }

        _b.batch_end();
        _b.flush();
    }
    
}

//==============================================================================
//==============================================================================

void ComponentDrawGridImagePlane::add_to_owner (ObjectBase *owner)
{
    ComponentBase::add_to_owner(owner);
    
    World *w = owner->world();

    w->register_for_draw(owner, make_callback(this, &type::draw));
}

void ComponentDrawGridImagePlane::remove_from_owner (void)
{
    World *w = owner()->world();

    w->unregister_for_draw(owner(), make_callback(this, &type::draw));

    ComponentBase::remove_from_owner();
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ComponentDrawImagePlane.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentDrawImagePlane.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/ArchivePropertyReaderWriter.hpp"
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

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentDrawImagePlane,"Drawing","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentDrawImagePlane)

IMPLEMENT_PLUG_INFO_INDEX(_material)
IMPLEMENT_PLUG_INFO_INDEX(_shader)
IMPLEMENT_PLUG_INFO_INDEX(_color)
IMPLEMENT_PLUG_INFO_INDEX(_flip_horz)
IMPLEMENT_PLUG_INFO_INDEX(_flip_vert)

IMPLEMENT_PLUG_INFO_INDEX(_tex_mul_s)
IMPLEMENT_PLUG_INFO_INDEX(_tex_mul_t)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentDrawImagePlane)
        
    PLUG_INIT(_material, "Material")
		.set_input(true);

    PLUG_INIT(_shader, "Shader")
		.set_input(true);
    
    PLUG_INIT(_color, "Color")
		.set_input(true);
        
    PLUG_INIT(_flip_horz, "Flip_Horz")
		.set_input(true);
        
    PLUG_INIT(_flip_vert, "Flip_Vert")
		.set_input(true);

    PLUG_INIT(_tex_mul_s, "Tex_Mul_S")
		.set_input(true);

    PLUG_INIT(_tex_mul_t, "Tex_Mul_T")
		.set_input(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentDrawImagePlane::ComponentDrawImagePlane (void)
    :   _material           (PLUG_INFO_INDEX(_material)),
        _shader             (PLUG_INFO_INDEX(_shader)),
        _color              (PLUG_INFO_INDEX(_color), Color4f(1.0F,1.0F,1.0F,1.0F)),
        _aspect             (1.0F),
        _flip_horz          (PLUG_INFO_INDEX(_flip_horz), false),
        _flip_vert          (PLUG_INFO_INDEX(_flip_vert), false),
        _tex_mul_s          (PLUG_INFO_INDEX(_tex_mul_s), 1.0F),
        _tex_mul_t          (PLUG_INFO_INDEX(_tex_mul_t), 1.0F)
{

}
		
ComponentDrawImagePlane::ComponentDrawImagePlane (const ComponentDrawImagePlane &rhs)
    :   ComponentBase       (rhs),
        _material           (rhs._material),
        _shader             (rhs._shader),
        _color              (rhs._color),
        _aspect             (rhs._aspect),
        _flip_horz          (rhs._flip_horz),
        _flip_vert          (rhs._flip_vert),
        _tex_mul_s          (rhs._tex_mul_s),
        _tex_mul_t          (rhs._tex_mul_t)
{

}

ComponentDrawImagePlane & ComponentDrawImagePlane::operator = (const ComponentDrawImagePlane &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
        _material = rhs._material;
        _aspect = rhs._aspect;
        _color = rhs._color;
        
        _flip_horz = rhs._flip_horz;
        _flip_vert = rhs._flip_vert;
        
        _tex_mul_s = rhs._tex_mul_s;
        _tex_mul_t = rhs._tex_mul_t;
    }
    return (*this);
}
			
ComponentDrawImagePlane::~ComponentDrawImagePlane (void)
{

}

//==============================================================================
//==============================================================================

void ComponentDrawImagePlane::initialize (void)
{
	ComponentBase::initialize();
}

//==============================================================================
//==============================================================================

void ComponentDrawImagePlane::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentBase::archive(archive);

    archive->push_domain (class_id ());
	
    if (archive->isa(ArchivePropertyReaderWriter::kind()))
        *archive << ARCHIVE_DATA_ACCESSORS("Material", ComponentDrawImagePlane::material, ComponentDrawImagePlane::set_material_prop, DATA_PERSISTENT | DATA_SETTABLE);
	else
        *archive << ARCHIVE_DATA_ACCESSORS("Material", ComponentDrawImagePlane::material, ComponentDrawImagePlane::set_material, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA_ACCESSORS("Shader", ComponentDrawImagePlane::shader, ComponentDrawImagePlane::set_shader, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_color, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_aspect, DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_flip_horz, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_flip_vert, DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_mul_s, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_tex_mul_t, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

const std::shared_ptr<MaterialResource>& ComponentDrawImagePlane::material (void) const
{
    return _material.as_ref();
}

void ComponentDrawImagePlane::set_material (const std::shared_ptr<MaterialResource> &material)
{
    _material = material;
}

void ComponentDrawImagePlane::set_material_prop (const std::shared_ptr<MaterialResource> &material)
{
    _material = material;
    
//#ifdef DT3_EDITOR
//
//    //
//    // Try to automatically set the aspect ratio
//    //
//
//    if (_material->isValid()) {
//        
//        // Take a stab at the aspect ratio
//        (*_material)->setCurrentUnit(0);
//        
//        Vector3 scale = (*_material)->getScale();
//        if (scale.y > 0.0F) 
//            setAspect(scale.x/scale.y);
//    }
//#endif
}

//==============================================================================
//==============================================================================

const std::shared_ptr<ShaderResource>& ComponentDrawImagePlane::shader (void) const
{
    return _shader.as_ref();
}

void ComponentDrawImagePlane::set_shader (const std::shared_ptr<ShaderResource> &shader)
{
    _shader = shader;
}

//==============================================================================
//==============================================================================

void ComponentDrawImagePlane::draw (const std::shared_ptr<CameraObject> &camera, const DTfloat lag)
{
    if (!_material.as_ref() || !_shader.as_ref())
        return;
        
    if (color().a_as_float() <= 0)
        return;
        
    PlaceableObject *placeable = checked_cast<PlaceableObject*>(owner());
    if (!placeable)
        return;
        
    DTfloat width, height;
    if (_aspect > 1.0F) {
        width = 1.0F;
        height = 1.0F / _aspect;
        
    } else if (_aspect < 1.0F) {
        width = _aspect;
        height = 1.0F;

    } else {
        width = 1.0F;
        height = 1.0F;

    }
    
    DTfloat tc_s_min = 0.0F;
    DTfloat tc_s_max = _tex_mul_s;
    DTfloat tc_t_min = 0.0F;
    DTfloat tc_t_max = _tex_mul_t;
    
    if (_flip_horz) std::swap(tc_s_min, tc_s_max);
    if (_flip_vert) std::swap(tc_t_min, tc_t_max);
    
    Color4b c(_color);
    
    _b.batch_begin(  camera,
                    _material,
                    _shader,
                    placeable->transform(),
                    DT3GL_PRIM_TRI_STRIP,
                    DrawBatcher::FMT_V | DrawBatcher::FMT_T0 | DrawBatcher::FMT_C,
                    4);
    
    _b.add().v(-width,+height,0.0F)  .t0(tc_s_min,tc_t_max)  .c(c);
    _b.add().v(-width,-height,0.0F)  .t0(tc_s_min,tc_t_min)  .c(c);
    _b.add().v(+width,+height,0.0F)  .t0(tc_s_max,tc_t_max)  .c(c);
    _b.add().v(+width,-height,0.0F)  .t0(tc_s_max,tc_t_min)  .c(c);
    
    _b.batch_end();
    _b.flush();
}

//==============================================================================
//==============================================================================

void ComponentDrawImagePlane::add_to_owner (ObjectBase *owner)
{
    ComponentBase::add_to_owner(owner);
    
    World *w = owner->world();

    w->register_for_draw(owner, make_callback(this, &type::draw));
}

void ComponentDrawImagePlane::remove_from_owner (void)
{
    World *w = owner()->world();

    w->unregister_for_draw(owner(), make_callback(this, &type::draw));

    ComponentBase::remove_from_owner();
}

//==============================================================================
//==============================================================================

} // DT3


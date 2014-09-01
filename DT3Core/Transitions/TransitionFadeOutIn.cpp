//==============================================================================
///	
///	File: TransitionFadeOutIn.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Transitions/TransitionFadeOutIn.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(TransitionFadeOutIn)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TransitionFadeOutIn::TransitionFadeOutIn (void)
	:	_transition			(0.0F)
{

}
			
TransitionFadeOutIn::~TransitionFadeOutIn (void)
{

}

//==============================================================================
//==============================================================================


DTboolean TransitionFadeOutIn::begin_new_world (void)
{
	if (!_material) {
		_material = MaterialResource::import_resource(FilePath("{SYS_TRANSITION_MATERIAL}"));
		_shader = ShaderResource::import_resource(FilePath("{SYS_TRANSITION_SHADER}"));
    }

    ASSERT(_shader);
    ASSERT(_material);
		
	return _transition > 0.6F;
}

void TransitionFadeOutIn::end_new_world (void)
{
	DTfloat fade = remap_zero_one (_transition, 0.6F,1.0F);
	   
    Color4f c(0.0F,0.0F,0.0F,1.0F-fade);
    
    _b.batch_begin(nullptr, _material, _shader, Matrix4::identity(), DT3GL_PRIM_TRI_STRIP, DrawBatcher::FMT_V | DrawBatcher::FMT_C);
    
    _b.add().v(-1.0F, 2.0F, 0.0F).c(c);
    _b.add().v(-1.0F, -1.0F, 0.0F).c(c);
    _b.add().v(2.0F, 2.0F, 0.0F).c(c);
    _b.add().v(2.0F, -1.0F, 0.0F).c(c);

    _b.batch_end();
    _b.flush();
}

DTboolean TransitionFadeOutIn::begin_old_world (void)
{
	if (!_material) {
		_material = MaterialResource::import_resource(FilePath("{SYS_TRANSITION_MATERIAL}"));
		_shader = ShaderResource::import_resource(FilePath("{SYS_TRANSITION_SHADER}"));
    }

    ASSERT(_shader);
    ASSERT(_material);

	return _transition <= 0.4F;
}

void TransitionFadeOutIn::end_old_world (void)
{
	DTfloat fade = remap_zero_one (_transition, 0.0F,0.4F);
		  
    Color4f c(0.0F,0.0F,0.0F,fade);
    
    _material->activate();
    
    _b.batch_begin(nullptr, _material, _shader, Matrix4::identity(), DT3GL_PRIM_TRI_STRIP, DrawBatcher::FMT_V | DrawBatcher::FMT_C);
    
    _b.add().v(-1.0F, 2.0F, 0.0F).c(c);
    _b.add().v(-1.0F, -1.0F, 0.0F).c(c);
    _b.add().v(2.0F, 2.0F, 0.0F).c(c);
    _b.add().v(2.0F, -1.0F, 0.0F).c(c);

    _b.batch_end();
    _b.flush();
}

void TransitionFadeOutIn::transition (const DTfloat t)
{
	_transition = t;
}

void TransitionFadeOutIn::draw (void)
{

}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: LightObject.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/LightObject.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(LightObject)
IMPLEMENT_PLUG_NODE(LightObject)

IMPLEMENT_PLUG_INFO_INDEX(_gain_r)
IMPLEMENT_PLUG_INFO_INDEX(_gain_g)
IMPLEMENT_PLUG_INFO_INDEX(_gain_b)
IMPLEMENT_PLUG_INFO_INDEX(_gain_a)
IMPLEMENT_PLUG_INFO_INDEX(_color)
IMPLEMENT_PLUG_INFO_INDEX(_specular_color)
IMPLEMENT_PLUG_INFO_INDEX(_specular_gain)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(LightObject)

	PLUG_INIT(_gain_r,"Gain_R")
		.set_input(true)
		.set_output(true);

	PLUG_INIT(_gain_g,"Gain_G")
		.set_input(true)
		.set_output(true);

	PLUG_INIT(_gain_b,"Gain_B")
		.set_input(true)
		.set_output(true);

	PLUG_INIT(_gain_a,"Gain_A")
		.set_input(true)
		.set_output(true);

	PLUG_INIT(_color,"Color")
		.set_input(true)
		.set_output(true);

	PLUG_INIT(_specular_color,"Specular_Color")
		.set_input(true)
		.set_output(true);

	PLUG_INIT(_specular_gain,"Specular_Gain")
		.set_input(true)
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

LightObject::LightObject (void)
	:	_gain_r				(PLUG_INFO_INDEX(_gain_r), 1.0F),
		_gain_g				(PLUG_INFO_INDEX(_gain_g), 1.0F),
		_gain_b				(PLUG_INFO_INDEX(_gain_b), 1.0F),
		_gain_a				(PLUG_INFO_INDEX(_gain_a), 1.0F),
		_color              (PLUG_INFO_INDEX(_color), Color4f(1.0F,1.0F,1.0F,1.0F)),
        _specular_color     (PLUG_INFO_INDEX(_specular_color), Color4f(1.0F,1.0F,1.0F,1.0F)),
        _specular_gain      (PLUG_INFO_INDEX(_specular_gain), 20.0F)
{  	

}
		
LightObject::LightObject (const LightObject &rhs)
    :   PlaceableObject     (rhs),
		_gain_r             (rhs._gain_r),
		_gain_g             (rhs._gain_g),
		_gain_b             (rhs._gain_b),
		_gain_a             (rhs._gain_a),
		_color              (rhs._color),
		_specular_color     (rhs._specular_color),
        _specular_gain      (rhs._specular_gain)
{   

}

LightObject & LightObject::operator = (const LightObject &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		PlaceableObject::operator = (rhs);
		
		_gain_r = rhs._gain_r;
		_gain_g = rhs._gain_g;
		_gain_b = rhs._gain_b;
		_gain_a = rhs._gain_a;
		
		_color = rhs._color;
		_specular_color = rhs._specular_color;
		_specular_gain = rhs._specular_gain;
	}
    return (*this);
}
			
LightObject::~LightObject (void)
{

}

//==============================================================================
//==============================================================================

void LightObject::archive (const std::shared_ptr<Archive> &archive)
{
    PlaceableObject::archive(archive);

    archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_gain_r, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_gain_g, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_gain_b, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_gain_a, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_color, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_specular_color, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_specular_gain, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

} // DT3


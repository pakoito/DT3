//==============================================================================
///	
///	File: ScriptingMaterial.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingMaterial.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingMaterial,"Materials",NULL)
IMPLEMENT_PLUG_NODE(ScriptingMaterial)

IMPLEMENT_PLUG_INFO_INDEX(_out_material)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingMaterial)

	PLUG_INIT(_out_material,"Out_Material")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingMaterial::ScriptingMaterial (void)
    :   _material		(NULL),
		_out_material	(PLUG_INFO_INDEX(_out_material))
{  

}
		
ScriptingMaterial::ScriptingMaterial (const ScriptingMaterial &rhs)
    :   ScriptingBase	(rhs),
		_material		(rhs._material),
		_out_material	(rhs._out_material)
{   

}

ScriptingMaterial & ScriptingMaterial::operator = (const ScriptingMaterial &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_material = rhs._material;
		_out_material = rhs._out_material;
	}
    return (*this);
}
			
ScriptingMaterial::~ScriptingMaterial (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingMaterial::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingMaterial::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_DATA_ACCESSORS("Material", ScriptingMaterial::material_property, ScriptingMaterial::set_material_property, DATA_PERSISTENT | DATA_SETTABLE);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingMaterial::set_material_property (const std::shared_ptr<MaterialResource> &material)
{
	PROFILER(SCRIPTING);

	_material = material;
	_out_material.set_dirty();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingMaterial::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_material) {
		_out_material = _material;
		_out_material.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


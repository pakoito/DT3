//==============================================================================
///	
///	File: ScriptingGeometry.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingGeometry.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Resources/ResourceTypes/GeometryResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingGeometry,"Resources",NULL)
IMPLEMENT_PLUG_NODE(ScriptingGeometry)

IMPLEMENT_PLUG_INFO_INDEX(_out_geometry)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingGeometry)

	PLUG_INIT(_out_geometry,"Out_Geometry")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingGeometry::ScriptingGeometry (void)
    :   _geometry		(NULL),
		_out_geometry	(PLUG_INFO_INDEX(_out_geometry))
{  

}
		
ScriptingGeometry::ScriptingGeometry (const ScriptingGeometry &rhs)
    :   ScriptingBase	(rhs),
		_out_geometry	(rhs._out_geometry),
		_geometry		(NULL)
{   
	_geometry = rhs._geometry;
	_out_geometry = rhs._out_geometry;
}	

ScriptingGeometry & ScriptingGeometry::operator = (const ScriptingGeometry &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_geometry = rhs._geometry;
		_out_geometry = rhs._out_geometry;
	}
    return (*this);
}
			
ScriptingGeometry::~ScriptingGeometry (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingGeometry::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingGeometry::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_DATA_ACCESSORS("Geometry", ScriptingGeometry::geometry_property, ScriptingGeometry::set_geometry_property, DATA_PERSISTENT | DATA_SETTABLE);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingGeometry::set_geometry_property (const std::shared_ptr<GeometryResource> &geometry)
{
	_geometry = geometry;
	_out_geometry.set_dirty();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingGeometry::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_geometry) {
		_out_geometry = _geometry;
		_out_geometry.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


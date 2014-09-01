//==============================================================================
///	
///	File: ScriptingVector3UnprojectToZ.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3UnprojectToZ.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Objects/CameraObject.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3UnprojectToZ,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3UnprojectToZ)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_camera)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3UnprojectToZ)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
		
	PLUG_INIT(_camera,"Camera")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3UnprojectToZ::ScriptingVector3UnprojectToZ (void)
    :   _in             (PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_camera         (PLUG_INFO_INDEX(_camera)),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F)),
        _z              (0.0F)
{  

}
		
ScriptingVector3UnprojectToZ::ScriptingVector3UnprojectToZ (const ScriptingVector3UnprojectToZ &rhs)
    :   ScriptingBase	(rhs),
		_in             (rhs._in),
		_camera         (rhs._camera),
		_out			(rhs._out),
        _z              (rhs._z)
{   

}

ScriptingVector3UnprojectToZ & ScriptingVector3UnprojectToZ::operator = (const ScriptingVector3UnprojectToZ &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_camera = rhs._camera;
		_out = rhs._out;
        _z = rhs._z;
	}
    return (*this);
}
			
ScriptingVector3UnprojectToZ::~ScriptingVector3UnprojectToZ (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3UnprojectToZ::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3UnprojectToZ::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_z, DATA_PERSISTENT | DATA_SETTABLE);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector3UnprojectToZ::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
        
        if ( *_camera && _camera.has_incoming_connection() && (**_camera).isa(CameraObject::kind()) ) {
            std::shared_ptr<CameraObject> camera = checked_static_cast<CameraObject>(*_camera);
        
            Vector3 ray_src = camera->unproject_point( Vector3(_in->x,_in->y,-1.0F) );
            Vector3 ray_dest = camera->unproject_point( Vector3(_in->x,_in->y,1.0F) );

            DTfloat t = (_z - ray_src.z) / (ray_dest.z - ray_src.z);
            
            _out = (ray_dest - ray_src) * t + ray_src;

        } else {
            _out = Vector3(0.0F,0.0F,0.0F);
        }

		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ScriptingVector3Project.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Project.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Project,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Project)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_camera)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Project)

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

ScriptingVector3Project::ScriptingVector3Project (void)
    :   _in             (PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_camera         (PLUG_INFO_INDEX(_camera), NULL),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{

}
		
ScriptingVector3Project::ScriptingVector3Project (const ScriptingVector3Project &rhs)
    :   ScriptingBase	(rhs),
		_in             (rhs._in),
		_camera         (rhs._camera),
		_out			(rhs._out)
{

}

ScriptingVector3Project & ScriptingVector3Project::operator = (const ScriptingVector3Project &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_camera = rhs._camera;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingVector3Project::~ScriptingVector3Project (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Project::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Project::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingVector3Project::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
        
        if ( *_camera && _camera.has_incoming_connection() && (**_camera).isa(CameraObject::kind()) ) {
            std::shared_ptr<CameraObject> camera = checked_static_cast<CameraObject>(*_camera);
        
            _out = camera->project_point(_in);
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

void ScriptingVector3Project::tick (const DTfloat dt)
{
    _out.set_dirty();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Project::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingVector3Project::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ScriptingCameraPredictor.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingCameraPredictor.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingCameraPredictor,"Camera",NULL)
IMPLEMENT_PLUG_NODE(ScriptingCameraPredictor)

IMPLEMENT_PLUG_INFO_INDEX(_object_translation)
IMPLEMENT_PLUG_INFO_INDEX(_object_velocity)
IMPLEMENT_PLUG_INFO_INDEX(_offset)
IMPLEMENT_PLUG_INFO_INDEX(_out_translation)
IMPLEMENT_PLUG_INFO_INDEX(_out_velocity)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingCameraPredictor)

	PLUG_INIT(_object_translation,"Obj_Translation")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_velocity));

	PLUG_INIT(_object_velocity,"Obj_Velocity")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_velocity));
		
	PLUG_INIT(_offset,"Offset")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_velocity));
		
	PLUG_INIT(_out_translation,"Out_Translation")
		.set_output(true);

	PLUG_INIT(_out_velocity,"Out_Velocity")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingCameraPredictor::ScriptingCameraPredictor (void)
    :   _object_translation	(PLUG_INFO_INDEX(_object_translation), Vector3(0.0F,0.0F,0.0F)),
		_object_velocity	(PLUG_INFO_INDEX(_object_velocity), Vector3(0.0F,0.0F,0.0F)),
		_offset				(PLUG_INFO_INDEX(_offset), Vector3(0.0F,0.0F,75.0F)),
		_out_translation		(PLUG_INFO_INDEX(_out_translation), Vector3(0.0F,0.0F,0.0F)),
		_out_velocity		(PLUG_INFO_INDEX(_out_velocity), Vector3(0.0F,0.0F,0.0F)),
		_prediction_strength(0.5F),
		_prediction_smooth	(0.98F),
		_prediction			(Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingCameraPredictor::ScriptingCameraPredictor (const ScriptingCameraPredictor &rhs)
    :   ScriptingBase		(rhs),
		_object_translation	(rhs._object_translation),
		_object_velocity	(rhs._object_velocity),
		_offset				(rhs._offset),
		_out_translation		(rhs._out_translation),
		_out_velocity		(rhs._out_velocity),
		_prediction_strength(rhs._prediction_strength),
		_prediction_smooth	(rhs._prediction_smooth),
		_prediction			(rhs._prediction)
{   

}

ScriptingCameraPredictor & ScriptingCameraPredictor::operator = (const ScriptingCameraPredictor &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_object_translation = rhs._object_translation;
		_object_velocity = rhs._object_velocity;
		_offset = rhs._offset;
		_out_translation = rhs._out_translation;
		_out_velocity = rhs._out_velocity;
		
		_prediction_strength = rhs._prediction_strength;
		_prediction_smooth = rhs._prediction_smooth;
		_prediction = rhs._prediction;
		
	}
    return (*this);
}
			
ScriptingCameraPredictor::~ScriptingCameraPredictor (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingCameraPredictor::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingCameraPredictor::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	    
	*archive << ARCHIVE_PLUG(_object_translation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_object_velocity, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_offset, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_translation, DATA_PERSISTENT);
					
	*archive << ARCHIVE_DATA(_prediction_strength, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_prediction_smooth, DATA_PERSISTENT | DATA_SETTABLE);
																																												    					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingCameraPredictor::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    _prediction = ((1.0F-_prediction_smooth) * (_object_velocity) * _prediction_strength) + (_prediction_smooth * _prediction);
    
    Vector3 new_translation = (_object_translation) + (_offset) + _prediction;
    
    if (dt > 0.0F)
        _out_velocity = (new_translation - (_out_translation.value_without_compute())) / dt;
    else
        _out_velocity = Vector3(0.0F,0.0F,0.0F);
        
    _out_translation = new_translation;

}

//==============================================================================
//==============================================================================

void ScriptingCameraPredictor::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingCameraPredictor::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


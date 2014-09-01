//==============================================================================
///	
///	File: ScriptingAnimation.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingAnimation.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Animation/AnimationPose.hpp"
#include "DT3Core/Resources/ResourceTypes/AnimationResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingAnimation,"Resources",NULL)
IMPLEMENT_PLUG_NODE(ScriptingAnimation)

IMPLEMENT_PLUG_INFO_INDEX(_time);
IMPLEMENT_PLUG_INFO_INDEX(_out_pose);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingAnimation)

	PLUG_INIT(_time,"Time")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_pose));

	PLUG_INIT(_out_pose,"Out_Pose")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingAnimation::ScriptingAnimation (void)
    :   _animation	(NULL),
		_time		(PLUG_INFO_INDEX(_time), 0.0F),
		_out_pose	(PLUG_INFO_INDEX(_out_pose))
{  

}
		
ScriptingAnimation::ScriptingAnimation (const ScriptingAnimation &rhs)
    :   ScriptingBase	(rhs),
		_animation		(rhs._animation),
		_time			(rhs._time),
		_out_pose		(rhs._out_pose)
{   

}

ScriptingAnimation & ScriptingAnimation::operator = (const ScriptingAnimation &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_animation = rhs._animation;
		_time = rhs._time;
		_out_pose = rhs._out_pose;
	}
    return (*this);
}
			
ScriptingAnimation::~ScriptingAnimation (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingAnimation::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingAnimation::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_DATA_ACCESSORS("Animation", ScriptingAnimation::animation_property, ScriptingAnimation::set_animation_property, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT | DATA_SETTABLE);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingAnimation::set_animation_property (const std::shared_ptr<AnimationResource> &animation)
{
	PROFILER(SCRIPTING);

	_animation = animation;
	_out_pose.set_dirty();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingAnimation::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_pose) {
		
		// Create the pose data
		if (!(*_out_pose))
			_out_pose = AnimationPose::create();
        
		(**_out_pose).update(_time, _animation);
		
		_out_pose.set_clean();
		
		return true;
	} 
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


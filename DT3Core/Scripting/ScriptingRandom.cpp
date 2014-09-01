//==============================================================================
///	
///	File: ScriptingRandom.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingRandom.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingRandom,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingRandom)

IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_PLUG_INFO_INDEX(_continuous)
IMPLEMENT_EVENT_INFO_INDEX(_generate_e)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingRandom)
		
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
	PLUG_INIT(_continuous,"Continuous")
		.set_input(true);

	EVENT_INIT(_generate_e,"Generate")
        .set_input(true)
        .set_event(&ScriptingRandom::eventGenerate);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingRandom::ScriptingRandom (void)
    :   _continuous		(PLUG_INFO_INDEX(_continuous), true),
		_out			(PLUG_INFO_INDEX(_out), 0.0F),
		_generate_e     (EVENT_INFO_INDEX(_generate_e))
{  

}
		
ScriptingRandom::ScriptingRandom (const ScriptingRandom &rhs)
    :   ScriptingBase	(rhs),
        _continuous     (rhs._continuous),
		_out			(rhs._out),
		_generate_e     (EVENT_INFO_INDEX(_generate_e))
{   

}

ScriptingRandom & ScriptingRandom::operator = (const ScriptingRandom &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_continuous = rhs._continuous;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingRandom::~ScriptingRandom (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingRandom::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingRandom::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_PLUG(_continuous, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT | DATA_SETTABLE);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingRandom::eventGenerate (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    _out = MoreMath::random_float();
}

//==============================================================================
//==============================================================================

void ScriptingRandom::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (_continuous) {
        eventGenerate(NULL);
    }
}

//==============================================================================
//==============================================================================

void ScriptingRandom::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingRandom::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


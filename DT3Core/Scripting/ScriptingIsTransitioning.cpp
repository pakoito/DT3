//==============================================================================
///	
///	File: ScriptingIsTransitioning.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingIsTransitioning.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/Application.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingIsTransitioning,"Engine",NULL)
IMPLEMENT_PLUG_NODE(ScriptingIsTransitioning)

IMPLEMENT_PLUG_INFO_INDEX(_is_transitioning)		
IMPLEMENT_EVENT_INFO_INDEX(_begin_transition_e)		
IMPLEMENT_EVENT_INFO_INDEX(_end_transition_e)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingIsTransitioning)
        
	PLUG_INIT(_is_transitioning,"isTransitioning")
		.set_output(true);
                
	EVENT_INIT(_begin_transition_e,"Transition_Beginning")
        .set_output(true);

	EVENT_INIT(_end_transition_e,"Transition_Ended")
        .set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingIsTransitioning::ScriptingIsTransitioning (void)
    :   _is_transitioning       (PLUG_INFO_INDEX(_is_transitioning), false),
        _last_is_transitioning  (false),
        _begin_transition_e     (EVENT_INFO_INDEX(_begin_transition_e)),
        _end_transition_e       (EVENT_INFO_INDEX(_end_transition_e))
{  

}
		
ScriptingIsTransitioning::ScriptingIsTransitioning (const ScriptingIsTransitioning &rhs)
    :   ScriptingBase           (rhs),
		_is_transitioning       (rhs._is_transitioning),
        _last_is_transitioning  (rhs._last_is_transitioning),
        _begin_transition_e     (rhs._begin_transition_e),
        _end_transition_e       (rhs._end_transition_e)
{   

}

ScriptingIsTransitioning & ScriptingIsTransitioning::operator = (const ScriptingIsTransitioning &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_is_transitioning = rhs._is_transitioning;
        _last_is_transitioning = rhs._last_is_transitioning;
	}
    return (*this);
}
			
ScriptingIsTransitioning::~ScriptingIsTransitioning (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingIsTransitioning::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingIsTransitioning::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingIsTransitioning::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    _is_transitioning = System::application()->is_transitioning();
    
    if (_is_transitioning && !_last_is_transitioning)
        _begin_transition_e.send(this);
    else if (!_is_transitioning && _last_is_transitioning)
        _end_transition_e.send(this);
    
    _last_is_transitioning = _is_transitioning;
}

//==============================================================================
//==============================================================================

void ScriptingIsTransitioning::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingIsTransitioning::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ScriptingGlobalPoll.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingGlobalPoll.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingGlobalPoll,"Globals",NULL)
IMPLEMENT_PLUG_NODE(ScriptingGlobalPoll)

IMPLEMENT_PLUG_INFO_INDEX(_global)		
IMPLEMENT_PLUG_INFO_INDEX(_value)		
IMPLEMENT_PLUG_INFO_INDEX(_read)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingGlobalPoll)

	PLUG_INIT(_global,"Global")
		.set_input(true);

	PLUG_INIT(_value,"Value")
		.set_output(true);

	PLUG_INIT(_read,"read")
		.set_input(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingGlobalPoll::ScriptingGlobalPoll (void)
    :   _global         (PLUG_INFO_INDEX(_global), ""),
		_value          (PLUG_INFO_INDEX(_value), ""),
		_read           (PLUG_INFO_INDEX(_read), true),
        _substitute     (false)
{  

}
		
ScriptingGlobalPoll::ScriptingGlobalPoll (const ScriptingGlobalPoll &rhs)
    :   ScriptingBase	(rhs),
        _global         (rhs._global),
        _value          (rhs._value),
        _read           (rhs._read),
        _substitute     (rhs._substitute)
{   

}

ScriptingGlobalPoll & ScriptingGlobalPoll::operator = (const ScriptingGlobalPoll &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_global = rhs._global;
        _value = rhs._value;
        _read = rhs._read;
        _substitute = rhs._substitute;
	}
    return (*this);
}
			
ScriptingGlobalPoll::~ScriptingGlobalPoll (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingGlobalPoll::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingGlobalPoll::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_global, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_value, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_read, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_substitute, DATA_PERSISTENT | DATA_SETTABLE);
		  
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingGlobalPoll::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (_read) {
        if (_substitute) {
            _value = Globals::substitute_global(_global);
        } else {
            _value = Globals::global(_global);
        }
    }
}

//==============================================================================
//==============================================================================

void ScriptingGlobalPoll::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);

    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingGlobalPoll::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


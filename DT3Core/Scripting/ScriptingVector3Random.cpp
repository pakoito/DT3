//==============================================================================
///	
///	File: ScriptingVector3Random.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Random.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Random,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Random)

IMPLEMENT_PLUG_INFO_INDEX(_length)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Random)
		
	PLUG_INIT(_length,"Length")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Random::ScriptingVector3Random (void)
    :   _length			(PLUG_INFO_INDEX(_length), 1.0F),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F))
{  

}
		
ScriptingVector3Random::ScriptingVector3Random (const ScriptingVector3Random &rhs)
    :   ScriptingBase	(rhs),
		_length			(rhs._length),
		_out			(rhs._out)
{   

}

ScriptingVector3Random & ScriptingVector3Random::operator = (const ScriptingVector3Random &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_length = rhs._length;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingVector3Random::~ScriptingVector3Random (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Random::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Random::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	   
	*archive << ARCHIVE_PLUG(_length, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Random::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat theta = 2.0F * std::acos(std::sqrt( MoreMath::random_float() ));
    DTfloat phi = TWO_PI * MoreMath::random_float();
    
    _out = Vector3(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta)) * (_length);
}

//==============================================================================
//==============================================================================

void ScriptingVector3Random::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingVector3Random::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ScriptingGlobalWriter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingGlobalWriter.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingGlobalWriter,"Globals",NULL)
IMPLEMENT_PLUG_NODE(ScriptingGlobalWriter)

IMPLEMENT_PLUG_INFO_INDEX(_global)		
IMPLEMENT_PLUG_INFO_INDEX(_value)		
IMPLEMENT_PLUG_INFO_INDEX(_persistent)		

IMPLEMENT_EVENT_INFO_INDEX(_write_e)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingGlobalWriter)

	PLUG_INIT(_global,"Global")
		.set_input(true);

	PLUG_INIT(_value,"Value")
		.set_input(true);
        
	PLUG_INIT(_persistent,"Persistent")
		.set_input(true);
        
	EVENT_INIT(_write_e,"Write_Event")
        .set_input(true)
        .set_event(&ScriptingGlobalWriter::write);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingGlobalWriter::ScriptingGlobalWriter (void)
    :   _global         (PLUG_INFO_INDEX(_global), ""),
		_value          (PLUG_INFO_INDEX(_value), ""),
		_persistent     (PLUG_INFO_INDEX(_persistent), false),
        _write_e        (EVENT_INFO_INDEX(_write_e))
{  

}
		
ScriptingGlobalWriter::ScriptingGlobalWriter (const ScriptingGlobalWriter &rhs)
    :   ScriptingBase	(rhs),
		_global         (rhs._global),
        _value          (rhs._value),
        _persistent     (rhs._persistent),
        _write_e        (rhs._write_e)
{   

}

ScriptingGlobalWriter & ScriptingGlobalWriter::operator = (const ScriptingGlobalWriter &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_global = rhs._global;
        _value = rhs._value;
        _persistent = rhs._persistent;
	}
    return (*this);
}
			
ScriptingGlobalWriter::~ScriptingGlobalWriter (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingGlobalWriter::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingGlobalWriter::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_global, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_value, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_persistent, DATA_PERSISTENT | DATA_SETTABLE);
		        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingGlobalWriter::write (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    LOG_MESSAGE << "Writing global " << _global << " = " << _value;

    Globals::set_global(_global, _value, (_persistent ? Globals::PERSISTENT : Globals::VOLATILE) );
}

//==============================================================================
//==============================================================================

} // DT3


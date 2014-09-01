//==============================================================================
///	
///	File: ScriptingGlobalReader.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingGlobalReader.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingGlobalReader,"Globals",NULL)
IMPLEMENT_PLUG_NODE(ScriptingGlobalReader)

IMPLEMENT_PLUG_INFO_INDEX(_global)		
IMPLEMENT_PLUG_INFO_INDEX(_value)		

IMPLEMENT_EVENT_INFO_INDEX(_read_e)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingGlobalReader)

	PLUG_INIT(_global,"Global")
		.set_input(true);

	PLUG_INIT(_value,"Value")
		.set_output(true);
                
	EVENT_INIT(_read_e,"Read_Event")
        .set_input(true)
        .set_event(&ScriptingGlobalReader::read);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingGlobalReader::ScriptingGlobalReader (void)
    :   _global         (PLUG_INFO_INDEX(_global), ""),
		_value          (PLUG_INFO_INDEX(_value), ""),
        _read_e         (EVENT_INFO_INDEX(_read_e))
{  

}
		
ScriptingGlobalReader::ScriptingGlobalReader (const ScriptingGlobalReader &rhs)
    :   ScriptingBase	(rhs),
		_global         (rhs._global),
        _value          (rhs._value),
        _read_e         (rhs._read_e)
{   

}

ScriptingGlobalReader & ScriptingGlobalReader::operator = (const ScriptingGlobalReader &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_global = rhs._global;
        _value = rhs._value;
	}
    return (*this);
}
			
ScriptingGlobalReader::~ScriptingGlobalReader (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingGlobalReader::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingGlobalReader::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_global, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_value, DATA_PERSISTENT | DATA_SETTABLE);
		        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingGlobalReader::read (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    _value = Globals::global(_global);
}

//==============================================================================
//==============================================================================

} // DT3


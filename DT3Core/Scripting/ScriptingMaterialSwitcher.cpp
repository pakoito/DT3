//==============================================================================
///	
///	File: ScriptingMaterialSwitcher.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingMaterialSwitcher.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingMaterialSwitcher,"Switches",NULL)
IMPLEMENT_PLUG_NODE(ScriptingMaterialSwitcher)

IMPLEMENT_PLUG_INFO_INDEX(_in1)
IMPLEMENT_PLUG_INFO_INDEX(_in_material1)
IMPLEMENT_PLUG_INFO_INDEX(_in2)
IMPLEMENT_PLUG_INFO_INDEX(_in_material2)
IMPLEMENT_PLUG_INFO_INDEX(_in3)
IMPLEMENT_PLUG_INFO_INDEX(_in_material3)
IMPLEMENT_PLUG_INFO_INDEX(_in4)
IMPLEMENT_PLUG_INFO_INDEX(_in_material4)
IMPLEMENT_PLUG_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingMaterialSwitcher)

	PLUG_INIT(_in1,"In1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_material1,"Material_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in2,"In2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_material2,"Material_2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in3,"In3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_material3,"Material_3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in4,"In4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in_material4,"Material_4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingMaterialSwitcher::ScriptingMaterialSwitcher (void)
    :   _in1			(PLUG_INFO_INDEX(_in1), false),
		_in_material1	(PLUG_INFO_INDEX(_in_material1)),
		_in2			(PLUG_INFO_INDEX(_in2), false),
		_in_material2	(PLUG_INFO_INDEX(_in_material2)),
		_in3			(PLUG_INFO_INDEX(_in3), false),
		_in_material3	(PLUG_INFO_INDEX(_in_material3)),
		_in4			(PLUG_INFO_INDEX(_in4), false),
		_in_material4	(PLUG_INFO_INDEX(_in_material4)),
        _buffer_output  (false),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingMaterialSwitcher::ScriptingMaterialSwitcher (const ScriptingMaterialSwitcher &rhs)
    :   ScriptingBase	(rhs),
		_in1			(rhs._in1),
		_in_material1	(rhs._in_material1),
		_in2			(rhs._in2),
		_in_material2	(rhs._in_material2),
		_in3			(rhs._in3),
		_in_material3	(rhs._in_material3),
		_in4			(rhs._in4),
		_in_material4	(rhs._in_material4),
        _buffer_output  (rhs._buffer_output),
		_out			(rhs._out)
{   

}

ScriptingMaterialSwitcher & ScriptingMaterialSwitcher::operator = (const ScriptingMaterialSwitcher &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in1 = rhs._in1;
		_in_material1 = rhs._in_material1;
		_in2 = rhs._in2;
		_in_material2 = rhs._in_material2;
		_in3 = rhs._in3;
		_in_material3 = rhs._in_material3;
		_in4 = rhs._in4;
		_in_material4 = rhs._in_material4;
        _buffer_output = rhs._buffer_output;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingMaterialSwitcher::~ScriptingMaterialSwitcher (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingMaterialSwitcher::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingMaterialSwitcher::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_in1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_material1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_material2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_material3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_material4, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_buffer_output, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingMaterialSwitcher::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {

		if (_in4)		
			_out = _in_material4;
		else if (_in3)	
			_out = _in_material3;
		else if (_in2)	
			_out = _in_material2;
		else if (_in1)	
			_out = _in_material1;
		else if (!_buffer_output) 
			_out = std::shared_ptr<MaterialResource>();

		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ScriptingCRC32.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingCRC32.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingCRC32,"String",NULL)
IMPLEMENT_PLUG_NODE(ScriptingCRC32)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_checksum)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingCRC32)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_checksum));

	PLUG_INIT(_checksum,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingCRC32::ScriptingCRC32 (void)
    :   _in			(PLUG_INFO_INDEX(_in), ""),
		_checksum	(PLUG_INFO_INDEX(_checksum), 1.0F)
{  

}
		
ScriptingCRC32::ScriptingCRC32 (const ScriptingCRC32 &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_checksum		(rhs._checksum)
{   

}

ScriptingCRC32 & ScriptingCRC32::operator = (const ScriptingCRC32 &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_checksum = rhs._checksum;
	}
    return (*this);
}
			
ScriptingCRC32::~ScriptingCRC32 (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingCRC32::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingCRC32::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_checksum, DATA_PERSISTENT);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingCRC32::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_checksum) {
		_checksum = MoreMath::calc_crc32(_in->c_str(), _in->size());
		_checksum.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


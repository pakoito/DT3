//==============================================================================
///	
///	File: BaseClass.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::atomic<DTuint> BaseClass::_unique_id_counter(0);

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

BaseClass::BaseClass (void)
	:	_streamable				(true)
{  
	new_unique_id();
}
		
BaseClass::BaseClass (const BaseClass &rhs)
    :   _streamable             (rhs._streamable)
{
	new_unique_id();
}

BaseClass & BaseClass::operator = (const BaseClass &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		_streamable = rhs._streamable;
    }
    return (*this);
}
			
BaseClass::~BaseClass (void)
{

}

//==============================================================================
//==============================================================================

void BaseClass::archive    (const std::shared_ptr<Archive> &archive)
{
    archive->push_domain (class_id ());

    DTuint unique_id = _unique_id;	// Just in case the next line does nothing
    *archive << ARCHIVE_DATA(unique_id, DATA_PERSISTENT);
    _unique_id = unique_id;

    // Make sure unique_id will stay unique - all new objects will have an id that is larger than this one.
    if (unique_id > _unique_id_counter)
        _unique_id_counter = unique_id;

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void BaseClass::initialize (void)
{

}

void BaseClass::uninitialize (void)
{

}
		
//==============================================================================
//==============================================================================

void BaseClass::new_unique_id (void)
{
	// Increments counter and returns previous value
    _unique_id = _unique_id_counter.fetch_add(1);
}

//==============================================================================
//==============================================================================

} // DT3


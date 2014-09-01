//==============================================================================
///	
///	File: ArchiveData.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Constants
//==============================================================================

const DTint DATA_PERSISTENT = 1 << 0;
const DTint DATA_SETTABLE = 1 << 1;

const DTint DATA_FLUSH_UI = 1 << 5;
const DTint DATA_DIRTY = 1 << 31;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveData::ArchiveData (void)
	:	_flags      (0),
        _range_min  (0.0F),
        _range_max  (0.0F),
        _bitfield   (false)
{    

}
		
ArchiveData::ArchiveData (const ArchiveData &rhs)
	:	_flags      (rhs._flags),
		_title      (rhs._title),
        _enums      (rhs._enums),
        _range_min  (rhs._range_min),
        _range_max  (rhs._range_max),
        _bitfield   (rhs._bitfield)
{

}

ArchiveData & ArchiveData::operator = (const ArchiveData &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		_flags = rhs._flags;
		_title = rhs._title;
        _enums = rhs._enums;
        _range_min = rhs._range_min;
        _range_max = rhs._range_max;
        _bitfield = rhs._bitfield;
    }
    return (*this);
}
			
ArchiveData::~ArchiveData (void)
{

}

//==============================================================================
//==============================================================================

ArchiveData& ArchiveData::set_title (const std::string &s)
{
    _title = s;
    return (*this);
}

ArchiveData& ArchiveData::add_flags (DTuint flags)
{   
    _flags |= flags;    
    return (*this);
}

ArchiveData& ArchiveData::add_enum (const std::string &s)
{
    _enums.push_back(s);
    return (*this);
}

ArchiveData& ArchiveData::add_range (DTfloat range_min, DTfloat range_max)
{
    _range_min = range_min;
    _range_max = range_max;
    return (*this);
}

ArchiveData& ArchiveData::set_bitfield (DTboolean bitfield)
{
    _bitfield = bitfield;
    return (*this);
}

//==============================================================================
//==============================================================================

} // DT3


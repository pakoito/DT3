//==============================================================================
///	
///	File: ArchiveBinaryBufferReader.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferReader.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/Error.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveBinaryBufferReader::ArchiveBinaryBufferReader (void)
{    

}
			
ArchiveBinaryBufferReader::~ArchiveBinaryBufferReader (void)
{

}

//==============================================================================
//==============================================================================

Archive& ArchiveBinaryBufferReader::operator << (const ArchiveData& data)
{
	// Fill out the current domain with ArchiveData objects
	if (data.flags() & DATA_PERSISTENT) {
		data.set_value(_buffer);
	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchiveBinaryBufferReader::push_domain (const std::string &domain)
{
	DTubyte marker;
	_buffer >> marker;
	ERROR(marker == 0xFF, "Error streaming buffer");
}

void ArchiveBinaryBufferReader::pop_domain (void)
{
	DTubyte marker;
	_buffer >> marker;
	ERROR(marker == 0xAA, "Error streaming buffer");
}

//==============================================================================
//==============================================================================

DTboolean ArchiveBinaryBufferReader::is_reading (void) const
{
	return true;
}

DTboolean ArchiveBinaryBufferReader::is_writing (void) const
{
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


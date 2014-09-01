//==============================================================================
///	
///	File: ArchiveBinaryBufferWriter.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferWriter.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveBinaryBufferWriter::ArchiveBinaryBufferWriter (void)
{    

}
			
ArchiveBinaryBufferWriter::~ArchiveBinaryBufferWriter (void)
{

}

//==============================================================================
//==============================================================================

Archive &	ArchiveBinaryBufferWriter::operator << (const ArchiveData& data)
{
	if (data.flags() & DATA_PERSISTENT) {
		data.value(_buffer);
	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchiveBinaryBufferWriter::push_domain (const std::string &domain)
{
	DTubyte marker = 0xFF;
	_buffer << marker;
}

void ArchiveBinaryBufferWriter::pop_domain (void)
{
	DTubyte marker = 0xAA;
	_buffer << marker;
}

//==============================================================================
//==============================================================================

DTboolean ArchiveBinaryBufferWriter::is_reading (void) const
{
	return false;
}

DTboolean ArchiveBinaryBufferWriter::is_writing (void) const
{
	return true;
}

//==============================================================================
//==============================================================================

} // DT3


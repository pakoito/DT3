//==============================================================================
///	
///	File: ArchiveTextBufferWriter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveTextBufferWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveTextBufferWriter::ArchiveTextBufferWriter (void)
	:	_indent	(0)
{    

}
			
ArchiveTextBufferWriter::~ArchiveTextBufferWriter (void)
{

}

//==============================================================================
//==============================================================================

std::string ArchiveTextBufferWriter::indent (void) const
{
	std::string indent;
	
	for (int i = 0; i < _indent; ++i) {
		indent = indent + "    ";
	}
	
	return indent;
}

//==============================================================================
//==============================================================================

Archive &	ArchiveTextBufferWriter::operator << (const ArchiveData& data)
{
	if (data.flags() & DATA_PERSISTENT) {
		if ( data.title() != "" ) {
			TextBufferStream stream;
			data.value(stream);
		
			_buffer << indent() << data.title() << "=" << stream.buffer() << "\n";
		} else {
			TextBufferStream stream;
			data.value(stream);

			_buffer << indent() << stream.buffer() << "\n";
		}
	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchiveTextBufferWriter::push_domain (const std::string &domain)
{
	_buffer << indent() << domain << " {" << "\n";
	++_indent;
}

void ArchiveTextBufferWriter::pop_domain (void)
{
	--_indent;
	_buffer << indent() << "}" << "\n";
}

//==============================================================================
//==============================================================================

DTboolean ArchiveTextBufferWriter::is_reading (void) const
{
	return false;
}

DTboolean ArchiveTextBufferWriter::is_writing (void) const
{
	return true;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: ArchiveTextWriter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveTextWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/FileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveTextWriter::ArchiveTextWriter (void)
	:	_indent	(0)
{    

}
			
ArchiveTextWriter::~ArchiveTextWriter (void)
{

}

//==============================================================================
//==============================================================================

DTerr ArchiveTextWriter::open (const FilePath &pathname)
{	
    // open the file
	DTerr error = FileManager::open(_outfile, pathname, false);
	if (error != DT3_ERR_NONE)
		return error;

	// write out some header information
	_outfile << Config::engine_version() << "\n";
	_outfile << AppConfig::app_version() << "\n";
	
	// start the contents of the file
	push_domain ("Archive");
	
	return DT3_ERR_NONE;
}

void ArchiveTextWriter::close (void)
{
	pop_domain();
    _outfile.close();
}

//==============================================================================
//==============================================================================

std::string ArchiveTextWriter::indent (void) const
{
	std::string indent;
    indent.resize(_indent*2, ' ');
	return indent;
}

//==============================================================================
//==============================================================================

Archive &	ArchiveTextWriter::operator << (const ArchiveData& data)
{
	if (data.flags() & DATA_PERSISTENT) {
		if ( data.title() != "" ) {
			TextBufferStream stream;
			data.value(stream);
		
			_outfile << indent() << data.title() << "=" << stream.buffer() << "\n";
		} else {
			TextBufferStream stream;
			data.value(stream);

			_outfile << indent() << stream.buffer() << "\n";
		}
	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchiveTextWriter::push_domain (const std::string &domain)
{
	_outfile << indent() << domain << " {" << "\n";
	++_indent;
}

void ArchiveTextWriter::pop_domain (void)
{
	--_indent;
	_outfile << indent() << "}" << "\n";
}

//==============================================================================
//==============================================================================

DTboolean ArchiveTextWriter::is_reading (void) const
{
	return false;
}

DTboolean ArchiveTextWriter::is_writing (void) const
{
	return true;
}

//==============================================================================
//==============================================================================

} // DT3


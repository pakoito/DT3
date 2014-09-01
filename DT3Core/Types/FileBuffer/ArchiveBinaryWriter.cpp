//==============================================================================
///	
///	File: ArchiveBinaryWriter.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveBinaryWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Utility/Time.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveBinaryWriter::ArchiveBinaryWriter (void)
{    

}
			
ArchiveBinaryWriter::~ArchiveBinaryWriter (void)
{

}

//==============================================================================
//==============================================================================

DTerr ArchiveBinaryWriter::open (const FilePath &pathname)
{	
    // open the file
	DTerr error = FileManager::open(_outfile, pathname, false);
	if (error != DT3_ERR_NONE)
		return error;
   
	// write out some header information
	_outfile << static_cast<DTuint>(DT3_BINARY_TOKEN);
    
	_outfile << Config::engine_version();
	_outfile << AppConfig::app_version();
		
	return DT3_ERR_NONE;
}

void ArchiveBinaryWriter::close (void)
{
    _outfile.close();
}

//==============================================================================
//==============================================================================

Archive& ArchiveBinaryWriter::operator << (const ArchiveData& data)
{
	if (data.flags() & DATA_PERSISTENT) {
		data.value(_outfile);
	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchiveBinaryWriter::push_domain (const std::string &domain)
{
	DTubyte marker = 0xFF;
	_outfile << marker;
}

void ArchiveBinaryWriter::pop_domain (void)
{
	DTubyte marker = 0xAA;
	_outfile << marker;
}

//==============================================================================
//==============================================================================

DTboolean ArchiveBinaryWriter::is_reading (void) const
{
	return false;
}

DTboolean ArchiveBinaryWriter::is_writing (void) const
{
	return true;
}

//==============================================================================
//==============================================================================

} // DT3


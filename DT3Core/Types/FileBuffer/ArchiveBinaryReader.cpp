//==============================================================================
///	
///	File: ArchiveBinaryReader.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveBinaryReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/Error.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveBinaryReader::ArchiveBinaryReader (void)
	:	_engine_version		(0),
		_app_version	(0)
{    

}
			
ArchiveBinaryReader::~ArchiveBinaryReader (void)
{

}

//==============================================================================
//==============================================================================

DTerr ArchiveBinaryReader::open (const FilePath &pathname, std::shared_ptr<Progress> progress)
{
    // open the file
	DTerr error = FileManager::open(_infile, pathname, true, progress);
	if (error != DT3_ERR_NONE)
		return error;

	// Check magic number
	DTuint magic;
	_infile >> magic;
	
	if (magic != DT3_BINARY_TOKEN) {
		_infile.close();
		return DT3_ERR_FILE_WRONG_TYPE;
	}

   	// read in some header information
	_infile >> _engine_version;
	
	if (_engine_version >= 85) {
		_infile >> _app_version;
	}
	    
    // Make sure that the App Version number is kept in sync.
    if (AppConfig::app_version() < _app_version)
        return DT3_ERR_ARCHIVE_TOO_NEW;
        
    if (Config::engine_version() < _engine_version)
        return DT3_ERR_ARCHIVE_TOO_NEW;

	return DT3_ERR_NONE;
} 

void ArchiveBinaryReader::close (void)
{
	_infile.close();
}

//==============================================================================
//==============================================================================

Archive& ArchiveBinaryReader::operator << (const ArchiveData& data)
{
	// Fill out the current domain with ArchiveData objects
	if (data.flags() & DATA_PERSISTENT) {
		data.set_value(_infile);
	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchiveBinaryReader::push_domain (const std::string &domain)
{
	DTubyte marker;
	_infile >> marker;
	ERROR(marker == 0xFF, "Error streaming file");
}

void ArchiveBinaryReader::pop_domain (void)
{
	DTubyte marker;
	_infile >> marker;
	ERROR(marker == 0xAA, "Error streaming file");
}

//==============================================================================
//==============================================================================

DTuint ArchiveBinaryReader::get_engine_version (void) const
{
	return _engine_version;
}

DTuint ArchiveBinaryReader::get_app_version (void) const
{
	return _app_version;
}

//==============================================================================
//==============================================================================

DTboolean ArchiveBinaryReader::is_reading (void) const
{
	return true;
}

DTboolean ArchiveBinaryReader::is_writing (void) const
{
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


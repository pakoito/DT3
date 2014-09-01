//==============================================================================
///	
///	File: ArchiveTextReader.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveTextReader.hpp"
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

ArchiveTextReader::ArchiveTextReader (void)
	:	_version		(0),
		_app_version	(0)
{    

}
			
ArchiveTextReader::~ArchiveTextReader (void)
{

}

//==============================================================================
//==============================================================================

void ArchiveTextReader::eat_whitespace (void)
{
	DTcharacter c = _infile.peek();
	while (isspace(c)) {
		_infile.ignore();
		c = _infile.peek();
	}
}

//==============================================================================
//==============================================================================

DTerr ArchiveTextReader::open (const FilePath &pathname, std::shared_ptr<Progress> progress)
{
    // open the file
	DTerr error = FileManager::open(_infile, pathname, true, progress);
	if (error != DT3_ERR_NONE)
		return error;
	
	// read in some header information
	_infile >> _version;

	if (_version >= 85) {
		_infile >> _app_version;
	}
	
    // Make sure that the App Version number is kept in sync.
    if (AppConfig::app_version() < _app_version)
        return DT3_ERR_ARCHIVE_TOO_NEW;
        
    if (Config::engine_version() < _version)
        return DT3_ERR_ARCHIVE_TOO_NEW;

	eat_whitespace();

	// start the contents of the file
	push_domain ("Archive");
	
	return DT3_ERR_NONE;
} 

void ArchiveTextReader::close (void)
{
	_infile.close();
}

//==============================================================================
//==============================================================================

Archive &	 ArchiveTextReader::operator << (const ArchiveData& data)
{
	// Fill out the current domain with ArchiveData objects
	if (data.flags() & DATA_PERSISTENT) {
		eat_whitespace();
		
		// read in a line
		DTcharacter line_buf[2048];
		_infile.line(line_buf, 2048);
        		
		// Parse the line out
		std::string line(line_buf);
        line = MoreStrings::trim(line);
				
        std::string::size_type sep = line.find_first_of('=');
        
        std::string parameter, value;
        if (sep == std::string::npos) {
            parameter = "";
            value = line;
        } else {
            parameter = line.substr(0, sep);
            value = line.substr(sep + 1);
            
            // TODO: Something more robust would be nice
            // Mostly correct way to verify the parameters 
            if (parameter != data.title()) {
                LOG_ERROR << "Error streaming in archive. Got " << parameter << " but expected " << data.title();
                return *this;
            }

        }
		
		TextBufferStream stream(value);
        stream.set_ignore_whitespace(true);
		data.set_value(stream);
	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchiveTextReader::push_domain (const std::string &domain)
{
	std::string d;
	_infile >> d;
	
    if (domain != d) {
        ERRORMSG("push_domain: File cannot be read");
    }
        
	std::string open_brace;
	_infile >> open_brace;
	eat_whitespace();
	
    if (open_brace != "{") {
        ERRORMSG("push_domain: File cannot be read");
    }

}

void ArchiveTextReader::pop_domain (void)
{
	std::string close_brace;
	_infile >> close_brace;
	eat_whitespace();

    if (close_brace != "}") {
        ERRORMSG("pop_domain: File cannot be read");
    }
}

//==============================================================================
//==============================================================================

DTuint ArchiveTextReader::version (void) const
{
	return _version;
}

DTuint ArchiveTextReader::app_version (void) const
{
	return _app_version;
}

//==============================================================================
//==============================================================================

DTboolean ArchiveTextReader::is_reading (void) const
{
	return true;
}

DTboolean ArchiveTextReader::is_writing (void) const
{
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


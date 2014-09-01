//==============================================================================
///	
///	File: ArchiveTextBufferReader.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveTextBufferReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include <cctype>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchiveTextBufferReader::ArchiveTextBufferReader (void)
{    

}
			
ArchiveTextBufferReader::~ArchiveTextBufferReader (void)
{

}

//==============================================================================
//==============================================================================

void ArchiveTextBufferReader::eat_whitespace (void)
{
	DTcharacter c = _buffer.peek();
	while (::isspace(c)) {
		_buffer.ignore();
		c = _buffer.peek();
	}
}

//==============================================================================
//==============================================================================

Archive& ArchiveTextBufferReader::operator << (const ArchiveData& data)
{
	// Fill out the current domain with ArchiveData objects
	if (data.flags() & DATA_PERSISTENT) {
		eat_whitespace();
		
		// read in a line
		DTcharacter line_buf[2048];
		_buffer.line(line_buf, sizeof(line_buf));
        		
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

void ArchiveTextBufferReader::push_domain (const std::string &domain)
{
	std::string d;
	_buffer >> d;
	
    if (domain != d) {
        ERRORMSG("push_domain: File cannot be read");
    }
        
	std::string open_brace;
	_buffer >> open_brace;
	eat_whitespace();
	
    if (open_brace != "{") {
        ERRORMSG("push_domain: File cannot be read");
    }

}

void ArchiveTextBufferReader::pop_domain (void)
{
	std::string close_brace;
	_buffer >> close_brace;
	eat_whitespace();

    if (close_brace != "}") {
        ERRORMSG("pop_domain: File cannot be read");
    }
}

//==============================================================================
//==============================================================================

DTboolean ArchiveTextBufferReader::is_reading (void) const
{
	return true;
}

DTboolean ArchiveTextBufferReader::is_writing (void) const
{
	return false;
}

//==============================================================================
//==============================================================================

} // DT3


//==============================================================================
///	
///	File: FileHandleUncompressedFD.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/FileHandleUncompressedFD.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include <fcntl.h>
#include <unistd.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

FileHandleUncompressedFD::FileHandleUncompressedFD (void)
	:   _file       (0),
        _length		(0),
        _eof        (false)
{

}
			
FileHandleUncompressedFD::~FileHandleUncompressedFD (void)
{
	close();
}

//==============================================================================
//==============================================================================

DTsize FileHandleUncompressedFD::read (DTubyte *buffer, DTsize size)
{		
	DTsize gcount = ::read( _file, (char*)buffer, (size_t) size);
    
    _eof = (gcount == 0);

//	// update Progress
//	if (cache->_last_update > 1024 && cache->_progress) {	
//		cache->_progress->update(cache->_file_g - cache->_start, cache->_length);
//		cache->_last_update = 0;
//	}
	
	return gcount;
}

void FileHandleUncompressedFD::write (const DTubyte *buffer, DTsize size)
{	
	::write( _file, (char*)buffer, (size_t) size);
}
		
//==============================================================================
//==============================================================================

DTsize FileHandleUncompressedFD::p (void)
{
    return (DTsize) ::lseek(_file, 0, SEEK_CUR);
}

DTsize FileHandleUncompressedFD::g (void)
{
    return (DTsize) ::lseek(_file, 0, SEEK_CUR);
}

//==============================================================================
//==============================================================================

DTcharacter FileHandleUncompressedFD::peek (void)
{
    DTcharacter c;
    
    DTsize cur = g();
    read ( (DTubyte *) &c, 1);
    seek_g(cur, FROM_BEGINNING);

    return c;
}

void FileHandleUncompressedFD::ignore (void)
{
    DTubyte b;
    read(&b, 1);
}

DTboolean FileHandleUncompressedFD::is_eof (void)
{
    return _eof;
}

//==============================================================================
//==============================================================================

DTerr FileHandleUncompressedFD::open_file (const FilePath &pathname, DTboolean read)
{
    close();
    
	if (read)	_file = ::open(pathname.full_path().c_str(), O_RDONLY);
	else		_file = ::open(pathname.full_path().c_str(), O_WRONLY);
	
    if (_file < 0) {
        return DT3_ERR_FILE_OPEN_FAILED;
    }
		
	// Set start and length of file
    _length = (DTsize) ::lseek(_file, 0, SEEK_END);
    ::lseek(_file, 0, SEEK_SET);

    _eof = false;

	return DT3_ERR_NONE;
}

DTerr FileHandleUncompressedFD::set_fd (DTint fd)
{
    _file = fd;
    
    if (_file < 0) {
        return DT3_ERR_FILE_OPEN_FAILED;
    }

	// Set start and length of file
    _length = (DTsize) ::lseek(_file, 0, SEEK_END);
    ::lseek(_file, 0, SEEK_SET);

    _eof = false;

	return DT3_ERR_NONE;
}

void FileHandleUncompressedFD::close (void)
{
    if (_file)
        ::close(_file);
    _file = 0;
}		

void FileHandleUncompressedFD::seek_p (DToffset p, Relative r)
{
	switch (r) {
		case FROM_CURRENT:		::lseek(_file, p, SEEK_CUR);		break;
		case FROM_BEGINNING:	::lseek(_file, p, SEEK_SET);		break;
		case FROM_END:			::lseek(_file, p, SEEK_END);		break;
	};
    
}

void FileHandleUncompressedFD::seek_g (DToffset g, Relative r)
{
	switch (r) {
		case FROM_CURRENT:		::lseek(_file, g, SEEK_CUR);		break;
		case FROM_BEGINNING:	::lseek(_file, g, SEEK_SET);		break;
		case FROM_END:			::lseek(_file, g, SEEK_END);		break;
	};
	
//    // update Progress
//    if (cache->_progress) {	
//        cache->_progress->update(cache->_file_g - cache->_start, cache->_length);
//        cache->_last_update = 0;
//    }

    _eof = false;
}

//==============================================================================
//==============================================================================
		
} // DT3

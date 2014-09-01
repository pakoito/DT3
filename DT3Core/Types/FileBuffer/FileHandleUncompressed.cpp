//==============================================================================
///	
///	File: FileHandleUncompressed.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/FileHandleUncompressed.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

FileHandleUncompressed::FileHandleUncompressed (void)
	:	_length		(0)
{

}
			
FileHandleUncompressed::~FileHandleUncompressed (void)
{
	close();
}

//==============================================================================
//==============================================================================

DTsize FileHandleUncompressed::read (DTubyte *buffer, DTsize size)
{		
	_file.read( (char*)buffer, (size_t) size);
	
	DTsize gcount = _file.gcount();
    
    _file.clear();

//	// update Progress
//	if (cache->_last_update > 1024 && cache->_progress) {	
//		cache->_progress->update(cache->_file_g - cache->_start, cache->_length);
//		cache->_last_update = 0;
//	}
	
	return gcount;
}

void FileHandleUncompressed::write (const DTubyte *buffer, DTsize size)
{	
	_file.write( (char*)buffer, (size_t) size);
}
		
//==============================================================================
//==============================================================================

DTerr FileHandleUncompressed::open_file (const FilePath &pathname, DTboolean read)
{
	if (read) {
        _file.open(pathname.full_path().c_str(), std::ios::in | std::ios::binary);

        // set start and length of file
        _file.seekg(0,std::ios::end);
        _length = (DTsize) _file.tellg();
        _file.seekg(0,std::ios::beg);
	} else {
        _file.open(pathname.full_path().c_str(), /*std::ios::in |*/ std::ios::out | std::ios::binary);
        
        _length = 0;
    }
	
    if (!_file) {
        return DT3_ERR_FILE_OPEN_FAILED;
    }
	
	return DT3_ERR_NONE;
}

void FileHandleUncompressed::close (void)
{
	_file.close();
}		

void FileHandleUncompressed::seek_p (DToffset p, Relative r)
{
	switch (r) {
		case FROM_CURRENT:		_file.seekp(p,std::ios::cur);		break;
		case FROM_BEGINNING:	_file.seekp(p,std::ios::beg);		break;
		case FROM_END:			_file.seekp(p,std::ios::end);		break;
	};
}

void FileHandleUncompressed::seek_g (DToffset g, Relative r)
{
	switch (r) {
		case FROM_CURRENT:		_file.seekg(g,std::ios::cur);		break;
		case FROM_BEGINNING:	_file.seekg(g,std::ios::beg);		break;
		case FROM_END:			_file.seekg(g,std::ios::end);		break;
	};
	
//    // update Progress
//    if (cache->_progress) {	
//        cache->_progress->update(cache->_file_g - cache->_start, cache->_length);
//        cache->_last_update = 0;
//    }

}

//==============================================================================
//==============================================================================
		
} // DT3

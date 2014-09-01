//==============================================================================
///	
///	File: FilePath.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/FileManager.hpp"
#include DT3_HAL_INCLUDE_PATH
#include <fstream>

#include "md5.hh"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

FilePath::FilePath (void)
{

}

FilePath::FilePath (const std::string &path)
{
	set_path(path);
}

		
FilePath::FilePath (const FilePath &rhs)
    :   _original_path  (rhs._original_path),
        _path           (rhs._path)
{

}

FilePath::FilePath (FilePath &&rhs)
    :   _original_path  (std::move(rhs._original_path)),
        _path           (std::move(rhs._path))
{

}

FilePath& FilePath::operator = (const FilePath &rhs)
{
	_original_path = rhs._original_path;
	_path = rhs._path;
    
    return (*this);
}

FilePath& FilePath::operator = (FilePath &&rhs)
{
	_original_path = std::move(rhs._original_path);
	_path = std::move(rhs._path);
    
    return (*this);
}
	
FilePath::~FilePath (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const FilePath&v)
{
	s << v.original_path();
	return s;
}

Stream& operator >>(Stream &s, FilePath&v)
{
	std::string original_path;
	s >> original_path;
	v.set_path(original_path);
	
	return s;
}

//==============================================================================
//==============================================================================

void FilePath::recache_path			(void)
{
	_path = Globals::substitute_global(_original_path);

    // Make sure the path separators are correct for this platform

    std::string correct_separator = path_separator();
    std::string wrong_separator;
    
    if (correct_separator == "/")       wrong_separator = "\\";
    else                                wrong_separator = "/";
    
    
    _path = MoreStrings::find_and_replace(_path,wrong_separator,correct_separator);
}

//==============================================================================
//==============================================================================

void FilePath::set_path (const std::string &path)
{
	_original_path = path;
	recache_path();
}

std::string FilePath::full_path (void) const
{
	return _path;
}
		
std::string FilePath::original_path		(void) const
{
	return _original_path;
}

//==============================================================================
//==============================================================================
		
std::string FilePath::path_separator	(void)
{
    return HAL::path_separator();
}

std::string FilePath::package_separator	(void)
{
	return ":";
}

//==============================================================================
//==============================================================================

std::string FilePath::file_name (void) const
{
    std::string path = MoreStrings::filter_out(_path, "{}");

	std::string::size_type last_path_sep = path.find_last_of( FilePath::path_separator() );
	std::string::size_type last_package_sep = path.find_last_of( FilePath::package_separator() );
	std::string::size_type sep;
	
	if (last_package_sep != std::string::npos && last_path_sep != std::string::npos)
		sep = MoreMath::max(last_path_sep, last_package_sep) + 1;
	else if (last_path_sep != std::string::npos)
		sep = last_path_sep + 1;
	else if (last_package_sep != std::string::npos)
		sep = last_package_sep + 1;
	else
		sep = 0;

	return path.substr(sep);
}

std::string FilePath::file_name_no_ext (void) const
{
	std::string file_name_with_ext = file_name();
	return file_name_with_ext.substr(0, file_name_with_ext.find_last_of('.') );
}

//==============================================================================
//==============================================================================

std::string FilePath::directory_path (void) const
{
    std::string path = MoreStrings::filter_out(_path,"{}");

	return path.substr(0, path.find_last_of( FilePath::path_separator() ) );
}

//==============================================================================
//==============================================================================

std::string FilePath::file_ext (void) const
{
    std::string path = MoreStrings::filter_out(_path,"{}");

	std::string::size_type pos = path.find_last_of('.');
	
	if (pos != std::string::npos) {
		std::string extension = path.substr(pos+1);
	
		// make sure extension is in lowercase
		for (std::string::size_type c = 0; c < extension.size(); ++c)
			extension[c] = static_cast<std::string::value_type>(::tolower(extension[c]));
	
		return extension;
	} else {
		return "";
	}
}


//==============================================================================
//==============================================================================

std::string FilePath::package (void) const
{
    std::string path = MoreStrings::filter_out(_path,"{}");

	std::string::size_type last_path_sep = path.find_last_of( FilePath::path_separator() );
	std::string::size_type last_package_sep = path.find_last_of( FilePath::package_separator() );

    if (last_path_sep == std::string::npos)
        last_path_sep = -1;     // +1 added later

	if (last_package_sep == std::string::npos)
		return "";
	else	
		return path.substr(last_path_sep + 1, last_package_sep - last_path_sep - 1);
}

std::string FilePath::package_path (void) const
{
    std::string path = MoreStrings::filter_out(_path,"{}");

	std::string::size_type last_package_sep = path.find_last_of( FilePath::package_separator() );
	if (last_package_sep == std::string::npos)
		return "";
        
    return path.substr(0, last_package_sep);
}

//==============================================================================
//==============================================================================

DTboolean FilePath::in_package (void) const
{
    std::string path = MoreStrings::filter_out(_path,"{}");

	std::string::size_type last_package_sep = path.find_last_of( FilePath::package_separator() );
	std::string::size_type last_path_sep = path.find_last_of( FilePath::path_separator() );
	
	// Extra logic here to make sure the colon is in the last part of the path for packages. Aywhere else
	// and it might be in a dir name which isn't a package.
	if (last_path_sep == std::string::npos)
		return (last_package_sep != std::string::npos);
	else 
		return (last_package_sep != std::string::npos && last_package_sep > last_path_sep);
}

//==============================================================================
//==============================================================================

void FilePath::clear (void)
{
	_original_path.clear();
	_path.clear();
}

//==============================================================================
//==============================================================================

DTuint64 FilePath::modification_date	(void) const
{
    return HAL::modification_date(*this);
}

DTboolean FilePath::is_dir (void) const
{
    return HAL::is_dir(*this);
}

//==============================================================================
//==============================================================================

DTboolean FilePath::exists (void) const
{
	// ASSUMPTION: If it's in a package then it exists.
	if (in_package()) {
		return true;
		
	} else {
		std::ifstream check_file(_path.c_str());
		if (check_file)
			return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

DTboolean FilePath::move (const FilePath &new_path) const
{
    return HAL::move_file(*this, new_path);
}

DTboolean FilePath::del (void) const
{
    return HAL::delete_file(*this);
}

//==============================================================================
//==============================================================================

std::string FilePath::checksum (void) const
{
	// Open the file
	BinaryFileStream file;
	FileManager::open(file, *this, true);
	
	// read the header
	const DTuint CHUNK_SIZE = 1024*16;
	DTubyte chunk[CHUNK_SIZE];
	
    MD5 md5;
	DTsize num = file.read_raw(chunk, CHUNK_SIZE);
    while (num > 0) {
        md5.update(chunk, static_cast<DTuint>(num));
        num = file.read_raw(chunk, CHUNK_SIZE);
    }
    md5.finalize();
    
    return md5.hex_digest();
}

//==============================================================================
//==============================================================================

DTsize FilePath::length (void) const
{
    // Open the file
	BinaryFileStream file;
	FileManager::open(file, *this, true);

    return file.length();
}

//==============================================================================
//==============================================================================

} // DT3

//==============================================================================
///	
///	File: DirectoryLister.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/DirectoryLister.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DirectoryLister::DirectoryLister (void)
{

}

DirectoryLister::DirectoryLister (const FilePath &pathname, DTboolean recursive)
{
	scan_path (pathname, recursive);
}

		
DirectoryLister::DirectoryLister (const DirectoryLister &rhs)
    :   _paths  (rhs._paths)
{

}

DirectoryLister::DirectoryLister (DirectoryLister &&rhs)
    :   _paths  (std::move(rhs._paths))
{

}

DirectoryLister & DirectoryLister::operator = (const DirectoryLister &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {	
        _paths = rhs._paths;
    }
    return (*this);
}

DirectoryLister & DirectoryLister::operator = (DirectoryLister &&rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {	
        _paths = std::move(rhs._paths);
    }
    return (*this);
}
	
DirectoryLister::~DirectoryLister (void)
{

}

//==============================================================================
//==============================================================================

void DirectoryLister::scan_path (const FilePath &pathname, DTboolean recursive)
{
    HAL::list_directory(pathname, recursive, _paths);
}

DTuint DirectoryLister::num_files (void)
{
	return static_cast<DTuint>(_paths.size());
}

std::string DirectoryLister::file_name (DTuint index)
{
	ASSERT(index >= 0 && index < _paths.size());

	return _paths[index].file_name();
}

std::string DirectoryLister::file_name_no_ext (DTuint index)
{
	ASSERT(index >= 0 && index < _paths.size());
	
	return _paths[index].file_name_no_ext();
}

FilePath DirectoryLister::file_path (DTuint index)
{
	ASSERT(index >= 0 && index < _paths.size());
	return _paths[index];
}

DTboolean DirectoryLister::is_directory(DTuint index) const
{
	ASSERT(index >= 0 && index < _paths.size());
	return HAL::is_dir(_paths[index]);
}
		
//==============================================================================
//==============================================================================

} // DT3

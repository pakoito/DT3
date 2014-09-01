//==============================================================================
///	
///	File: Package.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/Package.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/Globals.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Package::Package (void)
{

}
		
Package::~Package (void)
{

}

//==============================================================================
//==============================================================================
		
DTerr Package::load_package (const FilePath &pathname)
{
    LOG_MESSAGE << "Loading package: " << pathname.full_path();
    
    BinaryFileStream file;

    DTerr err = FileManager::open(file, pathname, true, NULL);
    if (err != DT3_ERR_NONE) {
        LOG_MESSAGE << "Unable to load package file " << pathname.full_path();
        return err;
    }
    
    // If the package is itself in another package or archive, there is another offset
    _offset = file.start();
    
    // Read magic number
    DTuint magic;
    file >> magic;
    
    if (magic != 0x5041434B) // i.e. PACK
        return DT3_ERR_FILE_WRONG_TYPE;
    	
	// read the number of paths in the package
	DTuint num_paths;
	file >> num_paths;
	
	// Info for all of the entries in the package
	for (DTuint i = 0; i < num_paths; ++i) {
		Entry entry;		
		file >> entry._name;
		file >> entry._start;
		file >> entry._length;
        file >> entry._uncompressed_length;
		
		_entries[entry._name] = entry;
        
        //LOG_MESSAGE << "Package file: " << entry._name << "   " << (pathname.full_path() + ":" + entry._name);
        LOG_MESSAGE     << "Package file: "
                        << entry._name << "   "
                        << entry._length << "   "
                        << entry._start;
		
		// add the entry to the globals manager
		Globals::set_global(entry._name, pathname.full_path() + ":" + entry._name, Globals::VOLATILE);
	}
    	
	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

DTerr Package::start_and_length(const FilePath &pathname, DTsize &start, DTsize &length, DTsize &uncompressed_length) const
{
	std::map<std::string, Entry>::const_iterator entry = _entries.find(pathname.file_name());
	if (entry == _entries.end())
		return DT3_ERR_FILE_OPEN_FAILED;

    start = entry->second._start + _offset;
    length = entry->second._length;
    uncompressed_length = entry->second._uncompressed_length;
    
    return DT3_ERR_NONE;
}
	
//==============================================================================
//==============================================================================

} // DT3

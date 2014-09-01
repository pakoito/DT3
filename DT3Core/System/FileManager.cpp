//==============================================================================
///	
///	File: FileManager.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Utility/DirectoryLister.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Progress.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/FileBuffer/Package.hpp"
#include "DT3Core/Types/FileBuffer/FileHandleUncompressed.hpp"
#include "DT3Core/Types/FileBuffer/FileHandleCompressed.hpp"
#include "DT3Core/Types/FileBuffer/TextFileStream.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::map<std::string, std::shared_ptr<Package>>                                            FileManager::_packages;
std::shared_ptr<Callback<const FilePath &, std::shared_ptr<FileHandle> &, DTsize &, DTsize &, void *>>    FileManager::_cb;
void                                                                                        *FileManager::_data = NULL;

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(2,FileManager::initialize())
GLOBAL_STATIC_DESTRUCTION(2,FileManager::destroy())

//==============================================================================
//==============================================================================

void FileManager::initialize (void)
{
    scan_files();
}

void FileManager::destroy (void)
{

}

//==============================================================================
//==============================================================================

void FileManager::set_file_open_cb (std::shared_ptr<Callback<const FilePath &, std::shared_ptr<FileHandle> &, DTsize &, DTsize &, void *>> cb, void *data)
{
    _cb = cb;
    _data = data;
}

//==============================================================================
//==============================================================================

void FileManager::scan_files	(void)
{
	DirectoryLister lister;

#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID || DT3_OS == DT3_WINDOWS
    FilePath p("{APPDIR}");
#else
    FilePath p("{APPDIR}/data");
#endif

    
	LOG_MESSAGE << "Scanning files at " << p.full_path();
	lister.scan_path(p);

    DTuint num_files = lister.num_files();
	for (DTuint i = 0; i < num_files; ++i) {
        if (!lister.file_path(i).is_dir()) {
            Globals::set_global(lister.file_name(i), lister.file_path(i).full_path(), Globals::VOLATILE);
         
            LOG_MESSAGE << lister.file_name(i) << "  " << lister.file_path(i).full_path();
        }
	}
	
}

//==============================================================================
//==============================================================================

DTerr FileManager::register_package	(const FilePath &packagename)
{
	std::shared_ptr<Package> package = Package::create();
	
	DTerr err;
	if ((err = package->load_package(packagename)) != DT3_ERR_NONE) {
		return err;
	}
		
	_packages[packagename.file_name()] = package;
	
	return DT3_ERR_NONE;
}

void FileManager::unregister_packages (void)
{
    _packages.clear();
}

//==============================================================================
//==============================================================================

DTerr FileManager::start_and_length  (const FilePath &pathname, FilePath &packagename, DTsize &start, DTsize &length, DTsize &uncompressed_length)
{
    if (!pathname.in_package())
        return DT3_ERR_FILE_NOT_IN_PACKAGE;
        
    // Look up the package and offsets into it
    std::string package_name = pathname.package();
    
    // find the package. If it doesn't exist then we fail.
    auto package = _packages.find(package_name);
    if (package == _packages.end())
        return DT3_ERR_FILE_OPEN_FAILED;
        
    packagename = FilePath(pathname.package_path());
    
    return package->second->start_and_length(pathname, start, length, uncompressed_length);
}

//==============================================================================
//==============================================================================

DTerr FileManager::open_from_fragment (TextFileStream &file, const FilePath &pathname, DTsize start, DTsize length, DTsize uncompressed_length, DTboolean read, std::shared_ptr<Progress> progress)
{

    // Open uncompressed file
    if (length == uncompressed_length) {
        std::shared_ptr<FileHandleUncompressed> file_handle = FileHandleUncompressed::create();

        DTerr err;
        if ((err = file_handle->open_file(pathname, read)) != DT3_ERR_NONE) {
            return err;
        }

        file.open_file_fragment (file_handle, start, file_handle->length(), progress);

        return DT3_ERR_NONE;

    // Open compressed file
    } else {
        std::shared_ptr<FileHandleCompressed> file_handle = FileHandleCompressed::create();

        // For compressed files, the file is decompressed into a buffer in memory. So after it
        // is opened, it's treated like a regular file (except coming from memory) instead of a file
        // coming from a package.

        DTerr err;
        if ((err = file_handle->open_file(pathname, start, length, uncompressed_length)) != DT3_ERR_NONE) {
            return err;
        }
        
        file.open_file_fragment(file_handle, 0, uncompressed_length, progress);

        return DT3_ERR_NONE;
    }
}

DTerr FileManager::open_from_fragment (BinaryFileStream &file, const FilePath &pathname, DTsize start, DTsize length, DTsize uncompressed_length, DTboolean read, std::shared_ptr<Progress> progress)
{

    // Open uncompressed file
    if (length == uncompressed_length) {
        std::shared_ptr<FileHandleUncompressed> file_handle = FileHandleUncompressed::create();

        DTerr err;
        if ((err = file_handle->open_file(pathname, read)) != DT3_ERR_NONE) {
            return err;
        }

        file.open_file_fragment (file_handle, start, file_handle->length(), progress);

        return DT3_ERR_NONE;
        
    // Open compressed file
    } else {
        std::shared_ptr<FileHandleCompressed> file_handle = FileHandleCompressed::create();

        // For compressed files, the file is decompressed into a buffer in memory. So after it
        // is opened, it's treated like a regular file (except coming from memory) instead of a file
        // coming from a package.

        DTerr err;
        if ((err = file_handle->open_file(pathname, start, length, uncompressed_length)) != DT3_ERR_NONE) {
            return err;
        }
        
        file.open_file_fragment(file_handle, 0, uncompressed_length, progress);

        return DT3_ERR_NONE;
    }

}

//==============================================================================
//==============================================================================

DTerr FileManager::open (TextFileStream &file, const FilePath &pathname, DTboolean read, std::shared_ptr<Progress> progress)
{

    // Call the callback if it exists
    if (_cb) {
    
        std::shared_ptr<FileHandle> file_handle;
        DTsize start,length;
    
        (*_cb)(pathname, file_handle, start, length, _data);
        
        if (file_handle) {
            file.open_file_fragment(file_handle, start, length, progress);
            return DT3_ERR_NONE;
        }
    
    }

	if (pathname.in_package()) {
        ASSERT(read);

        FilePath package_name;
        DTsize start,length,uncompressed_length;

        DTerr err = start_and_length (pathname, package_name, start, length, uncompressed_length);
        if (err != DT3_ERR_NONE)
            return err;

        open_from_fragment (file, package_name, start, length, uncompressed_length, read, progress);

	} else {
        open_from_fragment (file, pathname, 0, -1, -1, read, progress);
	}
    
    // File is opened
    if (progress)   
        progress->started();
	
	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

DTerr FileManager::open (BinaryFileStream &file, const FilePath &pathname, DTboolean read, std::shared_ptr<Progress> progress)
{
    // Call the callback if it exists
    if (_cb) {
    
        std::shared_ptr<FileHandle> file_handle;
        DTsize start,length;
    
        (*_cb)(pathname, file_handle, start, length, _data);
        
        if (file_handle) {
            file.open_file_fragment(file_handle, start, length, progress);
            return DT3_ERR_NONE;
        }
    
    }

	if (pathname.in_package()) {
        ASSERT(read);

        FilePath package_name;
        DTsize start,length,uncompressed_length;

        DTerr err = start_and_length (pathname, package_name, start, length, uncompressed_length);
        if (err != DT3_ERR_NONE)
            return err;

        open_from_fragment (file, package_name, start, length, uncompressed_length, read, progress);

	} else {
        open_from_fragment (file, pathname, 0, -1, -1, read, progress);
	}

    // File is opened
    if (progress)   
        progress->started();

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

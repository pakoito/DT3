//==============================================================================
///	
///	File: FileHandleUncompressedFD_test.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/UnitTest.hpp"
#include "DT3Core/System/Factory.hpp"

#if DT3_UNIT_TESTS

#include <fstream>
#include <fcntl.h>
#include <unistd.h>

#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/Configure.hpp"
#include "DT3Core/Types/FileBuffer/FileHandleUncompressedFD.hpp"
#include "DT3Core/Types/FileBuffer/FileHandleCompressedFD.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/FileBuffer/Package.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class FileHandleUncompressedFD_test: public UnitTest {
    public:
        DEFINE_TYPE(FileHandleUncompressedFD_test,UnitTest);
        DEFINE_CREATE
        
        virtual void    run_test       (void);
        static void     file_open_cb   (const FilePath &path, std::shared_ptr<FileHandle> &file_handle, DTsize &start, DTsize &length, void *data);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (FileHandleUncompressedFD_test)

//==============================================================================
// Implement the test
//==============================================================================

void FileHandleUncompressedFD_test::file_open_cb (const FilePath &path, std::shared_ptr<FileHandle> &file_handle, DTsize &start, DTsize &length, void *data)
{
    if (path.full_path() == "Asset@Kaiser-debug.apk") {
        std::string pp = "/Users/tod/Documents/GameProjects/kaiser/trunk/install/data/Kaiser-debug.apk";
        DTint fd = ::open (pp.c_str(), 0);
        auto fh = FileHandleUncompressedFD::create();
        fh->set_fd(fd);

        file_handle = fh;
        start = 3768;
        length = 3209107;

    } else if (path.full_path() == "Asset@config.txt") {
        std::string pp = "/Users/tod/Documents/GameProjects/kaiser/trunk/install/data/Kaiser-debug.apk";
        DTint fd = ::open (pp.c_str(), 0);
        auto fh = FileHandleUncompressedFD::create();
        fh->set_fd(fd);

        file_handle = fh;
        start = 52;
        length = 3668;

    } else if (path.in_package()) {
        // Opening package file

        FilePath packagename;
        DTsize file_start, file_length, file_uncompressed_length;

        FileManager::start_and_length(path, packagename, file_start, file_length, file_uncompressed_length);

        if (file_length == file_uncompressed_length) {
            std::string pp = "/Users/tod/Documents/GameProjects/kaiser/trunk/install/data/Kaiser-debug.apk";
            DTint fd = ::open (pp.c_str(), 0);
            auto fh = FileHandleUncompressedFD::create();
            fh->set_fd(fd);

            file_handle = fh;
            start = file_start;
            length = file_length;

        } else {
            std::string pp = "/Users/tod/Documents/GameProjects/kaiser/trunk/install/data/Kaiser-debug.apk";
            DTint fd = ::open (pp.c_str(), 0);
            auto fh = FileHandleCompressedFD::create();
            fh->set_fd(fd,file_start,file_length,file_uncompressed_length);

            file_handle = fh;
            start = 0;
            length = file_uncompressed_length;
        }
    }
}

void FileHandleUncompressedFD_test::run_test (void)
{
//    std::string s("ABCDEFG");
//
//    // Build a test file
//    FilePath pathname("text.txt");
//
//    // open the file
//    std::ofstream outfile;
//
//    outfile.open(pathname.full_path().c_str());
//    outfile << s;
//    outfile.close();

    Globals::set_global("Kaiser-debug.apk", "Asset@Kaiser-debug.apk", Globals::VOLATILE);
    Globals::set_global("config.txt", "Asset@config.txt", Globals::VOLATILE);

    FileManager::set_file_open_cb(make_callback(&FileHandleUncompressedFD_test::file_open_cb), NULL);

//    //
//    // Do Test
//    //
//
//    BinaryFileStream infile;
//	DTerr error = FileManager::open(infile, pathname, true, NULL);
//    TEST_ASSERTION(error == DT3_ERR_NONE);
//
//    DTubyte buffer[1024];
//	DTsize size = infile.read_raw(buffer, s.length());
//    TEST_ASSERTION(size == s.length());

    FileManager::register_package (FilePath("{Kaiser-debug.apk}"));
    Configure::import_config (FilePath("{config.txt}"));


    FileManager::set_file_open_cb(NULL,NULL);

}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


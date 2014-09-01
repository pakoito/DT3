//==============================================================================
///	
///	File: FilePath_test.cpp
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

#include "FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class FilePath_test: public UnitTest {
    public:
        DEFINE_TYPE(FilePath_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (FilePath_test)

//==============================================================================
// Implement the test
//==============================================================================

void FilePath_test::run_test (void)
{
	FilePath path1("/a/b/c/package.pack:file.ext");
	LOG_MESSAGE << path1.package();
	LOG_MESSAGE << path1.file_name();
    TEST_ASSERTION(path1.package() == "package.pack");
    TEST_ASSERTION(path1.file_name() == "file.ext");
    TEST_ASSERTION(path1.file_ext() == "ext");

	FilePath path2("/a/b/c/file.ext");
	LOG_MESSAGE << path2.package();
	LOG_MESSAGE << path2.file_name();
    TEST_ASSERTION(path2.package() == "");
    TEST_ASSERTION(path2.file_name() == "file.ext");
    TEST_ASSERTION(path2.file_ext() == "ext");

	FilePath path3("package.pack:file.ext");
	LOG_MESSAGE << path3.package();
	LOG_MESSAGE << path3.file_name();
    TEST_ASSERTION(path3.package() == "package.pack");
    TEST_ASSERTION(path3.file_name() == "file.ext");
    TEST_ASSERTION(path3.file_ext() == "ext");

	FilePath path4("{file.ext}");
	LOG_MESSAGE << path4.package();
	LOG_MESSAGE << path4.file_name();
    TEST_ASSERTION(path4.package() == "");
    TEST_ASSERTION(path4.file_name() == "file.ext");
    TEST_ASSERTION(path4.file_ext() == "ext");
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


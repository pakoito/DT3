//==============================================================================
///	
///	File: URL_test.cpp
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

#include "URL.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class URL_test: public UnitTest {
    public:
        DEFINE_TYPE(URL_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (URL_test)

//==============================================================================
// Implement the test
//==============================================================================

void URL_test::run_test (void)
{
	URL url1("http://www.smellslikedonkey.com:123/foo/bar.html");
	
	TEST_ASSERTION(url1.protocol() == "http");
	TEST_ASSERTION(url1.hostname() == "www.smellslikedonkey.com");
	TEST_ASSERTION(url1.port() == 123);
	
	URL url2("http://www.smellslikedonkey.com/foo/bar.html");
	
	TEST_ASSERTION(url2.hostname() == "www.smellslikedonkey.com");
	TEST_ASSERTION(url2.port() == 0);


    std::string some_url = "This is a field was it clear (already)?";
    std::string encoded_url = URL::encode_URL(some_url);
    std::string decoded_url = URL::decode_URL(encoded_url);
    
	TEST_ASSERTION(encoded_url == "This%20is%20a%20field%20was%20it%20clear%20%28already%29%3F");
	TEST_ASSERTION(some_url == decoded_url);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


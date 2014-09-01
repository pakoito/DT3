//==============================================================================
///	
///	File: Globals_test.cpp
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

#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include <thread>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class Globals_test: public UnitTest {
    public:
        DEFINE_TYPE(Globals_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (Globals_test)

//==============================================================================
// Implement the test
//==============================================================================

namespace {

void do_thread (void)
{			        
    for (DTuint i = 0; i < 100; ++i) {

        DTuint rr = MoreMath::random_int() % 25;
        DTuint rw = MoreMath::random_int() % 25;
        
        Globals::set_global(MoreStrings::cast_to_string(rw), MoreStrings::cast_to_string(rw), Globals::VOLATILE);
        std::string s1 = Globals::global(MoreStrings::cast_to_string(rr));
        std::string s2 = Globals::substitute_global("{" + MoreStrings::cast_to_string(rr) + "}");
        
    }
    
    LOG_MESSAGE << "Thread finished";
}
		  
};


void Globals_test::run_test (void)
{
	Globals::set_global("TEST_A","Foo{TEST_B}",Globals::VOLATILE);
	Globals::set_global("TEST_B","Bar",Globals::VOLATILE);
	Globals::set_global("TEST_C","1",Globals::VOLATILE);
	Globals::set_global("TEST_D","0",Globals::VOLATILE);
	Globals::set_global("TEST_E","123.456",Globals::VOLATILE);

	Globals::set_global("TEST_F","HelloThere",Globals::PERSISTENT_OBFUSCATED);
	Globals::set_global("TEST_G","HelloThere",Globals::PERSISTENT_OBFUSCATED);
	
	char blah[] = "abcdefghijklmnopqrstuvwxyz";
	std::string ashex = MoreStrings::to_hex_string(blah, sizeof(blah));
	MoreStrings::from_hex_string(ashex, blah, sizeof(blah));

	
	std::string substitution = Globals::substitute_global("Blah{TEST_A}Blah{TEST_B}");
	TEST_ASSERTION(substitution == "BlahFooBarBlahBar");
	
	TEST_ASSERTION(MoreStrings::cast_from_string<DTboolean>(Globals::global("TEST_C")) == true);
	TEST_ASSERTION(MoreStrings::cast_from_string<DTboolean>(Globals::global("TEST_D")) != true);
	
	TEST_ASSERTION(MoreStrings::cast_from_string<DTfloat>(Globals::global("TEST_E")) == 123.456F);
    
    
    // Test thread safety
    
    std::thread t1(do_thread);
    std::thread t2(do_thread);
    std::thread t3(do_thread);
    std::thread t4(do_thread);
    std::thread t5(do_thread);
    std::thread t6(do_thread);
    std::thread t7(do_thread);
    std::thread t8(do_thread);
    std::thread t9(do_thread);
    std::thread t10(do_thread);
    
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	t10.join();
	
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


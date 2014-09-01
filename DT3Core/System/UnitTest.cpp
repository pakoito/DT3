//==============================================================================
///	
///	File: UnitTest.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/UnitTest.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

extern std::map<std::string, std::shared_ptr<CreatorBase>>& unit_test_map(void);

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_UNIT_TEST(UnitTest)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

UnitTest::UnitTest (void)
{  

}
		
UnitTest::UnitTest (const UnitTest &rhs)
    :   BaseClass(rhs)
{   

}

UnitTest & UnitTest::operator = (const UnitTest &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		BaseClass::operator = (rhs);
    }
    return (*this);
}
			
UnitTest::~UnitTest (void)
{

}

//==============================================================================
//==============================================================================

void UnitTest::run_all_tests (void)
{
	LOG_MESSAGE << "Running all unit tests...";
	
	std::map<std::string, std::shared_ptr<CreatorBase>>& tests = unit_test_map();
	
	FOR_EACH (i,tests) {
		run_one_test(i->first);
	}

	LOG_MESSAGE << "Finished all unit tests.";
}

//==============================================================================
//==============================================================================

void UnitTest::run_one_test (std::string test)
{
	// See if the name was a valid test
	std::shared_ptr<BaseClass> utb = Factory::create_unit_test(test);
	if (!utb || !utb->isa(UnitTest::kind())) {
	
		// If not, try again with "_test" appended
		utb = Factory::create_unit_test(test+"_test");
		if (!utb || !utb->isa(UnitTest::kind())) {
		
			LOG_MESSAGE << "Cannot find unit test: " << test;
			return;
			
		}
	}
	
	// run the test
	std::shared_ptr<UnitTest> ut = checked_static_cast<UnitTest>(utb);
	
	LOG_MESSAGE << "================================================================================";
	LOG_MESSAGE << "Running unit test: " << test;
	ut->run_test();
	LOG_MESSAGE << "Finished unit test: " << test;

}

//==============================================================================
//==============================================================================

} // DT3


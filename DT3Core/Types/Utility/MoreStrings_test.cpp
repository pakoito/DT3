//==============================================================================
///	
///	File: MoreStrings_test.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/UnitTest.hpp"

#if DT3_UNIT_TESTS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class MoreStrings_test: public UnitTest {
    public:
        DEFINE_TYPE(MoreStrings_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (MoreStrings_test)

//==============================================================================
// Implement the test
//==============================================================================

void MoreStrings_test::run_test (void)
{

    TEST_ASSERTION(MoreStrings::trim_end_digits("abc123") == "abc");
    TEST_ASSERTION(MoreStrings::end_digits("abc123") == "123");

}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


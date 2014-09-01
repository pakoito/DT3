//==============================================================================
///	
///	File: Coroutine_test.cpp
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
#include "DT3Core/Types/Utility/Coroutine.hpp"
#include <iostream>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class Coroutine_test: public UnitTest {
    public:
        DEFINE_TYPE(Coroutine_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (Coroutine_test)

//==============================================================================
// Implement the test
//==============================================================================

class CoTest {
    public:
        CoTest (void)
            :   _co(this, &CoTest::run_co, 1024*64)
        {}

        // In the coroutine
        void run_co (Coroutine<CoTest> *co)
        {
            LOG_MESSAGE << "Run CoA";
            co->yield();
            
            LOG_MESSAGE << "Run CoB";
            co->yield();
            
            LOG_MESSAGE << "Run CoC";
            co->yield();
            
            LOG_MESSAGE << "Run CoD";
            co->yield();
        
        }
        
        // Not in the coroutine
        void run (void) {
            
            LOG_MESSAGE << "Run A";
            _co.resume();
            LOG_MESSAGE << "Run B";
            _co.resume();
            LOG_MESSAGE << "Run C";
            _co.resume();
            LOG_MESSAGE << "Run D";
            _co.resume();
            
        }
        
    private:
        Coroutine<CoTest>   _co;
};

void Coroutine_test::run_test (void)
{
    CoTest cot;
    cot.run();
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS


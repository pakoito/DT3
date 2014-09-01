//==============================================================================
///	
///	File: SmartPtr_test.cpp
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
#include "DT3Core/Types/Utility/SmartPtr.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class SmartPtr_test: public UnitTest {
    public:
        DEFINE_TYPE(SmartPtr_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (SmartPtr_test)

//==============================================================================
// Implement the test
//==============================================================================

namespace {

    class A: public SharedPointee<A> {};
    class B: public A {};

};

void SmartPtr_test::run_test (void)
{
//    A *ap = new A();
//    B *bp = new B();
//    
//    SharedPtr<A> a(ap->as_shared());
//    TEST_ASSERTION(a.use_count() == 1);
//    
//    SharedPtr<B> b(bp->as_shared());
//    TEST_ASSERTION(b.use_count() == 1);
//
//    SharedPtr<A> aa(b);         // Test implicit conversions
//    TEST_ASSERTION(aa.use_count() == 2);
//    TEST_ASSERTION(b.use_count() == 2);
//    
//    aa = b;                     // Test implicit conversions
//    TEST_ASSERTION(aa.use_count() == 2);
//    TEST_ASSERTION(b.use_count() == 2);
//
//    SharedPtr<B> bb;            // Plain assignments
//    TEST_ASSERTION(bb.use_count() == 1);
//
//    aa = a;
//    TEST_ASSERTION(aa.use_count() == 2);
//    TEST_ASSERTION(a.use_count() == 2);

//    bb = b;
//
//    //SharedPtr<B> bbb(a);         // Should error. No implicit conversion.
//    
//    a.reset();
//    b.reset();
//    aa.reset();
//    bb.reset();
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS

